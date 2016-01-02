/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "elosearch.h"
#include "database.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/* EloSearch class
 * **********************/
EloSearch::EloSearch(Database* database, int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlackElo):Search(database)
{
    setEloSearch(minWhiteElo, maxWhiteElo, minBlackElo, maxBlackElo);
    initialize();
}

void EloSearch::initialize()
{
    m_matches = m_database->index()->listInRange(TagNameWhiteElo, m_minWhiteElo, m_maxWhiteElo);
    m_matches &= m_database->index()->listInRange(TagNameBlackElo, m_minBlackElo, m_maxBlackElo);
}

void EloSearch::setEloSearch(int minWhiteElo, int maxWhiteElo, int minBlackElo, int maxBlackElo)
{
    m_minWhiteElo = minWhiteElo;
    m_maxWhiteElo = maxWhiteElo;
    m_minBlackElo = minBlackElo;
    m_maxBlackElo = maxBlackElo;
    initialize();
}

int EloSearch::maxWhiteElo() const
{
    return m_maxWhiteElo;
}

int EloSearch::minWhiteElo() const
{
    return m_minWhiteElo;
}

int EloSearch::maxBlackElo() const
{
    return m_maxBlackElo;
}

int EloSearch::minBlackElo() const
{
    return m_minBlackElo;
}

int EloSearch::matches(GameId index)
{
    return m_matches[index];
}
