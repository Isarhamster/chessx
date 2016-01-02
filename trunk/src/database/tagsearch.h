/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TAGSEARCH_H
#define TAGSEARCH_H

#include "search.h"
#include <QBitArray>

/** @ingroup Search
The TagSearch class is used for tag search. Only simple substring searches
are supported for now.

@todo
Improved performance by creating a bit array of matching values */
class TagSearch : public Search
{
    Q_OBJECT

public:
    /** Simple constructor */
    TagSearch(Database* database, const QString& tag, const QString& value, bool partial = true);
    /** Range constructor */
    TagSearch(Database* database, const QString& tag, const QString& value, const QString& value2);
    /** Range constructor */
    TagSearch(Database *database, const QString &tag, int value, int value2);
    /** @return tag to be searched */
    QString tag() const;
    /** @return value to be matched. */
    QString value() const;
    QString minValue() const;
    QString maxValue() const;
    /** Set tag to be searched */
    void setTag(const QString& tag);
    /** Set value to be matched */
    void setValue(const QString& value);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);
    /** initialize the search. Done automatically in constructor, or when values are changed */
    void initialize();

private:
    QString m_tagName;
    QString m_value;
    QString m_value2;
    QBitArray m_matches;
    bool m_bPartial;
};

#endif // TAGSEARCH_H
