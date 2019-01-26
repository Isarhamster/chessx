/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DATESEARCH_H
#define DATESEARCH_H

#include "search.h"
#include "partialdate.h"

/** @ingroup Search
The DataSearch class defines a search based on a date range */
class DateSearch : public Search
{
    Q_OBJECT

public:
    /** Standard constructor. */
    DateSearch();
    /** Constructor for searching games in given time period. */
    DateSearch(const PartialDate &minDate, const PartialDate &maxDate);
    /** @return beginning of the acceptable period. */
    PartialDate minDate() const;
    /** @return end of the acceptable period. */
    PartialDate maxDate() const;
    /** Sets whole period. */
    void setDateRange(const PartialDate &minDate, const PartialDate &maxDate);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index) const;

private:
    PartialDate m_minDate;
    PartialDate m_maxDate;
    QBitArray m_matches;
};

#endif // DATESEARCH_H
