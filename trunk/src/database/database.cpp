/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include "database.h"

Database::Database() : m_break(false)
{
    connect(&m_index, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
}

Database::~Database()
{
}

Index* Database::index()
{
    return &m_index;
}

const Index* Database::index() const
{
    return &m_index;
}

QString Database::name() const
{
    QFileInfo fi(filename());
    return fi.completeBaseName();
}

