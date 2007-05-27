#include "guess.h"
#include "position.h"
#include "engine.h"

namespace Guess
{
	Result guessMove(const char* fen, int square)
	{
		Result r;
		r.error = -1;

		squareT sq = square;

		Position pos;
		pos.ReadFromFEN(fen);

		MoveList mlist;
		pos.GenerateMoves(&mlist);
		mlist.SelectBySquare(sq);
		if (mlist.Size() == 0)
			return r;

		if (mlist.Size() > 1) {
			Engine * engine = new Engine();
			engine->SetSearchTime(25);    // Do a 25 millisecond search
			engine->SetPosition(&pos);
			engine->Think(&mlist);
			delete engine;
		}

		simpleMoveT * sm = mlist.Get(0);
		ASSERT (sq == sm->from  ||  sq == sm->to);

		r.from = sm->from;
		r.to = sm->to;
		r.error = 0;
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

