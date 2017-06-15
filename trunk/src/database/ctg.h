/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
*
* Internal structure definitions of ctg
* Code taken from Daydreamer.
* The huffman codes and ctg move decoding are
* based on Stephan Vermeire's ctg code for Brutus.
****************************************************************************/

#ifndef CTG_H
#define CTG_H

#define read_24(buf, pos)   \
    ((buf[pos]<<16) + (buf[(pos)+1]<<8) + (buf[(pos)+2]))
#define read_32(buf, pos)   \
    ((buf[pos]<<24) + (buf[pos+1]<<16) + (buf[(pos)+2]<<8) + (buf[(pos+3)+2]))

typedef struct _page_bounds_t {
    int pad;
    int low;
    int high;
} page_bounds_t;

typedef struct _ctg_signature_t {
    unsigned char buf[64];
    int buf_len;
} ctg_signature_t;

typedef struct _ctg_entry_t{
    int num_moves;
    unsigned char moves[100];
    int total;
    int wins;
    int losses;
    int draws;
    int unknown1;
    int avg_rating_games;
    int avg_rating_score;
    int perf_rating_games;
    int perf_rating_score;
    int recommendation;
    int unknown2;
    int comment;
} ctg_entry_t;

typedef struct _ctg_move_t{
    int file_from;
    int file_to;
    int rank_from;
    int rank_to;
} ctg_move_t;


#endif // CTG_H
