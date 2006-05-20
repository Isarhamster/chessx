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
 
#include "nag.h"

#include "pgndatabase.h"
 
PgnDatabase::PgnDatabase(const QString& filename) : m_searchFilter(0)
{
	m_filename = filename;
	m_newFile = 0;
	m_newStream = 0;
	m_count = 0;
	m_gameOffsets = 0;
	m_allocated = 0;
	
	//open file
	m_file = new QFile(filename);
	m_file->open(IO_ReadWrite);
	m_filePos = 0;
	m_currentLineSize = 0;
	
	//indexing game positions in the file, game contents are ignored
	while(!m_file->atEnd()) {		
		readJunk();
		addOffset();
		readTags();
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
	
	int startIndex = 0;
	int newIndex = 0;
	int rangeSize;
	int newOffset = 0;
	int index = filter.firstIndex();
		
	while(index != -1) {
		if(startIndex < index) {
			rangeSize = offset(index) - offset(startIndex);
			copyRange(startIndex, index - 1, newIndex, newOffset);
			newIndex += index - startIndex;
			newOffset = + rangeSize;
		}
		startIndex = index + 1;
		index = filter.nextGame(index,1);
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

Filter PgnDatabase::executeSearch(const Search& search)
{	
	//Turn search into a query and execute it
	Query query;
	query.append(&search);
	return executeQuery(query);
}

Filter PgnDatabase::executeSearch(const Search& search, Search::Operator searchOperator, Filter filter)
{
	//Turn search into a query and execute it
	Query query;
	FilterSearch filterSearch(filter);
	query.append(&search);
	query.append(&filterSearch);
	query.append(searchOperator);
	return executeQuery(query);
}

Filter PgnDatabase::executeQuery(Query& query)
{
	//parse query into tree and searches for smart execution
	m_triStateTree = TriStateTree(query);
	m_searchTags = false;
	m_searchGame = false;
	int leafNode = 0;
	
	for(int element = 0; element < query.count(); element++) {
		if(query.isElementSearch(element)) {
			Search* search = query.search(element);		
			switch(search->type()) {
				case Search::DateSearch:
					m_searchTags = true;
					m_dateSearches.append(QPair<DateSearch, int>(*static_cast<const DateSearch*>(search), leafNode));
					break;
				case Search::EloSearch:
					m_searchTags = true;
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
					m_searchTags = true;
					m_tagSearches.append(QPair<TagSearch, int>(*static_cast<const TagSearch*>(search), leafNode));
					break;
				default:
					break;
			}
			leafNode++;
		}
	}
	
	//execute query
	m_searchFilter = Filter(m_count);
	Game game;
	
	for(int searchIndex = 0; searchIndex < m_count; searchIndex++) {
		seekGame(searchIndex);
		m_triStateTree.clear();
		
		//do filter searches before examining game
		for(int search = 0; search < m_filterSearches.size(); search++) {
			if(m_triStateTree.setState(m_filterSearches.at(search).second,m_filterSearches.at(search).first.contains(searchIndex))) {
				break;
			}
		}
		
		if(m_triStateTree.state() == TriStateTree::Unknown) {
			game.clear();
			if(m_searchTags) {
				parseTags(NULL);
			} else {
				readTags();
			}
			if(m_triStateTree.state() == TriStateTree::Unknown) {
				parseMoves(&game);
			}
		}
		
		if(m_triStateTree.state() != TriStateTree::True) {
			m_searchFilter.remove(searchIndex);
		}
	}
	
	return m_searchFilter;
}

Filter PgnDatabase::executeQuery(const Query& query, Search::Operator searchOperator, Filter filter)
{
	//Add operation to filter and execute it
	Query filterQuery(query);
	FilterSearch filterSearch(filter);
	filterQuery.append(&filterSearch);
	filterQuery.append(searchOperator);
	return executeQuery(filterQuery);
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
	int whiteElo = -1;
	int blackElo = -1;
	PartialDate date;
	
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
			if(tag == "WhiteElo") {
				whiteElo = value.toInt();
				if(blackElo != - 1) {
					for(int search = 0; search < m_eloSearches.size(); search++) {
						if(m_triStateTree.setState(m_eloSearches.at(search).second, m_eloSearches.at(search).first.withinEloRange(whiteElo, blackElo))) {
							return;
						}
					}
				}
			} else if(tag == "BlackElo") {
				blackElo = value.toInt();
				if(whiteElo != -1) {
					for(int search = 0; search < m_eloSearches.size(); search++) {
						if(m_triStateTree.setState(m_eloSearches.at(search).second, m_eloSearches.at(search).first.withinEloRange(whiteElo, blackElo))) {
							return;
						}
					}
				}
			} else if(tag == "Date") {
				date = PartialDate(value);
				for(int search = 0; search < m_dateSearches.size(); search++) {
					if(m_triStateTree.setState(m_dateSearches.at(search).second, m_dateSearches.at(search).first.withinDateRange(date))) {
						return;
					}
				}
			} else {
				//tag searches
				for(int search = 0; search < m_tagSearches.size(); search++) {
		    	if(tag == m_tagSearches.at(search).first.tag() && value == m_tagSearches.at(search).first.value()) {
						if(m_triStateTree.setState(m_tagSearches.at(search).second, true)) {
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
		for(int search = 0; search < m_eloSearches.size(); search++) {
    	if(m_triStateTree.state(m_eloSearches.at(search).second) == TriStateTree::Unknown) {
				if(m_triStateTree.setState(m_eloSearches.at(search).second, false)) {
					return;
				}
			}   
    }
		for(int search = 0; search < m_dateSearches.size(); search++) {
    	if(m_triStateTree.state(m_dateSearches.at(search).second) == TriStateTree::Unknown) {
				if(m_triStateTree.setState(m_dateSearches.at(search).second, false)) {
					return;
				}
			}   
    }
		for(int search = 0; search < m_tagSearches.size(); search++) {
    	if(m_triStateTree.state(m_tagSearches.at(search).second) == TriStateTree::Unknown) {
				if(m_triStateTree.setState(m_tagSearches.at(search).second, false)) {
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

void PgnDatabase::parseMoves(Game* game)
{
	m_gameOver = false;
	m_inComment = false;
	m_comment = "";
	m_newVariation = false;
	m_variation = 0;
	
	//check starting position as well as position after each move
	if(m_searchGame) {
		for(int search = 0; search < m_positionSearches.size(); search++) {
			if(game->board() == m_positionSearches.at(search).first.position()) {
				if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
		for(int search = 0; search < m_positionSearches.size(); search++) {
    	if(m_triStateTree.state(m_positionSearches.at(search).second) == TriStateTree::Unknown) {
				if(m_triStateTree.setState(m_positionSearches.at(search).second, false)) {
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
					for(int search = 0; search < m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
				for(int search = 0; search < m_positionSearches.size(); search++) {
					if(game->board() == m_positionSearches.at(search).first.position()) {
						if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
					for(int search = 0; search < m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
					for(int search = 0; search < m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
					for(int search = 0; search < m_positionSearches.size(); search++) {
						if(game->board() == m_positionSearches.at(search).first.position()) {
							if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
						for(int search = 0; search < m_positionSearches.size(); search++) {
							if(game->board() == m_positionSearches.at(search).first.position()) {
								if(m_triStateTree.setState(m_positionSearches.at(search).second, true)) {
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
