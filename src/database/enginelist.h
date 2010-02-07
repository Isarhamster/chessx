/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ENGINELIST_H
#define ENGINELIST_H

#include <QtCore>
#include "enginedata.h"


/** The EngineList class contains configuration of all installed engines. */
class EngineList : public QList<EngineData>
{
public:
	/** Standard constructor. */
	EngineList();
	/** Restore configuration from INI file. */
	void restore();
	/** Store configuration in INI file. */
	void save();
	/** Retrieve list of engine names in current order. */
	QStringList names() const;
};

#endif // ENGINELIST_H
