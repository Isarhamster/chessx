/***************************************************************************
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __DATABASEINFO_H__
#define __DATABASEINFO_H__

#include "game.h"
#include <QString>

class Database;
class Filter;

/** @ingroup Database
The DatabaseInfo class is a simple class to keep various database-related
information together. */

class DatabaseInfo: public QThread
{
    Q_OBJECT
public:
    void run();
	enum {NewGame = -1};
    /** Create information for clipboard database */
	DatabaseInfo();
	/** Create information for file database */
	DatabaseInfo(const QString& filename);
	/** Close database and free memory */
	~DatabaseInfo();
	/** Open database. */
    bool open(bool utf8);
	/** Close database. */
	void close();
	/** @return @p true if database is valid */
    bool isValid() const  {return m_bLoaded?m_database!=0:false;}
	/** @return Database object */
    Database* database()  {return m_database;}
	/** @return current filter */
	Filter* filter()  {return m_filter;}
	/** @return current game  */
	Game& currentGame() {return m_game;}
	/** @return index of current game  */
	int currentIndex() const   {return m_index;}
	/** Load game @p index */
	bool loadGame(int index, bool reload=0);
	/** Starts new game, giving it @p index equal to the number of games in the database */
	void newGame();
	/** Saves game, replacing current one or adding new. If @ref currentIndex() is
	in the database, game will be replaced, else if will be appended at the end. */
	bool saveGame();

	/** Resizes the filter to the number of games in the database and puts
         *  all games in the filter.*/
	void resetFilter();
    QString filePath() const { return m_filename; }
    bool IsLoaded() const { return m_bLoaded; }
signals:
    void LoadFinished(DatabaseInfo*);
private:
	Database* m_database;
	Filter* m_filter;
	Game m_game;
	QString m_filename;
	int m_index;
    bool m_bLoaded;
    bool m_utf8;
};

#endif

