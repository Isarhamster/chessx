/***************************************************************************
                          search.h  -  Holds different types of searches
                             -------------------
    begin                : 06/12/2005
    copyright            : (C) 2005-2006 Marius Roets <saidinwielder@users.sourceforge.net>
                           (C) 2006 William Hoggarth <whoggarth@users.sourceforge.net>
                           (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __SEARCH_H__
#define __SEARCH_H__
class Query;

#include "board.h"
#include "partialdate.h"

/** @ingroup Database
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
  enum Type { NullSearch, PositionSearch, EloSearch, DateSearch, TagSearch, FilterSearch};
  /** Operator for joining filters */
  enum Operator {NullOperator, Not, And, Or, Add, Remove }; 

  /** Standard constructor. */
   Search();
  /** Cloning search object - probably obsolete */
  virtual Search* clone() const = 0;
  /** Standard destructor. */
  virtual ~Search() = 0;
  virtual Type type() const = 0;
};

/** @ingroup Database
The NullSearch class is empty search, doing nothing. */
class NullSearch : public Search
{
  public :
    NullSearch();
  virtual NullSearch* clone() const;
    virtual ~NullSearch();
    virtual Type type() const;
};

/** @ingroup Database
The PositionSearch class is a search that checks for given position. */
class PositionSearch : public Search
{
public:
  /** Empty constructor. */
  PositionSearch();
  /** Standard constructor. */
  PositionSearch(const Board& position);
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
private:
  Board m_position;
};

/** @ingroup Database
The EloSearch class is used for searching both white and black player Elo. */
class EloSearch : public Search
{
public:
  /** Standard constructor. */
  EloSearch(int minWhiteElo = 0, int maxWhiteElo = 4000, int minBlackElo = 0, int maxBlacElo =
      4000);
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
  /** @return @p true if given ratings are within accepted ranges. */
  bool withinEloRange(int whiteElo, int blackElo) const;
  /** Set acceptable rating ranges. */
  void setEloSearch(int minWhiteElo = 0, int maxWhiteElo = 4000, int minBlackElo =
      0, int maxBlacElo = 4000);
private:
  int m_minWhiteElo;
  int m_maxWhiteElo;
  int m_minBlackElo;
  int m_maxBlackElo;
};

/** @ingroup Database
The DataSearch class defines a search based on a date range/ */
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
  /** @return @p true if given data is within acceptable period. */
  bool withinDateRange(PartialDate date) const;
  /** Sets whole period. */
  void setDateRange(PartialDate minDate, PartialDate maxDate);

private:
  PartialDate m_minDate;
  PartialDate m_maxDate;
};


/** @ingroup Database
The TagSearch class is used for tag search. Only simple substring searches
are supported for now. */
class TagSearch : public Search
{
public:
  /** Simple constructor */
  TagSearch(const QString& tag = "tag", const QString& value = "value");
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
  /** Set tag to be searched */
  void setTag(const QString& tag);
  /** Set value to be matched */
  void setValue(const QString& value);
private:
  QString m_tag;
  QString m_value;
};

#endif // __SEARCH_H__
