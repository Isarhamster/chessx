/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "tags.h"

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


