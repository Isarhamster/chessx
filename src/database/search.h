/***************************************************************************
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __SEARCH_H__
#define __SEARCH_H__

class Database;

#include "gameid.h"
#include <QtCore>

/**
 * @defgroup Search Search - different ways to lookup games
 **/

/** @ingroup Search
The Search class is a base class that represents a search on a database with one criteria.
*/
class Search : public QObject
{
    Q_OBJECT

public:
    enum Type { NullSearch, PositionSearch, EloSearch, DateSearch, TagSearch, FilterSearch, NumberSearch, DuplicateSearch, ListSearch};

    /** Standard constructor. */
    explicit Search(Database* db = 0);
    /** Standard destructor. */
    virtual ~Search();
    virtual void Prepare(volatile bool&) {};
    virtual int matches(GameId index) const = 0;
signals:
    void prepareUpdate(int);
protected:
    Database *m_database;
};

/** @ingroup Search
The NullSearch class is empty search, doing nothing. */
class NullSearch : public Search
{
    Q_OBJECT

public :
    NullSearch();
    virtual int matches(GameId index) const;
};


#endif // __SEARCH_H__
