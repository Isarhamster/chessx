/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TAGSEARCH_H
#define TAGSEARCH_H

#include "search.h"
#include <QBitArray>

/** @ingroup Search
The TagSearch class is used for tag search. */

class TagSearch : public Search
{
    Q_OBJECT

public:
    /** Simple constructor */
    TagSearch(Database* database, const QString& tag, const QString& value);
    /** Range constructor */
    TagSearch(Database* database, const QString& tag, const QString& value, const QString& maxValue);
    /** Range constructor */
    TagSearch(Database *database, const QString &tag, int minValue, int maxValue);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index) const;

private:
    QBitArray m_matches;
};

#endif // TAGSEARCH_H
