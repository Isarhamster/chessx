/***************************************************************************
                          pgndatabase.cpp  -  pgn database class
                             -------------------
    begin                : 8 December 2005
    copyright            : (C) 2005, 2006 William Hoggarth
													 <whoggarth@users.sourceforge.net>
													 (C) 2005 Ejner Borgbjerg 
                           <ejner@users.sourceforge.net>
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
#include "history.h"
#include "nag.h"

#include "pgndatabase.h"
 
PgnDatabase::PgnDatabase() : m_moveStatCache(MaxMoveStatCacheSize)
{
	initialise();
}

PgnDatabase::~PgnDatabase()
{
	if(m_isOpen) {
		close();
	}
}

bool PgnDatabase::open(const QString& filename)
{
	if(m_isOpen) {
		return false;
	}
   m_index.setCacheEnabled(true);
	
	m_filename = filename;

	//open file
	m_filePos = 0;
	m_currentLineSize = 0;
	m_file = new QFile(filename);
	if(!m_file->exists()) {
		delete m_file;
	   return false;
	}
	m_file->open(QIODevice::ReadOnly);

	//indexing game positions in the file, game contents are ignored
	while(!m_file->atEnd()) {		
		readJunk();
		addOffset();
		parseTagsIntoIndex(); // This will parse the tags into memory
		readMoves();
	}
	
   m_index.setCacheEnabled(false);
	m_isOpen = true;
	return true;
}

QString PgnDatabase::filename() const
{
	return m_filename;
}

void PgnDatabase::close()
{
	if(!m_isOpen) {
		return;
	}
	
	//close the file, and delete objects
	m_file->close();
	delete m_file;
	delete[] m_gameOffsets;
	
	//reset member variables
	initialise();
}

bool PgnDatabase::loadGame(int index, Game& game)
{
	if(!m_isOpen || index >= m_count) {
		return false;
	}
	
	//parse the game
	game.clear();
   loadGameHeaders(index, game);
	seekGame(index);
	readTags();
	if((game.tag("FEN") != QString::null) && (game.tag("FEN") != "?")) {
      qDebug() << game.tag("FEN");
		Board board;
		board.fromFEN(game.tag("FEN"));
		game.setStartBoard(board);
	}
	parseMoves(&game);
	
	return m_variation != -1;
}

bool PgnDatabase::save(int index, Game& game)
{
	if(!m_isOpen || index >= m_count) {
		return false;
	}
	
	startCopy();
	if(index) {
		copyRange(0, index - 1, 0, 0);
	}
	
	writeTags(game);
	writeMoves(game);
	*m_newStream << flush;
	
	if(index < m_count - 1) {
		copyRange(index + 1, m_count - 1, index + 1, m_newFile->pos());
	}
	return finishCopy();
}

bool PgnDatabase::add(Game& game)
{
	if(!m_isOpen) {
		return false;
	}
	
	//set the offset and then save as normal
	addOffset(m_file->size());
	
	if(save(m_count - 1, game)) {
		return true;
	} else {
		removeOffset(m_count--);
		return false;
	}
}

bool PgnDatabase::remove(int index)
{
	if(!m_isOpen || index >= m_count) {
		return false;
	}
	
	startCopy();
	if(index) {
		copyRange(0, index - 1, 0, 0);
	}
	if(index < m_count - 1) {
		copyRange(index + 1, m_count - 1, index, offset(index));
	}
	finishCopy();
	m_count--;
	
	return true;
}

bool PgnDatabase::remove(const Filter& filter)
{
  if(!m_isOpen || filter.size() != m_count) {
    return false;
  }

  startCopy();

  int startIndex = 0;
  int newIndex = 0;
  int rangeSize;
  int newOffset = 0;
  for(int index = 0; index < filter.count(); index++)
    if (filter.contains(index))
    {
      if(startIndex < index) {
        rangeSize = offset(index) - offset(startIndex);
        copyRange(startIndex, index - 1, newIndex, newOffset);
        newIndex += index - startIndex;
        newOffset = + rangeSize;
      }
      startIndex = index + 1;
    }

  if(startIndex < m_count) {
    copyRange(startIndex, m_count - 1, newIndex, newOffset);
  }

  finishCopy();
  m_count -= filter.count();

  return true;
}

bool PgnDatabase::supportedSearchType(Search::Type searchType)
{
	switch(searchType) {
		case Search::DateSearch:
		case Search::EloSearch:
		case Search::FilterSearch:
		case Search::PositionSearch:
		case Search::TagSearch:
			return true;
		default:
			return false;
	}
}

PgnDatabase::MoveStatList PgnDatabase::moveStats(const MoveList& line)
{
	MoveStatList moveStatList;
	
	if(!m_isOpen) {
		return moveStatList;
	}
	
   //locate any cached results that can be used
	Board board;
   int ply = 0;
	quint64 key = 0;
   int closestPly = 0;
   quint64 closestKey = 0;
      
	board.setStandardPosition();
	key = board.getHashValue();
   if(m_moveStatCache.contains(key)) {
      closestPly = ply;
      closestKey = key;
   }

   for(MoveList::ConstIterator it = line.constBegin(); it != line.constEnd(); ++it) {
      board.doMove(*it);
		ply++;
      key = board.getHashValue();
      
      if(m_moveStatCache.contains(key)) {
         closestPly = ply;
         closestKey = key;
      }
   }
   
   //if already located in cache return stored result
   if(closestKey == key && m_moveStatCache.contains(closestKey)) {
      return m_moveStatCache.object(closestKey)->moveStatList;
   }

   //get position and filter for final position in line (using any cached filter found previously)
#warning Needs porting to new Filter API
  // I don't know what is going on here : mrudolf
   /*
   Filter filter(count());
   if(closestKey) {
      filter = Filter(m_moveStatCache.object(closestKey)->bitFilter);
   }
   filter.setDatabase(this);
   Game game;
   
   for(MoveList::ConstIterator it = line.constBegin(); it != line.constEnd(); ++it) {
      game.addMove(*it);
      game.forward();
      
      //only search if past point of cached filter results
      if(game.ply() > closestPly) {
         filter.executeSearch(PositionSearch(game.board()), Search::And);
      }
   }
   
   //calculate stats by doing a search on each legal move
   MoveStat moveStat;
   MoveList legalMoves = game.board().legalMoves();
   
   for(MoveList::iterator it = legalMoves.begin(); it != legalMoves.end(); it++) {
      moveStat.move = *it;
      game.replaceMove(*it);
      game.forward();
      
      moveStat.eco = game.ecoClassify();
      Filter childFilter(filter);
      childFilter.setDatabase(this);
      childFilter.executeSearch(PositionSearch(game.board()), Search::And);
      if(childFilter.count()) {
         moveStat.frequency = (float)childFilter.count() / filter.count();
         moveStatList.append(moveStat);
      }
      
      game.backward();
   }
   
   //store result in cache
   MoveStatCacheEntry* cacheEntry = new MoveStatCacheEntry;
   cacheEntry->moveStatList = moveStatList;
   cacheEntry->bitFilter = filter.asBitArray();
   m_moveStatCache.insert(key, cacheEntry, filter.size());
  */

   return moveStatList;
} 

void PgnDatabase::initialise()
{
	m_isOpen = false;
	m_filename = "";
   m_newFile = 0;
   m_newStream = 0;
   m_count = 0;
   m_gameOffsets = 0;
   m_allocated = 0;
   m_searching = false;
}

qint64 PgnDatabase::offset(int index)
{
	return m_gameOffsets[index];
}

void PgnDatabase::addOffset()
{
	addOffset(m_filePos);
}

void PgnDatabase::addOffset(qint64 offset)
{
	if(m_count == m_allocated) {
		//out of space reallocate memory
		qint64* newAllocation = new qint64[m_allocated += AllocationSize];
		memcpy(newAllocation, m_gameOffsets, sizeof(qint64) * m_count);
		delete m_gameOffsets;
		m_gameOffsets = newAllocation;
	}
	
	m_gameOffsets[m_count++] = offset;
}

void PgnDatabase::setOffset(int index)
{
	setOffset(index, m_filePos);
}

void PgnDatabase::setOffset(int index, qint64 offset)
{
	m_gameOffsets[index] = offset;
}

void PgnDatabase::removeOffset(int index)
{
	if(index < m_count - 1) {
		memmove(m_gameOffsets + index, m_gameOffsets + index + 1, m_count - (index + 1));
	}
	
	m_count--;
		
	if(m_count < m_allocated - (AllocationSize * 2)) {
		//using too much space release memory
		qint64* newAllocation = new qint64[m_allocated -= AllocationSize * 2];
		memcpy(newAllocation, m_gameOffsets, sizeof(qint64) * m_count);
		delete m_gameOffsets;
		m_gameOffsets = newAllocation;
	}
}

void PgnDatabase::startCopy()
{
	Q_ASSERT(!m_newFile && !m_newStream);
	
	//rename the file as a back up
	QDir dir;
	dir.rename(m_filename, m_filename + "~");
	
	//open new file and initialise stream
	m_newFile = new QFile(m_filename);
	m_newFile->open(QIODevice::ReadWrite);
	m_newStream = new QTextStream(m_newFile);
}

void PgnDatabase::copyRange(int startIndex, int endIndex, int newIndex, qint64 newOffset)
{
	Q_ASSERT(m_newFile && m_newStream);

	// copy specified games to new file
	qint64 endOffset;
	if(endIndex < m_count - 1) {
		endOffset = offset(endIndex + 1) - 1;
	} else {
		endOffset = m_file->size() - 1;
	}
	
	seekGame(startIndex);
	do {
		*m_newStream << m_charLine;
		readLine();
	} while(m_filePos < endOffset || m_filePos < 0);
	*m_newStream << m_charLine;
	
	// update offsets to match positions in the new file
	qint64 diff = offset(startIndex) - newOffset;
	
	if(newIndex < startIndex) {
		for(int index = 0; index <= (endIndex - startIndex); index++) {
			setOffset(newIndex + index, offset(startIndex + index) - diff);
		}
	} else {
		for(int index = (endIndex - startIndex); index >= 0; index--) {
			setOffset(newIndex + index, offset(startIndex + index) - diff);
		}
	}
}

bool PgnDatabase::finishCopy()
{
	Q_ASSERT(m_newFile && m_newStream);
	
	//flush buffer
	*m_newStream << flush;
	delete m_newStream;
	
	//if successful remove backup, otherwise restore it
	bool successful = m_newFile->error() == QFile::NoError;
	QDir dir;
	
	if(successful) {
		m_file->close();
		delete m_file;
		m_file = m_newFile;
		dir.remove(m_filename + "~");
	} else {
		m_newFile->close();
		delete m_newFile;
		dir.remove(m_filename);
		dir.rename(m_filename + "~", m_filename);
	}
	
	m_newFile = 0;
	m_newStream = 0;
	
	return successful;
}

void PgnDatabase::readLine()
{
	m_filePos += m_currentLineSize;
	m_currentLineSize = m_file->readLine(m_charLine, MaxLineLength);
	m_currentLine = QString(m_charLine).trimmed();
}

void PgnDatabase::seekGame(int index)
{
	m_file->seek(m_filePos = offset(index));
	m_currentLineSize = m_file->readLine(m_charLine, MaxLineLength);
	m_currentLine = QString(m_charLine).trimmed();
}

void PgnDatabase::parseTagsIntoIndex()
{
	do {
		if(!m_currentLine.startsWith(QString("["))) {
			break;
		}
		
		// parse tag
		QString tag = m_currentLine.mid(1, m_currentLine.indexOf(' ') - 1);
		QString value = m_currentLine.section('"', 1, 1);	

      // update index
      m_index.setTag(tag,value,m_count-1);

		readLine();
	} while(!m_file->atEnd() || m_currentLine != "");
	
	//swallow trailing whitespace
	while(m_currentLine == "" && !m_file->atEnd()) {
		readLine();
	}
}

void PgnDatabase::parseMoves(Game* game)
{
	m_gameOver = false;
	m_inComment = false;
	m_comment = "";
	m_newVariation = false;
	m_variation = 0;
	
	//check starting position as well as position after each move
	if(m_searchGame) {
		for(int search = 0; search < (int)m_positionSearches.size(); search++) {
			if(game->board() == m_positionSearches.at(search).first.position()) {
				if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
					return;
				}
			}
		}
	}
		
	do {
		if (m_inComment) {
			parseComment(game);
    }
    else {
			parseLine(game);
			if(m_variation == -1) {
				return;
			}
    }
	} while(!m_gameOver && (!m_file->atEnd() || m_currentLine != ""));
	
	//mark any unmatched position searches as false
	if(m_searchGame) {
		for(int search = 0; search < (int)m_positionSearches.size(); search++) {
    	if(m_externalFilter->state(m_positionSearches.at(search).second) == TriStateTree::Unknown) {
				if(m_externalFilter->setState(m_positionSearches.at(search).second, false)) {
					return;
				}
			}   
    }
  }
}

void PgnDatabase::parseLine(Game* game)
{
	QStringList list = m_currentLine.split(" ");
	m_pos = 0;
  
	for (QStringList::Iterator it = list.begin(); it != list.end() && !m_inComment; it++) {
		if(*it != "") {
			parseToken(game, *it);
			if(m_variation == -1) {
				return;
			}
		}
		m_pos += (*it).length() + 1;
  }
	
	if (!m_inComment){
		readLine();
  }
}

void PgnDatabase::parseToken(Game* game, QString token)
{
	switch(token.at(0).toLatin1()) {
		case '(':
			m_newVariation = true;
			break;
		case ')':
			if(!m_variation) {
				// position search on last position		
				if(m_searchGame) {
					game->forward();
					for(int search = 0; search < (int)m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
								return;
							}
						}
					}
				}
				
				game->exitVariation();
			}
			m_newVariation = false;
			m_variation = 0;
			break;
		case '{':
			m_comment = "";
			m_inComment = true;
			m_currentLine = m_currentLine.right((m_currentLine.length() - m_pos) - 1);
			break;
		case '$':
			game->addNag((Nag)token.mid(1).toInt(), m_variation);
			break;
		case '!':
			if(token == "!") {
				game->addNag(GoodMove, m_variation);
			} else if(token == "!!") {
				game->addNag(VeryGoodMove, m_variation);
			} else if(token == "!?") {
				game->addNag(SpeculativeMove, m_variation);
			}
			break;
		case '?':
			if(token == "?") {
				game->addNag(PoorMove, m_variation);
			} else if(token == "??") {
				game->addNag(VeryPoorMove, m_variation);
			} else if(token == "?!") {
				game->addNag(QuestionableMove, m_variation);
			}
			break;
		case '+':
			if(token == "+=") {
				game->addNag(WhiteHasASlightAdvantage, m_variation);
			} else if(token == "+/-") {
				game->addNag(WhiteHasAModerateAdvantage, m_variation);
			}
			break;
		case '-':
			if(token == "-/+") {
				game->addNag(BlackHasAModerateAdvantage, m_variation);
			}
			break;
		case '=':
			if(token == "=") {
				game->addNag(DrawishPosition, m_variation);
			} else if(token == "=+") {
				game->addNag(BlackHasASlightAdvantage, m_variation);
			}
			break;
		case '*':
			game->setResult(Unknown);
			m_gameOver = true;
			
			// position search on last position
			if(m_searchGame) {
				game->forward();
				for(int search = 0; search < (int)m_positionSearches.size(); search++) {
					if(game->board() == m_positionSearches.at(search).first.position()) {
						if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
							return;
						}
					}
				}
			}
			break;
		case '1':
			if(token == "1-0") {
				game->setResult(WhiteWin);
				m_gameOver = true;
				// position search on last position
				if(m_searchGame) {
					game->forward();
					for(int search = 0; search < (int)m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
								return;
							}
						}
					}
				}
				break;
			} else if(token == "1/2-1/2") {
				game->setResult(Draw);
				m_gameOver = true;
				// position search on last position
				if(m_searchGame) {
					game->forward();
					for(int search = 0; search < (int)m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
								return;
							}
						}
					}
				}
				break;
			}
			
		case '0':
			if(token == "0-1") {
				game->setResult(BlackWin);
				m_gameOver = true;
				// position search on last position
				if(m_searchGame) {
					game->forward();
					for(int search = 0; search < (int)m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
								return;
							}
						}
					}
				}
				break;
			}
		default:
			//strip any move numbers
			if(token.contains("...")) {
				token = token.section("...", 1, 1);
			} else if (token.contains('.')) {
				token = token.section('.',	1, 1);
			} 
			
			//look for nags
			Nag nag = NullNag;
			if(token.endsWith("!")) {
				if(token.endsWith("!!")) {
					nag = VeryGoodMove;
				} else if(token.endsWith("!?")) {
					nag = SpeculativeMove;
				} else {
					nag = GoodMove;
				}
			} else if(token.endsWith("?")) {
				if(token.endsWith("??")) {
					nag = VeryPoorMove;
				} else if(token.endsWith("?!")) {
					nag = QuestionableMove;
				} else {
					nag = PoorMove;
				}
			}
	
			if(token != "") {							
				if(m_newVariation) {
					m_variation = game->addMove(token, QString::null, nag);
					m_newVariation = false;
				} else {				
					game->enterVariation(m_variation);
					m_variation = game->addMove(token, QString::null, nag);
							
					/* position search, on new position */
					if(m_searchGame) {
						for(int search = 0; search < (int)m_positionSearches.size(); search++) {
							if(game->board() == m_positionSearches.at(search).first.position()) {
								if(m_externalFilter->setState(m_positionSearches.at(search).second, true)) {
									m_variation = -1;
									return;
								}
							}
						}
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
    game->setAnnotation(m_comment, m_variation);
    m_currentLine = m_currentLine.right((m_currentLine.length() - end) - 1);
	}
  else {
		m_comment.append(m_currentLine + " ");
    readLine();
  }
}

void PgnDatabase::readJunk()
{	
	while(!m_currentLine.startsWith(QString("[")) && !m_file->atEnd()) {
		readLine();
	}
}

void PgnDatabase::readTags()
{
	while(m_currentLine.startsWith(QString("[")) && !m_file->atEnd()) {
		readLine();
	}
	
	//swallow trailing whitespace
	while(m_currentLine == "" && !m_file->atEnd()) {
		readLine();
	}
}

void PgnDatabase::readMoves()
{
	while(m_currentLine != "" && !m_file->atEnd()) {
		readLine();
	}
	
	//swallow trailing whitespace
	while(m_currentLine == "" && !m_file->atEnd()) {
		readLine();
	}
}

void PgnDatabase::writeTags(const Game& game)
{
	*m_newStream << "[Event \"" + game.tag("Event") + "\"]" << endl;
	*m_newStream << "[Site \"" + game.tag("Site") + "\"]" << endl;
	*m_newStream << "[Date \"" + game.tag("Date") + "\"]" << endl;
	*m_newStream << "[Round \"" + game.tag("Round") + "\"]" << endl;
	*m_newStream << "[White \"" + game.tag("White") + "\"]" << endl;
	*m_newStream << "[Black \"" + game.tag("Black") + "\"]" << endl;
	*m_newStream << "[Result \"" + resultString(game.result()) + "\"]" << endl;
	
	//add FEN string if non-standard start position
	Board standardPosition;
	standardPosition.setStandardPosition();
	if(game.startBoard() != standardPosition) {
		*m_newStream << "[FEN \"" + game.startBoard().toFEN() + "\"]" << endl;
 	}

	*m_newStream << endl;
}

void PgnDatabase::writeMoves(Game& game)
{
	//write move text to one long string
	game.moveToStart();
	m_gameText = "";
	writeVariation(game);
	m_gameText += resultString(game.result());
	
	//break text into 80 char lines and write
	int length = m_gameText.length();
	int start = 0;
	int end;
	
	while(start < length) {
		end = m_gameText.lastIndexOf(' ', start + 80);
		if(end == -1) {
			end = m_gameText.length();
		}
		*m_newStream << m_gameText.mid(start, end - start ) << endl;
		start = end + 1;
	}
	
	*m_newStream << endl;
}

void PgnDatabase::writeVariation(Game& game)
{
	bool commentEnded = true;
	bool variationEnded = false;
	while(!game.atEnd()) {		
		
		//add move no
		if(!(game.ply() % 2)) {
			m_gameText += QString::number(game.ply() / 2 + 1) + ".";
		} else if(commentEnded || variationEnded) {
			m_gameText += QString::number(game.ply() / 2 + 1) + "...";
		}
		
		//add move		
		m_gameText += game.board().moveToSAN(game.move()) + " ";
			
		//add nags and annotation
		if(game.nags().count() > 0) {
			m_gameText += "$" + game.nags().toPGNString() + " ";
		}
		
		if(game.annotation() != QString::null) {
			m_gameText += "{" + game.annotation() + "} ";
			commentEnded = true;
		} else {
			commentEnded = false;
		}
		
		variationEnded = false;
		
		//add any variations
		int variationCount = game.variationCount();
		if(variationCount > 1) {
			for(int variation = 1; variation < variationCount; variation++) {
				
				m_gameText += "( ";
				
				//add move no
				if(!(game.ply() % 2)) {
					m_gameText += QString::number(game.ply() / 2 + 1) + ".";
				} else {
					m_gameText += QString::number(game.ply() / 2 + 1) + "...";
				}
				
				//add move
				m_gameText += game.board().moveToSAN(game.move(variation)) + " ";
				
				//add nags and annotation
				if(game.nags(variation).count()) {
					m_gameText += "$" + game.nags(variation).toPGNString() + " ";
				}
				
				if(game.annotation(variation) != QString::null) {
					m_gameText += "{" + game.annotation(variation) + "} ";
					commentEnded = true;
				} else {
					commentEnded = false;
				}
				
				game.enterVariation(variation);
				writeVariation(game);
				game.exitVariation();
				
				m_gameText += ") ";
			}
			
			variationEnded = true;
		}
		
		//export next move
		game.forward();
	}
}

void PgnDatabase::initSearch(Query& , Filter* )
{
   m_searching = true;
}

void PgnDatabase::finalizeSearch()
{
   m_searching = false;
}

void PgnDatabase::searchGame(int index)
{

   Game game;
   
   if((m_externalFilter->state() == TriStateTree::Unknown) && m_searchIndex) {
      loadGameHeaders(index,game);
   }
   if (m_externalFilter->state() == TriStateTree::Unknown) {
      loadGame(index,game);
   }
   
}




