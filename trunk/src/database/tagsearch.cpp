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
TagSearch::TagSearch(Database* database, const QString& tag, const QString& value, bool partial):Search(database)
{
    m_tagName = tag;
    m_value = value;
    m_bPartial = partial;
    initialize();
}

TagSearch::TagSearch(Database* database, const QString& tag, const QString& value, const QString& value2):Search(database)
{
    m_tagName = tag;
    m_value = value;
    m_value2 = value2;
    m_bPartial = false;
    m_matches = m_database->index()->listInRange(m_tagName, m_value, m_value2);
}

TagSearch::TagSearch(Database* database, const QString& tag, int value, int value2):Search(database)
{
    m_tagName = tag;
    m_value = QString::number(value);
    m_value2 = QString::number(value2);
    m_bPartial = false;
    m_matches = m_database->index()->listInRange(m_tagName, value, value2);
}

void TagSearch::initialize()
{
    if(m_bPartial)
    {
        m_matches = m_database->index()->listPartialValue(m_tagName, m_value);
    }
    else
    {
        m_matches = m_database->index()->listContainingValue(m_tagName, m_value);
    }
}

QString TagSearch::tag() const
{
    return m_tagName;
}

QString TagSearch::value() const
{
    return m_value;
}

QString TagSearch::minValue() const
{
    return m_value;
}

QString TagSearch::maxValue() const
{
    return m_value2;
}

void TagSearch::setTag(const QString& tag)
{
    m_tagName = tag;
    initialize();
}

void TagSearch::setValue(const QString& value)
{
    m_value = value;
    initialize();
}

int TagSearch::matches(GameId index)
{
    return m_matches[index];
}
