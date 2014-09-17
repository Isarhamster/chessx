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
class Filter;

#include "board.h"
#include "game.h"
#include "partialdate.h"
#include <QBitArray>

/**
 * @defgroup Search Search - different ways to lookup games
 **/

/** @ingroup Search
The Search class is an abstract base class that represents a search on one criteria.
@todo
- Operators for joining may be obsolete.
- Cloning may be obsolete
- Type may be replaced by dynamic typecasting.
- Or and And are the same, they probably should be joined.
*/
class Search
{
public:
    enum Type { NullSearch, PositionSearch, EloSearch, DateSearch, TagSearch, FilterSearch, NumberSearch};
    /** Operator for joining filters */
    enum Operator {NullOperator, Not, And, Or, Remove };

    /** Standard constructor. */
    Search();
    /** Cloning search object - probably obsolete */
    virtual Search* clone() const = 0;
    /** Standard destructor. */
    virtual ~Search() = 0;
    virtual Type type() const = 0;
    virtual int matches(GameId index) = 0;
    virtual void setDatabase(Database* database);
protected:
    Database *m_database;
    Game m_game;

};

/** @ingroup Search
The NullSearch class is empty search, doing nothing. */
class NullSearch : public Search
{
public :
    NullSearch();
    virtual NullSearch* clone() const;
    virtual ~NullSearch();
    virtual Type type() const;
    virtual int matches(GameId index);
};

/** @ingroup Search
The PositionSearch class is a search that checks for given position.
@todo Performance is seriously bad
*/
class PositionSearch : public Search
{
public:
    /** Empty constructor. */
    PositionSearch();
    /** Standard constructor. */
    PositionSearch(Database* db, const Board& position);
    /** Object cloning - probably obsolete. */
    virtual PositionSearch *clone() const;
    /** Standard destructor. */
    virtual ~PositionSearch();
    /** Type - probably obsolete. */
    virtual Search::Type type() const;
    /** @return sought position. */
    Board position() const;
    /** Sets sought position. */
    void setPosition(const Board & position);
    /** Return moveId the move of  after which the game matches the search + 1. E.g. for standard game and chess start position
    	1 is returned.
    */
    virtual int matches(GameId index);
private:
    Board m_position;
};

/** @ingroup Search
The EloSearch class is used for searching both white and black player Elo. */
class EloSearch : public Search
{
public:
    /** Standard constructor. */
    EloSearch(Database* database, int minWhiteElo = 0, int maxWhiteElo = 4000,
              int minBlackElo = 0, int maxBlackElo = 4000);
    /** Cloning - probably obsolete. */
    virtual EloSearch *clone() const;
    /** Standard destructor. */
    virtual ~EloSearch();
    /** Type - probably obsolete. */
    virtual Type type() const;
    /** @return minimum Elo of white player. */
    int minWhiteElo() const;
    /** @return maximum Elo of white player. */
    int maxWhiteElo() const;
    /** @return minimum Elo of black player. */
    int minBlackElo() const;
    /** @return maximum Elo of black player. */
    int maxBlackElo() const;
    /** Set acceptable rating ranges. */
    void setEloSearch(int minWhiteElo = 0, int maxWhiteElo = 4000, int minBlackElo =
                          0, int maxBlackElo = 4000);
    void initialize();
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);

private:
    int m_minWhiteElo;
    int m_maxWhiteElo;
    int m_minBlackElo;
    int m_maxBlackElo;
    QBitArray m_matches;
};

/** @ingroup Search
The DataSearch class defines a search based on a date range */
class DateSearch : public Search
{
public:
    /** Standard constructor. */
    DateSearch();
    /** Constructor for searching games in given time period. */
    DateSearch(PartialDate minDate, PartialDate maxDate);
    /** Object cloning - probably obsolete. */
    virtual DateSearch *clone() const;
    /** Standard destructor. */
    ~DateSearch();
    /** Type - probably obsolete. */
    Type type() const;
    /** @return beginning of the acceptable period. */
    PartialDate minDate() const;
    /** @return end of the acceptable period. */
    PartialDate maxDate() const;
    /** Sets whole period. */
    void setDateRange(PartialDate minDate, PartialDate maxDate);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);

private:
    PartialDate m_minDate;
    PartialDate m_maxDate;
    QBitArray m_matches;
};


/** @ingroup Search
The TagSearch class is used for tag search. Only simple substring searches
are supported for now.

@todo
Improved performance by creating a bit array of matching values */
class TagSearch : public Search
{
public:
    /** Simple constructor */
    TagSearch(Database* database, const QString& tag, const QString& value, bool partial = true);
    /** Range constructor */
    TagSearch(Database* database, const QString& tag, const QString& value, const QString& value2);
    /** Range constructor */
    TagSearch(Database *database, const QString &tag, int value, int value2);
    /** Makes a deep copy of TagSearch object. Probably obsolete */
    virtual TagSearch* clone() const;
    /** Standard destructor. */
    ~TagSearch();
    /** Type - probably obsolete. */
    Search::Type type() const;
    /** @return tag to be searched */
    QString tag() const;
    /** @return value to be matched. */
    QString value() const;
    QString minValue() const;
    QString maxValue() const;
    /** Set tag to be searched */
    void setTag(const QString& tag);
    /** Set value to be matched */
    void setValue(const QString& value);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);
    /** initialize the search. Done automatically in constructor, or when values are changed */
    void initialize();

private:
    QString m_tagName;
    QString m_value;
    QString m_value2;
    QBitArray m_matches;
    bool m_bPartial;
};

/** @ingroup Search
The NumberSearch class is used for game number search.  */

class NumberSearch : public Search
{
public:
    /** Simple constructor */
    NumberSearch(Database* database, GameId start, GameId end);
    /** Simple constructor. Supports "N1" or "N1-N2" format */
    NumberSearch(Database* database, const QString& value);
    /** Type - probably obsolete. */
    Search::Type type() const;
    /** Makes a deep copy of TagSearch object. Probably obsolete */
    virtual NumberSearch* clone() const;
    /** Set range to be selected */
    void setRange(GameId start, GameId end);
    /** Set range to be selected. Supports "N1" or "N1-N2" format. */
    void setRange(const QString& value);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);
private:
    GameId m_start, m_end;
};





/** @ingroup Search
 *  The FilterSearch class is not a real search class. It is used to
 *  combine other searches with a filter, in order to solve the search
 *  tree quicker if possible. For instance if a search is combined with a
 *  filter using the AND operator, only games in the filter are searched.
 *  If it is combined with the OR operator, only games not in the filter
 *  are searched. */
class FilterSearch : public Search
{
public:
    FilterSearch();
    FilterSearch(Filter* filter);
    virtual FilterSearch* clone() const;
    virtual ~FilterSearch();
    Type type() const;

    bool contains(GameId game) const;
    Filter* filter() const;
    void setFilter(Filter* filter);
    virtual int matches(GameId game);
private:
    Filter *m_filter;
};

#endif // __SEARCH_H__
