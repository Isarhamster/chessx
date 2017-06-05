/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ABK_H
#define ABK_H

#pragma pack(1)

struct ABK_MOVE {
    unsigned char   from;
    unsigned char   to;
    unsigned char   promotion;
};

struct ABK_ENTRY {
    ABK_MOVE        move;
    unsigned char   priority;
    unsigned int    games;
    unsigned int    won_games;
    unsigned int    lost_games;
    unsigned int    hz;
    int             first_child;
    int             next_sibling;
};

#pragma pack()

#endif // ABK_H
