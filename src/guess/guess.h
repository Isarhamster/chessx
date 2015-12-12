#ifndef GUESS_H
#define GUESS_H

#include "movelist.h"

namespace Guess
{
typedef struct Result
{
    int error;
    int from, to;
    int score;

    Result()
    {
        error = -1;
        score = 0;
    }
} Result;

int scorePosFromFen(const char* fen);
Result guessMove(const char* fen, bool chess960, int square, MoveList& mlist, int thinkTime = 25);
Result evalPos(const char* fen, bool chess960, int thinkTime = 125);
int pickBest(const char* fen, bool chess960, int from1, int to1, int from2, int to2, int ms);
void setGuessAllowed(bool allow);
bool guessAllowed();
}

#endif

