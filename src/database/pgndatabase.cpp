/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2006 Ejner Borgbjerg <ejner@users.sourceforge.net>                *
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QDir>
#include <QStringList>
#include <QtDebug>
#include <QMutexLocker>
#include <QRegularExpression>
#include "board.h"
#include "nag.h"

#include "pgndatabase.h"
#include "settings.h"
#include "tags.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

PgnDatabase::PgnDatabase() : Database()
{
    initialise();
}

PgnDatabase::~PgnDatabase()
{
    close();
}

bool PgnDatabase::open(const QString& filename, bool utf8)
{
    if(!m_file)
    {
        m_break = false;
        m_filename = filename;
        if(openFile(filename))
        {
            m_utf8 = utf8;
            return true;
        }
    }
    return false;
}

void PgnDatabase::parseGame()
{
    skipMoves();
}

bool PgnDatabase::readIndexFile(QDataStream &in, volatile bool* breakFlag, short version)
{
    return (index()->read(in, breakFlag, version));
}

bool PgnDatabase::writeIndexFile(QDataStream& out) const
{
    return (index()->write(out));
}

QString PgnDatabase::offsetFilename(const QString& filename) const
{
    QFileInfo fi = QFileInfo(filename);
    QString basefile = fi.completeBaseName();
    basefile.append(".cxi");
    QString indexPath = AppSettings->indexPath();
    return(indexPath + QDir::separator() + basefile);
}

bool PgnDatabase::hasIndexFile() const
{
    return AppSettings->getValue("/General/useIndexFile").toBool();
}

bool PgnDatabase::readOffsetFile(const QString& filename, volatile bool *breakFlag, bool& bUpdate)
{
    if(!hasIndexFile())
    {
        return false;
    }

    QFile file(offsetFilename(filename));
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream in(&file);

    short version;
    unsigned short magic;

    in >> version;
    in >> magic;

    if (magic != INDEX_FILE_MAGIC) return false;
    if (version > VERSION_INDEX_CURRENT) return false;
    if ((version&0xFF00) != (VERSION_INDEX_CURRENT&0xFF00)) return false;

    int streamVersion;
    in >> streamVersion;

    in.setVersion(streamVersion);
    QFileInfo fi = QFileInfo(filename);

    QString basefile;
    QDateTime lastModified;

    in >> basefile;
    in >> lastModified;

    if(basefile != fi.completeBaseName())
    {
        return false;
    }

    QDateTime lastModifiedStored = fi.lastModified();
    if(lastModified != lastModifiedStored)
    {
        return false;
    }

    in >> m_allocated;
    in >> bUse64bit;

    emit progress(1);

    in >> m_gameOffsets64;
    emit progress(5);
    in >> m_gameOffsets32;
    emit progress(10);

    if (bUse64bit)
    {
        if (m_gameOffsets32.count())
        {
            m_allocated     = 0;
            m_gameOffsets32.clear();
            m_gameOffsets64.clear();
            return false;
        }
    }
    else
    {
        if (m_gameOffsets64.count())
        {
            m_allocated     = 0;
            m_gameOffsets32.clear();
            m_gameOffsets64.clear();
            return false;
        }
    }

    in >> magic;
    if (*breakFlag || (magic != INDEX_FILE_MAGIC)) return false;

    emit progress(20);

    readIndexFile(in, breakFlag, version);
    bUpdate = (version < VERSION_INDEX_CURRENT);

    emit progress(80);

    unsigned short finalMagic;
    in >> finalMagic;
    if(*breakFlag || (finalMagic != 0x55ec))
    {
        m_index.clear();
        m_gameOffsets32.clear();
        m_gameOffsets64.clear();
        m_allocated     = 0;
        return false;
    }

    return true;
}

bool PgnDatabase::writeOffsetFile(const QString& filename) const
{
    if(!hasIndexFile())
    {
        return false;
    }

    QFile file(offsetFilename(filename));
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QDataStream out(&file);

    short version = VERSION_INDEX_CURRENT;
    unsigned short magic = INDEX_FILE_MAGIC;
    int streamVersion = out.version();

    out << version;
    out << magic;
    out << streamVersion;

    QFileInfo fi = QFileInfo(filename);
    QString basefile = fi.completeBaseName();

    out << basefile;
    out << fi.lastModified().toUTC();

    out << m_count;
    out << bUse64bit;
    out << m_gameOffsets64;
    out << m_gameOffsets32;
    out << magic;

    writeIndexFile(out);

    unsigned short finalMagic = 0x55ec;
    out << finalMagic;

    return true;
}

bool PgnDatabase::parseFile()
{
    bool bUpdate = false;
    if(readOffsetFile(m_filename, &m_break, bUpdate))
    {
        m_count = m_allocated;
        emit progress(99);
        if (bUpdate)
        {
            writeOffsetFile(m_filename);
        }
        emit progress(100);
        return true;
    }

    if(m_break)
    {
        return false;
    }

    bool ok = parseFileIntern();
    if (ok)
    {
        writeOffsetFile(m_filename);
    }
    return ok;
}

bool PgnDatabase::parseFileIntern()
{
    //indexing game positions in the file, game contents are ignored
    qint64 size = m_file->size();
    int oldFp = -3;

    qint64 countDiff = size / 100;
    qint64 nextDiff = countDiff;
    percentDone = 0;
    m_index.reserve(size/1000);

    while(!m_file->atEnd() || !m_currentLine.isEmpty())
    {
        if(m_break)
        {
            return false;
        }
        IndexBaseType fp;
        if (!m_file->atEnd())
        {
            fp = skipJunk();
            if(fp == oldFp)
            {
                skipLine();
                fp = skipJunk();
            }
            oldFp = fp;
        }
        else
        {
            fp = m_file->pos() - m_lineBuffer.size();
            if (fp == oldFp)
            {
                break;
            }
            oldFp = fp;
            prepareNextLineForMoveParser();
        }
        if(fp != -1)
        {
            if(!m_currentLine.isEmpty())
            {
                if (!addOffset(fp))
                {
                    m_file->seek(m_file->size());
                }
                else
                {
                    parseTagsIntoIndex(); // This will parse the tags into memory
                    parseGame();
                    if (!m_index.hasTag(TagNameLength, m_count - 1)) m_index.setTag_nolock(TagNameLength, "0", m_count - 1);
                    if (!m_index.hasTag(TagNameResult, m_count - 1)) m_index.setTag_nolock(TagNameResult, "*", m_count - 1);
                }

                if(!m_file->atEnd())
                {
                    if(fp > nextDiff)
                    {
                        nextDiff += countDiff;
                        emit progress(++percentDone);
                    }
                }
                else
                {
                    emit progress(100);
                }
            }
        }
    }
    m_gameOffsets32.squeeze();
    m_gameOffsets64.squeeze();
    m_index.squeeze();
    return true;
}

bool PgnDatabase::openFile(const QString& filename)
{
    //open file
    QFile* file = new QFile(filename);
    if(!file->exists())
    {
        delete file;
        return false;
    }
    file->open(QIODevice::ReadOnly);
    m_file = file;
    return true;
}

bool PgnDatabase::openString(const QString& content)
{
    //open file
    initialise();
    m_filename = "Internal.pgn";
    QByteArray byteArray;
    byteArray.append(content.toLatin1());
    QBuffer* buffer = new QBuffer(&byteArray);
    buffer->open(QIODevice::ReadOnly | QIODevice::Text);
    m_file = buffer;
    m_utf8 = false;
    return parseFile();
}

quint64 PgnDatabase::count() const
{
    return m_count;
}

QString PgnDatabase::filename() const
{
    return m_filename;
}

void PgnDatabase::clear()
{
    initialise();
    Database::clear();
}

void PgnDatabase::close()
{
    //close the file, and delete objects
    while (getReferences())
    {
        QCoreApplication::processEvents();
        QThread::sleep(1);
    }
    if(m_file)
    {
        m_file->close();
    }
    delete m_file;

    //reset member variables
    initialise();
}

void PgnDatabase::loadGameMoves(GameId gameId, GameX& game)
{
    QMutexLocker m(&m_mutex);
    if(!m_file || gameId >= m_count)
    {
        return;
    }
    game.clear();
    m_variationStack.clear();
    seekGame(gameId);
    skipTags();
    QString fen = m_index.tagValue(TagNameFEN, gameId);
    QString variant = m_index.tagValue(TagNameVariant, gameId).toLower();
    bool chess960 = (variant.startsWith("fischer", Qt::CaseInsensitive) || variant.endsWith("960"));
    if(fen != "?")
    {
        game.dbSetStartingBoard(fen, chess960);
    }
    parseMoves(&game);
}

int PgnDatabase::findPosition(GameId index, const BoardX &position)
{
    GameX g;
    loadGameMoves(index, g);
    return g.cursor().findPosition(position);
}

bool PgnDatabase::loadGame(GameId gameId, GameX& game)
{
    if(!m_file || gameId >= m_count)
    {
        return false;
    }
    QMutexLocker m(&m_mutex);
    //parse the game
    game.clear();
    m_variationStack.clear();
    loadGameHeaders(gameId, game);
    seekGame(gameId);
    skipTags();
    QString fen = m_index.tagValue(TagNameFEN, gameId);
    QString variant = m_index.tagValue(TagNameVariant, gameId).toLower();
    bool chess960 = (variant.startsWith("fischer", Qt::CaseInsensitive) || variant.endsWith("960"));
    if(fen != "?")
    {
        game.dbSetStartingBoard(fen, chess960);
    }

    parseMoves(&game);

    return m_variation != -1 || fen != "?";  // Not sure of all of the ramifications of this
    // but it seeems to fix the problem with FENs
}

void PgnDatabase::initialise()
{
    m_file = nullptr;
    m_inComment = false;
    m_inPreComment = false;
    m_filename = QString();
    m_count = 0;
    m_allocated = 0;
}

void PgnDatabase::readLine()
{
    if(m_file->atEnd())
    {
        m_lineBuffer.clear();
        m_currentLine.clear();
        return;
    }
    m_lineBuffer = m_file->readLine();
    prepareNextLineForMoveParser();
}

void PgnDatabase::readTagLine()
{
    m_lineBuffer = m_file->readLine();
    prepareNextLine();
}

void PgnDatabase::skipLine()
{
    m_lineBuffer = m_file->readLine();
}

void PgnDatabase::seekGame(GameId gameId)
{
    IndexBaseType n = offset(gameId);
    if(!m_file->seek(n))
    {
        qDebug() << "Seeking offset " << QString::number(n) << " failed!";
    }
    readLine();
}

void PgnDatabase::parseTagIntoIndex(const QString& tag, QString value)
{
    if(value.contains("\\\""))
    {
        value.replace("\\\"", "\"");
    }

    if(tag == TagNameResult && value == "1/2")
    {
        value = "1/2-1/2";
    }
    m_index.setTag_nolock(tag, value, m_count - 1); // PERF von 30s von 115s (26%)
}

void PgnDatabase::parseTagsIntoIndex()
{
    while(m_currentLine.startsWith(QString("[")))
    {
        int pos = 0;
        int lastPos = -1;
        while(pos != -1)
        {
            int tagStart = m_currentLine.indexOf('[', pos);
            if (tagStart != -1)
            {
                ++tagStart;
                int tagEnd = m_currentLine.indexOf(' ', tagStart);
                if (tagEnd != -1)
                {
                    int valueStart = m_currentLine.indexOf('\"', tagEnd + 1);
                    if (valueStart != -1)
                    {
                        ++valueStart;
                        int valueEnd = m_currentLine.indexOf('\"', valueStart);
                        if (valueEnd != -1)
                        {
                            int tagValueEnd = m_currentLine.indexOf(']', valueEnd + 1);
                            if (tagValueEnd != -1)
                            {
                                lastPos = tagValueEnd+1;
                                pos = m_currentLine.indexOf('[', tagValueEnd);
                                parseTagIntoIndex(m_currentLine.mid(tagStart, tagEnd-tagStart), m_currentLine.mid(valueStart, valueEnd-valueStart));
                                continue;
                            }
                        }
                    }
                }
                else
                {
                    int tagValueEnd = m_currentLine.indexOf(']', tagStart);
                    if (tagValueEnd != -1)
                    {
                        lastPos = tagValueEnd+1; // This line has no valid tag format
                    }
                }
            }
            break;
        }

        m_currentLine = m_currentLine.mid(lastPos);

        if (!m_currentLine.isEmpty())
        {
            pos = m_currentLine.indexOf("[");
            if (pos != -1)
            {
                QString remainder = m_currentLine.mid(pos);
                if (m_file->atEnd()) { m_currentLine = remainder; break; }
                readTagLine();
                m_currentLine.prepend(remainder); // TODO - Problem hier ist, dass der m_lineBuffer leer ist bei m_file->atEnd() und dann der remainder nicht richtig geparst wird
            }
        }
        else
        {
            if (m_file->atEnd()) break;
            readTagLine();
        }
    }

    // skip empty lines
    while(m_currentLine.isEmpty() && !m_file->atEnd())
    {
        readLine();
    }
}

bool PgnDatabase::parseMoves(GameX* game)
{
    m_gameOver = false;
    m_inComment = false;
    m_inPreComment = false;
    m_comment.clear();
    m_precomment.clear();
    m_newVariation = false;
    m_variation = 0;

    do
    {
        if(m_inComment)
        {
            parseComment(game);
        }
        else
        {
            parseLine(game);
            if(m_variation == -1)
            {
                return false;
            }
        }
    }
    while(!m_gameOver && (!m_file->atEnd() || m_currentLine != ""));

    if(m_gameOver)
    {
        if(game->plyCount() == 0)
        {
            if(!m_precomment.isEmpty())
            {
                game->dbSetAnnotation(m_precomment);
                m_precomment.clear();
                m_inPreComment = false;
            }
        }
        if (game->needsCleanup())
        {
            game->clearDummyNodes();
        }
    }
    return true;
}

char PgnDatabase::peek(QStringRef::const_iterator s)
{
    QStringRef::const_iterator n = s+1;
    if (n != m_currentLine.constEnd())
    {
        return (*n).toLatin1();
    }
    return 0;
}

void PgnDatabase::splitTokenList(QVector<QStringRef>& list)
{
    auto s = m_currentLine.constBegin();
    int start = 0;
    int n = 0;
    bool inNag = false;
    int dots = 0;
    bool breakout = false;

    while (!breakout && (s != m_currentLine.constEnd()))
    {
        n++;
        char c = (*s).toLatin1();
        switch (c)
        {
            case ' ': // WS - Separator of tokens
            case '\t':
            if (n>1)
            {
                list.push_back(QStringRef(&m_currentLine, start, n-1));
                start += n;
                n = 0;
            }
            else
            {
                start++;
                n=0;
            }
            dots = 0;
            inNag = false;
            break;

            case '.': // Count dots to figure out which side is moving.
            if (!dots)
            {
                if (m_currentLine[start].isDigit())
                {
                    list.push_back(QStringRef(&m_currentLine, start, n-1)); // Current move counter
                }
                start += (n-1);
                n = 1;
            }
            dots++;
            break;

            case '-': // Can be 0-0, -+, a2-a4 (LAN), 1-0, 0-1, 1/2-1/2, Q-a4,
            if (!inNag)
            {
                if (n>1)
                {
                    QStringRef t(&m_currentLine, start, n-1);
                    QChar c = t.at(0);
                    if (!c.isLetterOrNumber() && (c!='.'))
                    {
                        // It's a token, not part of a move
                        list.push_back(t);
                        start += (n-1);
                        n = 1;
                        inNag = true;
                    }
                }
                else
                {
                   inNag = true;
                }
            }
            break;

            case '=': // Avoid b8=Q to be cut in two token
            if (!inNag && !isalpha(peek(s)))
            {
                if (n>1) list.push_back(QStringRef(&m_currentLine, start, n-1));
                start += (n-1);
                n = 1;
                inNag = true;
            }
            break;

            case 0:
            {
                Nag nag = NagSet::fromString(*s);
                if (nag != NullNag)
                {
                   if (n>1) list.push_back(QStringRef(&m_currentLine, start, n-1));
                   start += n-1;
                   list.push_back(QStringRef(&m_currentLine, start, 1));
                   start += 1;
                   n = 0;
                }
                else
                {
                    m_variation = -1; // Illegal character -> Skip parsing this game
                }
            }
            break;

            case '!': // Cut a5! / a5 ! into two token, and a5!! is still two tokens
            case '?':
            case '+': // Watch out for 0-0+
            case '$':
            if (!inNag)
            {
                if (n>1) list.push_back(QStringRef(&m_currentLine, start, n-1));
                start += (n-1);
                n = 1;
                inNag = true;
            }
            break;

            case '{':
            {
                if (n>1) list.push_back(QStringRef(&m_currentLine, start, n-1));
                list.push_back(QStringRef(&m_currentLine, start+n-1, 1));
                start += n;
                n = 0;
                breakout = true;
                break; // Let someone else parse the comments
            }

            case '(': // Actually b8(Q) for b8=Q would be an issue here
            case ')':
            case '}':
            {
                if (n>1) list.push_back(QStringRef(&m_currentLine, start, n-1));
                list.push_back(QStringRef(&m_currentLine, start+n-1, 1));
                start += n;
                n = 0;
                dots = 0;
                inNag = false;
            }
            break;

            default:
            if (c<0)
            {
                Nag nag = NagSet::fromString(*s);
                if (nag != NullNag)
                {
                   if (n>1) list.push_back(QStringRef(&m_currentLine, start, n-1));
                   start += n-1;
                   list.push_back(QStringRef(&m_currentLine, start, 1));
                   start += 1;
                   n = 0;
                }
                else
                {
                    m_variation = -1; // Illegal character -> Skip parsing this game
                }
            }
            break;
        }
        s++;
    }
    if (n)
    {
        list.push_back(QStringRef(&m_currentLine, start, n));
    }
}

void PgnDatabase::parseLine(GameX* game)
{
    QVector<QStringRef> list;
    splitTokenList(list);
    if(m_variation != -1) for(auto it = list.begin(); it != list.end() && !m_inComment; ++it)
    {
        parseToken(game, *it);
        if(m_variation == -1)
        {
            if(!(m_currentLine.startsWith("[")))
            {
                skipLine(); // illegal move in the buffer!
            }
            return;
        }
    }

    if(!m_inComment)
    {
        readLine();
    }
    else
    {
        m_currentLine = m_currentLine.mid(m_currentLine.indexOf("{") + 1); // Implicit assumption that there is no other '{' in the line
    }
}

inline void PgnDatabase::parseMoveToken(GameX* game, QString token)
{
    QChar c = token.at(0);
    if (c.isDigit())
    {
        moveNumberFound = token.toInt();
        return;
    }

    if (token.startsWith("..."))
    {
        white = false;
        dotFound = true;
        token.remove(0,3);
    }
    else if (token.startsWith("."))
    {
        white = true;
        dotFound = true;
        token.remove(0,1);
    }

    if (token.isEmpty()) return;

    if(m_newVariation)
    {
        bool dummyNeeded = dotFound && (((white && game->board().whiteToMove()) ||
                                         (!white && game->board().blackToMove())));
        if (dummyNeeded)
        {
            if (!game->move(game->currentMove()).isDummyMove())
            {
                game->dbAddMove(game->board().dummyMove());
                game->forward();
                game->setNeedsCleanup(true);
            }
        }
        game->backward();
        m_variation = game->dbAddSanVariation(token, QString());
        if(!m_precomment.isEmpty())
        {
            game->dbSetAnnotation(m_precomment, m_variation, GameX::BeforeMove);
            m_precomment.clear();
            m_inPreComment = false;
        }
        m_newVariation = false;
    }
    else
    {
        m_variation = game->dbAddSanMove(token, QString());

        if(!m_precomment.isEmpty())
        {
            game->dbSetAnnotation(m_precomment, m_variation, GameX::BeforeMove);
            m_precomment.clear();
            m_inPreComment = false;
        }
    }

    if (dotFound && AppSettings->getValue("/General/strictMoveCounter").toBool() && ( m_variation != NO_MOVE ))
    {
        int currentMoveNumber = game->moveNumber();
        if (currentMoveNumber != moveNumberFound)
        {
            game->removeNode();
            m_variation = NO_MOVE;
        }
    }

    dotFound = false;
}

void PgnDatabase::parseToken(GameX* game, const QStringRef& token)
{
    if (token.isEmpty()) return;
    // qDebug() << "Parsing Token:" << token << ":";
    char c = token.at(0).toLatin1();
    switch(c)
    {
    case 0:
        {
            Nag nag = NagSet::fromString(token.at(0));
            game->dbAddNag(nag);
        }
        break;
    case '(':
        {
            m_newVariation = true;
            m_variationStack.push(game->currentMove());
        }
        break;
    case ')':
        MoveId move;
        if (!m_variationStack.isEmpty())
        {
            move = m_variationStack.pop();
        }
        else
        {
            move = game->parentMove();
        }
        game->dbMoveToId(move);
        game->forward();
        m_newVariation = false;
        m_variation = 0;
        break;
    case '{':
        m_comment.clear();
        m_inComment = true;
        break;
    case '$':
        if (token.length()>1)
        {
            game->dbAddNag((Nag)token.mid(1).toInt());
        }
        break;
    case '!':
        if(token == "!")
        {
            game->dbAddNag(GoodMove);
        }
        else if(token == "!!")
        {
            game->dbAddNag(VeryGoodMove);
        }
        else if(token == "!?")
        {
            game->dbAddNag(SpeculativeMove);
        }
        break;
    case '?':
        if(token == "?")
        {
            game->dbAddNag(PoorMove);
        }
        else if(token == "??")
        {
            game->dbAddNag(VeryPoorMove);
        }
        else if(token == "?!")
        {
            game->dbAddNag(QuestionableMove);
        }
        break;
    case '+':
        if(token == "+=")
        {
            game->dbAddNag(WhiteHasASlightAdvantage);
        }
        else if(token == "+/-")
        {
            game->dbAddNag(WhiteHasAModerateAdvantage);
        }
        break;
    case '=':
        if(token == "=")
        {
            game->dbAddNag(DrawishPosition);
        }
        else if(token == "=+")
        {
            game->dbAddNag(BlackHasASlightAdvantage);
        }
        break;

    case '*':
        game->dbSetResult(ResultUnknown);
        m_gameOver = true;
        break;

    case '1':
        if(token == "1-0")
        {
            game->dbSetResult(WhiteWin);
            m_gameOver = true;
            break;
        }
        else if(token == "1/2-1/2" || token == "1/2")
        {
            game->dbSetResult(Draw);
            m_gameOver = true;
            break;
        }
        parseMoveToken(game, token.toString());
        break;

    case '0':
        if(token == "0-1")
        {
            game->dbSetResult(BlackWin);
            m_gameOver = true;
            break;
        }
        parseMoveToken(game, token.toString());
        break;

    case 'Z':
    case '-':
        if(token == "-/+")
        {
            game->dbAddNag(BlackHasAModerateAdvantage);
            break;
        }
        parseMoveToken(game, token.toString());
        break;

    default:
        if (c<0)
        {
            Nag nag = NagSet::fromString(token.at(0));
            game->dbAddNag(nag);
        }
        else
        {
            parseMoveToken(game, token.toString());
        }
        break;
    }
}

void PgnDatabase::parseComment(GameX* game)
{
    int end = m_currentLine.indexOf('}');

    if(end >= 0)
    {
        m_comment.append(m_currentLine.left(end));
        m_inComment = false;
        if(m_newVariation || game->plyCount() == 0)
        {
            if (game->plyCount()==0 && m_inPreComment)
            {
                game->dbSetAnnotation(m_precomment, 0);
                m_inPreComment = false;
            }
            else
            {
                m_precomment = m_comment.trimmed();
                m_inPreComment = true;
            }
        }
        else
        {
            QString currentComment = game->annotation();
            if (!currentComment.isEmpty())
            {
                currentComment.append("\n");
                m_comment.prepend(currentComment);
            }
            game->dbSetAnnotation(m_comment.trimmed());
        }
        m_currentLine = m_currentLine.right((m_currentLine.length() - end) - 1);
    }
    else
    {
        m_comment.append(m_currentLine + ' ');
        readLine();
    }
}

inline bool onlyWhitespace(const QByteArray& b)
{
    for(int i = 0; i < b.length(); ++i)
    {
        if(!isspace(b[i]))
        {
            return false;
        }
    }
    return true;
}

void PgnDatabase::prepareNextLine()
{
    if(m_utf8)
    {
        QTextStream textStream(m_lineBuffer);
#if QT_VERSION < 0x060000
        textStream.setCodec("UTF-8");
#endif
        m_currentLine = textStream.readLine().simplified();
    }
    else
    {
		m_currentLine = QString::fromLatin1(m_lineBuffer).simplified();
    }
}

void PgnDatabase::prepareNextLineForMoveParser()
{
    prepareNextLine();
}

IndexBaseType PgnDatabase::skipJunk()
{
    IndexBaseType fp = -2;
    if(m_file->atEnd())
    {
        fp = -1;
    }

    while((!m_lineBuffer.length()
            || (m_lineBuffer[0] != '[' && !QChar::isNumber(m_lineBuffer[0])))
            && !m_file->atEnd())
    {
        fp = m_file->pos();
        skipLine();
    }

    if(fp == -2)
    {
        fp = m_file->pos() - m_lineBuffer.size();
    }

    prepareNextLineForMoveParser();

    return fp;
}

void PgnDatabase::skipTags()
{
    while(m_lineBuffer.length() && (m_lineBuffer[0] == '[') && !m_file->atEnd())
    {
        skipLine();
    }

    //swallow trailing whitespace
    while(onlyWhitespace(m_lineBuffer) && !m_file->atEnd())
    {
        skipLine();
    }

    prepareNextLineForMoveParser();
}

void PgnDatabase::skipMoves()
{
    QString tag = m_index.tagValue(TagNamePlyCount, m_count - 1);
    if(tag == "?")
    {
        tag.clear();
    }
    if(!tag.isEmpty())
    {
        while(!onlyWhitespace(m_lineBuffer) && !m_file->atEnd())
        {
            skipLine();
        }

        tag = QString::number((tag.toInt() + 1) / 2);
        m_index.setTag_nolock(TagNameLength, tag, m_count - 1);
    }
    else
    {
        // PERF Costs about 5s in 157s for the ref database (3.1%)
        QRegularExpression gameNumber("\\s(\\d+)\\s*\\.");

        QString gameText = " ";

        while(!onlyWhitespace(m_lineBuffer) && !m_file->atEnd())
        {
            gameText += QString(m_lineBuffer) + " ";
            skipLine();
        }

        gameText = gameText.remove(QRegularExpression("\\([^\\(\\)]*\\)"));

        QRegularExpressionMatch match;
        if (gameText.lastIndexOf(gameNumber, 0, &match) >= 0)
        {
            m_index.setTag_nolock(TagNameLength, match.captured(1), m_count - 1);
        }
    }

    //swallow trailing whitespace
    while(onlyWhitespace(m_lineBuffer) && !m_file->atEnd())
    {
        skipLine();
    }

    if (m_file->atEnd())
    {
        m_lineBuffer.clear();
    }

    prepareNextLine();
}

//offset methods
/** Returns the file offset for the given game */
IndexBaseType PgnDatabase::offset(GameId gameId) const
{
    if(bUse64bit)
    {
        return m_gameOffsets64.at(gameId);
    }
    else
    {
        return m_gameOffsets32.at(gameId);
    }
}

bool PgnDatabase::get64bit() const
{
    return bUse64bit;
}

void PgnDatabase::set64bit(bool value)
{
    bUse64bit = value;
}

#define BASE_ALLOC_SIZE 0x10000
/** Adds a new file offset */
bool PgnDatabase::addOffset(IndexBaseType offset)
{
    if(m_count >= m_allocated)
    {
        int AllocationSize;
        if (percentDone)
        {
            AllocationSize = m_allocated * (100.0/percentDone) + BASE_ALLOC_SIZE;
        }
        else
        {
            AllocationSize = BASE_ALLOC_SIZE;
        }
        m_allocated = AllocationSize;
        //out of space reallocate memory
        if(bUse64bit)
        {
            m_gameOffsets64.resize(m_allocated);
        }
        else
        {
            m_gameOffsets32.resize(m_allocated);
        }
    }

    if(bUse64bit)
    {
        m_gameOffsets64[m_count] = offset;
    }
    else
    {
        m_gameOffsets32[m_count] = offset;
    }
    ++m_count;
    return true;
}

