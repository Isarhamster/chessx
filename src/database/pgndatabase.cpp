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

#include "board.h"
#include "nag.h"

#include "pgndatabase.h"
#include "settings.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

PgnDatabase::PgnDatabase(bool b64bit) :
    Database(),
    bUse64bit(b64bit)
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

    m_gameOffsets32 = 0;
    m_gameOffsets64 = 0;

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

    int streamVersion = QDataStream::Qt_4_6;
    if(version > 0)
    {
        in >> streamVersion;
    }

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

    emit progress(1);

    if(bUse64bit)
    {
        m_gameOffsets64 = new qint64[m_allocated];
        for(int i = 0; i < m_allocated; ++i)
        {
            in >> m_gameOffsets64[i];
        }
    }
    else
    {
        m_gameOffsets32 = new qint32[m_allocated];
        for(int i = 0; i < m_allocated; ++i)
        {
            in >> m_gameOffsets32[i];
        }
    }

    emit progress(2);

    in >> magic;

    readIndexFile(in, breakFlag, version);
    bUpdate = (version < VERSION_INDEX_CURRENT);

    unsigned short finalMagic;
    in >> finalMagic;
    if(*breakFlag || (finalMagic != 0x55ec))
    {
        delete[] m_gameOffsets32;
        delete[] m_gameOffsets64;
        m_index.clear();
        m_gameOffsets32 = 0;
        m_gameOffsets64 = 0;
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

    if(bUse64bit)
    {
        for(int i = 0; i < m_count; ++i)
        {
            out << m_gameOffsets64[i];
        }
    }
    else
    {
        for(int i = 0; i < m_count; ++i)
        {
            out << m_gameOffsets32[i];
        }
    }

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

    writeOffsetFile(m_filename);
    return ok;
}

bool PgnDatabase::parseFileIntern()
{
    //indexing game positions in the file, game contents are ignored
    qint64 size = m_file->size();
    int oldFp = -3;

    qint64 countDiff = size / 100;
    qint64 nextDiff = countDiff;
    int percentDone = 0;

    while(!m_file->atEnd())
    {
        if(m_break)
        {
            return false;
        }
        IndexBaseType fp = skipJunk();
        if(fp == oldFp)
        {
            skipLine();
            fp = skipJunk();
        }
        oldFp = fp;
        if(fp != -1)
        {
            if(!m_currentLine.isEmpty())
            {
                addOffset(fp);
                parseTagsIntoIndex(); // This will parse the tags into memory
                parseGame();
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

void PgnDatabase::close()
{
    //close the file, and delete objects
    QMutexLocker m(getAccessMutex());

    if(m_file)
    {
        m_file->close();
    }
    delete m_file;
    m_file = 0;
    delete[] m_gameOffsets64;
    delete[] m_gameOffsets32;

    //reset member variables
    initialise();
}

void PgnDatabase::loadGameMoves(GameId gameId, Game& game)
{
    QMutexLocker m(&m_mutex);
    if(!m_file || gameId >= m_count)
    {
        return;
    }
    game.clear();
    seekGame(gameId);
    skipTags();
    QString fen = m_index.tagValue(TagNameFEN, gameId);
    QString variant = m_index.tagValue(TagNameVariant, gameId).toLower();
    bool chess960 = (variant.startsWith("fischer") || variant.endsWith("960"));
    if(fen != "?")
    {
        game.dbSetStartingBoard(fen, chess960);
    }
    parseMoves(&game);
}

bool PgnDatabase::loadGame(GameId gameId, Game& game)
{
    if(!m_file || gameId >= m_count)
    {
        return false;
    }
    QMutexLocker m(&m_mutex);
    //parse the game
    game.clear();
    loadGameHeaders(gameId, game);
    seekGame(gameId);
    skipTags();
    QString fen = m_index.tagValue(TagNameFEN, gameId);
    QString variant = m_index.tagValue(TagNameVariant, gameId).toLower();
    bool chess960 = (variant.startsWith("fischer") || variant.endsWith("960"));
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
    m_file = 0;
    m_gameOffsets64 = 0;
    m_gameOffsets32 = 0;
    m_inComment = false;
    m_inPreComment = false;
    m_filename = QString();
    m_count = 0;
    m_allocated = 0;
}

void PgnDatabase::addOffset()
{
    IndexBaseType fp = m_file->pos();
    addOffset(fp);
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
    if(m_utf8)
    {
        QTextStream textStream(m_lineBuffer);
        m_currentLine = textStream.readLine().simplified();
    }
    else
    {
        QTextStream textStream(m_lineBuffer);
        QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1");
        if(textCodec)
        {
            textStream.setCodec(textCodec);
        }
        m_currentLine = textStream.readLine().simplified();
    }

    if(m_inComment || !m_currentLine.startsWith("["))
    {
        m_currentLine.replace("(", " ( ");
        m_currentLine.replace(")", " ) ");
        m_currentLine.replace("{", " { ");
        m_currentLine.replace("}", " } ");
        m_currentLine.replace("$", " $");
    }
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
        qDebug() << "Seeking offset " << n << " failed!";
    }
    readLine();
}

void PgnDatabase::parseTagsIntoIndex()
{
    m_index.setTag(TagNameLength, "0", m_count - 1);
    m_index.setTag(TagNameResult, "*", m_count - 1);
    while(m_currentLine.startsWith(QString("[")) && !m_file->atEnd())
    {
        int tagend = m_currentLine.indexOf(' ');
        QString tag = m_currentLine.mid(1, tagend - 1);
        int valuestart = m_currentLine.indexOf('\"', tagend + 1);
        QString value = m_currentLine.mid(valuestart + 1);
        bool hasNextTag = false;

        while(!value.endsWith("]") && !m_file->atEnd())
        {
            readLine();
            if(m_currentLine.isEmpty() || m_currentLine.startsWith("["))
            {
                hasNextTag = true;
                break;
            }
            value += ' ' + m_currentLine;
        }
        int valueend = value.lastIndexOf('\"');
        if(valueend != -1)
        {
            value.truncate(valueend);
        }
        if(value.contains("\\\""))
        {
            value.replace("\\\"", "\"");
        }

        // quick fix for non-standard draw mark.
        if(tag == TagNameResult && value == "1/2")
        {
            value = "1/2-1/2";
        }

        // update index
        m_index.setTag(tag, value, m_count - 1);

        if(!hasNextTag)
        {
            readLine();
        }
    }

    // skip trailing whitespace
    while(m_currentLine.isEmpty() && !m_file->atEnd())
    {
        readLine();
    }
}

bool PgnDatabase::parseMoves(Game* game)
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
    }
    return true;
}

void PgnDatabase::parseLine(Game* game)
{
    QStringList list = m_currentLine.split(" ", QString::SkipEmptyParts);

    for(QStringList::Iterator it = list.begin(); it != list.end() && !m_inComment; ++it)
    {
        if(*it != "")
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
    }

    if(!m_inComment)
    {
        readLine();
    }
}

inline void PgnDatabase::parseDefaultToken(Game* game, QString token)
{
    //strip any move numbers
    if(token.contains("..."))
    {
        token = token.section("...", 1, 1);
    }
    else if(token.contains('.'))
    {
        token = token.section('.',	1, 1);
    }
    else if(token.indexOf(QRegExp("[1-9]")) == 0)
    {
        token.clear();
    }

    //look for nags
    Nag nag = NullNag;
    if(token.endsWith("!"))
    {
        if(token.endsWith("!!"))
        {
            nag = VeryGoodMove;
        }
        else if(token.endsWith("?!"))
        {
            nag = QuestionableMove;
        }
        else
        {
            nag = GoodMove;
        }
        token = token.section('!',	0, 0);
    }
    else if(token.endsWith("?"))
    {
        if(token.endsWith("??"))
        {
            nag = VeryPoorMove;
        }
        else if(token.endsWith("!?"))
        {
            nag = SpeculativeMove;
        }
        else
        {
            nag = PoorMove;
        }
        token = token.section('?',	0, 0);
    }

    if(!token.isEmpty())
    {
        if(m_newVariation)
        {
            game->backward();
            m_variation = game->dbAddVariation(token, QString(), nag);
            if(!m_precomment.isEmpty())
            {
                game->dbSetAnnotation(m_precomment, m_variation, Game::BeforeMove);
                m_precomment.clear();
                m_inPreComment = false;
            }
            m_newVariation = false;
        }
        else  	// First move in the game
        {
            m_variation = game->dbAddMove(token, QString(), nag);
            if(!m_precomment.isEmpty())
            {
                game->dbSetAnnotation(m_precomment, m_variation, Game::BeforeMove);
                m_precomment.clear();
                m_inPreComment = false;
            }
        }
    }
}

void PgnDatabase::parseToken(Game* game, const QString& token)
{
    // qDebug() << "Parsing Token:" << token << ":";
    switch(token.at(0).toLatin1())
    {
    case '(':
        m_newVariation = true;
        break;
    case ')':
        game->dbMoveToId(game->parentMove());
        game->forward();
        m_newVariation = false;
        m_variation = 0;
        break;
    case '{':
        m_comment.clear();
        m_inComment = true;
        m_currentLine = m_currentLine.mid(m_currentLine.indexOf("{") + 1); // Implicit assumption that there is no other '{' in the line
        break;
    case '$':
        game->dbAddNag((Nag)token.mid(1).toInt());
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
        // From here, cases may fall through into default!!
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

    case '0':
        if(token == "0-1")
        {
            game->dbSetResult(BlackWin);
            m_gameOver = true;
            break;
        }

    case '-':
        if(token == "-/+")
        {
            game->dbAddNag(BlackHasAModerateAdvantage);
            break;
        }
        else if(token == "--")
        {
            // parse a null move!
            parseDefaultToken(game, token);
            break;
        }

    default:
        parseDefaultToken(game, token);
        break;
    }
}

void PgnDatabase::parseComment(Game* game)
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
            m_precomment = m_comment.trimmed();
            m_inPreComment = true;
        }
        else
        {
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

inline bool onlyWhite(const QByteArray& b)
{
    for(int i = 0; i < b.length(); ++i)
        if(!isspace(b[i]))
        {
            return false;
        }
    return true;
}

IndexBaseType PgnDatabase::skipJunk()
{
    IndexBaseType fp = -2;
    if(m_file->atEnd())
    {
        fp = -1;
    }

    while((!m_lineBuffer.length()
            || (m_lineBuffer[0] != '[' && m_lineBuffer[0] != '1'))
            && !m_file->atEnd())
    {
        fp = m_file->pos();
        skipLine();
    }

    if(fp == -2)
    {
        fp = m_file->pos() - m_lineBuffer.size();
    }

    if(m_utf8)
    {
        QTextStream textStream(m_lineBuffer);
        m_currentLine = textStream.readLine().simplified();
    }
    else
    {
        QTextStream textStream(m_lineBuffer);
        QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1");
        if(textCodec)
        {
            textStream.setCodec(textCodec);
        }
        m_currentLine = textStream.readLine().simplified();
    }

    if(m_inComment || !m_currentLine.startsWith("["))
    {
        m_currentLine.replace("(", " ( ");
        m_currentLine.replace(")", " ) ");
        m_currentLine.replace("{", " { ");
        m_currentLine.replace("}", " } ");
        m_currentLine.replace("$", " $");
    }

    return fp;
}

void PgnDatabase::skipTags()
{
    while(m_lineBuffer.length() && (m_lineBuffer[0] == '[') && !m_file->atEnd())
    {
        skipLine();
    }

    //swallow trailing whitespace
    while(onlyWhite(m_lineBuffer) && !m_file->atEnd())
    {
        skipLine();
    }

    if(m_utf8)
    {
        QTextStream textStream(m_lineBuffer);
        m_currentLine = textStream.readLine().simplified();
    }
    else
    {
        QTextStream textStream(m_lineBuffer);
        QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1");
        if(textCodec)
        {
            textStream.setCodec(textCodec);
        }
        m_currentLine = textStream.readLine().simplified();
    }

    if(m_inComment || !m_currentLine.startsWith("["))
    {
        m_currentLine.replace("(", " ( ");
        m_currentLine.replace(")", " ) ");
        m_currentLine.replace("{", " { ");
        m_currentLine.replace("}", " } ");
        m_currentLine.replace("$", " $");
    }
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
        while(!onlyWhite(m_lineBuffer) && !m_file->atEnd())
        {
            skipLine();
        }

        tag = QString::number((tag.toInt() + 1) / 2);
        m_index.setTag(TagNameLength, tag, m_count - 1);
    }
    else
    {
        QRegExp gameNumber("\\s(\\d+)\\s*\\.");

        QString gameText = " ";

        while(!onlyWhite(m_lineBuffer) && !m_file->atEnd())
        {
            gameText += QString(m_lineBuffer) + " ";
            skipLine();
        }

        gameText = gameText.remove(QRegExp("\\([^\\(\\)]*\\)"));

        gameNumber.lastIndexIn(gameText);
        m_index.setTag(TagNameLength, gameNumber.cap(1), m_count - 1);
    }

    //swallow trailing whitespace
    while(onlyWhite(m_lineBuffer) && !m_file->atEnd())
    {
        skipLine();
    }

    if(m_utf8)
    {
        QTextStream textStream(m_lineBuffer);
        m_currentLine = textStream.readLine().simplified();
    }
    else
    {
        QTextStream textStream(m_lineBuffer);
        QTextCodec* textCodec = QTextCodec::codecForName("ISO 8859-1");
        if(textCodec)
        {
            textStream.setCodec(textCodec);
        }
        m_currentLine = textStream.readLine().simplified();
    }
}

