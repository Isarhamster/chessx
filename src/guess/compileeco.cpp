#include "guess.h"
#include "common.h"
#include "board.h"
#include "game.h"

#include <QFile>
#include <QList>

// Parsed data containers
static QMap<quint64, QString> ecoPositions;
static QMap<quint64, QList<Square> > gtmPositions;

// Number of milliseconds to spend deciding which of two possible moves
//  is the better one for the guess-the-move feature to offer user
#define CALCULATE_MS	20

// Create GTM (Guess-The-Move) hash data.
//
// This new hash has a key on "position + square" and contains the move
// that the guess-the-move feature should display when the user hovers
// the mouse over this square in this position.
//
// It can be looked up quickly by the guess-the-move feature by
// combining the hash for the current position + a unique hash for each
// of the 64 squares.  Thus providing an individual hash key for each
// ECO position / target square combination.
//
// Because we can only offer the user a single choice, if the ECO data
// suggests more than one possibility for a given position-square combo,
// then use the Scid internal chess engine to decide which is the "best"
// move to display to the user.
//
void updateFinalGuess(const Board& board, int target, const Move& move)
{
	int from = move.from();
	int to = move.to();

	QList<Square> data;
	data << from << to;

	quint64 key = board.getHashPlusSquare(target);
	if (gtmPositions.contains(key)) {
		int ofrom = gtmPositions[key][0];
		int oto = gtmPositions[key][1];
		// For some reason the ECO data has a few dupes
		if (from == ofrom && to == oto)
			return;
		int ret = Guess::pickBest(
				qPrintable(board.toFen()),
				from, to, ofrom, oto,
				CALCULATE_MS);
		if (ret == 0)
			gtmPositions[key] = data;
		else if (ret < 0) {
			qDebug() << "ERROR deciding between moves..";
			qDebug() << board.toFen()
				<< target
				<< from << to
				<< gtmPositions[key][0] << gtmPositions[key][1];
		}
	} else	gtmPositions[key] = data;
}

bool parseAsciiEcoData(const QString& ecoFile)
{
	ecoPositions.clear();

	QFile file(ecoFile);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	QTextStream ecoStream(&file);
	ecoStream.setCodec("ISO-8859-1");

	QString line;
	Board board;
	QString ecoCode;
	QRegExp ecoRegExp("[A-Z]\\d{2}[a-z]?");
	QStringList tokenList;
	QString token;
	Move move;

	while (!ecoStream.atEnd()) {
		line = ecoStream.readLine();

		//ignore comments and blank lines
		if (line.startsWith("#") || line == "") {
			continue;
		}

		//if line starts with eco code, store and begin new line
		if (line.indexOf(ecoRegExp) == 0) {
			ecoCode = line.section(' ', 0, 0);
			ecoCode += " " + line.section('"', 1, 1);
			board.setStandardPosition();
			line = line.section('"', 2);
		}

		//parse any moves on line
		tokenList = line.split(" ");
		for (QStringList::Iterator iterator = tokenList.begin(); iterator != tokenList.end(); iterator++) {
			token = *iterator;
			if (token == "*") {
				// Record final position of this variation along with its ECO code
				ecoPositions.insert(board.getHashValue(), ecoCode);

				if (!move.isLegal())
					continue;

				// Guess the move is based on second-to-last move, so undo last move
				Board guess(board);
				guess.undoMove(move);

				// We update twice because user might put mouse over
				//  the "to" or "from" squares.
				updateFinalGuess(guess, move.to(), move);
				updateFinalGuess(guess, move.from(), move);

				continue;
			}
			if (token.contains('.')) {
				token = token.section('.', 1, 1);
			}
			if (token != "") {
				move = board.parseMove(token);
				if (move.isLegal()) {
					board.doMove(move);
				} else {
					qDebug() << "failed on" << token;
					ecoPositions.clear();
					return false;
				}
			}
		}

	}

	return true;
}

// "chessx.eco.txt" -> "chessx.eco", "chessx.gtm"
bool compileAsciiEcoFile(const QString& filenameIn, const QString& filenameOut, const QString& gtmFile)
{
    // Read in the ECO data
    if (!parseAsciiEcoData(filenameIn))
    {
        return false;
    }

	// Write out the main ECO file
    QFile file(filenameOut);
	file.open(QIODevice::WriteOnly);
	QDataStream sout(&file);
	sout << COMPILED_ECO_FILE_ID;
	sout << ecoPositions;
	file.close();

	// Write out the GTM (guess-the-move) ECO file
    QFile gfile(gtmFile);
	gfile.open(QIODevice::WriteOnly);
	QDataStream gout(&gfile);
	gout << COMPILED_GUESS_FILE_ID;
	gout << gtmPositions;
	gfile.close();

    ecoPositions.clear();
    gtmPositions.clear();

    return true;
}
