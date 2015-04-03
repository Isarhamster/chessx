/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "engineoptiondata.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

EngineOptionData::EngineOptionData()
{
}

EngineOptionData* EngineOptionData::FindInList(QString name, QList<EngineOptionData>& list)
{
    QList<EngineOptionData>::iterator it = list.begin();
    while(it != list.end())
    {
        if((*it).m_name == name)
        {
            return &(*it);
        }
        ++it;
    }
    return 0;
}
