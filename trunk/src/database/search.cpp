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

#include <QtCore>

#include "search.h"
#include "database.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* Search abstract base clase
 * ********************************/
Search::Search(Database *db) : m_database(db)
{
    m_nextSearch = 0;
    m_searchOperator = FilterOperator::NullOperator;
}

Search::~Search()
{
    delete m_nextSearch;
    m_database = 0;
}

void Search::AddSearch(Search* search, FilterOperator op)
{
    m_nextSearch = search;
    m_searchOperator = op;
}

FilterOperator Search::searchOperator() const
{
    return m_searchOperator;
}

Search *Search::nextSearch() const
{
    return m_nextSearch;
}

/* NullSearch Class
 * *********************/
NullSearch::NullSearch():Search(0)
{
}

int NullSearch::matches(GameId) const
{
    return false;
}
