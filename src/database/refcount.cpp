/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "refcount.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

void RefCounter::addRef()
{
    ++references;
}

bool RefCounter::deleteRef()
{
    --references;
    return (references == 0);
}

int RefCounter::getReferences() const
{
    return references;
}
