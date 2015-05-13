/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
*   Copyright (C) 2006 by Tobias Koenig                                     *
*   Copyright (C)      by pro                                               *
****************************************************************************/

#include "document.h"
#include "styleinformation.h"
#include "converter.h"
#include "formatproperty.h"

#include <QTextCharFormat>
#include <QDomDocument>
#include <QQueue>
#include <QUrl>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextList>
#include <QTextTableCell>
#include <QDomElement>
#include <QDomText>
#include <QXmlSimpleReader>
#include <QPixmap>
#include <QDebug>
#include <QDateTime>

using namespace OOO;

class Style {
public:
    Style(const QTextBlockFormat &blockFormat, const QTextCharFormat &textFormat);

    QTextBlockFormat blockFormat() const;
    QTextCharFormat textFormat() const;

private:
    QTextBlockFormat mBlockFormat;
    QTextCharFormat mTextFormat;
};

Style::Style(const QTextBlockFormat &blockFormat, const QTextCharFormat &textFormat)
: mBlockFormat(blockFormat), mTextFormat(textFormat) {
}

QTextBlockFormat Style::blockFormat() const {
    return mBlockFormat;
}

QTextCharFormat Style::textFormat() const {
    return mTextFormat;
}

Converter::Converter()
: mTextDocument(0), mCursor(0),
mStyleInformation(0) {
}

QTextDocument* Converter::convert(const QString &fileName) {
    Document oooDocument(fileName);
    if (!oooDocument.open()) {
        //// emit error(oooDocument.lastErrorString(), -1);
        return 0;
    }
    mTextDocument = new QTextDocument;
    mCursor = new QTextCursor(mTextDocument);

    /**
     * Create the dom of the content
     */
    QXmlSimpleReader reader;

    QXmlInputSource source;
    source.setData(oooDocument.content());
    QString errorMsg;
    QDomDocument document;
    if (!document.setContent(&source, &reader, &errorMsg)) {
        setError(QString("Invalid XML document: %1").arg(errorMsg), -1);
        delete mCursor;
        return mTextDocument;
    }

    mStyleInformation = new StyleInformation();

    if (oooDocument.styles().size() > 0) {
        StyleParser styleParser(&oooDocument, document, mStyleInformation);
        if (!styleParser.parse()) {
            setError("Unable to read style information", -1);
            delete mCursor;
            return 0;
        }
    }
    if (oooDocument.content().size() == 0)
    {
        setError("Empty document", -1);
    }

    /**
     * Read the style properties, so the are available when
     * parsing the content.
     */



    /**
     * Add all images of the document to resource framework
     */

    imageLIST = oooDocument.images();
    QMapIterator<QString, QByteArray> it(imageLIST);
    while (it.hasNext())
    {
        it.next();
        mTextDocument->addResource(QTextDocument::ImageResource, QUrl(it.key()), QImage::fromData(it.value()));
    }

    /**
     * Set the correct page size
     */

    const QString masterLayout = mStyleInformation->masterPageName();

    if (mStyleInformation->pagePropertyExists(masterLayout))
    {
        const int DPX = 231; /// im.logicalDpiX(); // 231
        const int DPY = 231; // im.logicalDpiY(); // 231
        const int A4Width = MM_TO_POINT(210); /// A4 210 x297 mm
        const int A4Height = MM_TO_POINT(297);

        const PageFormatProperty property = mStyleInformation->pageProperty(masterLayout);
        int pageWidth = qRound(property.width() / 72.0 * DPX);
        if (pageWidth < 1) {
            pageWidth = A4Width;
        }
        int pageHeight = qRound(property.height() / 72.0 * DPY);
        if (pageHeight < 1) {
            pageHeight = A4Height;
        }
        mTextDocument->setPageSize(QSize(pageWidth, pageHeight));

        QTextFrameFormat frameFormat;
        frameFormat.setMargin(qRound(property.margin()));

        QTextFrame *rootFrame = mTextDocument->rootFrame();
        rootFrame->setFrameFormat(frameFormat);
    }

    /**
     * Parse the content of the document
     */
    const QDomElement documentElement = document.documentElement();

    QDomElement element = documentElement.firstChildElement();
    while (!element.isNull()) {
        if (element.tagName() == QLatin1String("body")) {
            if (!convertBody(element)) {
                setError("Unable to convert document content", -1);
                delete mCursor;
                return 0;
            }
        }

        element = element.nextSiblingElement();
    }

    return mTextDocument;
}

void Converter::setError(QString msg, int /*vars*/)
{
    qDebug() << msg;
}

bool Converter::convertBody(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("text"))
        {
            if (!convertText(child))
            {
                return false;
            }
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertText(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("p")) {
            mCursor->insertBlock();
            if (!convertParagraph(mCursor, child))
                return false;
        } else if (child.tagName() == QLatin1String("h")) {
            mCursor->insertBlock();
            if (!convertHeader(mCursor, child))
                return false;
        } else if (child.tagName() == QLatin1String("list")) {
            if (!convertList(mCursor, child))
                return false;
        } else if (child.tagName() == QLatin1String("table")) {
            if (!convertTable(child))
                return false;
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertHeader(QTextCursor *cursor, const QDomElement &element)
{
    const QString styleName = element.attribute("style-name");
    const StyleFormatProperty property = mStyleInformation->styleProperty(styleName);

    QTextBlockFormat blockFormat;
    QTextCharFormat textFormat;
    property.applyBlock(&blockFormat);
    property.applyText(&textFormat);

    cursor->setBlockFormat(blockFormat);

    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            const QDomElement childElement = child.toElement();
            if (childElement.tagName() == QLatin1String("span")) {
                if (!convertSpan(cursor, childElement, textFormat))
                    return false;
            }
        } else if (child.isText()) {
            const QDomText childText = child.toText();
            if (!convertTextNode(cursor, childText, textFormat))
                return false;
        }

        child = child.nextSibling();
    }

    //// emit addTitle(element.attribute("outline-level", 0).toInt(), element.text(), cursor->block());

    return true;
}

bool Converter::convertParagraph(QTextCursor *cursor, const QDomElement &element, const QTextBlockFormat &parentFormat, bool merge)
{
    const QString styleName = element.attribute("style-name");
    const StyleFormatProperty property = mStyleInformation->styleProperty(styleName);

    QTextBlockFormat blockFormat(parentFormat);
    QTextCharFormat textFormat;
    property.applyBlock(&blockFormat);
    property.applyText(&textFormat);

    if (merge)
        cursor->mergeBlockFormat(blockFormat);
    else
        cursor->setBlockFormat(blockFormat);

    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            const QDomElement childElement = child.toElement();
            if (childElement.tagName() == QLatin1String("span")) {
                if (!convertSpan(cursor, childElement, textFormat))
                    return false;
            } else if (childElement.tagName() == QLatin1String("tab")) {
                mCursor->insertText("    ");
            } else if (childElement.tagName() == QLatin1String("s")) {
                QString spaces;
                spaces.fill(' ', childElement.attribute("c").toInt());
                mCursor->insertText(spaces);
            } else if (childElement.tagName() == QLatin1String("frame")) {
                const int xpos = cursor->position();
                mCursor->setPosition(xpos, QTextCursor::MoveAnchor);
                if (!convertFrame(childElement))
                    return false;
            } else if (childElement.tagName() == QLatin1String("a")) {
                if (!convertLink(cursor, childElement, textFormat))
                    return false;
            } else if (childElement.tagName() == QLatin1String("annotation")) {
                if (!convertAnnotation(cursor, childElement))
                    return false;
            }
        } else if (child.isText()) {
            const QDomText childText = child.toText();
            if (!convertTextNode(cursor, childText, textFormat))
                return false;
        }

        child = child.nextSibling();
    }

    return true;
}

bool Converter::convertTextNode(QTextCursor *cursor, const QDomText &element, const QTextCharFormat &format)
{
    cursor->insertText(element.data(), format);
    return true;
}

bool Converter::convertSpan(QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format)
{
    const QString styleName = element.attribute("style-name");
    const StyleFormatProperty property = mStyleInformation->styleProperty(styleName);

    QTextCharFormat textFormat(format);
    property.applyText(&textFormat);

    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        //// search <text:s/> 
        if (child.isText()) {
            const QDomText childText = child.toText();
            if (!convertTextNode(cursor, childText, textFormat))
                return false;
        } else if (child.isElement()) {
            const QDomElement childElement = child.toElement();
            if (childElement.tagName() == QLatin1String("tab")) {
                cursor->insertText("    ");
            } else if (childElement.tagName() == QLatin1String("s")) {
                QString spaces;
                spaces.fill(' ', childElement.attribute("c").toInt());
                cursor->insertText(spaces);
            } else if (childElement.tagName() == QLatin1String("frame")) {
                const int xpos = cursor->position();
                mCursor->setPosition(xpos, QTextCursor::MoveAnchor);
                if (!convertFrame(childElement))
                    return false;
            }

        }
        child = child.nextSibling();
    }

    return true;
}

bool Converter::convertList(QTextCursor *cursor, const QDomElement &element)
{
    const QString styleName = element.attribute("style-name");
    const ListFormatProperty property = mStyleInformation->listProperty(styleName);

    QTextListFormat format;

    if (cursor->currentList()) { // we are in a nested list
        format = cursor->currentList()->format();
        format.setIndent(format.indent() + 1);
    }

    property.apply(&format, 0);

    QTextList *list = cursor->insertList(format);

    QDomElement itemChild = element.firstChildElement();
    int loop = 0;
    while (!itemChild.isNull()) {
        if (itemChild.tagName() == QLatin1String("list-item")) {
            loop++;

            QDomElement childElement = itemChild.firstChildElement();
            while (!childElement.isNull()) {

                QTextBlock prevBlock;

                if (childElement.tagName() == QLatin1String("p")) {
                    if (loop > 1)
                        cursor->insertBlock();

                    prevBlock = cursor->block();

                    if (!convertParagraph(cursor, childElement, QTextBlockFormat(), true))
                        return false;

                } else if (childElement.tagName() == QLatin1String("list")) {
                    prevBlock = cursor->block();

                    if (!convertList(cursor, childElement))
                        return false;
                }

                if (prevBlock.isValid())
                    list->add(prevBlock);

                childElement = childElement.nextSiblingElement();
            }
        }

        itemChild = itemChild.nextSiblingElement();
    }

    return true;
}

static void enqueueNodeList(QQueue<QDomNode> &queue, const QDomNodeList &list) {
    for (int i = 0; i < list.count(); ++i) {
        queue.enqueue(list.at(i));
    }
}

bool Converter::convertTable(const QDomElement &element)
{
    /**
     * Find out dimension of the table
     */

    int rowCounter = 0;
    int columnCounter = 0;

    QQueue<QDomNode> nodeQueue;
    enqueueNodeList(nodeQueue, element.childNodes());
    while (!nodeQueue.isEmpty()) {
        QDomElement el = nodeQueue.dequeue().toElement();
        if (el.isNull())
            continue;

        if (el.tagName() == QLatin1String("table-row")) {
            rowCounter++;

            int counter = 0;
            QDomElement columnElement = el.firstChildElement();
            while (!columnElement.isNull()) {
                if (columnElement.tagName() == QLatin1String("table-cell")) {
                    counter++;
                }
                columnElement = columnElement.nextSiblingElement();
            }

            columnCounter = qMax(columnCounter, counter);
        } else if (el.tagName() == QLatin1String("table-header-rows")) {
            enqueueNodeList(nodeQueue, el.childNodes());
        }
    }

    /**
     * Create table
     */
    QTextTable *table = mCursor->insertTable(rowCounter, columnCounter);
    mCursor->movePosition(QTextCursor::End);

    /**
     * Fill table
     */
    nodeQueue.clear();
    enqueueNodeList(nodeQueue, element.childNodes());

    QTextTableFormat tableFormat;

    rowCounter = 0;
    while (!nodeQueue.isEmpty()) {
        QDomElement el = nodeQueue.dequeue().toElement();
        if (el.isNull())
            continue;

        if (el.tagName() == QLatin1String("table-row")) {

            int columnCounter = 0;
            QDomElement columnElement = el.firstChildElement();
            while (!columnElement.isNull()) {
                if (columnElement.tagName() == QLatin1String("table-cell")) {
                    const StyleFormatProperty property = mStyleInformation->styleProperty(columnElement.attribute("style-name"));

                    QTextBlockFormat format;
                    property.applyTableCell(&format);

                    QDomElement paragraphElement = columnElement.firstChildElement();
                    while (!paragraphElement.isNull()) {
                        if (paragraphElement.tagName() == QLatin1String("p")) {
                            QTextTableCell cell = table->cellAt(rowCounter, columnCounter);
                            // Insert a frame into the cell and work on that, so we can handle
                            // different parts of the cell having different block formatting
                            QTextCursor cellCursor = cell.lastCursorPosition();
                            QTextFrameFormat frameFormat;
                            frameFormat.setMargin(1); // TODO: this shouldn't be hard coded
                            QTextFrame *frame = cellCursor.insertFrame(frameFormat);
                            QTextCursor frameCursor = frame->firstCursorPosition();
                            frameCursor.setBlockFormat(format);

                            if (!convertParagraph(&frameCursor, paragraphElement, format))
                                return false;
                        } else if (paragraphElement.tagName() == QLatin1String("list")) {
                            QTextTableCell cell = table->cellAt(rowCounter, columnCounter);
                            // insert a list into the cell
                            QTextCursor cellCursor = cell.lastCursorPosition();
                            if (!convertList(&cellCursor, paragraphElement)) {
                                return false;
                            }
                        }

                        paragraphElement = paragraphElement.nextSiblingElement();
                    }
                    columnCounter++;
                }
                columnElement = columnElement.nextSiblingElement();
            }

            rowCounter++;
        } else if (el.tagName() == QLatin1String("table-column")) {
            const StyleFormatProperty property = mStyleInformation->styleProperty(el.attribute("style-name"));
            const QString tableColumnNumColumnsRepeated = el.attribute("number-columns-repeated", "1");
            int numColumnsToApplyTo = tableColumnNumColumnsRepeated.toInt();
            for (int i = 0; i < numColumnsToApplyTo; ++i) {
                property.applyTableColumn(&tableFormat);
            }
        }
    }

    table->setFormat(tableFormat);

    return true;
}

bool Converter::convertFrame(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    QDomElement suchild = element.lastChildElement("title");
    /// svg:title alt && title 
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("image")) {
            const QString href = child.attribute("href");
            const QString xname = element.attribute("name");
            QString alttitle = QString("Name:%1 - Ref:%2").arg(xname).arg(href);
            if (suchild.tagName() == QLatin1String("title")) {
                alttitle.append(QString(" - Title:%1").arg(suchild.text()));
            }
            QTextImageFormat format;
            format.setToolTip(alttitle);
            format.setWidth(StyleParser::convertUnit(element.attribute("width")));
            format.setHeight(StyleParser::convertUnit(element.attribute("height")));
            format.setName(href);
            mCursor->insertImage(format);
        }

        child = child.nextSiblingElement();
    }

    return true;
}

bool Converter::convertLink(QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format) {
    int startPosition = cursor->position();

    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            const QDomElement childElement = child.toElement();
            if (childElement.tagName() == QLatin1String("span")) {
                if (!convertSpan(cursor, childElement, format))
                    return false;
            }
        } else if (child.isText()) {
            const QDomText childText = child.toText();
            if (!convertTextNode(cursor, childText, format))
                return false;
        }

        child = child.nextSibling();
    }

    int endPosition = cursor->position();
    USETODONEXT(endPosition);
    USETODONEXT(startPosition);
    ////Okular::Action *action = new Okular::BrowseAction(element.attribute("href"));
    //// emit addAction(action, startPosition, endPosition);

    return true;
}

bool Converter::convertAnnotation(QTextCursor *cursor, const QDomElement &element) {
    QStringList contents;
    QString creator;
    QDateTime dateTime;

    int position = cursor->position();

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == QLatin1String("creator")) {
            creator = child.text();
        } else if (child.tagName() == QLatin1String("date")) {
            dateTime = QDateTime::fromString(child.text(), Qt::ISODate);
        } else if (child.tagName() == QLatin1String("p")) {
            contents.append(child.text());
        }

        child = child.nextSiblingElement();
    }

    USETODONEXT(position);

    /* 
    Okular::TextAnnotation *annotation = new Okular::TextAnnotation;
    annotation->setAuthor(creator);
    annotation->setContents(contents.join("\n"));
    annotation->setCreationDate(dateTime);
    annotation->style().setColor(QColor("#ffff00"));
    annotation->style().setOpacity(0.5);
     * */


    return true;
}

Converter::~Converter() {
}






