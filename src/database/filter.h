/***************************************************************************
 *   (C) 2005 Ejner Borgbjerg <ejner@users.sourceforge.net>                *
 *   (C) 2006 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <QBitArray>
#include <QPair>
#include <QPointer>
#include <QThread>

#include "gameid.h"
#include "filteroperator.h"

class Search;
class Database;

/** @ingroup Database
   The Filter class represents a set of games. It is always associated with
   some Database object. On creation it has the same size as database,
   but it is not automatically resized when database size changes.
*/

class Filter : public QThread
{
    Q_OBJECT
public:
    typedef short value_type;
    /** Operator for joining filters */

    void runSingleSearch(Search* s, FilterOperator op);
    void run();
    void cancel();

    /** Construct filter of given size. Add all games to the filter. */
    Filter(Database* database);
    Filter(Filter const& rhs);
    /** Destructor. */
    ~Filter();

    Filter& operator= (Filter const& rhs);
    void lock(Filter* locked);

    /** @return a pointer to the database on which the filter is. */
    Database* database();
    const Database* database() const;
    /** Add or remove game @p game . Does nothing if the game is not in filter. */
    void set(GameId game, Filter::value_type value);
    /** Set all games in the filter to the same value. */
    void setAll(value_type value);
    /** @return true if the game is in the filter. */
    bool contains(GameId game) const;
    /** @return the ply at which the game in the filter is. Zero if game is not in filter */
    value_type gamePosition(GameId game) const;
    /** @return number of games in the filter. */
    inline int count() const { return m_count; }
    /** @return the size of the filter. */
    unsigned int size() const;
    /** Resize the filter to the specified size, keeping current content. If the filter is increased,
    added game will be initialized to @p includeNew (by default - not in filter). */
    void resize(unsigned int newsize, bool includeNew = 0);
    /** Reverse the filter (complement set). */
    void invert();
    /** Executes search 'search' on database m_database,
       and modifies this filter with the results. */
    void executeSearch(Search *search, FilterOperator searchOperator=FilterOperator::NullOperator);

signals:
    void searchProgress(int);
    void searchFinished();

protected:

    int m_count;
    QVector<value_type>* m_vector;
    Database* m_database;

    /* Search statistics variables */
    int m_gamesSearched;
    int m_searchTime;

    QPointer<Search> currentSearch;
    FilterOperator currentSearchOperator;
    volatile bool m_break;
    Filter* m_lock;
};

#endif // FILTER_H_INCLUDED

