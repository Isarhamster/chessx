/***************************************************************************
                          datebaseinfo  -  Single database information
                             -------------------
    begin                : 23 Aug 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
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

class DatabaseInfo
{
public:
	/** Create information for open database */
	DatabaseInfo(const QString& fname = QString::null);
	/** Close database and free memory */
	~DatabaseInfo();
	/** @return @p true if database is valid */
	bool isValid() const  {return m_database;}
	/** @return Database object */
	Database* database()  {return m_database;}
	/** @return current filter */
	Filter* filter()  {return m_filter;}
	/** @return current game  */
	Game& currentGame() {return m_game;}
	/** @return index of current game  */
	int currentIndex() const   {return m_index;}
	/** Load game @p index */
	bool loadGame(int index);
	/** Starts new game, giving it @p index equal to the number of games in the database */
	void newGame();
	/** Database name */
	QString name() const;
private:
	Database* m_database;
	Filter* m_filter;
	Game m_game;
	int m_index;
};

#endif

