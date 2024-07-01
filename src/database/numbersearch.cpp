/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "numbersearch.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* Number class
 * ***************/
NumberSearch::NumberSearch(Database* database, GameId start, GameId end):Search(database)
{
    setRange(start, end);
}

NumberSearch::NumberSearch(Database* database, const QString& range):Search(database)
{
    setRange(range);
}

void NumberSearch::setRange(GameId start, GameId end)
{
    m_start = start - 1;
    m_end = end - 1;
}

void NumberSearch::setRange(const QString& range)
{
    int sep = range.indexOf('-');
    if(sep != -1)
    {
        m_start = range.left(sep).toInt() - 1;
        m_end = range.mid(sep + 1).toInt() - 1;
    }
    else
    {
        m_start = m_end = range.toInt() - 1;
    }
}

int NumberSearch::matches(GameId index) const
{
    return index >= m_start && index <= m_end;
}
