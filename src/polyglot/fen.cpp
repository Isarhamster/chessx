
// fen.cpp

// includes

#include <cctype>
#include <cstdio>

#ifdef WINCE
#include <stdlib.h>
#else
#include <cstdlib>
#endif

#include "board.h"
#include "colour.h"
#include "fen.h"
#include "option.h"
#include "piece.h"
#include "square.h"
#include "util.h"

// "constants"

// const char * StartFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w HAha - 0 1";
const char * StartFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// variables

static const bool Strict = false;

// functions

// board_from_fen()

bool board_from_fen(board_t * board, const char string[]) {

   int pos;
   int file, rank, sq;
   int c;
   int i, len;
   int piece;
   int king_pos[ColourNb];

   ASSERT(board!=NULL);
   ASSERT(string!=NULL);

   board_clear(board);

   king_pos[White] = SquareNone;
   king_pos[Black] = SquareNone;

   pos = 0;
   c = string[pos];

   // piece placement

   for (rank = 7; rank >= 0; rank--) {

      for (file = 0; file < 8;) {

         sq = square_make(file,rank);

         if (c >= '1' && c <= '8') { // empty square(s)

            len = c - '0';
            if (file + len > 8) my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);

            for (i = 0; i < len; i++) {
               board->square[sq++] = Empty;
               file++;
            }

         } else { // piece

            piece = piece_from_char(c);
            if (piece == PieceNone256) my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);

            if (piece_is_king(piece)) king_pos[piece_colour(piece)] = sq;

            board->square[sq++] = piece;
            file++;
         }

         c = string[++pos];
      }

      if (rank > 0) {
         if (c != '/') my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
         c = string[++pos];
     }
   }

   // active colour

   if (c != ' ') my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   c = string[++pos];

   switch (c) {
   case 'w':
      board->turn = White;
      break;
   case 'b':
      board->turn = Black;
      break;
   default:
      my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
      break;
   }

   c = string[++pos];

   // castling

   if (c != ' ') my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   c = string[++pos];

   board->castle[White][SideH] = SquareNone;
   board->castle[White][SideA] = SquareNone;
   board->castle[Black][SideH] = SquareNone;
   board->castle[Black][SideA] = SquareNone;

   if (c == '-') { // no castling rights

      c = string[++pos];

   } else {

      // TODO: filter out illegal rights

      do {

         if (false) {

         } else if (c == 'K') {

            for (sq = H1; sq > king_pos[White]; sq--) {
               if (board->square[sq] == WhiteRook256) {
                  board->castle[White][SideH] = sq;
                  break;
               }
            }

         } else if (c == 'Q') {

            for (sq = A1; sq < king_pos[White]; sq++) {
               if (board->square[sq] == WhiteRook256) {
                  board->castle[White][SideA] = sq;
                  break;
               }
            }

         } else if (c == 'k') {

            for (sq = H8; sq > king_pos[Black]; sq--) {
               if (board->square[sq] == BlackRook256) {
                  board->castle[Black][SideH] = sq;
                  break;
               }
            }

         } else if (c == 'q') {

            for (sq = A8; sq < king_pos[Black]; sq++) {
               if (board->square[sq] == BlackRook256) {
                  board->castle[Black][SideA] = sq;
                  break;
               }
            }

         } else if (c >= 'A' && c <= 'H') {

            // white castling right

            sq = square_make(file_from_char(tolower(c)),Rank1);

            if (sq > king_pos[White]) { // h side
               board->castle[White][SideH] = sq;
            } else { // a side
               board->castle[White][SideA] = sq;
            }

         } else if (c >= 'a' && c <= 'h') {

            // black castling right

            sq = square_make(file_from_char(tolower(c)),Rank8);

            if (sq > king_pos[Black]) { // h side
               board->castle[Black][SideH] = sq;
            } else { // a side
               board->castle[Black][SideA] = sq;
            }

         } else {

            my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
         }

         c = string[++pos];

      } while (c != ' ');
   }

   // en-passant

   if (c != ' ') my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   c = string[++pos];

   if (c == '-') { // no en-passant

      sq = SquareNone;
      c = string[++pos];

   } else {

      if (c < 'a' || c > 'h') my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
      file = file_from_char(c);
      c = string[++pos];

      if (c < '1' || c > '8') my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
      rank = rank_from_char(c);
      c = string[++pos];

      sq = square_make(file,rank);
   }

   board->ep_square = sq;

   // halfmove clock

   board->ply_nb = 0;
   board->move_nb = 0; // HACK, in case of broken syntax

   if (c != ' ') {
      if (!Strict) goto update;
      my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   }
   c = string[++pos];

   if (!isdigit(c)) {
      if (!Strict) goto update;
      my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   }

   board->ply_nb = atoi(&string[pos]);
   do c = string[++pos]; while (isdigit(c));

   // fullmove number

   board->move_nb = 0;

   if (c != ' ') {
      if (!Strict) goto update;
      my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   }
   c = string[++pos];

   if (!isdigit(c)) {
      if (!Strict) goto update;
      my_fatal("board_from_fen(): bad FEN (pos=%d)\n",pos);
   }

   board->move_nb = atoi(&string[pos]) - 1;
   do c = string[++pos]; while (isdigit(c));

   // board update

update:
   board_init_list(board);

   return true;
}

// end of fen.cpp

