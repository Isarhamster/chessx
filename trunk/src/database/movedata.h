/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef MOVEDATA_H
#define MOVEDATA_H

#include "game.h"
#include "move.h"

class MoveData
{
public:
    enum MoveType {StandardMove, GameEnd};
    MoveData();
    void addGame(Game& g, Color c, MoveType movetype = StandardMove);
    double percentage() const;
    int averageRating() const;
    int averageYear() const;
    QString san;
    unsigned int count;
    unsigned int result[4];
    long rating;
    unsigned int rated;
    long year;
    unsigned int dated;
    Move move;
    friend bool operator<(const MoveData& m1, const MoveData& m2);
    friend bool operator>(const MoveData& m1, const MoveData& m2);
    friend bool compareMove(const MoveData& m1, const MoveData& m2);
    friend bool compareScore(const MoveData& m1, const MoveData& m2);
    friend bool compareRating(const MoveData& m1, const MoveData& m2);
    friend bool compareYear(const MoveData& m1, const MoveData& m2);
    bool hasPercent() const;
};

bool operator<(const MoveData& m1, const MoveData& m2);
bool operator>(const MoveData& m1, const MoveData& m2);
bool compareMove(const MoveData& m1, const MoveData& m2);
bool compareScore(const MoveData& m1, const MoveData& m2);
bool compareRating(const MoveData& m1, const MoveData& m2);
bool compareYear(const MoveData& m1, const MoveData& m2);

#endif // MOVEDATA_H
