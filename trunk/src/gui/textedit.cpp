/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/
/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QImageReader>
#include <QSpinBox>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QMessageBox>
#include <QMimeData>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#endif

#include "textedit.h"
#include "ooo/converter.h"

const QString rsrcPath = ":/textedit";

TextEdit::TextEdit(QWidget *parent, QMenu *menu)
    : ToolMainWindow(parent)
{
    setObjectName("Scratchpad");
    textEdit = new PasteTextEdit(this);

    setupFileActions(menu);
    setupEditActions(menu);
    setupTextActions(menu);
    setupStyleActions();

    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));

    setCentralWidget(textEdit);
    textEdit->setFocus();
    setCurrentFileName(QString());

    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    textEdit->setFont(textFont);
    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            actionSave, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
            actionUndo, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(redoAvailable(bool)),
            actionRedo, SLOT(setEnabled(bool)));

    setWindowModified(textEdit->document()->isModified());
    actionSave->setEnabled(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

    connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);

    connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

    connect(actionPickBoard, SIGNAL(triggered()), this, SLOT(pickBoard()));

#ifndef QT_NO_CLIPBOARD
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
#endif

    fileNew();
}

void TextEdit::setupFileActions(QMenu* xmenu)
{
    QToolBar *tb = new QToolBar(this);
    tb->setObjectName("ScrpFileActions");
    tb->setWindowTitle(tr("File Actions"));
    addToolBar(tb);

    QMenu *menu = xmenu->addMenu(tr("File"));

    QAction *a;

    QIcon newIcon = QIcon(rsrcPath + "/filenew.png");
    a = new QAction( newIcon, tr("New"), this);
    a->setPriority(QAction::LowPriority);
    connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon(rsrcPath + "/fileopen.png"),
                    tr("Open..."), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    actionSave = a = new QAction(QIcon(rsrcPath + "/filesave.png"),
                                 tr("Save"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(tr("Save As..."), this);
    a->setPriority(QAction::LowPriority);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    menu->addAction(a);
    menu->addSeparator();

#ifndef QT_NO_PRINTER
    a = new QAction(QIcon(rsrcPath + "/fileprint.png"),
                    tr("Print..."), this);
    a->setPriority(QAction::LowPriority);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon(rsrcPath + "/fileprint.png"),
                    tr("Print Preview..."), this);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    menu->addAction(a);

    a = new QAction(QIcon(rsrcPath + "/exportpdf.png"),
                    tr("Export PDF..."), this);
    a->setPriority(QAction::LowPriority);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();
#endif

}

void TextEdit::setupEditActions(QMenu* xmenu)
{
    QToolBar *tb = new QToolBar(this);
    tb->setObjectName("ScrpEditActions");
    tb->setWindowTitle(tr("Edit Actions"));
    addToolBar(tb);

    QMenu *menu = xmenu->addMenu(tr("Edit"));

    QAction *a;
    a = actionUndo = new QAction(QIcon(rsrcPath + "/editundo.png"),
                                              tr("Undo"), this);
    tb->addAction(a);
    menu->addAction(a);
    a = actionRedo = new QAction(QIcon(rsrcPath + "/editredo.png"),
                                              tr("Redo"), this);
    a->setPriority(QAction::LowPriority);
    tb->addAction(a);
    menu->addAction(a);
    menu->addSeparator();
    a = actionCut = new QAction(QIcon(rsrcPath + "/editcut.png"),
                                             tr("Cut"), this);
    a->setPriority(QAction::LowPriority);
    tb->addAction(a);
    menu->addAction(a);
    a = actionCopy = new QAction(QIcon(rsrcPath + "/editcopy.png"),
                                 tr("Copy"), this);
    a->setPriority(QAction::LowPriority);
    tb->addAction(a);
    menu->addAction(a);
    a = actionPaste = new QAction(QIcon(rsrcPath + "/editpaste.png"),
                                  tr("Paste"), this);
    a->setPriority(QAction::LowPriority);
    tb->addAction(a);
    menu->addAction(a);
    menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(canInsertFromMimeData(md));
#endif

    a = actionPickBoard = new QAction(QIcon(":/images/new_board.png"), tr("Pick board"), this);
    a->setPriority(QAction::LowPriority);
    tb->addAction(a);
    menu->addAction(a);
}

void TextEdit::setupTextActions(QMenu *xmenu)
{
    QToolBar *tb = new QToolBar(this);
    tb->setObjectName("ScrpTextActions");
    tb->setWindowTitle(tr("Format Actions"));
    addToolBar(tb);

    QMenu *menu = xmenu->addMenu(tr("Format"));

    actionTextBold = new QAction(QIcon(rsrcPath + "/textbold.png"),
                                 tr("Bold"), this);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    tb->addAction(actionTextBold);
    menu->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(QIcon(rsrcPath + "/textitalic.png"),
                                   tr("Italic"), this);
    actionTextItalic->setPriority(QAction::LowPriority);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    tb->addAction(actionTextItalic);
    menu->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(QIcon(rsrcPath + "/textunder.png"),
                                      tr("Underline"), this);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    tb->addAction(actionTextUnderline);
    menu->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();
    tb->addSeparator();

    actionImageSize = new QAction(QIcon(rsrcPath + "/editresize.png"),
                                  tr("Resize"), this);
    connect(actionImageSize, SIGNAL(triggered()), this, SLOT(imageResize()));

    imageSize = new QSpinBox(this);
    imageSize->setMinimum(10);
    imageSize->setMaximum(200);
    imageSize->setValue(100);

    tb->addAction(actionImageSize);
    tb->addWidget(imageSize);
    menu->addAction(actionImageSize);

    menu->addSeparator();
    tb->addSeparator();

    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

    // Make sure the alignLeft  is always left of the alignRight
    if (QApplication::isLeftToRight())
    {
        actionAlignLeft = new QAction(QIcon(rsrcPath + "/textleft.png"),
                                      tr("Left"), grp);
        actionAlignCenter = new QAction(QIcon(rsrcPath + "/textcenter.png"),
                                        tr("Center"), grp);
        actionAlignRight = new QAction(QIcon(rsrcPath + "/textright.png"),
                                       tr("Right"), grp);
    }
    else
    {
        actionAlignRight = new QAction(QIcon(rsrcPath + "/textright.png"),
                                       tr("Right"), grp);
        actionAlignCenter = new QAction(QIcon(rsrcPath + "/textcenter.png"),
                                        tr("Center"), grp);
        actionAlignLeft = new QAction(QIcon(rsrcPath + "/textleft.png"),
                                      tr("Left"), grp);
    }
    actionAlignJustify = new QAction(QIcon(rsrcPath + "/textjustify.png"),
                                     tr("Justify"), grp);

    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    tb->addActions(grp->actions());
    menu->addActions(grp->actions());

    menu->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, tr("Color..."), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    tb->addAction(actionTextColor);
    menu->addAction(actionTextColor);
}

void TextEdit::setupStyleActions()
{
    QToolBar* tb = new QToolBar(this);
    tb->setObjectName("ScrpStyleActions");
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    tb->setWindowTitle(tr("Style Actions"));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);

    QComboBox* comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    comboStyle->addItem("Ordered List (Roman lower)");
    comboStyle->addItem("Ordered List (Roman upper)");
    connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));
}

bool TextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    if (f.endsWith("odt"))
    {
        OOO::Converter *doc_odt = new OOO::Converter();
        QTextDocument *doc = doc_odt->convert(f);
        // Fix bug in Qt
        QTextCursor cursor = QTextCursor(doc->findBlockByLineNumber(0));
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.deleteChar();

        // Set Document
        textEdit->setDocument(doc);
    }
    else
    {
        QByteArray data = file.readAll();
        QTextCodec *codec = Qt::codecForHtml(data);
        QString str = codec->toUnicode(data);
        if (Qt::mightBeRichText(str))
        {
            textEdit->setHtml(str);
        }
        else
        {
            str = QString::fromLocal8Bit(data);
            textEdit->setPlainText(str);
        }
    }

    setCurrentFileName(f);
    return true;
}

bool TextEdit::saveDocument()
{
    if (!textEdit->document()->isModified())
        return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The scratchpad has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*]").arg(shownName));
    setWindowModified(false);
}

void TextEdit::fileNew()
{
    if (saveDocument())
    {
        textEdit->clear();
        setCurrentFileName(QString());
    }
}

void TextEdit::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("ODF files (*.odt);;HTML-Files (*.htm *.html);;All Files (*)"));
    if (!fn.isEmpty())
    {
        load(fn);
    }
}

bool TextEdit::fileSave()
{
    if (fileName.isEmpty())
    {
        return fileSaveAs();
    }
    if (fileName.startsWith(QStringLiteral(":/")))
    {
        return fileSaveAs();
    }

    // Second nasty surprise: the writer cannot write images in HTML correctly
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success)
    {
        textEdit->document()->setModified(false);
    }
    return success;
}

bool TextEdit::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),
                                              tr("ODF files (*.odt);;HTML-Files "
                                                 "(*.htm *.html);;All Files (*)"));
    if (fn.isEmpty())
    {
        return false;
    }
    if (!(fn.endsWith(".odt", Qt::CaseInsensitive)
          || fn.endsWith(".htm", Qt::CaseInsensitive)
          || fn.endsWith(".html", Qt::CaseInsensitive)))
    {
        fn += ".odt"; // default
    }
    setCurrentFileName(fn);
    return fileSave();
}

void TextEdit::filePrint()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
        textEdit->print(&printer);
    delete dlg;
#endif
}

void TextEdit::filePrintPreview()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}


void TextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                    QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        textEdit->document()->print(&printer);
    }
#endif
}

void TextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextEdit::textStyle(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();

    if (styleIndex != 0)
    {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (styleIndex) {
            default:
            case 1:
                style = QTextListFormat::ListDisc;
                break;
            case 2:
                style = QTextListFormat::ListCircle;
                break;
            case 3:
                style = QTextListFormat::ListSquare;
                break;
            case 4:
                style = QTextListFormat::ListDecimal;
                break;
            case 5:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat::ListUpperAlpha;
                break;
            case 7:
                style = QTextListFormat::ListLowerRoman;
                break;
            case 8:
                style = QTextListFormat::ListUpperRoman;
                break;
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    }
    else
    {
        // ####
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void TextEdit::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void TextEdit::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(Qt::AlignJustify);
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEdit::cursorPositionChanged()
{
    alignmentChanged(textEdit->alignment());
}

void TextEdit::pickBoard()
{
    QImage image;
    emit requestBoardImage(image);
    textEdit->insertImage(image);
}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(canInsertFromMimeData(md));
#endif
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);
}

bool TextEdit::canInsertFromMimeData(const QMimeData* source) const
{
    return textEdit->canInsertUsingMimeData(source);
}

void TextEdit::insertFromMimeData(const QMimeData* source)
{
    textEdit->insertUsingMimeData(source);
}

void TextEdit::imageResize()
{
    QPair<double,double> factor;
    factor.first = factor.second = imageSize->value();
    textEdit->resizeImage(factor);
}

///////////////////////////////////////////////////////////////////////////////////////

PasteTextEdit::PasteTextEdit(QWidget *parent) : QTextEdit(parent), m_imageCounter(0)
{
    installEventFilter(this);
}

bool PasteTextEdit::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Shortcut)
    {
        if (obj != this)
        {
            return true;
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}

bool PasteTextEdit::canInsertUsingMimeData(const QMimeData *source) const
{
    return canInsertFromMimeData(source);
}

void PasteTextEdit::insertUsingMimeData(const QMimeData *source)
{
    insertFromMimeData(source);
}

bool PasteTextEdit::canInsertFromMimeData(const QMimeData *source) const
{
    return source->hasImage() || source->hasUrls() || QTextEdit::canInsertFromMimeData(source);
}

void PasteTextEdit::insertImage(const QImage& image)
{
    QUrl url(QString("dropped_image_%1").arg(m_imageCounter++));
    dropImage(url, image);
}

void PasteTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasImage())
    {
        insertImage(qvariant_cast<QImage>(source->imageData()));
    }
    else if (source->hasUrls())
    {
        foreach (QUrl url, source->urls())
        {
            QFileInfo info(url.toLocalFile());
            if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
                dropImage(url, QImage(info.filePath()));
            else
                dropTextFile(url);
        }
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}

void PasteTextEdit::dropImage(const QUrl& url, const QImage& image)
{
    if (!image.isNull())
    {
        document()->addResource(QTextDocument::ImageResource, url, image);

        QTextImageFormat imageFormat;
        imageFormat.setWidth( image.width() );
        imageFormat.setHeight( image.height() );
        imageFormat.setName( url.toString() );
        textCursor().insertImage(imageFormat);
    }
}

void PasteTextEdit::dropTextFile(const QUrl& url)
{
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        textCursor().insertText(file.readAll());
    }
}

void PasteTextEdit::resizeImage(QPair<double, double> factor)
{
    QTextBlock currentBlock = textCursor().block();
    QTextBlock::iterator it;

    for (it = currentBlock.begin(); !(it.atEnd()); ++it)
    {
        QTextFragment fragment = it.fragment();

        if (fragment.isValid())
        {
            if(fragment.charFormat().isImageFormat())
            {
                QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();

                if (newImageFormat.isValid())
                {
                    double w = newImageFormat.width() * factor.first / 100.0;
                    double h = newImageFormat.height() * factor.first / 100.0;

                    if (w && h)
                    {
                        newImageFormat.setWidth(w);
                        newImageFormat.setHeight(h);

                        QTextCursor tc = textCursor();

                        tc.setPosition(fragment.position());
                        tc.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                        tc.setCharFormat(newImageFormat);
                    }
                }
            }
        }
    }
}

