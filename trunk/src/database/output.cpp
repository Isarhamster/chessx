/***************************************************************************
 *   (C) 2006-2007 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <algorithm>
#include <QMap>

#include "board.h"
#include "boardview.h"
#include "output.h"
#include "settings.h"
#include "tags.h"
#include "partialdate.h"

const char* TEMPLATE_DIR = "templates";
const char* DEFAULT_HTML_TEMPLATE = "html-default.template";
const char* DEFAULT_NOTATION_TEMPLATE = "notation-default.template";
const char* DEFAULT_LATEX_TEMPLATE = "latex-default.template";
const char* DEFAULT_PGN_TEMPLATE = "pgn-default.template";

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

QMap<Output::OutputType, QString> Output::m_outputMap;

Output::Output(OutputType output, const QString& pathToTemplateFile)
{
    m_outputType = output;
    switch(m_outputType)
    {
    case Html:
        m_options.createDefaultOptions("");
        break;
    case Pgn:
        m_options.createDefaultOptions("");
        break;
    case Latex:
        m_options.createDefaultOptions("");
        break;
    case NotationWidget:
        m_options.createDefaultOptions("GameText");
        break;
    default:
        Q_ASSERT(false);
    }

    setTemplateFile(pathToTemplateFile);
    initialize();
}

Output::~Output()
{
}

void Output::initialize()
{
    if(m_outputType == Pgn)
    {
        m_newlineChar = "\n";
    }
    else if(m_outputType == Html)
    {
        m_newlineChar = "<br>\n";
    }
    else if(m_outputType == Latex)
    {
        m_newlineChar = "\n";
    }
    else if(m_outputType == NotationWidget)
    {
        m_newlineChar = "<br>\n";
    }
    readTemplateFile(m_templateFilename);
}

void Output::readTemplateFile(const QString& path)
{
    QFile file(path);
    QStringList optionDefFields;
    enum ReadingStatus {NONE, ReadingOptionDefs, ReadingOptions, ReadingOutputHeader, ReadingOutputFooter, ReadingMarkupTags};
    ReadingStatus status = NONE;
    m_header = "";
    m_footer = "";
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;
        int i = 0;
        while(!stream.atEnd())
        {
            line = stream.readLine(); // line of text excluding '\n'
            i++;
            if((line.indexOf(QRegExp("^\\s*$")) != -1) || (line.indexOf(QRegExp("^\\s*#")) != -1))
            {
                // Skip blank lines and comments (#)
                continue;
            }
            if(line == "[OPTIONSDEFINITIONS]")
            {
                status = ReadingOptionDefs;
                continue;
            }
            else if(line == "[OPTIONS]")
            {
                status = ReadingOptions;
                continue;
            }
            else if(line == "[OUTPUTHEADER]")
            {
                status = ReadingOutputHeader;
                continue;
            }
            else if(line == "[OUTPUTFOOTER]")
            {
                status = ReadingOutputFooter;
                continue;
            }
            else if(line == "[MARKUPTAGS]")
            {
                status = ReadingMarkupTags;
                continue;
            }

            // Hack to replace \\n with newline but leavin tags like \\newenvironment intact
            line.replace("\\\\n", "\\\\^");
            line.replace("\\n", "\n");
            line.replace("\\\\^", "\\n");

            switch(status)
            {
            case ReadingOptionDefs:
                optionDefFields = line.split(",");
                if(!m_options.createOption(optionDefFields[0], optionDefFields[1],
                                           optionDefFields[2], optionDefFields[3], optionDefFields[4]))
                {
                    qWarning("Could not create option. Ignoring line %d in file %s :\n%s", i, path.toLatin1().constData(), line.toLatin1().constData());
                }

                break;
            case ReadingOptions:
                if(!m_options.setOption(line))
                {
                    qWarning("Invalid option or value in file %s line %d. Ignoring : %s", path.toLatin1().constData(), i, line.toLatin1().constData());
                }
                break;
            case ReadingOutputHeader:
                m_header += line;
                break;
            case ReadingOutputFooter:
                m_footer += line;
                break;
            case ReadingMarkupTags:
            {
                QString name = line.mid(0, line.indexOf('='));
                QStringList tags = (line.mid(line.indexOf('=') + 1)).split(',');
                if(name == "MarkupHeaderBlock")
                {
                    setMarkupTag(MarkupHeaderBlock, tags[0], tags[1]);
                }
                else if(name == "MarkupNotationBlock")
                {
                    setMarkupTag(MarkupNotationBlock, tags[0], tags[1]);
                }
                else if(name == "MarkupResult")
                {
                    setMarkupTag(MarkupResult, tags[0], tags[1]);
                }
                else if(name == "MarkupDiagram")
                {
                    setMarkupTag(MarkupDiagram, tags[0], tags[1]);
                }
                else if(name == "MarkupMainLineMove")
                {
                    setMarkupTag(MarkupMainLineMove, tags[0], tags[1]);
                }
                else if(name == "MarkupVariationMove")
                {
                    setMarkupTag(MarkupVariationMove, tags[0], tags[1]);
                }
                else if(name == "MarkupMainLine")
                {
                    setMarkupTag(MarkupMainLine, tags[0], tags[1]);
                }
                else if(name == "MarkupVariationInline")
                {
                    setMarkupTag(MarkupVariationInline, tags[0], tags[1]);
                }
                else if(name == "MarkupVariationResume")
                {
                    setMarkupTag(MarkupVariationResume, tags[0], tags[1]);
                }
                else if(name == "MarkupVariationIndent")
                {
                    setMarkupTag(MarkupVariationIndent, tags[0], tags[1]);
                }
                else if(name == "MarkupVariationIndent1")
                {
                    setMarkupTag(MarkupVariationIndent1, tags[0], tags[1]);
                }
                else if(name == "MarkupNag")
                {
                    setMarkupTag(MarkupNag, tags[0], tags[1]);
                }
                else if(name == "MarkupAnnotationInline")
                {
                    setMarkupTag(MarkupAnnotationInline, tags[0], tags[1]);
                }
                else if(name == "MarkupAnnotationIndent")
                {
                    setMarkupTag(MarkupAnnotationIndent, tags[0], tags[1]);
                }
                else if(name == "MarkupPreAnnotationInline")
                {
                    setMarkupTag(MarkupPreAnnotationInline, tags[0], tags[1]);
                }
                else if(name == "MarkupPreAnnotationIndent")
                {
                    setMarkupTag(MarkupPreAnnotationIndent, tags[0], tags[1]);
                }
                else if(name == "MarkupHeaderLine")
                {
                    setMarkupTag(MarkupHeaderLine, tags[0], tags[1]);
                }
                else if(name == "MarkupHeaderTagName")
                {
                    setMarkupTag(MarkupHeaderTagName, tags[0], tags[1]);
                }
                else if(name == "MarkupHeaderTagValue")
                {
                    setMarkupTag(MarkupHeaderTagValue, tags[0], tags[1]);
                }
                else if(name == "MarkupWhiteTag")
                {
                    setMarkupTag(MarkupWhiteTag, tags[0], tags[1]);
                }
                else if(name == "MarkupBlackTag")
                {
                    setMarkupTag(MarkupBlackTag, tags[0], tags[1]);
                }
                else if(name == "MarkupEventTag")
                {
                    setMarkupTag(MarkupEventTag, tags[0], tags[1]);
                }
                else if(name == "MarkupDateTag")
                {
                    setMarkupTag(MarkupDateTag, tags[0], tags[1]);
                }
                else if(name == "MarkupSiteTag")
                {
                    setMarkupTag(MarkupSiteTag, tags[0], tags[1]);
                }
                else if(name == "MarkupResultTag")
                {
                    setMarkupTag(MarkupResultTag, tags[0], tags[1]);
                }
                else if(name == "MarkupRoundTag")
                {
                    setMarkupTag(MarkupRoundTag, tags[0], tags[1]);
                }
                else if(name == "MarkupColumnStyleMove")
                {
                    setMarkupTag(MarkupColumnStyleMove, tags[0], tags[1]);
                }
                else if(name == "MarkupColumnStyleRow")
                {
                    setMarkupTag(MarkupColumnStyleRow, tags[0], tags[1]);
                }
                else if(name == "MarkupColumnStyleMainline")
                {
                    setMarkupTag(MarkupColumnStyleMainline, tags[0], tags[1]);
                }
                else if(name == "MarkupMate")
                {
                    setMarkupTag(MarkupMate, tags[0], tags[1]);
                }
                else
                {
                    qWarning("Unknown Markup Tag found in file %s line %d. Ignoring : %s", path.toLatin1().constData(), i,  line.toLatin1().constData());
                }
            }
            break;
            default :
                qWarning("Unknown Section in Template File %s line %d : %s", path.toLatin1().constData(), i, line.toLatin1().constData());
            }
        }
        file.close();
    }
}

QMap<Output::OutputType, QString>& Output::getFormats()
{
    m_outputMap.clear();
    m_outputMap[Html] = "Html Output";
    m_outputMap[Pgn] = "Pgn Output";
    m_outputMap[Latex] = "Latex Output";
    m_outputMap[NotationWidget] = "Notation Widget Output";
    return m_outputMap;
}

QString Output::writeDiagram(int n) const
{
    QString imageString;
    if((m_outputType == NotationWidget) && (AppSettings->getValue("/GameText/ShowDiagrams").toBool()))
    {
        QImage image;

        Game g = m_game;
        g.forward(1);

        BoardView::renderImageForBoard(g.board(), image, QSize(n,n));

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        image.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer
        QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
        imageString = QString("\n") +
                      m_startTagMap[MarkupDiagram] +
                      "<img alt='Diagram' src='data:image/gif;base64," + iconBase64 + "'>\n" +
                      m_endTagMap[MarkupDiagram];
    }

    return imageString;
}

QString Output::writeMove(MoveToWrite moveToWrite)
{
    QString text;
    QString mvno;
    QString nagString;
    QString imageString;
    QString precommentString;

    MoveId moveId;
    if(moveToWrite == NextMove)
    {
        moveId = m_game.nextMove();
    }
    else
    {
        moveId = m_game.currentMove();
    }

    if (moveId <= ROOT_NODE)  return text; // ?

    mvno = QString::number(moveId);
    if(m_game.nags(moveId).count() > 0)
    {
        if(m_options.getOptionAsBool("SymbolicNag"))
        {
            nagString += m_game.nags(moveId).toString(m_outputType == Html ? NagSet::HTML : NagSet::Simple);
            if((m_outputType == Html || m_outputType == NotationWidget) && (m_game.nags(moveId).contains(NagDiagram)))
            {
                int n = m_options.getOptionAsInt("DiagramSize");
                if(n)
                {
                    imageString = writeDiagram(n);
                }
            }
        }
        else
        {
            nagString += m_game.nags(moveId).toString(NagSet::PGN);
        }

    }
    // Read comments
    if(m_game.canHaveStartAnnotation(moveId))
        precommentString = (m_outputType == Pgn) ? m_game.annotation(moveId, Game::BeforeMove) :
                           m_game.textAnnotation(moveId, Game::BeforeMove);

    QString commentString = (m_outputType == Pgn) ? m_game.annotation(moveId) : m_game.textAnnotation(moveId);

    // Write precomment if any
    text += writeComment(precommentString, mvno, Precomment);

    Color c = m_game.board().toMove();

    if((m_options.getOptionAsBool("ColumnStyle")) &&
            (m_currentVariationLevel == 0) &&
            ((c == White) || m_dirtyBlack))
    {
        text += m_startTagMap[MarkupColumnStyleRow] + m_startTagMap[MarkupColumnStyleMove];
    }
    else if((m_options.getOptionAsBool("ColumnStyle")) &&
            (m_currentVariationLevel == 0) &&
            (c == Black))
    {
        text += m_startTagMap[MarkupColumnStyleMove];
    }

    // *** Determine actual san
    QString san;
    Game::MoveStringFlags flags = (m_outputType == NotationWidget) ? Game::TranslatePiece : Game::MoveOnly;
    if(moveToWrite == NextMove)
    {
        san = m_game.moveToSan(flags);
    }
    else
    {
        san = m_game.moveToSan((Game::MoveStringFlags)(flags | Game::MoveOnly), Game::PreviousMove);
    }

    if (!san.isEmpty())
    {
        // *** Write the move number
        if(moveToWrite == PreviousMove)
        {
            c = oppositeColor(c);
        }
        if(c == White)
        {
            text += QString::number(m_game.moveNumber(moveId)) + ". ";
        }
        else if(m_dirtyBlack)
        {
            text += QString::number(m_game.moveNumber(moveId)) + "... ";
            if((m_options.getOptionAsBool("ColumnStyle")) &&
                    (m_currentVariationLevel == 0))
            {
                text += m_endTagMap[MarkupColumnStyleMove] + m_startTagMap[MarkupColumnStyleMove];
            }
        }
        m_dirtyBlack = false;

        // *** Markup for the move
        if(m_currentVariationLevel > 0)
        {
            if(m_expandable[MarkupVariationMove])
            {
                text += m_startTagMap[MarkupVariationMove].arg(mvno);
            }
            else
            {
                text += m_startTagMap[MarkupVariationMove];
            }
        }
        else
        {
            if(m_expandable[MarkupMainLineMove])
            {
                text += m_startTagMap[MarkupMainLineMove].arg(mvno);
            }
            else
            {
                text += m_startTagMap[MarkupMainLineMove];
            }
        }

        // *** Write the actual move
        QString mate = m_startTagMap[MarkupMate] + "#" + m_endTagMap[MarkupMate];
        san.replace("#", mate);
        text += san;

        // *** End the markup for the move
        if(m_currentVariationLevel > 0)
        {
            text += m_endTagMap[MarkupVariationMove];
        }
        else
        {
            text += m_endTagMap[MarkupMainLineMove];
        }
        // *** Write the nags if there are any
        if(!nagString.isEmpty())
        {
            text += m_startTagMap[MarkupNag] + nagString + m_endTagMap[MarkupNag];
        }
    }

    if((m_options.getOptionAsBool("ColumnStyle")) &&
            (m_currentVariationLevel == 0) &&
            (c == White))
    {
        text += m_endTagMap[MarkupColumnStyleMove];
        m_game.forward();
        if(m_game.atGameEnd())
        {
            text += m_endTagMap[MarkupColumnStyleRow];
        }
        m_game.backward();
    }

    if((m_options.getOptionAsBool("ColumnStyle")) &&
            (m_currentVariationLevel == 0) &&
            (c == Black))
    {
        text += m_endTagMap[MarkupColumnStyleMove] + m_endTagMap[MarkupColumnStyleRow];
    }

    text += imageString;
    if(!imageString.isEmpty())
    {
        m_dirtyBlack = true;
    }

    text += writeComment(commentString, mvno, Comment);
    if (m_game.hasNextMove())
    {
        text += " ";
    }
    return text;
}

QString Output::writeMainLine(MoveId upToNode)
{
    QString text;
    do
    {
        bool hasNext = false;
        if (!m_game.atLineEnd())
        {
            // Training mode: abort main line with current node
            if(m_game.currentMove() == upToNode)
            {
                if(m_options.getOptionAsBool("ColumnStyle"))
                {
                    text += m_endTagMap[MarkupColumnStyleMainline];
                }
                text += "***";
                break;
            }
            // *** Write moves in the main line
            text += writeMove();
            hasNext = m_game.hasNextMove();
        }

        if(m_game.variationCount())
        {
            QList<MoveId> variations = m_game.variations();
            if(variations.size())
            {
                if(m_options.getOptionAsBool("ColumnStyle"))
                {
                    text += m_endTagMap[MarkupColumnStyleMainline];
                }
                for(int i = 0; i < variations.size(); ++i)
                {
                    // *** Enter variation i, and write the rest of the moves
                    m_game.dbMoveToId(variations[i]);
                    text += writeVariation();
                }
                if(hasNext && m_options.getOptionAsBool("ColumnStyle"))
                {
                    text += m_startTagMap[MarkupColumnStyleMainline];
                }
            }
            m_dirtyBlack = true;
            m_game.dbMoveToId(m_game.parentMove());
        }
        m_game.forward();
    } while(!m_game.atLineEnd());

    return text;
}

QString Output::writeVariation()
{
    QString text;
    m_currentVariationLevel++;
    bool indent = (m_currentVariationLevel < m_options.getOptionAsInt("VariationIndentLevel"));
    bool willStartIndent = (m_currentVariationLevel+1 == m_options.getOptionAsInt("VariationIndentLevel"));

    if (indent) text += m_startTagMap[MarkupVariationResume];
    text += m_startTagMap[indent ? (willStartIndent ? MarkupVariationIndent : MarkupVariationIndent1) : MarkupVariationInline];
    m_dirtyBlack = true;

    text += writeMove(PreviousMove);

    bool mustAddStart = false;

    while(!m_game.atLineEnd())
    {
        if (mustAddStart)
        {
            mustAddStart = false;
            if (indent) text += m_startTagMap[MarkupVariationResume];
        }
        // *** Writes move in the current variation
        text += writeMove();
        if(m_game.variationCount())
        {
            if (indent && !willStartIndent) text += m_endTagMap[MarkupVariationResume];
            QList<MoveId> variations = m_game.variations();
            if(!variations.empty())
            {
                for(int i = 0; i < variations.size(); ++i)
                {
                    // *** Enter variation i, and write the rest of the moves
                    if (m_game.dbMoveToId(variations[i]))
                    {
                        text += writeVariation();
                    }
                }
                mustAddStart = true;
            }
            m_dirtyBlack = true;
            m_game.dbMoveToId(m_game.parentMove());
        }
        m_game.forward();
    }

    text += m_endTagMap[indent ? (willStartIndent ? MarkupVariationIndent : MarkupVariationIndent1) : MarkupVariationInline];
    if (indent) text += m_endTagMap[MarkupVariationResume];

    m_currentVariationLevel--;
    return text;
}

QString Output::writeTag(const QString& tagName, const QString& tagValue) const
{
    QString text = m_startTagMap[MarkupHeaderLine] +
                   m_startTagMap[MarkupHeaderTagName] +
                   tagName + m_endTagMap[MarkupHeaderTagName] +
                   " " +
                   m_startTagMap[MarkupHeaderTagValue] +
                   tagValue +
                   m_endTagMap[MarkupHeaderTagValue] +
                   m_endTagMap[MarkupHeaderLine];
    return text;
}

QString Output::writeComment(const QString& comment, const QString& mvno, CommentType type)
{
    QString text;
    if(comment.isEmpty())
    {
        return text;
    }

    MarkupType markupIndent = type == Comment ? MarkupAnnotationIndent : MarkupPreAnnotationIndent;
    MarkupType markupInline = type == Comment ? MarkupAnnotationInline : MarkupPreAnnotationInline;

    if(m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 0))
    {
        text += m_endTagMap[MarkupColumnStyleMainline];
    }
    if((m_options.getOptionAsString("CommentIndent") == "Always")
            || ((m_options.getOptionAsString("CommentIndent") == "OnlyMainline")
                && (m_currentVariationLevel == 0)))
    {
        if(m_expandable[markupIndent])
        {
            text += m_startTagMap[markupIndent].arg(mvno) +
                    comment + m_endTagMap[markupIndent];
        }
        else
        {
            text += m_startTagMap[markupIndent] +
                    comment + m_endTagMap[markupIndent];
        }
    }
    else
    {
        if(m_expandable[markupInline])
        {
            text += m_startTagMap[markupInline].arg(mvno) +
                    comment + m_endTagMap[markupInline];
        }
        else
        {
            text += m_startTagMap[markupInline] +
                    comment + m_endTagMap[markupInline];
        }
    }
    if(m_options.getOptionAsBool("ColumnStyle") && (m_currentVariationLevel == 0))
    {
        text += m_startTagMap[MarkupColumnStyleMainline];
    }
    m_dirtyBlack = true;
    return text;
}

QString Output::writeGameComment(QString comment) const
{
    QString text;
    comment = comment.trimmed();
    if(comment.isEmpty())
    {
        return text;
    }

    MarkupType markupIndent = MarkupPreAnnotationIndent;
    MarkupType markupInline = MarkupPreAnnotationInline;

    if(m_options.getOptionAsBool("ColumnStyle"))
    {
        text += m_endTagMap[MarkupColumnStyleMainline];
    }
    if((m_options.getOptionAsString("CommentIndent") == "Always")
            || ((m_options.getOptionAsString("CommentIndent") == "OnlyMainline")))
    {

        text += m_startTagMap[markupIndent] +
                comment + m_endTagMap[markupIndent];
    }
    else
    {
        text += m_startTagMap[markupInline] +
                comment + m_endTagMap[markupInline];
    }
    if(m_options.getOptionAsBool("ColumnStyle"))
    {
        text += m_startTagMap[MarkupColumnStyleMainline];
    }
    return text;
}

QString Output::writeAllTags() const
{
    QString text;
    TagMap tags = m_game.tags();
    // write standard tags
    for(int i = 0; i < 7; ++i)
    {
        text += writeTag(StandardTags[i], tags[StandardTags[i]]);
        tags.remove(StandardTags[i]);
    }

    QList<QString> keys = tags.keys();
    std::sort(keys.begin(), keys.end());

    // write other tags written in ascii order, as suggested by standard
    foreach(QString key,keys)
    {
        QString value = tags.value(key);
        // workaround for problems with IndexItem implementation
        if(!value.isEmpty() && value != "?" && value != PDInvalidDate.asString() && key != TagNameLength)
        {
            text += writeTag(key, value);
        }
    }
    return text;
}

QString Output::writeBasicTagsHTML() const
{
    QString text;
    TagMap tags = m_game.tags();

    QString eco = tags[TagNameECO].left(3);
    if(eco == "?")
    {
        eco.clear();
    }
    else
    {
        eco = QString(" ") + eco;
    }

    QString whiteElo = tags[TagNameWhiteElo].length() > 1 ? QString(" (%1)").arg(tags[TagNameWhiteElo]) : QString();
    QString blackElo = tags[TagNameBlackElo].length() > 1 ? QString(" (%1)").arg(tags[TagNameBlackElo]) : QString();

    text += m_startTagMap[MarkupHeaderLine] +
            tags[TagNameWhite] + whiteElo + " - " + tags[TagNameBlack] + blackElo + eco +
            m_endTagMap[MarkupHeaderLine] + "\n";

    QString event = tags[TagNameEvent] != "?" ? QString("%1").arg(tags[TagNameEvent]) : QString();
    QString place = tags[TagNameSite]  != "?" ? QString("%1").arg(tags[TagNameSite]) : QString();
    QString round = tags[TagNameRound] != "?" ? QString(" (%1)").arg(tags[TagNameRound]) : QString();

    if(!(QString(event + place + round)).isEmpty())
        text += m_startTagMap[MarkupHeaderLine] +
                event +
                ((!event.isEmpty() && !place.isEmpty()) ? ", " : "") + place +
                round +
                m_endTagMap[MarkupHeaderLine] + "\n";
    return text;
}

QString Output::output(const Game* game, bool upToCurrentMove)
{
    QString text = m_header;
    postProcessOutput(text);

    text += outputTags(game);

    QString gameText = outputGame(game, upToCurrentMove);
    postProcessOutput(gameText);
    text += gameText;

    QString footer = m_footer;
    postProcessOutput(footer);
    text += footer;

    return text;
}

QString Output::outputTags(const Game* game)
{
    QString text;
    m_game = *game;
    if(m_options.getOptionAsBool("ShowHeader"))
    {
        text += m_startTagMap[MarkupHeaderBlock];
        if(m_outputType == Html)
        {
            text += writeBasicTagsHTML();
        }
        else
        {
            text += writeAllTags();
        }
        text += m_endTagMap[MarkupHeaderBlock];
    }
    return text;
}

QString Output::outputGame(const Game* g, bool upToCurrentMove)
{
    QString text;
    m_game = *g;
    int id = m_game.currentMove();
    int mainId = upToCurrentMove ? m_game.mainLineMove() : NO_MOVE;
    m_currentVariationLevel = 0;

    m_game.moveToStart();
    m_dirtyBlack = m_game.board().toMove() == Black;
    text += m_startTagMap[MarkupNotationBlock];
    text += m_startTagMap[MarkupMainLine];
    if(m_options.getOptionAsBool("ColumnStyle"))
    {
        text += m_startTagMap[MarkupColumnStyleMainline];
    }

    QString gameComment = (m_outputType == Pgn) ? m_game.annotation(0) : m_game.textAnnotation(0);
    text += writeGameComment(gameComment);

    text += writeMainLine(mainId);
    if(m_options.getOptionAsBool("ColumnStyle"))
    {
        text += m_endTagMap[MarkupColumnStyleMainline];
    }
    text += m_endTagMap[MarkupMainLine];
    text += m_endTagMap[MarkupNotationBlock];
    text += m_startTagMap[MarkupResult] + m_game.tag(TagNameResult) + m_endTagMap[MarkupResult];

    m_game.dbMoveToId(id);

    return text;
}

void Output::postProcessOutput(QString& text) const
{
    QRegExp var("@(\\w+)@");
    while(var.indexIn(text) != -1)
    {
        QStringList cap = var.capturedTexts();
        text.replace("@" + cap[1] + "@", m_options.getOptionAsString(cap[1]));
    }

    // Chop it up, if TextWidth option is not equal to 0
    int start = 0;
    int textWidth = m_options.getOptionAsInt("TextWidth");
    if(textWidth)
    {
        int length = text.length() - start;
        while(length > textWidth)
        {
            start = text.lastIndexOf(" ", start + textWidth);
            if(start == -1)
            {
                break;
            }
            text.replace(start, 1, '\n');
            length = text.length() - start;
        }
    }
}

void Output::output(QTextStream& out, Filter& filter)
{
    int percentDone = 0;
    Game game;
    QString header = m_header;
    postProcessOutput(header);
    out << header;

    for(int i = 0; i < filter.count(); ++i)
    {
        if(filter.database()->loadGame(i, game))
        {
            QString tagText = outputTags(&game);
            out << tagText;

            QString outText = outputGame(&game, false);
            postProcessOutput(outText);
            out << outText;
            out << "\n\n";
        }
        int percentDone2 = (i + 1) * 100 / filter.count();
        if(percentDone2 > percentDone)
        {
            emit progress((percentDone = percentDone2));
        }
    }

    QString footer = m_footer;
    postProcessOutput(footer);
    out << footer;
}

void Output::output(QTextStream& out, Database& database)
{
    if(!database.isUtf8() && (m_outputType == Pgn))
    {
        QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1");
        if(textCodec)
        {
            out.setCodec(textCodec);
        }
    }

    QString header = m_header;
    postProcessOutput(header);
    out << header;

    int percentDone = 0;
    Game game;
    for(int i = 0; i < (int)database.count(); ++i)
    {
        if(database.loadGame(i, game))
        {
            QString tagText = outputTags(&game);
            out << tagText;

            QString outText = outputGame(&game, false);
            postProcessOutput(outText);
            out << outText;
            out << "\n\n";
        }
        int percentDone2 = (i + 1) * 100 / database.count();
        if(percentDone2 > percentDone)
        {
            emit progress((percentDone = percentDone2));
        }
    }


    QString footer = m_footer;
    postProcessOutput(footer);
    out << footer;

    database.setModified(false);
}

void Output::output(const QString& filename, const Game& game)
{
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&f);
    if((m_outputType == Html) || (m_outputType == NotationWidget))
    {
        out.setCodec(QTextCodec::codecForName("utf8"));
    }
    out << output(&game);
    f.close();
}

void Output::output(const QString& filename, Filter& filter)
{
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&f);
    if((m_outputType == Html) || (m_outputType == NotationWidget))
    {
        out.setCodec(QTextCodec::codecForName("utf8"));
    }
    output(out, filter);
    f.close();
}

void Output::output(const QString& filename, Database& database)
{
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&f);
    if((m_outputType == Html) || (m_outputType == NotationWidget))
    {
        out.setCodec(QTextCodec::codecForName("utf8"));
    }
    output(out, database);
    f.close();
}

bool Output::append(const QString& filename, Game& game)
{
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return false;
    }
    QTextStream out(&f);
    if((m_outputType == Html) || (m_outputType == NotationWidget))
    {
        out.setCodec(QTextCodec::codecForName("utf8"));
    }
    else
    {
        QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1");
        if(textCodec)
        {
            out.setCodec(textCodec);
        }
    }
    out << endl;
    out << output(&game);
    f.close();
    return true;
}

void Output::append(const QString& filename, Database& database)
{
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&f);
    if((m_outputType == Html) || (m_outputType == NotationWidget))
    {
        out.setCodec(QTextCodec::codecForName("utf8"));
    }
    out << endl;
    output(out, database);
    f.close();
}

void Output::setTemplateFile(QString filename)
{
    if(filename.isEmpty())
    {
        switch(m_outputType)
        {
        case Html:
            filename = DEFAULT_HTML_TEMPLATE;
            break;
        case Latex:
            filename = DEFAULT_LATEX_TEMPLATE;
            break;
        case NotationWidget:
            filename = DEFAULT_NOTATION_TEMPLATE;
            break;
        case Pgn:
            filename = DEFAULT_PGN_TEMPLATE;
            break;
        default :
            qWarning("Could not decide which template file to use. Maybe strange OutputType");
        }
    }

    if(!QFile::exists(filename))
    {
        QString dataPath = AppSettings->dataPath();

        m_templateFilename = dataPath + "/" + TEMPLATE_DIR + "/" + filename;
        if(!QFile::exists(m_templateFilename))
        {
            m_templateFilename = QString(":/") + TEMPLATE_DIR + "/" + filename;
        }
    }
    else
    {
        m_templateFilename = filename;
    }
}

void Output::setMarkupTag(MarkupType type, const QString& startTag, const QString& endTag)
{
    m_startTagMap[type] = startTag;
    m_endTagMap[type] = endTag;
    if(startTag.contains("%1"))
    {
        m_expandable[type] = true;
    }
    else
    {
        m_expandable[type] = false;
    }

}
void Output::markupTag(MarkupType type , QString& startTag, QString& endTag)
{
    startTag = m_startTagMap[type];
    endTag = m_endTagMap[type];
}

bool Output::setOption(const QString& optionName, bool optionValue)
{
    return m_options.setOption(optionName, optionValue);
}
bool Output::setOption(const QString& optionName, int optionValue)
{
    return m_options.setOption(optionName, optionValue);
}
bool Output::setOption(const QString& optionName, const QString& optionValue)
{
    return m_options.setOption(optionName, optionValue);
}

/* Retrieving values */
int Output::getOptionAsInt(const QString& optionName)
{
    return m_options.getOptionAsInt(optionName);
}
QString Output::getOptionAsString(const QString& optionName)
{
    return m_options.getOptionAsString(optionName);
}
bool Output::getOptionAsBool(const QString& optionName)
{
    return m_options.getOptionAsBool(optionName);
}
QString Output::getOptionDescription(const QString& optionName)
{
    return m_options.getOptionDescription(optionName);
}
QStringList Output::getOptionList()
{
    return m_options.getOptionList();
}

