
namespace Guess
{
	typedef struct {
		int error;
		int from, to;
	} Result;

	Result guessMove(const char* fen, int square);
}

