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

#include "board.h"
#include "nag.h"

#include "pgndatabase.h"

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
	if (m_isOpen) {
		return false;
	}
    m_break = false;
	m_filename = filename;
    if (openFile(filename)) {
        m_isOpen = true;
        m_utf8 = utf8;
		return true;
	}
	return false;
}

void PgnDatabase::parseGame()
{
    skipMoves();
}

bool PgnDatabase::parseFile()
{
    //indexing game positions in the file, game contents are ignored
	m_index.setCacheEnabled(true);
	int percentDone = 0;
    qint64 size = m_file->size();

    while (!m_file->atEnd())
    {
        if (m_break) return false;
		skipJunk();
        if (m_file->atEnd())
            break;
		addOffset();
        if (!m_currentLine.isEmpty())
        {
            parseTagsIntoIndex(); // This will parse the tags into memory
            parseGame();
            if (m_file->atEnd())
                break;
            int percentDone2 = m_file->pos() * 100 / size;
            if (percentDone2 > percentDone)
            {
               emit progress((percentDone = percentDone2));
            }
        }
    }

	m_index.setCacheEnabled(false);
	return true;
}

bool PgnDatabase::openFile(const QString& filename)
{
	//open file
    QFile* file = new QFile(filename);
    if (!file->exists()) {
        delete file;
		return false;
	}
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    m_file = file;
	return true;
}

bool PgnDatabase::openString(const QString& content)
{
    //open file
    initialise();
    m_filename = "Internal.pgn";
    m_isOpen = true;
    QByteArray byteArray;
    byteArray.append(content);
    QBuffer* buffer = new QBuffer(&byteArray);
    buffer->open(QIODevice::ReadOnly | QIODevice::Text);
    m_file = buffer;
    m_utf8 = false;
    parseFile();
    return true;
}

QString PgnDatabase::filename() const
{
	return m_filename;
}

void PgnDatabase::close()
{
    //close the file, and delete objects
    if (m_file) m_file->close();
	delete m_file;
    m_file = 0;
	delete[] m_gameOffsets;

	//reset member variables
	initialise();
}

void PgnDatabase::loadGameMoves(int index, Game& game)
{
	if (!m_isOpen || index >= m_count)
		return;
	game.clear();
	seekGame(index);
	skipTags();
    QString fen = m_index.tagValue(TagFEN, index); // was m_count -1
	if (fen != "?")
		game.setStartingBoard(fen);
	parseMoves(&game);
}

bool PgnDatabase::loadGame(int index, Game& game)
{
	if (!m_isOpen || index >= m_count) {
		return false;
	}

	//parse the game
	game.clear();
	loadGameHeaders(index, game);
	seekGame(index);
	skipTags();
    QString fen = m_index.tagValue(TagFEN, index ); // was m_count - 1
	if (fen != "?")
		game.setStartingBoard(fen);
    parseMoves(&game);

    return m_variation != -1 || fen != "?";  // Not sure of all of the ramifications of this
                                             // but it seeems to fix the problem with FENs
}

void PgnDatabase::initialise()
{
    m_file = 0;
    m_gameOffsets = 0;
    m_inComment = false;
	m_isOpen = false;
	m_filename = QString();
	m_count = 0;
	m_gameOffsets = 0;
	m_allocated = 0;
}

qint64 PgnDatabase::offset(int index)
{
	return m_gameOffsets[index];
}

void PgnDatabase::addOffset()
{
	qint64 fp = m_file->pos();
	addOffset(fp);
}

void PgnDatabase::addOffset(qint64 offset)
{
	if (m_count == m_allocated) {
		//out of space reallocate memory
		qint32 *newAllocation = new qint32[m_allocated += AllocationSize];
		memcpy(newAllocation, m_gameOffsets, sizeof(qint32) * m_count);
		delete m_gameOffsets;
		m_gameOffsets = newAllocation;
	}

        m_gameOffsets[m_count++] = offset;
}


void PgnDatabase::readLine()
{
	m_lineBuffer = m_file->readLine();
    if (m_utf8)
    {
        QTextStream textStream(m_lineBuffer);
        m_currentLine = textStream.readLine().simplified();
    }
    else
    {
        m_currentLine = m_lineBuffer.simplified();
    }

    if (m_inComment || !m_currentLine.startsWith("[")) {
        m_currentLine.replace("(", " ( ");
        m_currentLine.replace(")", " ) ");
        m_currentLine.replace("{", " { ");
        m_currentLine.replace("}", " } ");
        m_currentLine.replace("$", " $");
    }
}

inline void PgnDatabase::skipLine()
{
    m_lineBuffer = m_file->readLine();
}

void PgnDatabase::seekGame(int index)
{
    m_file->seek(offset(index));
	readLine();
}

void PgnDatabase::parseTagsIntoIndex()
{
    m_index.setTag("Length", "0", m_count - 1);
	while (m_currentLine.startsWith(QString("[")) && !m_file->atEnd()) {
		int tagend = m_currentLine.indexOf(' ');
		QString tag = m_currentLine.mid(1, tagend - 1);
		int valuestart = m_currentLine.indexOf('\"', tagend + 1);
        QString value = m_currentLine.mid(valuestart + 1);
		bool hasNextTag = false;

		while (!value.endsWith("]") && !m_file->atEnd()) {
			readLine();
            if (m_currentLine.isEmpty() || m_currentLine.startsWith("[")) {
				hasNextTag = true;
				break;
			}
			value += ' ' + m_currentLine;
		}
		int valueend = value.lastIndexOf('\"');
		if (valueend != -1)
			value.truncate(valueend);
		if (value.contains("\\\""))
			value.replace("\\\"", "\"");
	
		// quick fix for non-standard draw mark.
		if (tag == "Result" && value == "1/2")
			value = "1/2-1/2";

		// update index
		m_index.setTag(tag, value, m_count - 1);

		if (!hasNextTag)
			readLine();
	}

	// skip trailing whitespace
	while (m_currentLine.isEmpty() && !m_file->atEnd()) 
		readLine();
}

void PgnDatabase::parseMoves(Game* game)
{
    m_gameOver = false;
	m_inComment = false;
	m_comment.clear();
	m_precomment.clear();
	m_newVariation = false;
	m_variation = 0;

	do {
		if (m_inComment) {
			parseComment(game);
		} else {
			parseLine(game);
			if (m_variation == -1) {
                return;
			}
		}
	} while (!m_gameOver && (!m_file->atEnd() || m_currentLine != ""));

    if( m_gameOver ) {
        if(game->plyCount() == 0) {
            if( !m_precomment.isEmpty()) {
                //game->setGameComment(m_precomment);
                game->setAnnotation(m_precomment);
                m_precomment.clear();
            }
        }
    }
}

void PgnDatabase::parseLine(Game* game)
{
    QStringList list = m_currentLine.split(" ");
	m_pos = 0;

    for (QStringList::Iterator it = list.begin(); it != list.end() && !m_inComment; ++it) {
		if (*it != "") {
			parseToken(game, *it);
			if (m_variation == -1) {
				return;
			}
		}
		m_pos += (*it).length() + 1;
	}

	if (!m_inComment) {
		readLine();
	}
}

inline void PgnDatabase::parseDefaultToken(Game* game, QString token)
{
    //strip any move numbers
    if (token.contains("..."))
    {
        token = token.section("...", 1, 1);
    }
    else if (token.contains('.'))
    {
        token = token.section('.',	1, 1);
    }
    else if (token.indexOf(QRegExp("[1-9]"))==0)
    {
        token.clear();
    }

    //look for nags
    Nag nag = NullNag;
    if (token.endsWith("!")) {
        if (token.endsWith("!!")) {
            nag = VeryGoodMove;
        } else if (token.endsWith("!?")) {
            nag = SpeculativeMove;
        } else {
            nag = GoodMove;
        }
    } else if (token.endsWith("?")) {
        if (token.endsWith("??")) {
            nag = VeryPoorMove;
        } else if (token.endsWith("?!")) {
            nag = QuestionableMove;
        } else {
            nag = PoorMove;
        }
    }

    if (!token.isEmpty()) {
        if (m_newVariation) {
            game->backward();
            m_variation = game->addVariation(token, QString(), nag);
            if (!m_precomment.isEmpty()) {
                game->setAnnotation(m_precomment, m_variation, Game::BeforeMove);
                m_precomment.clear();
            }
            m_newVariation = false;
        } else {	// First move in the game
            m_variation = game->addMove(token, QString(), nag);
            if (!m_precomment.isEmpty()) {
                game->setAnnotation(m_precomment, m_variation, Game::BeforeMove);
                m_precomment.clear();
            }
        }
    }
}

void PgnDatabase::parseToken(Game* game, const QString& token)
{
	switch (token.at(0).toLatin1()) {
	case '(':
		m_newVariation = true;
		break;
	case ')':
		game->moveToId(game->parentMove());
		game->forward();
		m_newVariation = false;
		m_variation = 0;
		break;
	case '{':
		m_comment.clear();
		m_precomment.clear();
		m_inComment = true;
		m_currentLine = m_currentLine.right((m_currentLine.length() - m_pos) - 1);
        break;
	case '$':
		game->addNag((Nag)token.mid(1).toInt());
		break;
	case '!':
		if (token == "!") {
			game->addNag(GoodMove);
		} else if (token == "!!") {
			game->addNag(VeryGoodMove);
		} else if (token == "!?") {
			game->addNag(SpeculativeMove);
		}
		break;
	case '?':
		if (token == "?") {
			game->addNag(PoorMove);
		} else if (token == "??") {
			game->addNag(VeryPoorMove);
		} else if (token == "?!") {
			game->addNag(QuestionableMove);
		}
		break;
	case '+':
		if (token == "+=") {
			game->addNag(WhiteHasASlightAdvantage);
		} else if (token == "+/-") {
			game->addNag(WhiteHasAModerateAdvantage);
		}
		break;
	case '=':
		if (token == "=") {
			game->addNag(DrawishPosition);
		} else if (token == "=+") {
			game->addNag(BlackHasASlightAdvantage);
		}
		break;
	case '*':
		game->setResult(Unknown);
		m_gameOver = true;
        break;
    // From here, cases may fall through into default!!
	case '1':
		if (token == "1-0") {
			game->setResult(WhiteWin);
			m_gameOver = true;
            break;
        }
        else if (token == "1/2-1/2" || token == "1/2")
        {
			game->setResult(Draw);
			m_gameOver = true;
            break;
        }

	case '0':
        if (token == "0-1")
        {
			game->setResult(BlackWin);
			m_gameOver = true;
			break;
		}

	case '-':
        if (token == "-/+")
        {
			game->addNag(BlackHasAModerateAdvantage);
            break;
        }
        else if (token == "--")
        {
            // parse a null move!
            parseDefaultToken(game,token);
            break;
        }

	default:
        parseDefaultToken(game,token);
        break;
	}
}

void PgnDatabase::parseComment(Game* game)
{
	int end = m_currentLine.indexOf('}');

	if (end >= 0) {
		m_comment.append(m_currentLine.left(end));
		m_inComment = false;
        if (m_newVariation || game->plyCount() == 0)
			m_precomment = m_comment.trimmed();
		else game->setAnnotation(m_comment.trimmed());
		m_currentLine = m_currentLine.right((m_currentLine.length() - end) - 1);
	} else {
		m_comment.append(m_currentLine + ' ');
		readLine();
	}
}

inline bool onlyWhite(const QString& b)
{
    for (int i = 0; i < b.length(); ++i)
        if (!isspace(b.at(i).toAscii()))
            return false;
    return true;
}

inline bool onlyWhite(const QByteArray& b)
{
    for (int i = 0; i < b.length(); ++i)
        if (!isspace(b[i]))
			return false;
	return true;
}

void PgnDatabase::skipJunk()
{
	while ((!m_lineBuffer.length() || m_lineBuffer[0] != '[') && !m_file->atEnd())
        skipLine();
    m_currentLine = m_lineBuffer.simplified();
}

void PgnDatabase::skipTags()
{
	while (m_lineBuffer.length() && (m_lineBuffer[0] == '[') && !m_file->atEnd())
		skipLine();

	//swallow trailing whitespace
    while (onlyWhite(m_lineBuffer) && !m_file->atEnd())
        skipLine();
    m_currentLine = m_lineBuffer.simplified();
}

void PgnDatabase::skipMoves()
{
    QString gameText = " ";
    QRegExp gameNumber("\\s(\\d+)\\s*\\.");
    while (!onlyWhite(m_lineBuffer) && !m_file->atEnd()) {
        gameText += QString(m_lineBuffer) + " ";
		skipLine();
	}
	gameNumber.lastIndexIn(gameText);
	m_index.setTag("Length", gameNumber.cap(1), m_count - 1);

	//swallow trailing whitespace
    while (onlyWhite(m_lineBuffer) && !m_file->atEnd())
        skipLine();
    m_currentLine = m_lineBuffer.simplified();
}

