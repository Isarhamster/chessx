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

quint32 Index::add (IndexItem item)
{
   m_gameIndex.append(item);
   return m_gameIndex.count()-1;
}

quint32 Index::add ()
{
   IndexItem item;
   m_gameIndex.append(item);
   return m_gameIndex.count()-1;
}

void Index::setBlackId(quint32 index, quint32 id)
{
   m_gameIndex[index].setBlackId(id);
}
void Index::setWhiteId(quint32 index, quint32 id)
{
   m_gameIndex[index].setWhiteId(id);
}
void Index::setEventId(quint32 index, quint32 id)
{
   m_gameIndex[index].setEventId(id);
}
void Index::setSiteId(quint32 index, quint32 id)
{
   m_gameIndex[index].setSiteId(id);
}
void Index::setWhiteElo(quint32 index, quint16 elo)
{
   m_gameIndex[index].setWhiteElo(elo);
}
void Index::setBlackElo(quint32 index, quint16 elo)
{
   m_gameIndex[index].setBlackElo(elo);
}
void Index::setEcoId(quint32 index, quint16 id)
{
   m_gameIndex[index].setEcoId(id);
}
void Index::setResult(quint32 index, Result result)
{
   m_gameIndex[index].setResult(result);
}
void Index::setRound(quint32 index, quint8 round)
{
   m_gameIndex[index].setRound(round);
}
void Index::setDate(quint32 index, const PartialDate& date)
{
   m_gameIndex[index].setDate(date);
}


IndexItem& Index::gameIndex(quint32 index)
{
   return m_gameIndex[index];
}

void Index::remove()
{
}

bool Index::find (quint32 gameId, Search* search, Tags& tags)
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

quint32 Index::whiteId(quint32 index)
{
   return m_gameIndex[index].whiteId();
}
QString Index::white(quint32 index,Tags& tags)
{
   return tags.value(Tags::PlayerName,m_gameIndex[index].whiteId());
}
quint32 Index::blackId(quint32 index)
{
   return m_gameIndex[index].blackId();
}
QString Index::black(quint32 index, Tags& tags)
{
   return tags.value(Tags::PlayerName,m_gameIndex[index].blackId());
}
quint32 Index::eventId(quint32 index)
{
   return m_gameIndex[index].eventId();
}
QString Index::event(quint32 index, Tags& tags)
{
   return tags.value(Tags::Event,m_gameIndex[index].eventId());
}
quint32 Index::siteId(quint32 index)
{
   return m_gameIndex[index].siteId();
}
QString Index::site(quint32 index, Tags& tags)
{
   return tags.value(Tags::Site,m_gameIndex[index].siteId());
}
quint16 Index::whiteElo(quint32 index)
{
   return m_gameIndex[index].whiteElo();
}
quint16 Index::blackElo(quint32 index)
{
   return m_gameIndex[index].blackElo();
}
quint16 Index::ecoId(quint32 index)
{
   return m_gameIndex[index].ecoId();
}
QString Index::eco(quint32 index, Tags& tags)
{
   return tags.value(Tags::ECO, m_gameIndex[index].ecoId());
}
PartialDate Index::date(quint32 index)
{
   return m_gameIndex[index].date();
}
QString Index::dateString(quint32 index)
{
   return m_gameIndex[index].date().asString();
}

Result Index::result(quint32 index)
{
   return m_gameIndex[index].result();
}
quint8 Index::round(quint32 index)
{
   return m_gameIndex[index].round();
}
