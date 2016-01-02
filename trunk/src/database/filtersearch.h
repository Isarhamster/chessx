/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FILTERSEARCH_H
#define FILTERSEARCH_H

#include <QPointer>
#include "search.h"
class Filter;

/** @ingroup Search
 *  The FilterSearch class is not a real search class. It is used to
 *  combine other searches with a filter, in order to solve the search
 *  tree quicker if possible. For instance if a search is combined with a
 *  filter using the AND operator, only games in the filter are searched.
 *  If it is combined with the OR operator, only games not in the filter
 *  are searched. */
class FilterSearch : public Search
{
    Q_OBJECT

public:
    FilterSearch();
    FilterSearch(Filter* filter);

    bool contains(GameId game) const;
    Filter* filter() const;
    void setFilter(Filter* filter);
    virtual int matches(GameId game);
private:
    QPointer<Filter> m_filter;
};

#endif // FILTERSEARCH_H
