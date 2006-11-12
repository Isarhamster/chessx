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
 
#include <qdir.h>
#include <qstringlist.h>
 
#include "history.h"
#include "nag.h"

#include "pgndatabase.h"
 
PgnDatabase::PgnDatabase(const QString& filename) : m_moveStatCache(MaxMoveStatCacheSize, 101)
{
	m_filename = filename;
	m_newFile = 0;
	m_newStream = 0;
	m_count = 0;
	m_gameOffsets = 0;
	m_allocated = 0;
   m_searching = false;
	
	
	//move stat cache owns the items
	m_moveStatCache.setAutoDelete(true);

	//open file
	m_filePos = 0;
	m_currentLineSize = 0;
	if (filename.isNull())
	   m_file = new QFile;
	else 
	   m_file = new QFile(filename);
	if (!m_file->exists())
	   return;
	m_file->open(IO_ReadWrite);

	//indexing game positions in the file, game contents are ignored
	while(!m_file->atEnd()) {		
		readJunk();
		addOffset();
		// readTags();
		parseTagsIntoIndex(); // This will parse the tags into memory
		readMoves();
	}
	
}

PgnDatabase::~PgnDatabase()
{
	//close the file, and delete objects
	m_file->close();
	delete m_file;
	delete[] m_gameOffsets;
}

bool PgnDatabase::load(int index, Game& game)
{
	if(index >= m_count) {
		return false;
	}
	
	//parse the game
	game.clear();
	seekGame(index);
	parseTags(&game);
	if(game.tag("FEN") != QString::null) {
		Board board;
		board.fromFEN(game.tag("FEN"));
		game.setStartBoard(board);
	}
	parseMoves(&game);
	
	return m_variation != -1;
}

bool PgnDatabase::save(int index, Game& game)
{
	if(index >= m_count) {
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
		copyRange(index + 1, m_count - 1, index + 1, m_newFile->at());
	}
	return finishCopy();
}

void PgnDatabase::add(Game& game)
{
	//set the offset and then save a normal
	addOffset((Q_LONG)m_file->size());
	save(m_count - 1, game);
}

void PgnDatabase::remove(int index)
{
	if(index >= m_count) {
		return;
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
}

void PgnDatabase::remove(const Filter& filter)
{
	if(filter.size() != m_count) {
		return;
	}
	
	startCopy();
	
	int index;
	int startIndex = 0;
	int newIndex = 0;
	int rangeSize;
	int newOffset = 0;
		
	for(int game = 0; game < filter.count(); game++) {
		index = filter.gameIndex(game);
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
}

void PgnDatabase::compact()
{
	//nothing to do, currently done on a per operation basis
}

int PgnDatabase::count()
{
	return m_count;
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
   //locate any cached results that can be used
   int ply = 0;
   int closestPly = 0;
   QString key = "";
   QString closestKey = "";
   MoveStatCacheEntry* entry = 0;
   MoveStatCacheEntry* closestEntry = 0;
      
   for(MoveList::ConstIterator it = line.constBegin(); it != line.constEnd(); ++it) {
      ply++;
      key += QString::number((*it).from()) + " " + QString::number((*it).to()) + " ";
      
      if((entry = m_moveStatCache.find(key))) {
         closestPly = ply;
         closestKey = key;
         closestEntry = entry;
      }
   }
   
   //if already located in cache return stored result
   if(closestKey == key && closestEntry) {
      return closestEntry->moveStatList;
   }

   //get position and filter for final position in line (using any cached filter found previously)
   Filter filter(count());
   if(closestEntry) {
      filter = Filter(closestEntry->bitFilter);
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
   MoveStatList moveStatList;
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
   MoveStatCacheEntry cacheEntry;
   cacheEntry.moveStatList = moveStatList;
   cacheEntry.bitFilter = filter.asBitArray();
   m_moveStatCache.insert(key, new MoveStatCacheEntry(cacheEntry), filter.size());

   return moveStatList;
} 

Q_LONG PgnDatabase::offset(int index)
{
	return m_gameOffsets[index];
}

void PgnDatabase::addOffset()
{
	addOffset(m_filePos);
}

void PgnDatabase::addOffset(Q_LONG offset)
{
	if(m_count == m_allocated) {
		//out of space reallocate memory
		Q_LONG* newAllocation = new Q_LONG[m_allocated += AllocationSize];
		memcpy(newAllocation, m_gameOffsets, sizeof(Q_LONG) * m_count);
		delete m_gameOffsets;
		m_gameOffsets = newAllocation;
	}
	
	m_gameOffsets[m_count++] = offset;
}

void PgnDatabase::setOffset(int index)
{
	setOffset(index, m_filePos);
}

void PgnDatabase::setOffset(int index, Q_LONG offset)
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
		Q_LONG* newAllocation = new Q_LONG[m_allocated -= AllocationSize * 2];
		memcpy(newAllocation, m_gameOffsets, sizeof(Q_LONG) * m_count);
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
	m_newFile->open(IO_ReadWrite);
	m_newStream = new QTextStream(m_newFile);
}

void PgnDatabase::copyRange(int startIndex, int endIndex, int newIndex, Q_LONG newOffset)
{
	Q_ASSERT(m_newFile && m_newStream);

	// copy specified games to new file
	Q_LONG endOffset;
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
	Q_LONG diff = offset(startIndex) - newOffset;
	
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
	bool successful = m_newFile->status() == (unsigned)IO_Ok;
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
	m_currentLine = QString(m_charLine).stripWhiteSpace();
}

void PgnDatabase::seekGame(int index)
{
	m_file->at(m_filePos = offset(index));
	m_currentLineSize = m_file->readLine(m_charLine, MaxLineLength);
	m_currentLine = QString(m_charLine).stripWhiteSpace();
}

void PgnDatabase::parseTags(Game* game)
{
	
	do {
		if(!m_currentLine.startsWith(QString("["))) {
			break;
		}
		
		//parse tag
		QString tag = m_currentLine.mid(1, m_currentLine.find(' ') - 1);
		QString value = m_currentLine.section('"', 1, 1);	
		if(game) {
			game->setTag(tag, value);
		} else {
         if (!IndexItem::isTagNameSupported(tag)) {
				//tag searches
				for(int search = 0; search < (int)m_tagSearches.size(); search++) {
               if(tag == m_tagSearches.at(search).first.tag() && value == m_tagSearches.at(search).first.value()) {
						if(m_externalFilter->setState(m_tagSearches.at(search).second, true)) {
							return;
						}
               }   
            }
			}
		}
		readLine();
	} while(!m_file->atEnd() || m_currentLine != "");
	
	//mark any unmatched tag searches as false
   if(!game) {
      for(int search = 0; search < (int)m_tagSearches.size(); search++) {
         if(m_externalFilter->state(m_tagSearches.at(search).second) == TriStateTree::Unknown) {
            if(m_externalFilter->setState(m_tagSearches.at(search).second, false)) {
               return;
            }
         }   
      }
   }
	
	//swallow trailing whitespace
	while(m_currentLine == "" && !m_file->atEnd()) {
		readLine();
	}
}
void PgnDatabase::parseTagsIntoIndex()
{
	PartialDate date;
   Q_UINT32 gameId = m_index.add();
	
	do {
		if(!m_currentLine.startsWith(QString("["))) {
			break;
		}
		
		//parse tag
		QString tag = m_currentLine.mid(1, m_currentLine.find(' ') - 1);
		QString value = m_currentLine.section('"', 1, 1);	

      // Color specific tags must be renamed to a color independant tag.
      if (tag == "White") {
         m_index.gameIndex(gameId).setWhiteId(m_tags.add(Tags::PlayerName,value));
      } else if (tag == "Black") {
         m_index.gameIndex(gameId).setBlackId(m_tags.add(Tags::PlayerName,value));
      } else if (tag == "Event") {
         m_index.gameIndex(gameId).setEventId(m_tags.add(Tags::Event,value));
      } else if (tag == "Site") {
         m_index.gameIndex(gameId).setSiteId(m_tags.add(Tags::Site,value));
      } else if (tag == "ECO") {
         m_index.gameIndex(gameId).setEcoId(m_tags.add(Tags::ECO,value));
      } else if (tag == "Round") {
         m_tags.add(Tags::Round,value);
         m_index.gameIndex(gameId).setRound(value.toUInt());
      } else if (tag == "Result") {
         m_tags.add(Tags::Result,value);
         if (value == "1-0") {
            m_index.gameIndex(gameId).setResult(WhiteWin);
         } else if (value == "0-1") {
            m_index.gameIndex(gameId).setResult(BlackWin);
         } else if (value == "1/2-1/2") {
            m_index.gameIndex(gameId).setResult(Draw);
         } else if (value == "*") {
            m_index.gameIndex(gameId).setResult(Unknown);
         }
      } else if (tag == "WhiteElo") {
         m_tags.add(Tags::PlayerElo,value);
         m_index.gameIndex(gameId).setWhiteElo(value.toUInt());
      } else if (tag == "BlackElo") {
         m_tags.add(Tags::PlayerElo,value);
         m_index.gameIndex(gameId).setBlackElo(value.toUInt());
      } else if (tag == "Date") {
         m_tags.add(Tags::Date,value);
         date.fromString(value);
         m_index.gameIndex(gameId).setDate(date);
      } else {
         m_tags.add(m_tags.tagId(tag),value);
      }

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
	QStringList list = QStringList::split(" ", m_currentLine, true);
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
	switch(token.at(0).latin1()) {
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
	int end = m_currentLine.find('}'); 
  
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
		end = m_gameText.findRev (' ', start + 80);
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

QString PgnDatabase::name() const
{
  return m_filename;
}

void PgnDatabase::initSearch(Query& query, Filter* filter)
{
   m_externalFilter = filter;
   m_searching = true;
	//clear any previous searches
	m_dateSearches.clear();
	m_eloSearches.clear();
	m_filterSearches.clear();
	m_positionSearches.clear();
	m_tagSearches.clear();
	m_indexSearches.clear();
	
	m_searchGame = false;
   m_searchIndex = false;
	int leafNode = 0;
	
	for(int element = 0; element < query.count(); element++) {
		if(query.isElementSearch(element)) {
			Search* search = query.search(element);		
			switch(search->type()) {
				case Search::DateSearch:
					m_searchIndex = true;
					m_dateSearches.append(QPair<DateSearch, int>(*static_cast<const DateSearch*>(search), leafNode));
					break;
				case Search::EloSearch:
					m_searchIndex = true;
					m_eloSearches.append(QPair<EloSearch, int>(*static_cast<const EloSearch*>(search), leafNode));
					break;
				case Search::FilterSearch:
					m_filterSearches.append(QPair<FilterSearch, int>(*static_cast<const FilterSearch*>(search), leafNode));
					break;
				case Search::PositionSearch:
					m_searchGame = true;
					m_positionSearches.append(QPair<PositionSearch, int>(*static_cast<const PositionSearch*>(search), leafNode));
					break;
				case Search::TagSearch:
               // If the search is a tag search, it could either be in the index, or we would
               // have to read it from the pgn file. We find out by calling IndexItem::isTagNameSupported
               TagSearch *ts;
               ts = static_cast<const TagSearch*>(search);
               if (IndexItem::isTagNameSupported(ts->tag())) {
                  m_searchIndex = true;
                  m_indexSearches.append(QPair<TagSearch,int>(*ts,leafNode));
               } else {
                  m_searchTags = true;
                  m_tagSearches.append(QPair<TagSearch, int>(*ts, leafNode));
               }
					break;
				default:
					break;
			}
			leafNode++;
		}
	}
   // return m_filterSearches;
}

void PgnDatabase::finalizeSearch()
{
   m_searching = true;
}

void PgnDatabase::searchGame(int index)
{

   Game game;
   
   if((m_externalFilter->state() == TriStateTree::Unknown) && m_searchIndex) {
      loadHeaders(index,game);
   }
   if (m_externalFilter->state() == TriStateTree::Unknown) {
      if (m_searchTags) {
         seekGame(index);
         parseTags(NULL);
      } else {
         load(index,game);
      }
   }
   
}

bool PgnDatabase::loadHeaders(int index, Game& game)
{
   //qDebug ("Loading headers for game %d",index);
   if (!m_searching) {
      game.setTag("White",m_index.white(index, m_tags));
      game.setTag("Black",m_index.black(index, m_tags));
      game.setTag("Event",m_index.event(index, m_tags));
      game.setTag("Site",m_index.site(index, m_tags));
      game.setTag("Date",m_index.dateString(index));
      game.setTag("ECO",m_index.eco(index, m_tags));
      game.setTag("Round",QString::number(m_index.round(index)));
      game.setTag("Result",resultString(m_index.result(index)));
      game.setTag("BlackElo",QString::number(m_index.blackElo(index)));
      game.setTag("WhiteElo",QString::number(m_index.whiteElo(index)));
      
   } else {
      for(int search = 0; search < (int)m_eloSearches.size(); search++) {
         if(m_externalFilter->setState(m_eloSearches.at(search).second, 
                  m_eloSearches.at(search).first.withinEloRange(
                     m_index.whiteElo(index),
                     m_index.blackElo(index)))) {
            return true;
         }
      }
      for(int search = 0; search < (int)m_dateSearches.size(); search++) {
         if(m_externalFilter->setState(m_dateSearches.at(search).second, 
                  m_dateSearches.at(search).first.withinDateRange(m_index.date(index)))) {
            return true;
         }
      }
      for(int search = 0; search < (int)m_indexSearches.size(); search++) {
         QString tag = m_indexSearches.at(search).first.tag();
         if (tag == "White") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second,
                     (m_index.white(index, m_tags) ==  m_indexSearches.at(search).first.value()) )) {
               return true;
            }
         } else if (tag == "Black") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second, 
                     (m_index.black(index, m_tags) ==  m_indexSearches.at(search).first.value()))) {
               return true;
            }
         } else if (tag == "Event") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second, 
                     (m_index.event(index, m_tags) ==  m_indexSearches.at(search).first.value()))) {
               return true;
            }
         } else if (tag == "Site") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second, 
                     (m_index.site(index, m_tags) ==  m_indexSearches.at(search).first.value()))) {
               return true;
            }
         } else if (tag == "ECO") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second, 
                     (m_index.eco(index, m_tags) ==  m_indexSearches.at(search).first.value()))) {
               return true;
            }
         } else if (tag == "Round") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second, 
                     (QString::number(m_index.round(index)) ==  m_indexSearches.at(search).first.value()))) {
               return true;
            }
         } else if (tag == "Result") {
            if(m_externalFilter->setState(m_indexSearches.at(search).second, 
                     (resultString(m_index.result(index)) ==  m_indexSearches.at(search).first.value()))) {
               return true;
            }
         }

      } 

   }


   return true;
}



