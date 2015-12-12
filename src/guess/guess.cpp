#include "guess.h"
#include "position.h"
#include "guessengine.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

namespace Guess
{

static bool s_guessAllowed = true;

int scorePosFromFen(const char *fen)
{
    Position pos;
    pos.ReadFromFEN(fen);
    return pos.ScoreMaterial();
}

Result guessMove(const char* fen, bool chess960, int square, MoveList& mlist, int thinkTime)
{
    Result r;

    squareT sq = square;

    Position pos;
    pos.ReadFromFEN(fen);
    pos.setChess960Castling(chess960);

    if (!pos.IsLegal()) return r;

    pos.GenerateMoves(&mlist);
    if (sq != 255) mlist.SelectBySquare(sq);
    if(mlist.size() == 0)
    {
        return r;
    }

    if(mlist.size() > 1)
    {
        if (!s_guessAllowed)
        {
            return r;
        }
        Engine engine;
        engine.SetSearchTime(thinkTime);
        engine.SetPosition(&pos);
        r.score = engine.Think(&mlist);
    }

    const simpleMoveT& sm = mlist.at(0);
    ASSERT(sq == 255 || sq == sm.from || sq == sm.to);

    r.from = sm.from;
    r.to = sm.to;
    r.error = 0;
    return r;
}

Result evalPos(const char* fen, bool chess960, int thinkTime)
{
    Result r;

    if (!s_guessAllowed)
    {
        return r;
    }

    MoveList mlist;

    Position pos;
    pos.ReadFromFEN(fen);
    pos.setChess960Castling(chess960);

    Engine engine;
    engine.SetSearchTime(thinkTime);
    engine.SetPosition(&pos);
    r.score = engine.Think(&mlist);
    simpleMoveT * sm = mlist.Get(0);

    r.from = sm->from;
    r.to = sm->to;
    r.error = 0;

    return r;
}

// Use chess engine to decide if (from1,to1) is better than (from2,to2) move
//  it will start from the position given in fen and think for ms milliseconds
//  0 is returned if the first move is better or 1 if the second is better
//  -1 is returned on error
int pickBest(const char* fen, bool chess960, int from1, int to1, int from2, int to2, int ms)
{
    if (!s_guessAllowed) return -1;

    Position pos;
    pos.ReadFromFEN(fen);
    pos.setChess960Castling(chess960);

    MoveList mlist;
    pos.GenerateMoves(&mlist);

    mlist.SelectBySquares(from1, to1, from2, to2);
    if(mlist.size() == 2)
    {
        Engine engine;
        engine.SetSearchTime(ms);    // Do a "ms" millisecond search
        engine.SetPosition(&pos);
        engine.Think(&mlist);

        simpleMoveT * sm = mlist.Get(0);
        if(sm->from == from1 && sm->to == to1)
        {
            return 0;
        }
        if(sm->from == from2 && sm->to == to2)
        {
            return 1;
        }
    }
    return -1;
}

void setGuessAllowed(bool allow)
{
    s_guessAllowed = allow;
}

bool guessAllowed()
{
    return s_guessAllowed;
}

}

