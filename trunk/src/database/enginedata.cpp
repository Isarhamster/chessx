/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "enginedata.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

EngineData::EngineData(const QString& newName) : name(newName), protocol(UCI)
{
}


QString EngineData::commandToName(const QString& command)
{
    QString name = command.section('/', -1, -1);
    if(!name.isEmpty())
    {
        name[0] = name[0].toUpper();
    }
    if(name.endsWith(".exe"))
    {
        name.truncate(name.length() - 4);
    }
    return name;
}
