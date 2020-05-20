
// move.cpp

// includes

#ifdef WINCE
#include <stdlib.h>
#else
#include <cstdlib>
#endif
#include <cstring>

#include "attack.h"
#include "colour.h"
#include "list.h"
#include "move.h"
#include "move_do.h"
#include "move_gen.h"
#include "move_legal.h"
#include "option.h"
#include "piece.h"
#include "square.h"
#include "util.h"

// "constants"

static const uint8 PromotePiece[5] = { PieceNone64, Knight64, Bishop64, Rook64, Queen64 };

// functions

// move_is_ok()

bool move_is_ok(int move) {

   if (move < 0 || move >= 65536) return false;

   if (move == MoveNone) return false;

   return true;
}

// move_make()

int move_make(int from, int to) {

   ASSERT(square_is_ok(from));
   ASSERT(square_is_ok(to));

   return (square_to_64(from) << 6) | square_to_64(to);
}

// move_make_flags()

int move_make_flags(int from, int to, int flags) {

   ASSERT(square_is_ok(from));
   ASSERT(square_is_ok(to));
   ASSERT((flags&~0xF000)==0);

   ASSERT(to!=from);

   return (square_to_64(from) << 6) | square_to_64(to) | flags;
}

// move_from()

int move_from(int move) {

   int from_64;

   ASSERT(move_is_ok(move));

   from_64 = (move >> 6) & 077;

   return square_from_64(from_64);
}

// move_to()

int move_to(int move) {

   int to_64;

   ASSERT(move_is_ok(move));

   to_64 = move & 077;

   return square_from_64(to_64);
}

// move_promote_hack()

int move_promote_hack(int move) {

   unsigned int code;

   ASSERT(move_is_ok(move));

   ASSERT(move_is_promote(move));

   code = (unsigned int) move >> 12;
   ASSERT(code>=1&&code<=4);

   return PromotePiece[(code > 4) ? 0 : code];
}

// move_is_capture()

bool move_is_capture(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   if (move_is_en_passant(move,board)) return true;
   if (board->square[move_to(move)] != Empty) return true;

   return false;
}

// move_is_promote()

bool move_is_promote(int move) {

   ASSERT(move_is_ok(move));

   return (move & MoveFlags) != 0;
}

// move_is_en_passant()

bool move_is_en_passant(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   return piece_is_pawn(move_piece(move,board))
       && move_to(move) == board->ep_square;
}

// move_is_castle()

bool move_is_castle(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   return colour_equal(board->square[move_to(move)],board->turn);
}

// move_piece()

int move_piece(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   return board->square[move_from(move)];
}

// move_capture()

int move_capture(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   if (move_is_en_passant(move,board)) {
      return piece_pawn_opp(move_piece(move,board));
   }

   return board->square[move_to(move)];
}

// move_promote()

int move_promote(int move, const board_t * board) {

   int code;

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   if (move_is_promote(move)) {
      code = move >> 12;
      ASSERT(code>=1&&code<=4);
      return PromotePiece[code] | board->turn;
   }

   return Empty;
}

// move_is_check()

bool move_is_check(int move, const board_t * board) {

   board_t new_board[1];

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   board_copy(new_board,board);
   move_do(new_board,move);
   ASSERT(!is_in_check(new_board,colour_opp(new_board->turn)));

   return board_is_check(new_board);
}

// move_is_mate()

bool move_is_mate(int move, const board_t * board) {

   board_t new_board[1];

   ASSERT(move_is_ok(move));
   ASSERT(board_is_ok(board));

   board_copy(new_board,board);
   move_do(new_board,move);
   ASSERT(!is_in_check(new_board,colour_opp(new_board->turn)));

   return board_is_mate(new_board);
}


// move_order()

int move_order(int move) {

   ASSERT(move_is_ok(move));

   return ((move & 07777) << 3) | (move >> 12); // from, to, promote
}

// end of move.cpp

