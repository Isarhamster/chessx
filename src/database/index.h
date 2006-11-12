/***************************************************************************
                          filename - description
                             -------------------
    begin                : 
    copyright            : (C) 2006 Marius Roets <saidinwielder@sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __INDEX_H__
#define __INDEX_H__

#include <qt34/qvaluevector.h>
#include "indexitem.h"
#include "search.h"
#include "tags.h"
//
// Needs searching
// QValueVector<Q_UINT32> find (Search* search, const Tags& tags)
//    Will return all games that match <search>
// bool find (Q_UINT32 gameId, Search* search, const Tags& tags)
//    Does game <gameId> match <search

/** The Index class holds a list of IndexItem instances, typically one
 * for each game in the current database. This enables fast access to 
 * certain game header information.
 */

class Index {
   public:
      Index();
      Q_UINT32 add (IndexItem item);
      Q_UINT32 add ();
      void remove();

      /** Sets the tag id for the black player name for game at index */
      void setBlackId(Q_UINT32 index, Q_UINT32 id);
      /** Sets the tag id for the white player name for game at index */
      void setWhiteId(Q_UINT32 index, Q_UINT32 id);
      /** Sets the tag id for the event name for game at index */
      void setEventId(Q_UINT32 index, Q_UINT32 id);
      /** Sets the tag id for the site name for game at index */
      void setSiteId(Q_UINT32 index, Q_UINT32 id);
      /** Sets the Elo rating of the white player for game at index */
      void setWhiteElo(Q_UINT32 index, Q_UINT16 elo);
      /** Sets the Elo rating of the black player for game at index */
      void setBlackElo(Q_UINT32 index, Q_UINT16 elo);
      /** Sets the tag id for the ECO code of the game for game at index */
      void setEcoId(Q_UINT32 index, Q_UINT16 id);
      /** Sets the result of the game for game at index */
      void setResult(Q_UINT32 index, Result result);
      /** Sets the round of the game for game at index */
      void setRound(Q_UINT32 index, Q_UINT8 round);
      /** Sets the date of the game for game at index */
      void setDate(Q_UINT32 index, const PartialDate& date);


      /** Returns the tag id for the white player name for game at index*/
      Q_UINT32 whiteId(Q_UINT32 index);
      /** Returns the name of the white player for game at index */
      QString white(Q_UINT32 index,Tags& tags);
      /** Returns the tag id for the black player name for game at index*/
      Q_UINT32 blackId(Q_UINT32 index);
      /** Returns the name of the black player for game at index */
      QString black(Q_UINT32 index, Tags& tags);
      /** Returns the tag id for the event name for game at index*/
      Q_UINT32 eventId(Q_UINT32 index);
      /** Returns the name of the event for game at index */
      QString event(Q_UINT32 index, Tags& tags);
      /** Returns the tag id for the site name for game at index*/
      Q_UINT32 siteId(Q_UINT32 index);
      /** Returns the name of the site for game at index */
      QString site(Q_UINT32 index, Tags& tags);
      /** Returns the Elo rating of the white player for game at index*/
      Q_UINT16 whiteElo(Q_UINT32 index);
      /** Returns the Elo rating of the black player for game at index*/
      Q_UINT16 blackElo(Q_UINT32 index);
      /** Returns tag id for the ECO code of the game for game at index*/
      Q_UINT16 ecoId(Q_UINT32 index);
      /** Returns the name of the ECO code for game at index */
      QString eco(Q_UINT32 index, Tags& tags);
      /** Returns the date of the game for game at index*/
      PartialDate date(Q_UINT32 index);
      /** Returns the date in string format for game at index */
      QString dateString(Q_UINT32 index);
      /** Returns the result of the game for game at index*/
      Result result(Q_UINT32 index);
      /** Returns the round of the game for game at index*/
      Q_UINT8 round(Q_UINT32 index);

      /** Returns a reference to the IndexItem instance for game at index*/
      IndexItem& gameIndex(Q_UINT32 index);
      /** Returns true if game at 'gameId' matches search 'search'. 
       * Needs reference to the relevant Tags instance to resolve tag values */
      bool find (Q_UINT32 gameId, Search* search, Tags& tags);

   private:
      QValueVector<IndexItem> m_gameIndex;

};

#endif   // __INDEX_H__

