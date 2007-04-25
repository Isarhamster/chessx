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

}

