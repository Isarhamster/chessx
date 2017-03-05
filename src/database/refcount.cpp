#include "refcount.h"

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
