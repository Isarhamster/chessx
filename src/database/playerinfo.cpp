/***************************************************************************
                          playerinfo  -  description
                             -------------------
	copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QMap>

#include "playerinfo.h"
#include "database.h"

PlayerInfo::PlayerInfo()
{
	m_database = 0;
}


PlayerInfo::~PlayerInfo()
{
}

PlayerInfo::PlayerInfo(Database* db, const QString & player)
{
	setDatabase(db);
	setName(player);
	update();
}


QString PlayerInfo::name() const
{
	return m_name;
}

void PlayerInfo::setDatabase(Database* db)
{
	m_database = db;
}

void PlayerInfo::setName(const QString& player)
{
	m_name = player;
}

void PlayerInfo::update()
{
}

void PlayerInfo::reset()
{
	for (int c = 0; c <= AnyColor; c++)
		for (int r = 0; r <= AnyResult; r++)
			m_results[c][r] = 0;
	m_eco[0].clear();
	m_eco[1].clear();
}


