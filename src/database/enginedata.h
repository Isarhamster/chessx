/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ENGINEDATA_H
#define ENGINEDATA_H

#include <QtCore>
#include <QList>

#include "engineoptiondata.h"

/** The EngineData class contains configuration of a single engine. */
class EngineData {
public:
	enum EngineProtocol {WinBoard, UCI};
	/** Standard constructor. */
	EngineData(const QString& newName = "");
	QString name;
	QString command;
	QString options;
	QString directory;
	EngineProtocol protocol;
    OptionValueList m_optionValues;
	static QString commandToName(const QString& command);

};


#endif // ENGINEDATA_H
