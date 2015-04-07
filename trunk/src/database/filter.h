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

#ifndef __FILTER_H__
#define __FILTER_H__

#include <QBitArray>
#include <QPair>
#include <QThread>

#include "tristatetree.h"

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
    void run();
    void cancel();

    /** Possible operations on filter. */
    enum Operator {And, Or, Xor, Minus};
    /** Construct filter of given size. Add all games to the filter. */
    Filter(Database* database);
    /** Destructor. */
    ~Filter();
    /** @return a pointer to the database on which the filter is. */
    Database* database();
    /** Add or remove game @p game . Does nothing if the game is not in filter. */
    void set(int game, int value);
    /** Set all games in the filter to the same value. */
    void setAll(int value);
    /** @return true if the game is in the filter. */
    bool contains(int game) const;
    /** @return the ply at which the game in the filter is. Zero if game is not in filter */
    int gamePosition(int game) const;
    /** @return number of games in the filter. */
    inline int count() const { return m_count; }
    /** @return the size of the filter. */
    int size() const;
    /** @return next game in the filter or @p -1 if there is none. */
    int nextGame(int current) const;
    /** @return previous game in the filter or @p -1 if there is none. */
    int previousGame(int current) const;
    /** @return @p index in filter of game with database index @p number. */
    int gameToIndex(int number);
    /** @return database index of @p number game from filter or @p -1 if the game
    is not in filter. */
    int indexToGame(int number);
    /** Resize the filter to the specified size, keeping current content. If the filter is increased,
    added game will be initialized to @p includeNew (by default - not in filter). */
    void resize(int newsize, bool includeNew = 0);
    /** Reverse the filter (complement set). */
    void reverse();
    /** Executes search 'search' on database m_database,
       and sets this filter to contain the results. */
    void executeSearch(Search *search);
    /** Executes search 'search' on database m_database,
       and modifies this filter with the results. */
    void executeSearch(Search *search, Search::Operator searchOperator);
    /** Executes query 'query' on database m_database,
        and sets this filter to contain the results. */
    void executeQuery(Query& query);

    /** Returns the number of games searched during the previous search */
    int gamesSearched() const;
    /** Returns the time taken for the previous search in milliseconds  */
    int searchTime() const;

signals:
    void searchProgress(int);
    void searchFinished();

protected:

    QVector<int> intVector() const;

    int m_count;
    QVector<int>* m_vector;
    QPair<int, int> m_cache;
    Database* m_database;
    TriStateTree m_triStateTree;

    /* Search statistics variables */
    int m_gamesSearched;
    int m_searchTime;

    Search* currentSearch;
    Search::Operator currentSearchOperator;
    volatile bool m_break;

};

#endif

