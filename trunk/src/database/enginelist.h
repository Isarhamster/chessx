/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#ifndef ENGINELIST_H
#define ENGINELIST_H

#include <QtCore>
#include "engine.h"

enum EngineProtocol {WinBoard, UCI};

struct EngineData {
		EngineData() {};
		EngineData(const QString& newName) : name(newName) {};
		QString name;
		QString command;
		QString options;
		QString directory;
		EngineProtocol protocol;
};

class EngineList : public QList<EngineData>
{
public:
    EngineList();
};

#endif // ENGINELIST_H
