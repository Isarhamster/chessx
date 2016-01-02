/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "datesearch.h"
#include "game.h"
#include "database.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* DateSearch class
 * **********************/
DateSearch::DateSearch()
{
    m_minDate = m_maxDate = PartialDate();
}

DateSearch::DateSearch(PartialDate minDate, PartialDate maxDate)
{
    Q_ASSERT(minDate < maxDate);

    m_minDate = minDate;
    m_maxDate = maxDate;
}

PartialDate DateSearch::minDate() const
{
    return m_minDate;
}

PartialDate DateSearch::maxDate() const
{
    return m_maxDate;
}

void DateSearch::setDateRange(PartialDate minDate, PartialDate maxDate)
{
    Q_ASSERT(minDate < maxDate);
    m_minDate = minDate;
    m_maxDate = maxDate;
}

int DateSearch::matches(GameId index)
{
    Game g;
    m_database->loadGameHeaders(index, g);
    PartialDate date(g.tag("Date"));

    return (date >= m_minDate && date <= m_maxDate);
}

