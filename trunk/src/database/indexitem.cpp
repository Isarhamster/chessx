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

#include "indexitem.h"

QList<QString> IndexItem::m_supportedTagNames;

void IndexItem::setSupportedTagName()
{
   m_supportedTagNames.clear();
   m_supportedTagNames.append("White");
   m_supportedTagNames.append("Black");
   m_supportedTagNames.append("Event");
   m_supportedTagNames.append("Site");
   m_supportedTagNames.append("Eco");
   m_supportedTagNames.append("Date");
   m_supportedTagNames.append("WhiteElo");
   m_supportedTagNames.append("BlackElo");
   m_supportedTagNames.append("Result");
   m_supportedTagNames.append("Round");
}
bool IndexItem::isTagNameSupported( const QString& tagName)
{
   if (m_supportedTagNames.isEmpty()) {
      IndexItem::setSupportedTagName();
   }
   return m_supportedTagNames.contains(tagName);

}
IndexItem::IndexItem()
{
      m_whiteId = -1; 
      m_blackId = -1;
      m_eventId = -1;
      m_siteId = -1;
      m_whiteElo = -1;
      m_blackElo = -1;
      m_ecoId = -1;
      m_result = Unknown;
      m_round = -1;
}

void IndexItem::setBlackId(quint32 id)
{
   m_blackId = id;
}

void IndexItem::setWhiteId(quint32 id)
{
   m_whiteId = id;
}
void IndexItem::setEventId(quint32 id)
{
   m_eventId = id;
}
void IndexItem::setSiteId(quint32 id)
{
   m_siteId = id;
}
void IndexItem::setWhiteElo(quint16 elo)
{
   m_whiteElo = elo;
}
void IndexItem::setBlackElo(quint16 elo)
{
   m_blackElo = elo;
}
void IndexItem::setEcoId(quint16 id)
{
   m_ecoId = id;
}
void IndexItem::setDate(const PartialDate& date)
{
   m_date = date;
}
void IndexItem::setResult(Result result)
{
   m_result = result;
}
void IndexItem::setRound(quint8 round)
{
   m_round = round;
}

quint32 IndexItem::blackId()
{
   return m_blackId;
}

quint32 IndexItem::whiteId()
{
   return m_whiteId;
}
quint32 IndexItem::eventId()
{
   return m_eventId;
}
quint32 IndexItem::siteId()
{
   return m_siteId;
}
quint16 IndexItem::whiteElo()
{
   return m_whiteElo;
}
quint16 IndexItem::blackElo()
{
   return m_blackElo;
}
quint16 IndexItem::ecoId()
{
   return m_ecoId;
}
PartialDate IndexItem::date()
{
   return m_date;
}
quint8 IndexItem::round()
{
   return m_round;
}
Result IndexItem::result()
{
   return m_result;
}
