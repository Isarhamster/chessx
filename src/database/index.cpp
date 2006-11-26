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

#include "index.h"

Index::Index()
{
   m_gameIndex.clear();
}

Q_UINT32 Index::add (IndexItem item)
{
   m_gameIndex.append(item);
   return m_gameIndex.count()-1;
}

Q_UINT32 Index::add ()
{
   IndexItem item;
   m_gameIndex.append(item);
   return m_gameIndex.count()-1;
}

void Index::setBlackId(Q_UINT32 index, Q_UINT32 id)
{
   m_gameIndex[index].setBlackId(id);
}
void Index::setWhiteId(Q_UINT32 index, Q_UINT32 id)
{
   m_gameIndex[index].setWhiteId(id);
}
void Index::setEventId(Q_UINT32 index, Q_UINT32 id)
{
   m_gameIndex[index].setEventId(id);
}
void Index::setSiteId(Q_UINT32 index, Q_UINT32 id)
{
   m_gameIndex[index].setSiteId(id);
}
void Index::setWhiteElo(Q_UINT32 index, Q_UINT16 elo)
{
   m_gameIndex[index].setWhiteElo(elo);
}
void Index::setBlackElo(Q_UINT32 index, Q_UINT16 elo)
{
   m_gameIndex[index].setBlackElo(elo);
}
void Index::setEcoId(Q_UINT32 index, Q_UINT16 id)
{
   m_gameIndex[index].setEcoId(id);
}
void Index::setResult(Q_UINT32 index, Result result)
{
   m_gameIndex[index].setResult(result);
}
void Index::setRound(Q_UINT32 index, Q_UINT8 round)
{
   m_gameIndex[index].setRound(round);
}
void Index::setDate(Q_UINT32 index, const PartialDate& date)
{
   m_gameIndex[index].setDate(date);
}


IndexItem& Index::gameIndex(Q_UINT32 index)
{
   return m_gameIndex[index];
}

void Index::remove()
{
}

bool Index::find (Q_UINT32 gameId, Search* search, Tags& tags)
{
   QString tagName;
   QVector<bool> vb;
   int tagId;
   TagSearch* ts;
   EloSearch* es;
   DateSearch* ds;
   switch (search->type()) {
      case Search::TagSearch:
         ts = static_cast<TagSearch*> (search);
         tagName = ts->tag();
         if ((tagName == "White") || (tagName == "Black")) {
            tagName = "Name";
         } else if ((tagName == "BlackElo") || (tagName == "WhiteElo")) {
            tagName = "Elo";
         }
         tagId = tags.tagId(tagName);
         if (tagId == -1) {
            return false;
         }
         vb = tags.find(tagId,ts->value());
         //m_gameIndex[gameId].
         tagName = ts->tag();
         if (tagName == "White") {
            if (vb[m_gameIndex[gameId].whiteId()]) {
               return true; 
            }
         } else if (tagName == "Black") {
            if (vb[m_gameIndex[gameId].blackId()]) {
               return true; 
            }
         } else if (tagName == "Site") {
            if (vb[m_gameIndex[gameId].siteId()]) {
               return true; 
            }
         } else if (tagName == "Event") {
            if (vb[m_gameIndex[gameId].eventId()]) {
               return true; 
            }
         } else if (tagName == "Eco") {
            if (vb[m_gameIndex[gameId].ecoId()]) {
               return true; 
            }
         }
         return false;
      case Search::EloSearch:
         es = static_cast<EloSearch*> (search);
         return es->withinEloRange(m_gameIndex[gameId].whiteElo(), m_gameIndex[gameId].blackElo());
      case Search::DateSearch:
         ds = static_cast<DateSearch*> (search);
         return ds->withinDateRange(m_gameIndex[gameId].date());
      default :
         // Unhandled search
         break;
   }

   return false;

}

Q_UINT32 Index::whiteId(Q_UINT32 index)
{
   return m_gameIndex[index].whiteId();
}
QString Index::white(Q_UINT32 index,Tags& tags)
{
   return tags.value(Tags::PlayerName,m_gameIndex[index].whiteId());
}
Q_UINT32 Index::blackId(Q_UINT32 index)
{
   return m_gameIndex[index].blackId();
}
QString Index::black(Q_UINT32 index, Tags& tags)
{
   return tags.value(Tags::PlayerName,m_gameIndex[index].blackId());
}
Q_UINT32 Index::eventId(Q_UINT32 index)
{
   return m_gameIndex[index].eventId();
}
QString Index::event(Q_UINT32 index, Tags& tags)
{
   return tags.value(Tags::Event,m_gameIndex[index].eventId());
}
Q_UINT32 Index::siteId(Q_UINT32 index)
{
   return m_gameIndex[index].siteId();
}
QString Index::site(Q_UINT32 index, Tags& tags)
{
   return tags.value(Tags::Site,m_gameIndex[index].siteId());
}
Q_UINT16 Index::whiteElo(Q_UINT32 index)
{
   return m_gameIndex[index].whiteElo();
}
Q_UINT16 Index::blackElo(Q_UINT32 index)
{
   return m_gameIndex[index].blackElo();
}
Q_UINT16 Index::ecoId(Q_UINT32 index)
{
   return m_gameIndex[index].ecoId();
}
QString Index::eco(Q_UINT32 index, Tags& tags)
{
   return tags.value(Tags::ECO, m_gameIndex[index].ecoId());
}
PartialDate Index::date(Q_UINT32 index)
{
   return m_gameIndex[index].date();
}
QString Index::dateString(Q_UINT32 index)
{
   return m_gameIndex[index].date().asString();
}

Result Index::result(Q_UINT32 index)
{
   return m_gameIndex[index].result();
}
Q_UINT8 Index::round(Q_UINT32 index)
{
   return m_gameIndex[index].round();
}
