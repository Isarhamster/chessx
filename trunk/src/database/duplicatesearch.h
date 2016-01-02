/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DUPLICATESEARCH_H
#define DUPLICATESEARCH_H

class Filter;

#include "search.h"
#include <QBitArray>
#include <QMultiHash>

/** @ingroup Search
The DuplicateSearch class defines a search for duplicates within a database */
class DuplicateSearch : public Search
{
    Q_OBJECT

public:
    typedef enum _DSMode
    {
        DS_Tags,
        DS_Tags_BestGame,
        DS_Both,
        DS_Both_All
    } DSMode;

    /** Standard constructor. */
    DuplicateSearch(Database* db=0, DSMode mode=DS_Both);
    DuplicateSearch(Filter *filter=0, DSMode mode=DS_Both_All);
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);

    virtual void Prepare(volatile bool& breakFlag);
    void PrepareFilter(volatile bool& breakFlag);

private:
    QMultiHash<quint64, GameId> m_hashToGames;
    QBitArray m_matches;
    DSMode m_mode;
    Filter* m_filter;
};

#endif // DUPLICATESEARCH_H
