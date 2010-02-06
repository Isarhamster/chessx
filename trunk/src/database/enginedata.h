/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

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
};

#endif // ENGINEDATA_H
