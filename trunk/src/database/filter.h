/***************************************************************************
                          filter.h  -  holds search results in memory
                             -------------------
    begin                : 27/11/2005
    copyright            : (C) 2005 Ejner Borgbjerg
                           <ejner@users.sourceforge.net>
                           (C) 2006 William Hoggarth
                           <whoggarth@users.sourceforge.net>														
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __FILTER_H__
#define __FILTER_H__
#include "common.h"
#include <qbitarray.h>
#include <qt34/qbytearray.h>

//TODO
/*
* unit tests (when design is more stable - can copy from /tests/filter)
* possible optimizations
  * cache one value (but what do we know about calling sequence ?? )
  * use QValueVector as representation in case of "sparse" filter
* saving/loading to disk (dataset).
*/

/**
   The Filter class represents a set of games, typically found when searching a database.
*/


class Filter
{
public:
/**
    flag for which games to include in filter
*/
        enum WhichGames{AllGames,NoGames};
/** construct filter of given size
*/
	Filter(const int size, const WhichGames whichGames = AllGames);
/** construct filter from another filter
*/
	Filter(const Filter& filter);
/** construct filter from a bit array
*/
	Filter(const QBitArray& bitArray);
/** assignment operator
*/
	Filter operator=(const Filter& game);
/** destructor
*/
	~Filter();
/** include game in filter, at the given move
*/
        bool set(const unsigned int game, const unsigned int moveIndex);
/** remove game from filter
*/
        bool remove(const unsigned int game);
/** return number of games in the filter
*/
        int count() const;
/** return the size of the filter
*/
        int size() const;
/** return true iff the game is in the filter
*/
	bool contains(const int game) const;
/** return the move index for a game that is in the filter
    0 if game is not in the filter
    1 means start position
*/
        int moveIndex(const int game) const;
/** return index of current game in filter
*/
	int currentIndex() const;
/** set index of current game in filter
*/
	bool setCurrentIndex(const unsigned int index);
/** return index of first game in filter
*/
	int lastIndex() const;
/** return index of first game in filter
*/
	int firstIndex() const;
/** make first game in filter the current one
*/
	void toFirst();
/** make last game in filter the current one
*/
	void toLast();
/** return index of the offset'th next game in filter relative to startIndex
*/
	int nextGame(const int startIndex, const int offset) const;
/** return index of the offset'th previous game in filter relative to endIndex
*/
	int previousGame(const unsigned int endIndex, const unsigned int offset) const;
/** reverse the filter (complement set)
*/
	void reverse();
/** intersect filter with another filter
*/
        void intersect(const Filter& filter);
/** add the games of another filter to the filter
*/
	void add(const Filter& filter);
/** remove the games of another filter from the filter
*/
	void remove(const Filter& filter);


private:

    QByteArray* m_byteArray;//array of chars
                            // =='0' means game with that index is not in the filter
                            // !='0' means game with that index is in the filter, and the value
                            //       denotes the move index for the match
    int m_count;//number of games in the filter
    int m_currentIndex;//the actual position within the filter
    int m_firstIndex;//the position of the first game that's in the filter
    int m_lastIndex;//the position of the last game that's in the filter
};


#endif


