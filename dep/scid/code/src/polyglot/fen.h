
// fen.h

#ifndef FEN_H
#define FEN_H

// includes

#include "board.h"
#include "util.h"

// "constants"

extern const char * StartFen;

// functions

extern bool board_from_fen (board_t * board, const char string[]);

#endif // !defined FEN_H

// end of fen.h

