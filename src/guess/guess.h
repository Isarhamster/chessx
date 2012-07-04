#ifndef GUESS_H
#define GUESS_H

#include "movelist.h"

namespace Guess
{
	typedef struct {
		int error;
		int from, to;
	} Result;

    Result guessMove(const char* fen, int square, MoveList& mlist);
	int pickBest(const char* fen, int from1, int to1, int from2, int to2, int ms);
}

#endif

