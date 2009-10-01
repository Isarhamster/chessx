/***************************************************************************
                          database.cpp  -  abstract database class
                             -------------------
    begin                : 9 April 2006
    copyright            : (C) 2006 William Hoggarth
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

#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include "database.h"

Database::Database() : m_count(0)
{
}

Database::~Database()
{}

Index* Database::index()
{
	return &m_index;
}

QString Database::name() const
{
	QFileInfo fi(filename());
	return fi.completeBaseName();
}

