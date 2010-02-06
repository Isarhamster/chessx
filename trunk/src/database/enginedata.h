/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#ifndef ENGINEDATA_H
#define ENGINEDATA_H

#include <QtCore>

enum EngineProtocol {WinBoard, UCI};

class EngineData {
public:
	 EngineData(const QString& newName = "");
	 QString name;
	 QString command;
	 QString options;
	 QString directory;
	 EngineProtocol protocol;
};

#endif // ENGINEDATA_H
