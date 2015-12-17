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
Result guessMove(const char* fen, bool chess960, quint64 castlingRooks, squareT square, MoveList& mlist, int thinkTime = 25);
Result evalPos(const char* fen, bool chess960, quint64 castlingRooks, int thinkTime = 125);
int pickBest(const char* fen, bool chess960, quint64 castlingRooks, squareT from1, squareT to1, squareT from2, squareT to2, int ms);
void setGuessAllowed(bool allow);
bool guessAllowed();
}

#endif

