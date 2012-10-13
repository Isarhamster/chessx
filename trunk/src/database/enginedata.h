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
	static QString commandToName(const QString& command);
};

enum OptionType
{
    OPT_TYPE_CHECK,
    OPT_TYPE_SPIN,
    OPT_TYPE_COMBO,
    OPT_TYPE_BUTTON,
    OPT_TYPE_STRING
};

#endif // ENGINEDATA_H
