/***************************************************************************
                          pgndatabase.h  -  pgn database class
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

#ifndef __PGNDATABASE_H__
#define __PGNDATABASE_H__

#include <QFile>
#include <QByteArray>

#include "database.h"

/** @ingroup Database
   The PgnDatabase class provides database access to PGN files.
   The class is derived from the Database class, providing methods for the
   loading and saving of games, and for performing searches and queries.
*/

class PgnDatabase : public Database
{
public:
	/** Default constructor */
	PgnDatabase();
	/** Destructor */
	~PgnDatabase();

	//database operations
	/** Opens the given database */
	virtual bool open(const QString& filename);
	/** File-based database name */
	virtual QString filename() const;
	/** Closes the database */
	virtual void close();
	/** Returns whether the database is read-only or not */
	virtual bool isReadOnly() { return true; }

	//game retrieval & storage
	/** Loads a game from the given position, returns true if successful */
	bool loadGame(int index, Game& game);
	/** Loads only moves into a game from the given position */
	void loadGameMoves(int index, Game& game);
	/** Returns true if the database has been modified. PgnDatabase is read only, 
	 * always returns false */
	virtual bool isModified() { return false; }

protected:
	//parsing methods
	/** Reads moves from the file and adds them to the game. Performs position searches if any are active */
	void parseMoves(Game* game);
	/** Parses a line from the file */
	void parseLine(Game* game);
	/** Parses a token from the file */
	void parseToken(Game* game, QString token);
	/** Parses a comment from the file */
	void parseComment(Game* game);
	/** Skips past any data which is not valid tag or move data */
	void skipJunk();
	/** Skips past any tag data */
	void skipTags();
	/** Skips past any move data */
	void skipMoves();
	/** Parses the tags, and adds the supported types to the index 'm_index' */
	void parseTagsIntoIndex();

	virtual bool parseFile();
	bool openFile(const QString& filename);
	/** Adds the current file position as a new offset */
	void addOffset();
	/** Adds a new file offset */
	void addOffset(qint32 offset);

	QFile* m_file;
	bool m_isOpen;
private:
	/** Resets/initialises important member variables. Called by constructor and close methods */
	void initialise();

	//offset methods
	/** Returns the file offset for the given game */
	qint32 offset(int index);

	//file methods
	/** Reads the next line of text from the PGN file */
	void readLine();
	/** Skips the next line of text from the PGN file */
	void skipLine();
	/** Moves the file position to the start of the given game */
	void seekGame(int index);


	//file variables
	QString m_filename;
	QString m_gameText;

	//parsing variables
	QString m_currentLine;
	int m_pos;
	bool m_gameOver;
	bool m_inComment;
	QString m_comment;
	bool m_newVariation;
	int m_variation;

	//game index
	qint32 m_filePos;
	qint32 m_currentLineSize;

	static const int AllocationSize = 512;
	int m_allocated;
	qint32* m_gameOffsets;
	QByteArray m_lineBuffer;

};

#endif
