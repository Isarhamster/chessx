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

#include <QVector>
#include "indexitem.h"
#include "search.h"
#include "tags.h"
//
// Needs searching
// QValueVector<quint32> find (Search* search, const Tags& tags)
//    Will return all games that match <search>
// bool find (quint32 gameId, Search* search, const Tags& tags)
//    Does game <gameId> match <search

/** @ingroup Database  
   The Index class holds a list of IndexItem instances, typically one
 * for each game in the current database. This enables fast access to 
 * certain game header information.
 */

class Index {
   public:
      Index();
      quint32 add (IndexItem item);
      quint32 add ();
      void remove();

      /** Sets the tag id for the black player name for game at index */
      void setBlackId(quint32 index, quint32 id);
      /** Sets the tag id for the white player name for game at index */
      void setWhiteId(quint32 index, quint32 id);
      /** Sets the tag id for the event name for game at index */
      void setEventId(quint32 index, quint32 id);
      /** Sets the tag id for the site name for game at index */
      void setSiteId(quint32 index, quint32 id);
      /** Sets the Elo rating of the white player for game at index */
      void setWhiteElo(quint32 index, quint16 elo);
      /** Sets the Elo rating of the black player for game at index */
      void setBlackElo(quint32 index, quint16 elo);
      /** Sets the tag id for the ECO code of the game for game at index */
      void setEcoId(quint32 index, quint16 id);
      /** Sets the result of the game for game at index */
      void setResult(quint32 index, Result result);
      /** Sets the round of the game for game at index */
      void setRound(quint32 index, quint8 round);
      /** Sets the date of the game for game at index */
      void setDate(quint32 index, const PartialDate& date);


      /** Returns the tag id for the white player name for game at index*/
      quint32 whiteId(quint32 index);
      /** Returns the name of the white player for game at index */
      QString white(quint32 index,Tags& tags);
      /** Returns the tag id for the black player name for game at index*/
      quint32 blackId(quint32 index);
      /** Returns the name of the black player for game at index */
      QString black(quint32 index, Tags& tags);
      /** Returns the tag id for the event name for game at index*/
      quint32 eventId(quint32 index);
      /** Returns the name of the event for game at index */
      QString event(quint32 index, Tags& tags);
      /** Returns the tag id for the site name for game at index*/
      quint32 siteId(quint32 index);
      /** Returns the name of the site for game at index */
      QString site(quint32 index, Tags& tags);
      /** Returns the Elo rating of the white player for game at index*/
      quint16 whiteElo(quint32 index);
      /** Returns the Elo rating of the black player for game at index*/
      quint16 blackElo(quint32 index);
      /** Returns tag id for the ECO code of the game for game at index*/
      quint16 ecoId(quint32 index);
      /** Returns the name of the ECO code for game at index */
      QString eco(quint32 index, Tags& tags);
      /** Returns the date of the game for game at index*/
      PartialDate date(quint32 index);
      /** Returns the date in string format for game at index */
      QString dateString(quint32 index);
      /** Returns the result of the game for game at index*/
      Result result(quint32 index);
      /** Returns the round of the game for game at index*/
      quint8 round(quint32 index);

      /** Returns a reference to the IndexItem instance for game at index*/
      IndexItem& gameIndex(quint32 index);
      /** Returns true if game at 'gameId' matches search 'search'. 
       * Needs reference to the relevant Tags instance to resolve tag values */
      bool find (quint32 gameId, Search* search, Tags& tags);

   private:
      QVector<IndexItem> m_gameIndex;

};

#endif   // __INDEX_H__

