/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

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
