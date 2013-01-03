#include "guess.h"
#include "position.h"
#include "guessengine.h"

namespace Guess
{
    Result guessMove(const char* fen, int square, MoveList& mlist, int thinkTime)
	{
		Result r;
		r.error = -1;
        r.score = 0;

		squareT sq = square;

		Position pos;
		pos.ReadFromFEN(fen);

		pos.GenerateMoves(&mlist);
		mlist.SelectBySquare(sq);
		if (mlist.Size() == 0)
			return r;

        if (mlist.Size() > 1) {
			Engine * engine = new Engine();
            engine->SetSearchTime(thinkTime);
			engine->SetPosition(&pos);
            r.score = engine->Think(&mlist);
			delete engine;
		}

		simpleMoveT * sm = mlist.Get(0);
		ASSERT (sq == sm->from  ||  sq == sm->to);

		r.from = sm->from;
		r.to = sm->to;
		r.error = 0;
		return r;
	}

    Result evalPos(const char* fen, int thinkTime)
    {
        Result r;
        MoveList mlist;

        Position pos;
        pos.ReadFromFEN(fen);
        r.whiteMove = (pos.GetToMove() == WHITE);

        Engine * engine = new Engine();
        engine->SetSearchTime(thinkTime);
        engine->SetPosition(&pos);
        r.score = engine->Think(&mlist);
        simpleMoveT * sm = mlist.Get(0);
        ASSERT (sq == sm->from  ||  sq == sm->to);

        r.from = sm->from;
        r.to = sm->to;
        r.error = 0;

        delete engine;
        return r;
    }

    // Use chess engine to decide if (from1,to1) is better than (from2,to2) move
	//  it will start from the position given in fen and think for ms milliseconds
	//  0 is returned if the first move is better or 1 if the second is better
	//  -1 is returned on error
	int pickBest(const char* fen, int from1, int to1, int from2, int to2, int ms)
	{
		Position pos;
		pos.ReadFromFEN(fen);

		MoveList mlist;
		pos.GenerateMoves(&mlist);

		mlist.SelectBySquares(from1, to1, from2, to2);
		if (mlist.Size() == 2) {
			Engine * engine = new Engine();
			engine->SetSearchTime(ms);    // Do a "ms" millisecond search
			engine->SetPosition(&pos);
			engine->Think(&mlist);
			delete engine;
			simpleMoveT * sm = mlist.Get(0);
			if (sm->from == from1 && sm->to == to1)
				return 0;
			if (sm->from == from2 && sm->to == to2)
				return 1;
		}
		return -1;
	}
}

