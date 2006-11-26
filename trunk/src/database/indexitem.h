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

#include <qglobal.h>
#include "partialdate.h"
#include "qlist.h"
#include "common.h"

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
      void setBlackId(Q_UINT32 id);
      /** Sets the tag id for the white player name */
      void setWhiteId(Q_UINT32 id);
      /** Sets the tag id for the event name */
      void setEventId(Q_UINT32 id);
      /** Sets the tag id for the site name */
      void setSiteId(Q_UINT32 id);
      /** Sets the Elo rating of the white player */
      void setWhiteElo(Q_UINT16 elo);
      /** Sets the Elo rating of the black player */
      void setBlackElo(Q_UINT16 elo);
      /** Sets the tag id for the ECO code of the game */
      void setEcoId(Q_UINT16 id);
      /** Sets the result of the game */
      void setResult(Result result);
      /** Sets the round of the game */
      void setRound(Q_UINT8 round);
      /** Sets the date of the game */
      void setDate(const PartialDate& date);
      /** Returns the tag id for the black player name */
      Q_UINT32 blackId();
      /** Returns the tag id for the white player name */
      Q_UINT32 whiteId();
      /** Returns the tag id for the event name */
      Q_UINT32 eventId();
      /** Returns the tag id for the site name */
      Q_UINT32 siteId();
      /** Returns the Elo rating of the white player */
      Q_UINT16 whiteElo();
      /** Returns the Elo rating of the black player */
      Q_UINT16 blackElo();
      /** Returns tag id for the ECO code of the game */
      Q_UINT16 ecoId();
      /** Returns the round of the game */
      Q_UINT8 round();
      /** Returns the result of the game */
      Result result();
      /** Returns the date of the game */
      PartialDate date();

      /** Static method to test whether tag name is supported in the index */
      static bool isTagNameSupported( const QString& tagName);
      /** Static method to populate list of supported tags */
      static void setSupportedTagName();


   private:

      Q_UINT32 m_whiteId; // 4 bytes
      Q_UINT32 m_blackId; // 4 bytes
      Q_UINT32 m_eventId; // 4 bytes
      Q_UINT32 m_siteId; // 4 bytes
      Q_UINT16 m_whiteElo; // 2 bytes
      Q_UINT16 m_blackElo; // 2 bytes
      Q_UINT16 m_ecoId; // 2 bytes
      Result m_result; // 1 bytes
      Q_UINT8 m_round; // 1 bytes
      PartialDate m_date; // 4 bytes? (could use a date Id?)

      // Total 28 bytes / 
      static QList<QString> m_supportedTagNames;

};

#endif	// __INDEXITEM_H__

