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
FilterSearch::FilterSearch() : m_filter(nullptr)
{
}

FilterSearch::FilterSearch(FilterX* filter) : m_filter(filter)
{
}

bool FilterSearch::contains(GameId game) const
{
    return m_filter->contains(game);
}

FilterX* FilterSearch::filter() const
{
    return m_filter;
}

void FilterSearch::setFilter(FilterX* filter)
{
    m_filter = filter;
}

int FilterSearch::matches(GameId index) const
{
    return m_filter->contains(index);
}

