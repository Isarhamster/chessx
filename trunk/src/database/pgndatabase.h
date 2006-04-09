/***************************************************************************
                          pgndatabase.h  -  pgn database class
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
 
#include <qfile.h>
#include <qmap.h>
#include <qpair.h>
#include <qtextstream.h>
#include <qvector.h>

#include "database.h"
#include "search.h"
#include "tristatetree.h"

/**
   PgnDatabase provides database access to PGN files.
   The class is derived from the Database class, providing methods for the 
   loading and saving of games, and for performing searches and queries.
*/

class PgnDatabase : public Database
{
	public:
		/** Creates the database object from the given pgn file */
		PgnDatabase(const QString& filename);
		/** Destructor */
		~PgnDatabase();
		
		//game retrieval & storage
		/** Loads and returns the game at the given position */
		Game load(int index);
		/** Saves a game at the given position */
		void save(int index, Game& game);
		/** Adds a game to the database */
		void add(Game& game);
		/** Removes a game from the database */
		void remove(int index);
		/** Removes multiple games from the database as specified by the filter */
		void remove(const Filter& filter);
		/** Compacts the database */
		void compact();
		/** Returns the number of games in the database */
		int count();
		
		//capability enquiry (so we can start using a format before every last search type is supported)
		/** Checks if the database supports the given type of search */
		bool supportedSearchType(Search::Type searchType);
		
		//basic search execution
		/** Executes a basic search, and returns the results in a filter */
		Filter executeSearch(const Search& search);
		/** Executes a basic search combined with a filter */
		Filter executeSearch(const Search& search, Search::Operator searchOperator, Filter filter);
		
		//complex query execution
		/** Executes a query, and returns the results in a filter */
		Filter executeQuery(Query& query);
		/** Executes a query combined with a filter */
		Filter executeQuery(const Query& query, Search::Operator searchOperator, Filter filter); 
		
	private:
		//offset methods
		/** Returns the file offset for the given game */
		Q_LONG offset(int index);
		/** Adds the current file position as a new offset */
		void addOffset();
		/** Adds a new file offset */
		void addOffset(Q_LONG offset);
		/** Changes the file offset for the given game to the current file position*/ 
		void setOffset(int index);
		/** Changes the file offset for the given game to the supplied position */
		void setOffset(int index, Q_LONG offset);
		/** Removes a file offset */
		void removeOffset(int index);
		
		//file methods
		/** Backs up the old pgn file and creates a new one */		
		void startCopy();
		/** Copies games from the old file to the new */
		void copyRange(int startIndex, int endIndex, int newIndex, Q_LONG offset);
		/** Removes the backup if file operations were sucessful */
		void finishCopy();
		/** Reads the next line of text from the PGN file */
		void readLine();
		/** Moves the file position to the start of the given game */
		void seekGame(int index);
		
		//parsing methods
		/** Reads tags from the file and adds them to the game. Performs tag searches if any are active */
		void parseTags(Game* game);
		/** Reads moves from the file and adds them to the game. Performs position searches if any are active */
		void parseMoves(Game* game);
		/** Parses a line from the file */
		void parseLine(Game* game);
		/** Parses a token from the file */
		void parseToken(Game* game, QString token);
		/** Parses a comment from the file */
		void parseComment(Game* game);
		/** Skips past any data which is not valid tag or move data */
		void readJunk();
		/** Skips past any tag data */
		void readTags();
		/** Skips past any move data */
		void readMoves();
		
		//output methods
		/** Writes out the tags from the given game to the file */
		void writeTags(const Game& game);
		/** Writes out the move data from the given game to the file */
		void writeMoves(Game& game);
		/** Writes out current variation from the given game to the file */
		void writeVariation(Game& game);
		
		//file variables
		QString m_filename;
		QFile* m_file;
		QFile* m_newFile;
		QTextStream* m_newStream;
		QString m_gameText;
		
		//parsing variables
		static const int MaxLineLength = 512;
		char m_charLine[MaxLineLength];
		QString m_currentLine;
		int m_pos;
		bool m_gameOver;
		bool m_inComment;
		QString m_comment;
		bool m_newVariation;
		int m_variation;
		
		//game index
		int m_count;
		Q_LONG m_filePos;
		Q_LONG m_currentLineSize;

		static const int AllocationSize = 512;
		int m_allocated;
		Q_LONG* m_gameOffsets;
		
		//query variables
		Filter m_searchFilter;
		TriStateTree m_triStateTree;
		bool m_searchTags;
		bool m_searchGame;
		
		QVector<QPair<DateSearch, int> > m_dateSearches;
		QVector<QPair<EloSearch, int> > m_eloSearches;
		QVector<QPair<FilterSearch, int> > m_filterSearches;
		QVector<QPair<PositionSearch, int> > m_positionSearches;
		QVector<QPair<TagSearch, int> > m_tagSearches;
};
