/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "database.h"
#include "duplicatesearch.h"
#include "index.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* DuplicateSearch class
 * **********************/
DuplicateSearch::DuplicateSearch(Database *db, DSMode mode):Search(db),m_filter(0)
{
   m_mode = mode;
}

DuplicateSearch::DuplicateSearch(Filter *filter, DSMode mode):Search(filter ? filter->database():0)
{
   m_mode = mode;
   m_filter = filter;
}

void DuplicateSearch::PrepareFilter(volatile bool &breakFlag)
{
    const Index* index = m_database->index();
    for (GameId i = 0; (int)i<index->count(); ++i)
    {
        if (!m_filter->contains(i)) continue;
        if (index->deleted(i)) continue; // Do not analyse deleted games
        if (breakFlag) break;
        unsigned int hashval = index->hashIndexItem(i);
        bool found = false;
        if (m_hashToGames.contains(hashval))
        {
            foreach(GameId j, m_hashToGames.values(hashval))
            {
                if (index->isIndexItemEqual(i,j))
                {
                    Game gI, gJ;
                    m_database->loadGame(i, gI);
                    m_database->loadGame(j, gJ);
                    found = gI.isEqual(gJ);
                }
            }
        }
        if (!found)
        {
            m_hashToGames.insert(hashval, i);
        }
    }
}

void DuplicateSearch::Prepare(volatile bool &breakFlag)
{
    if (m_database)
    {
        const Index* index = m_database->index();
        m_matches = QBitArray(index->count(), false);
        if (m_filter)
        {
            PrepareFilter(breakFlag);
        }

        for (GameId i = 0; (int)i<index->count(); ++i)
        {
            if (i % 1024 == 0) emit prepareUpdate(i*100/index->count());

            if (index->deleted(i)) continue; // Do not analyse deleted games

            if (breakFlag) break;
            unsigned int hashval = index->hashIndexItem(i);
            if (m_hashToGames.contains(hashval))
            {
                bool found = false;
                foreach(GameId j, m_hashToGames.values(hashval))
                {
                    if (index->isIndexItemEqual(i,j))
                    {
                        if ((m_mode == DS_Both) || (m_mode == DS_Both_All))
                        {
                            Game gI, gJ;
                            m_database->loadGame(i, gI);
                            m_database->loadGame(j, gJ);
                            found = gI.isEqual(gJ);
                            if ((m_mode == DS_Both_All) && found)
                            {
                               m_matches[j] = 1;
                            }
                        }
                        else if (m_mode == DS_Tags_BestGame)
                        {
                            Game gI, gJ;
                            m_database->loadGame(i, gI);
                            m_database->loadGame(j, gJ);
                            if (gJ.isBetterOrEqual(gI))
                            {
                                found = true;
                            }
                            else if (gI.isBetterOrEqual(gJ))
                            {
                                m_matches[j] = 1;
                                m_hashToGames.remove(hashval, i);
                                m_hashToGames.insert(hashval, j);
                                found = true;
                                break;
                            }
                        }
                        else // DS_Tags
                        {
                            found = true;
                        }
                        if (found)
                        {
                            m_matches[i] = 1;
                            break;
                        }
                    }
                }

                if (!found)
                {
                    m_hashToGames.insert(hashval, i);
                }
            }
            else
            {
                if (!m_filter)
                {
                    m_hashToGames.insert(hashval, i);
                }
            }
        }
    }
}

int DuplicateSearch::matches(GameId index)
{
    return m_matches[index];
}

