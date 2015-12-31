/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

bool isStandardTag(QString tag)
{
    for(int i = 0; i < 7; ++i)
    {
        if (tag.compare(StandardTags[i])==0)
        {
            return true;
        }
    }
    return false;
}


