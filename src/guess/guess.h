#ifndef GUESS_H
#define GUESS_H

#include "movelist.h"

namespace Guess
{
typedef struct
{
    int error;
    int from, to;
    int score;
    bool whiteMove;
} Result;

int scorePosFromFen(const char* fen);
Result guessMove(const char* fen, int square, MoveList& mlist, int thinkTime = 25);
Result evalPos(const char* fen, int thinkTime = 125);
int pickBest(const char* fen, int from1, int to1, int from2, int to2, int ms);
void setGuessAllowed(bool allow);
}

#endif

