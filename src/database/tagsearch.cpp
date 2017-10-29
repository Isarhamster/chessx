/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "tagsearch.h"
#include "database.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* TagSearch class
 * ***************/
TagSearch::TagSearch(Database* database, const QString& tag, const QString& value):Search(database)
{
    if (value.contains('|'))
    {
        QStringList l = value.split('|', QString::SkipEmptyParts);
        QSet<QString> set;
        foreach (QString s, l)
        {
            set.insert(s);
        }
        m_matches = database->index()->listInSet(tag, set);
    }
    else
    {
        m_matches = database->index()->listPartialValue(tag, value);
    }
}

TagSearch::TagSearch(Database* database, const QString& tag, const QString& minValue, const QString& maxValue):Search(database)
{
    m_matches = database->index()->listInRange(tag, minValue, maxValue);
}

TagSearch::TagSearch(Database* database, const QString& tag, int minValue, int maxValue):Search(database)
{
    m_matches = database->index()->listInRange(tag, minValue, maxValue);
}

int TagSearch::matches(GameId index) const
{
    return m_matches.at(index);
}
