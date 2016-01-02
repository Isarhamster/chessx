/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ELOSEARCH_H
#define ELOSEARCH_H

#include "search.h"
#include <QBitArray>

/** @ingroup Search
The EloSearch class is used for searching both white and black player Elo. */
class EloSearch : public Search
{
    Q_OBJECT

public:
    /** Standard constructor. */
    EloSearch(Database* database, int minWhiteElo = 0, int maxWhiteElo = 4000,
              int minBlackElo = 0, int maxBlackElo = 4000);
    /** @return minimum Elo of white player. */
    int minWhiteElo() const;
    /** @return maximum Elo of white player. */
    int maxWhiteElo() const;
    /** @return minimum Elo of black player. */
    int minBlackElo() const;
    /** @return maximum Elo of black player. */
    int maxBlackElo() const;
    /** Set acceptable rating ranges. */
    void setEloSearch(int minWhiteElo = 0, int maxWhiteElo = 4000, int minBlackElo =
                          0, int maxBlackElo = 4000);
    void initialize();
    /** Return true if the game at index matches the search */
    virtual int matches(GameId index);

private:
    int m_minWhiteElo;
    int m_maxWhiteElo;
    int m_minBlackElo;
    int m_maxBlackElo;
    QBitArray m_matches;
};

#endif // ELOSEARCH_H
