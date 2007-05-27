
namespace Guess
{
	typedef struct {
		int error;
		int from, to;
	} Result;

	Result guessMove(const char* fen, int square);
        int pickBest(const char* fen, int from1, int to1, int from2, int to2, int ms);
}

