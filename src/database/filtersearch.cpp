/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "filtersearch.h"
#include "filter.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* FilterSearch class
 * **********************/
FilterSearch::FilterSearch() : m_filter(0)
{
}

FilterSearch::FilterSearch(Filter* filter) : m_filter(filter)
{
}

bool FilterSearch::contains(GameId game) const
{
    return m_filter->contains(game);
}

Filter* FilterSearch::filter() const
{
    return m_filter;
}

void FilterSearch::setFilter(Filter* filter)
{
    m_filter = filter;
}

int FilterSearch::matches(GameId index)
{
    return m_filter->contains(index);
}

