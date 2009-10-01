/***************************************************************************
                          pgndatabase.cpp  -  pgn database class
                             -------------------
    begin                : 8 December 2005
    copyright            : (C) 2005, 2006 William Hoggarth <whoggarth@users.sourceforge.net>
                           (C) 2006 Ejner Borgbjerg <ejner@users.sourceforge.net>
                           (C) 2007 Marius Roets <roets.marius@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QDir>
#include <QStringList>
#include <QtDebug>

#include "board.h"
#include "nag.h"

#include "pgndatabase.h"

PgnDatabase::PgnDatabase()
{
	initialise();
}

PgnDatabase::~PgnDatabase()
{
	if (m_isOpen)
		close();
}

bool PgnDatabase::open(const QString& filename)
{
	if (m_isOpen) {
		return false;
	}
	m_filename = filename;
	if (openFile(filename) && parseFile()) {
		m_isOpen = true;
		return true;
	}
	return false;
}

bool PgnDatabase::parseFile()
{
	QString basefile = QFileInfo(filename()).fileName();
	emit fileOpened(tr("Opening %1...").arg(basefile));
	m_index.setCacheEnabled(true);
	int progress = 0;
	qint64 size = m_file->size();
	//indexing game positions in the file, game contents are ignored
	while (!m_file->atEnd()) {
		skipJunk();
		if (m_file->atEnd()) 
			break;
		addOffset();
		parseTagsIntoIndex(); // This will parse the tags into memory
		skipMoves();
		int progress2 = m_file->pos() * 100 / size;
		if (progress2 > progress) {
			progress = progress2;
			emit fileProgress(progress);
		}
	}
	m_index.setCacheEnabled(false);
	emit fileProgress(100);
	emit fileClosed(tr("%1 opened.").arg(basefile));
	return true;
}

bool PgnDatabase::openFile(const QString& filename)
{
	//open file
	m_filePos = 0;
	m_currentLineSize = 0;
	m_file = new QFile(filename);
	if (!m_file->exists()) {
		delete m_file;
		return false;
	}
	m_file->open(QIODevice::ReadOnly);
	return true;
}

QString PgnDatabase::filename() const
{
	return m_filename;
}

void PgnDatabase::close()
{
	if (!m_isOpen) {
		return;
	}

	//close the file, and delete objects
	m_file->close();
	delete m_file;
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
	QString fen = m_index.tagValue(TagFEN, m_count - 1);
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
	QString fen = m_index.tagValue(TagFEN, m_count - 1);
	if (fen != "?")
		game.setStartingBoard(fen);
	parseMoves(&game);

	return m_variation != -1;
}

void PgnDatabase::initialise()
{
	m_isOpen = false;
	m_filename = QString();
	m_count = 0;
	m_gameOffsets = 0;
	m_allocated = 0;
}

qint32 PgnDatabase::offset(int index)
{
	return m_gameOffsets[index];
}

void PgnDatabase::addOffset()
{
	addOffset(m_filePos);
}

void PgnDatabase::addOffset(qint32 offset)
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
	m_filePos += m_currentLineSize;
	m_lineBuffer = m_file->readLine();
	m_currentLineSize = m_lineBuffer.size();
	m_currentLine = QString(m_lineBuffer).trimmed();
	if (!m_currentLine.startsWith("[")) {
		m_currentLine.replace("(", " ( ");
		m_currentLine.replace(")", " ) ");
		m_currentLine.replace("{", " { ");
		m_currentLine.replace("}", " } ");
		m_currentLine.replace("$", " $");
	}
}

void PgnDatabase::skipLine()
{
	m_filePos += m_currentLineSize;
	m_lineBuffer = m_file->readLine();
	m_currentLineSize = m_lineBuffer.size();
}

void PgnDatabase::seekGame(int index)
{
	m_file->seek(m_filePos = offset(index));
	readLine();
}

void PgnDatabase::parseTagsIntoIndex()
{
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

}

void PgnDatabase::parseLine(Game* game)
{
	QStringList list = m_currentLine.split(" ");
	m_pos = 0;

	for (QStringList::Iterator it = list.begin(); it != list.end() && !m_inComment; it++) {
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

void PgnDatabase::parseToken(Game* game, QString token)
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
	case '-':
		if (token == "-/+") {
			game->addNag(BlackHasAModerateAdvantage);
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
	case '1':
		if (token == "1-0") {
			game->setResult(WhiteWin);
			m_gameOver = true;
			break;
		} else if (token == "1/2-1/2" || token == "1/2") {
			game->setResult(Draw);
			m_gameOver = true;
			break;
		}

	case '0':
		if (token == "0-1") {
			game->setResult(BlackWin);
			m_gameOver = true;
			break;
		}
	default:
		//strip any move numbers
		if (token.contains("...")) {
			token = token.section("...", 1, 1);
		} else if (token.contains('.')) {
			token = token.section('.',	1, 1);
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

bool onlyWhite(const QByteArray& b)
{
	for (int i = 0; i < b.length(); i++)
		if (!isspace(b[i]))
			return false;
	return true;
}

void PgnDatabase::skipJunk()
{
	while ((!m_lineBuffer.length() || m_lineBuffer[0] != '[') && !m_file->atEnd())
		skipLine();
	m_currentLine = m_lineBuffer.trimmed();
}

void PgnDatabase::skipTags()
{
	while (m_lineBuffer.length() && (m_lineBuffer[0] == '[') && !m_file->atEnd())
		skipLine();

	//swallow trailing whitespace
	while (onlyWhite(m_lineBuffer) && !m_file->atEnd())
		skipLine();
	m_currentLine = m_lineBuffer.trimmed();
}

void PgnDatabase::skipMoves()
{
	QString gameText = "";
	QRegExp gameNumber("\\s(\\d+)\\s*\\.");
	while (!onlyWhite(m_lineBuffer) && !m_file->atEnd()) {
		gameText += " " + QString(m_lineBuffer).trimmed();
		skipLine();
	}
	gameNumber.lastIndexIn(gameText);
	m_index.setTag("Length", gameNumber.cap(1), m_count - 1);

	//swallow trailing whitespace
	while (onlyWhite(m_lineBuffer) && !m_file->atEnd())
		skipLine();
	m_currentLine = m_lineBuffer.trimmed();
}

