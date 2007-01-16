/***************************************************************************
                          indexitem.h - description
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

#ifndef __INDEXITEM_H__
#define __INDEXITEM_H__

#include "partialdate.h"
#include "common.h"
#include <QList>

/** The IndexItem class holds one item that is listed in a index
 * It contains essential game header information that is kept in
 * memory for fast access. It is used in conjunction with the Tags
 * class. For most items it stores the id that refers to the relevant
 * tag item in the Tags instance. Where convenient, the actual value is
 * stored instead of the id.
 */

class IndexItem {
   public:
      IndexItem();
      /** Sets the tag id for the black player name */
      void setBlackId(quint32 id);
      /** Sets the tag id for the white player name */
      void setWhiteId(quint32 id);
      /** Sets the tag id for the event name */
      void setEventId(quint32 id);
      /** Sets the tag id for the site name */
      void setSiteId(quint32 id);
      /** Sets the Elo rating of the white player */
      void setWhiteElo(quint16 elo);
      /** Sets the Elo rating of the black player */
      void setBlackElo(quint16 elo);
      /** Sets the tag id for the ECO code of the game */
      void setEcoId(quint16 id);
      /** Sets the result of the game */
      void setResult(Result result);
      /** Sets the round of the game */
      void setRound(quint8 round);
      /** Sets the date of the game */
      void setDate(const PartialDate& date);
      /** Returns the tag id for the black player name */
      quint32 blackId();
      /** Returns the tag id for the white player name */
      quint32 whiteId();
      /** Returns the tag id for the event name */
      quint32 eventId();
      /** Returns the tag id for the site name */
      quint32 siteId();
      /** Returns the Elo rating of the white player */
      quint16 whiteElo();
      /** Returns the Elo rating of the black player */
      quint16 blackElo();
      /** Returns tag id for the ECO code of the game */
      quint16 ecoId();
      /** Returns the round of the game */
      quint8 round();
      /** Returns the result of the game */
      Result result();
      /** Returns the date of the game */
      PartialDate date();

      /** Static method to test whether tag name is supported in the index */
      static bool isTagNameSupported( const QString& tagName);
      /** Static method to populate list of supported tags */
      static void setSupportedTagName();


   private:

      quint32 m_whiteId; // 4 bytes
      quint32 m_blackId; // 4 bytes
      quint32 m_eventId; // 4 bytes
      quint32 m_siteId; // 4 bytes
      quint16 m_whiteElo; // 2 bytes
      quint16 m_blackElo; // 2 bytes
      quint16 m_ecoId; // 2 bytes
      Result m_result; // 1 bytes
      quint8 m_round; // 1 bytes
      PartialDate m_date; // 4 bytes? (could use a date Id?)

      // Total 28 bytes / 
      static QList<QString> m_supportedTagNames;

};

#endif	// __INDEXITEM_H__

