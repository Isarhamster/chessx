//////////////////////////////////////////////////////////////////////
//
//  FILE:       tkscid.cpp
//              Scid extensions to Tcl/Tk interpreter
//
//  Part of:    Scid (Shane's Chess Information Database)
//
//  Notice:     Copyright (c) 1999-2004 Shane Hudson.  All rights reserved.
//              Copyright (c) 2006-2007 Pascal Georges
//              Copyright (c) 2013-2014 Benini Fulvio
//
//  Scid is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation.
//
//  Scid is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Scid.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////



#include "crosstab.h"
#include "dstring.h"
#include "engine.h"
#include "game.h"
#include "optable.h"
#include "pbook.h"
#include "pgnparse.h"
#include "polyglot.h"
#include "position.h"
#include "probe.h"
#include "scidbase.h"
#include "searchpos.h"
#include "spellchk.h"
#include "stored.h"
#include "timer.h"
#include "tree.h"
#include "dbasepool.h"
#include "ui.h"
#include <algorithm>
#include <cstring>
#include <numeric>
#include <set>

//TODO: delete
#include "tkscid.h"


//TODO: delete
extern scidBaseT* db;
const int MAX_BASES = 9;
/////////////////


static Game * scratchGame = NULL;      // "scratch" game for searches, etc.
static std::unique_ptr<PBook> ecoBook; // eco classification pbook.
static SpellChecker* spellChk;         // Name correction.
static OpTable * reports[2] = {NULL, NULL};

void scid_Exit(void*) {
	DBasePool::closeAll();
	if (scratchGame != NULL) delete scratchGame;
	if (spellChk != NULL) delete spellChk;
	for (size_t i = 0, n = sizeof(reports) / sizeof(reports[0]); i < n; i++) {
		if (reports[i] != NULL) delete reports[i];
	}
}

/*! \mainpage
 * Scid is an open source software released under the GPL licence.
 * The core database library is written in c++ and the GUI uses
 * <a href="http://core.tcl.tk"> the tcl/tk framework</a>.
 *
 * \section Tests
 * Link to <a href="../gcov/index.html">code coverage</a>
 */
int main(int argc, char* argv[]) {
	srand(time(NULL));

	scratchGame = new Game;
	DBasePool::init();

	return UI_Main(argc, argv, scid_Exit);
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global variables:
static const char * reportTypeName[2] = { "opening", "player" };
static const uint REPORT_OPENING = 0;
static const uint REPORT_PLAYER = 1;

static char decimalPointChar = '.';
static uint htmlDiagStyle = 0;

// Tablebase probe modes:
#define PROBE_NONE 0
#define PROBE_RESULT 1
#define PROBE_SUMMARY 2
#define PROBE_REPORT 3
#define PROBE_OPTIMAL 4



//////////////////////////////////////////////////////////////////////
//
// Inline routines for setting Tcl result strings:
//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setResult():
//    Inline function to set the Tcl interpreter result to a
//    constant string.
inline int
setResult (Tcl_Interp * ti, const char * str)
{
    Tcl_SetResult (ti, (char *) str, TCL_STATIC);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setIntResult():
//    Inline function to set the Tcl interpreter result to a
//    signed integer value.
inline int
setIntResult (Tcl_Interp * ti, int i)
{
    char temp [20];
    sprintf (temp, "%d", i);
    Tcl_SetResult (ti, temp, TCL_VOLATILE);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setUintResult():
//    Inline function to set the Tcl interpreter result to an
//    unsigned integer value.
inline int
setUintResult (Tcl_Interp * ti, uint i)
{
    char temp [20];
    sprintf (temp, "%u", i);
    Tcl_SetResult (ti, temp, TCL_VOLATILE);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// appendUintResult:
//    Inline function to append the specified unsigned value to the
//    Tcl interpreter result.
inline int
appendUintResult (Tcl_Interp * ti, uint i)
{
    char temp [20];
    sprintf (temp, "%u", i);
    Tcl_AppendResult (ti, temp, NULL);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// appendUintElement:
//    Inline function to append the specified unsigned value to the
//    Tcl interpreter list result.
inline uint
appendUintElement (Tcl_Interp * ti, uint i)
{
    char temp[20];
    sprintf (temp, "%u", i);
    Tcl_AppendElement (ti, temp);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setUintWidthResult():
//    Inline function to set the Tcl interpreter result to an
//    unsigned integer value, with zeroes to pad to the desired width.
inline int
setUintWidthResult (Tcl_Interp * ti, uint i, uint width)
{
    char temp [20];
    sprintf (temp, "%0*u", width, i);
    Tcl_SetResult (ti, temp, TCL_VOLATILE);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// appendCharResult:
//    Inline function to append the specified character value to the
//    Tcl interpreter result.
inline int
appendCharResult (Tcl_Interp * ti, char ch)
{
    char tempStr [4];
    tempStr[0] = ch;
    tempStr[1] = 0;
    Tcl_AppendResult (ti, tempStr, NULL);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// translate:
//    Return the translation for a phrase.
//
inline const char *
translate (Tcl_Interp * ti, const char * name, const char * defaultText)
{
    const char * str = Tcl_GetVar2 (ti, "tr", (char *) name, TCL_GLOBAL_ONLY);
    if (str == NULL) { str = defaultText; }
    return str;
}

inline const char *
translate (Tcl_Interp * ti, const char * name)
{
    return translate (ti, name, name);
}

inline int errorResult (Tcl_Interp * ti, errorT err, const char* errorMsg = 0) {
    if (errorMsg != 0) Tcl_SetResult (ti, (char*) errorMsg, TCL_STATIC);
    ASSERT(err != OK);
    Tcl_SetObjErrorCode(ti, Tcl_NewIntObj(err));
    return TCL_ERROR;
}
inline int errorResult (Tcl_Interp * ti, const char* errorMsg) {
    return errorResult(ti, ERROR_BadArg, errorMsg);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// InvalidCommand():
//    Given a Tcl Interpreter, a major command name (e.g. "sc_base") and
//    a null-terminated array of minor commands, this function sets
//    the interpreter's result to a useful error message listing the
//    available subcommands.
//    Returns TCL_ERROR, so caller can simply:
//        return InvalidCommand (...);
//    instead of:
//        InvalidCommand (...);
//        return TCL_ERROR;
int
InvalidCommand (Tcl_Interp * ti, const char * majorCmd,
                const char ** minorCmds)
{
    ASSERT (majorCmd != NULL);
    Tcl_AppendResult (ti, "Invalid command: ", majorCmd,
                      " has the following minor commands:\n", NULL);
    while (*minorCmds != NULL) {
        Tcl_AppendResult (ti, "   ", *minorCmds, "\n", NULL);
        minorCmds++;
    }
    return TCL_ERROR;
}


/************ End of Tcl result routines ***********/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Standard error messages:
//
const char *
errMsgNotOpen (Tcl_Interp * ti)
{
    return translate (ti, "ErrNotOpen", "This is not an open database.");
}

const char *
errMsgSearchInterrupted (Tcl_Interp * ti)
{
    return translate (ti, "ErrSearchInterrupted",
                      "[Interrupted search; results are incomplete]");
}


/////////////////////////////////////////////////////////////////////
//  MISC functions
/////////////////////////////////////////////////////////////////////

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// str_is_prefix:
//    Provides a fast Tcl command "strIsPrefix" for checking if the
//    first string provided is a prefix of the second string, without
//    needing the standard slower [string match] or [string range]
//    routines.
int
str_is_prefix (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: strIsPrefix <shortStr> <longStr>");
    }

    return UI_Result(ti, OK, strIsPrefix (argv[1], argv[2]));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// str_prefix_len:
//    Tcl command that returns the length of the common text at the start
//    of two strings.
int
str_prefix_len (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: strPrefixLen <str> <str>");
    }

    return setUintResult (ti, strPrefix (argv[1], argv[2]));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_base_inUse
//  Returns 1 if the database slot is in use; 0 otherwise.
int
sc_base_inUse (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const scidBaseT* basePtr = db;
    if (argc > 2) {
        basePtr = DBasePool::getBase(strGetUnsigned(argv[2]));
        if (basePtr == 0) return UI_Result(ti, OK, false);
    }

    return UI_Result(ti, OK, basePtr->inUse);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  exportGame:
//    Called by sc_base_export() to export a single game.
void
exportGame (Game * g, FILE * exportFile, gameFormatT format, uint pgnStyle)
{
    char old_language = language;
 
    g->ResetPgnStyle (pgnStyle);
    g->SetPgnFormat (format);

    // Format-specific settings:
    switch (format) {
    case PGN_FORMAT_HTML:
    case PGN_FORMAT_LaTeX:
        g->AddPgnStyle (PGN_STYLE_SHORT_HEADER);
        break;
    default:
        language = 0;
        break;
    }

    g->SetHtmlStyle (htmlDiagStyle);
    std::pair<const char*, unsigned> pgn = g->WriteToPGN(75, true, format != PGN_FORMAT_LaTeX);
    //size_t nWrited =
    fwrite(pgn.first, 1, pgn.second, exportFile);
    //TODO:
    //if (nWrited != db->tbuf->GetByteCount()) error
    language = old_language;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_base_export:
//    Exports the current game or all filter games in the database
//    to a PGN, HTML or LaTeX file.
int
sc_base_export (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    FILE * exportFile = NULL;
    bool exportFilter = false;
    bool appendToFile = false;
    gameFormatT outputFormat = PGN_FORMAT_Plain;
    const char * startText = "";
    const char * endText = "";
    const char * usage = "Usage: sc_base export current|filter PGN|HTML|LaTeX <pgn_filename> options...";
    uint pgnStyle = PGN_STYLE_TAGS;

    const char * options[] = {
        "-append", "-starttext", "-endtext", "-comments", "-variations",
        "-spaces", "-symbols", "-indentComments", "-indentVariations",
        "-column", "-noMarkCodes", "-convertNullMoves", NULL
    };
    enum {
        OPT_APPEND, OPT_STARTTEXT, OPT_ENDTEXT, OPT_COMMENTS, OPT_VARIATIONS,
        OPT_SPACES, OPT_SYMBOLS, OPT_INDENTC, OPT_INDENTV,
        OPT_COLUMN, OPT_NOMARKS, OPT_CONVERTNULL
    };

    if (argc < 5) { return errorResult (ti, usage); }

    if (strIsPrefix (argv[2], "current")) {
        exportFilter = false;
    } else if (strIsPrefix (argv[2], "filter")) {
        exportFilter = true;
    } else {
        return errorResult (ti, usage);
    }

    if (! Game::PgnFormatFromString (argv[3], &outputFormat)) {
        return errorResult (ti, usage);
    }

    if (exportFilter  &&  !db->inUse) {
        return errorResult (ti, errMsgNotOpen(ti));
    }

    const char * exportFileName = argv[4];

    // Check for an even number of optional parameters:
    if ((argc % 2) != 1) { return errorResult (ti, usage); }

    // Parse all optional parameters:
    for (int arg = 5; arg < argc; arg += 2) {
        const char * value = argv[arg+1];
        bool flag = strGetBoolean (value);
        int option = strUniqueMatch (argv[arg], options);

        switch (option) {
        case OPT_APPEND:
            appendToFile = flag;
            break;

        case OPT_STARTTEXT:
            startText = value;
            break;

        case OPT_ENDTEXT:
            endText = value;
            break;

        case OPT_COMMENTS:
            if (flag) { pgnStyle |= PGN_STYLE_COMMENTS; }
            break;

        case OPT_VARIATIONS:
            if (flag) { pgnStyle |= PGN_STYLE_VARS; }
            break;

        case OPT_SPACES:
            if (flag) { pgnStyle |= PGN_STYLE_MOVENUM_SPACE; }
            break;

        case OPT_SYMBOLS:
            if (flag) { pgnStyle |= PGN_STYLE_SYMBOLS; }
            break;

        case OPT_INDENTC:
            if (flag) { pgnStyle |= PGN_STYLE_INDENT_COMMENTS; }
            break;

        case OPT_INDENTV:
            if (flag) { pgnStyle |= PGN_STYLE_INDENT_VARS; }
            break;

        case OPT_COLUMN:
            if (flag) { pgnStyle |= PGN_STYLE_COLUMN; }
            break;

        case OPT_NOMARKS:
            if (flag) { pgnStyle |= PGN_STYLE_STRIP_MARKS; }
            break;

        case OPT_CONVERTNULL:
            if (flag) { pgnStyle |= PGN_STYLE_NO_NULL_MOVES; }
            break;

        default:
            return InvalidCommand (ti, "sc_base export", options);
        }
    }
    exportFile = fopen (exportFileName, (appendToFile ? "r+" : "w"));
    if (exportFile == NULL) {
        return errorResult (ti, "Error opening file for exporting games.");
    }
    // Write start text or find the place in the file to append games:
    if (appendToFile) {
        if (outputFormat == PGN_FORMAT_Plain) {
            fseek (exportFile, 0, SEEK_END);
        } else {
            fseek (exportFile, 0, SEEK_SET);
            const char * endMarker = "";
            if (outputFormat == PGN_FORMAT_HTML) {
                endMarker = "</body>";
            } else if (outputFormat == PGN_FORMAT_LaTeX) {
                endMarker = "\\end{document}";
            }
            char line [1024];
            uint pos = 0;
            while (1) {
                char* err = fgets(line, 1024, exportFile);
                if (err == 0 || feof(exportFile)) break;
                const char * s = strTrimLeft (line, " ");
                if (strIsCasePrefix (endMarker, s)) {
                    // We have seen the line to stop at, so break out
                    break;
                }
                pos = ftell (exportFile);
            }
            fseek (exportFile, pos, SEEK_SET);
        }
    } else {
        fputs (startText, exportFile);
    }

    if (!exportFilter) {
        exportGame (db->game, exportFile, outputFormat, pgnStyle);
    } else { //TODO: remove this (duplicate of sc_filter export)
        Progress progress = UI_CreateProgress(ti);
        uint numSeen = 0;
        uint numToExport = db->dbFilter->Count();
        Game * g = scratchGame;
        for (gamenumT i=0, n=db->numGames(); i < n; i++) {
            if (db->dbFilter->Get(i)) { // Export this game:
                if (++numSeen % 1024 == 0) {  // Update the percentage done bar:
                    if (!progress.report(numSeen, numToExport)) break;
                }

                // Print the game, skipping any corrupt games:
                const IndexEntry* ie = db->getIndexEntry(i);
                if (ie->GetLength() == 0) { continue; }
                if (db->getGame(*ie, *g) != OK) {
                    continue;
                }
                exportGame (g, exportFile, outputFormat, pgnStyle);
            }
        }
        progress.report(1, 1);
    }
    fputs (endText, exportFile);
    fclose (exportFile);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_base_piecetrack:
//    Examines games in the filter of the current database and
//    returns a list of 64 integers indicating how frequently
//    the specified piece moves to each square.
int
sc_base_piecetrack (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage =
        "Usage: sc_base piecetrack [-g|-t] <minMoves> <maxMoves> <startSquare ...>";

    if (argc < 5) {
        return errorResult (ti, usage);
    }

    // Check for optional mode parameter:
    bool timeOnSquareMode = false;
    int arg = 2;
    if (argv[arg][0] == '-') {
        if (argv[arg][1] == 'g'  && strIsPrefix (argv[arg], "-games")) {
            timeOnSquareMode = false;
            arg++;
        } else if (argv[arg][1] == 't'  && strIsPrefix (argv[arg], "-time")) {
            timeOnSquareMode = true;
            arg++;
        } else {
            return errorResult (ti, usage);
        }
    }

    // Read the two move-number parameters:
    uint minPly = strGetUnsigned(argv[arg]) * 2;
    arg++;
    uint maxPly = strGetUnsigned(argv[arg]) * 2;
    arg++;

    // Convert moves to plycounts, e.g. "5-10" -> "9-20"
    if (minPly < 2) { minPly=2; }
    if (maxPly < minPly) { maxPly = minPly; }
    minPly--;

    // Parse the variable number of tracked square arguments:
    uint sqFreq[64] = {0};
    bool trackSquare[64] = { false };
    int nTrackSquares = 0;
    for (int a=arg; a < argc; a++) {
        squareT sq = strGetSquare (argv[a]);
        if (sq == NULL_SQUARE) { return errorResult (ti, usage); }
        if (!trackSquare[sq]) {
            // Seen another starting square to track.
            trackSquare[sq] = true;
            nTrackSquares++;
        }
    }

    // If current base is unused, filter is empty, or no track
    // squares specified, then just return a zero-filled list:

    if (! db->inUse  ||  db->dbFilter->Count() == 0  ||  nTrackSquares == 0) {
        for (uint i=0; i < 64; i++) { appendUintElement (ti, 0); }
        return TCL_OK;
    }

    // Examine every filter game and track the selected pieces:

    Progress progress = UI_CreateProgress(ti);
    const auto filter = db->getFilter("dbfilter");
    const size_t filterCount = filter->size();
    size_t filterSeen = 0;
    for (const auto gnum : filter) {
        if (++filterSeen % 1024 == 0) {
            if (!progress.report(filterSeen, filterCount)) {
                return UI_Result(ti, ERROR_UserCancel);
            }
        }

        const IndexEntry* ie = db->getIndexEntry(gnum);

        // Skip games with non-standard start or no moves:
        if (ie->GetStartFlag()) { continue; }
        if (ie->GetLength() == 0) { continue; }

        // Skip games too short to be useful:
        if (ie->GetNumHalfMoves() < minPly) { continue; }

        // Set up piece tracking for this game:
        bool movedTo[64] = { false };
        bool track[64];
        int ntrack = nTrackSquares;
        for (uint sq=0; sq < 64; sq++) { track[sq] = trackSquare[sq]; }

        // Process each game move until the maximum ply or end of
        // the game is reached:
        uint plyCount = 0;
        db->getGame(ie).mainLine([&](auto move) {
            if (plyCount++ >= maxPly)
                return false;

            squareT toSquare = move.getTo();
            squareT fromSquare = move.getFrom();

            // Special hack for castling:
            if (move.isCastle()) {
                if (toSquare == H1) {
                    if (track[H1]) {
                        fromSquare = H1;
                        toSquare = F1;
                    } else {
                        toSquare = G1;
                    }
                } else if (toSquare == A1) {
                    if (track[A1]) {
                        fromSquare = A1;
                        toSquare = D1;
                    } else {
                        toSquare = C1;
                    }
                } else if (toSquare == H8) {
                    if (track[H8]) {
                        fromSquare = H8;
                        toSquare = F8;
                    } else {
                        toSquare = G8;
                    }
                } else if (toSquare == A8) {
                    if (track[A8]) {
                        fromSquare = A8;
                        toSquare = D8;
                    } else {
                        toSquare = C8;
                    }
                }
            }

            // TODO: Special hack for en-passant capture?

            if (track[toSquare]) {
                // A tracked piece has been captured:
                track[toSquare] = false;
                ntrack--;
                if (ntrack <= 0)
                    return false;

            } else if (track[fromSquare]) {
                // A tracked piece is moving:
                track[fromSquare] = false;
                track[toSquare] = true;
                if (plyCount >= minPly) {
                    // If not time-on-square mode, and this
                    // new target square has not been moved to
                    // already by a tracked piece in this game,
                    // increase its frequency now:
                    if (!timeOnSquareMode  && !movedTo[toSquare]) {
                        sqFreq[toSquare]++;
                    }
                    movedTo[toSquare] = true;
                }
            }

            if (timeOnSquareMode  &&  plyCount >= minPly) {
                // Time-on-square mode: find all tracked squares
                // (there are ntrack of them) and increment the
                // frequency of each.
                int nleft = ntrack;
                for (uint i=0; i < 64; i++) {
                    if (track[i]) {
                        sqFreq[i]++;
                        nleft--;
                        // We can stop early when all tracked
                        // squares have been found:
                        if (nleft <= 0)
                            return false;
                    }
                }
            }

            return true;
        }); // while (plyCount < maxPly)
    } // foreach game

    progress.report(1, 1);

    // Now return the 64-integer list: if in time-on-square mode,
    // the value for each square is the number of plies when a
    // tracked piece was on it, so halve it to convert to moves:

    for (uint i=0; i < 64; i++) {
        appendUintElement (ti, timeOnSquareMode ? sqFreq[i] / 2 : sqFreq[i]);
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_base_duplicates:
//    Finds duplicate games and marks them deleted.
//    A pair of games are considered duplicates if the Event, Site,
//    White, Black, and Round values all match identically, and the
//    Date matches to within 2 days (that is, the same year, the same
//    month, and the days of month differ by 2 at most).
//
//    Furthermore, the moves of one game should, after truncating, be the
//    same as the moves of the other game, for them to be duplicates.

struct gNumListT {
    uint64_t hash;
    uint gNumber;
    bool operator<(const gNumListT& a) const { return hash < a.hash; }
};

struct dupCriteriaT {
    bool exactNames;
    bool sameColors;
    bool sameEvent;
    bool sameSite;
    bool sameRound;
    bool sameResult;
    bool sameYear;
    bool sameMonth;
    bool sameDay;
    bool sameEcoCode;
    bool sameMoves;
};

bool
checkDuplicate (scidBaseT * base,
                const IndexEntry * ie1, const IndexEntry * ie2,
                dupCriteriaT * cr)
{
    if (ie1->GetDeleteFlag()  ||  ie2->GetDeleteFlag()) { return false; }
    if (cr->sameEvent) {
        if (ie1->GetEvent() != ie2->GetEvent()) { return false; }
    }
    if (cr->sameSite) {
        if (ie1->GetSite() != ie2->GetSite()) { return false; }
    }
    if (cr->sameRound) {
        if (ie1->GetRound() != ie2->GetRound()) { return false; }
    }
    if (cr->sameYear) {
        if (ie1->GetYear() != ie2->GetYear()) { return false; }
    }
    if (cr->sameMonth) {
        if (ie1->GetMonth() != ie2->GetMonth()) { return false; }
    }
    if (cr->sameDay) {
        if (ie1->GetDay() != ie2->GetDay()) { return false; }
    }
    if (cr->sameResult) {
        if (ie1->GetResult() != ie2->GetResult()) { return false; }
    }
    if (cr->sameEcoCode) {
        ecoStringT a;
        ecoStringT b;
        eco_ToBasicString (ie1->GetEcoCode(), a);
        eco_ToBasicString (ie2->GetEcoCode(), b);
        if (a[0] != b[0]  ||  a[1] != b[1]  ||  a[2] != b[2]) { return false; }
    }

    // There are a lot of "place-holding" games in some database, that have
    // just one (usually wrong) move and a result, that are then replaced by
    // the full version of the game. Therefore, if we reach here and one
    // of the games (or both) have only one move or no moves, return true
    // as long as they have the same year, site and round:

    if (ie1->GetNumHalfMoves() <= 2  ||  ie2->GetNumHalfMoves() <= 2) {
        if (ie1->GetYear() == ie2->GetYear()  &&
            ie1->GetSite() == ie2->GetSite()  &&
            ie1->GetRound() == ie2->GetRound()) {
            return true;
        }
    }

    // Now check that the games contain the same moves, up to the length
    // of the shorter game:

    if (cr->sameMoves) {
        const byte * hpData1 = ie1->GetHomePawnData();
        const byte * hpData2 = ie2->GetHomePawnData();
        if (! hpSig_Prefix (hpData1, hpData2)) { return false; }
        // Now we have to check the actual moves of the games:
        uint length = std::min(ie1->GetNumHalfMoves(), ie2->GetNumHalfMoves());
        std::string a = base->getGame(ie1).getMoveSAN(0, length);
        std::string b = base->getGame(ie2).getMoveSAN(0, length);
        return (a == b);
    }
    return true;
}

UI_res_t sc_base_duplicates(scidBaseT* dbase, UI_handle_t ti, int argc,
                            const char** argv) {
    dupCriteriaT criteria;
    criteria.exactNames  = false;
    criteria.sameColors  = true;
    criteria.sameEvent   = true;
    criteria.sameSite    = true;
    criteria.sameRound   = true;
    criteria.sameYear    = true;
    criteria.sameMonth   = true;
    criteria.sameDay     = false;
    criteria.sameResult  = false;
    criteria.sameEcoCode = false;
    criteria.sameMoves = true;

    bool skipShortGames = false;
    bool keepAllCommentedGames = true;
    bool keepAllGamesWithVars  = true;
    bool setFilterToDups = false;
    bool onlyFilterGames = false;

    // Deletion strategy: delete the shorter game, the game with the
    // smaller game number, or the game with the larger game number.
    enum deleteStrategyT { DELETE_SHORTER, DELETE_OLDER, DELETE_NEWER };
    deleteStrategyT deleteStrategy = DELETE_SHORTER;

    // Parse command options in pairs of arguments:

    const char * options[] = {
        "-players", "-colors", "-event", "-site", "-round", "-year",
        "-month", "-day", "-result", "-eco", "-moves", "-skipshort",
        "-comments", "-variations", "-setfilter", "-usefilter",
        "-delete",
        NULL
    };
    enum {
        OPT_PLAYERS, OPT_COLORS, OPT_EVENT, OPT_SITE, OPT_ROUND, OPT_YEAR,
        OPT_MONTH, OPT_DAY, OPT_RESULT, OPT_ECO, OPT_MOVES, OPT_SKIPSHORT,
        OPT_COMMENTS, OPT_VARIATIONS, OPT_SETFILTER, OPT_USEFILTER,
        OPT_DELETE
    };

    for (int arg = 3; arg < argc; arg += 2) {
        const char * optStr = argv[arg];
        const char * valueStr = argv[arg + 1];
        bool b = strGetBoolean (valueStr);
        int index = strUniqueMatch (optStr, options);
        switch (index) {
            case OPT_PLAYERS:     criteria.exactNames = b;   break;
            case OPT_COLORS:      criteria.sameColors = b;   break;
            case OPT_EVENT:       criteria.sameEvent = b;    break;
            case OPT_SITE:        criteria.sameSite = b;     break;
            case OPT_ROUND:       criteria.sameRound = b;    break;
            case OPT_YEAR:        criteria.sameYear = b;     break;
            case OPT_MONTH:       criteria.sameMonth = b;    break;
            case OPT_DAY:         criteria.sameDay = b;      break;
            case OPT_RESULT:      criteria.sameResult = b;   break;
            case OPT_ECO:         criteria.sameEcoCode = b;  break;
            case OPT_MOVES:       criteria.sameMoves = b;    break;
            case OPT_SKIPSHORT:   skipShortGames = b;        break;
            case OPT_COMMENTS:    keepAllCommentedGames = b; break;
            case OPT_VARIATIONS:  keepAllGamesWithVars = b;  break;
            case OPT_SETFILTER:   setFilterToDups = b;       break;
            case OPT_USEFILTER:   onlyFilterGames = b;       break;
            case OPT_DELETE:
                if (dbase->isReadOnly())
                    return UI_Result(ti, ERROR_FileReadOnly);
                if (strIsCasePrefix (valueStr, "shorter")) {
                    deleteStrategy = DELETE_SHORTER;
                } else if (strIsCasePrefix (valueStr, "older")) {
                    deleteStrategy = DELETE_OLDER;
                } else if (strIsCasePrefix (valueStr, "newer")) {
                    deleteStrategy = DELETE_NEWER;
                } else {
                    return UI_Result(ti, ERROR_BadArg, "Invalid option.");
                }
                break;
            default:
                return UI_Result(ti, ERROR_BadArg, "Invalid option.");
        }
    }
    const gamenumT numGames = dbase->numGames();

    Filter tmp_filter(numGames);
    HFilter filter = setFilterToDups ? dbase->getFilter("dbfilter")
                                     : HFilter(&tmp_filter);
    filter.clear();

    // Setup duplicates array:
    auto duplicates = std::make_unique<gamenumT[]>(numGames);

    // We use a hashtable to limit duplicate game comparisons; each game
    // is only compared to others that hash to the same value.
    std::vector<gNumListT> hash(numGames);
    size_t n_hash = 0;
    const std::vector<uint32_t>& hashMap = (criteria.exactNames)
            ? std::vector<uint32_t>()
            : dbase->getNameBase()->generateHashMap(NAME_PLAYER);
    for (gamenumT i=0; i < numGames; i++) {
        const IndexEntry* ie = dbase->getIndexEntry(i);
        if (! ie->GetDeleteFlag()  /* &&  !ie->GetStartFlag() */
            &&  (!skipShortGames  ||  ie->GetNumHalfMoves() >= 10)
            &&  (!onlyFilterGames  ||  dbase->dbFilter->Get(i) > 0)) {

            uint32_t wh = ie->GetWhite();
            uint32_t bl = ie->GetBlack();
            if (!criteria.exactNames) {
                wh = hashMap[wh];
                bl = hashMap[bl];
            }
            if (!criteria.sameColors && bl > wh) {
                std::swap(wh, bl);
            }
            gNumListT* node = &(hash[n_hash++]);
            node->hash = (uint64_t(wh) << 32) + bl;
            node->gNumber = i;
        }
    }
    hash.resize(n_hash);
    std::sort(hash.begin(), hash.end());

    Progress progress = UI_CreateProgress(ti);
    // Now check same-hash games for duplicates:
    for (size_t i=0; i < n_hash; i++) {
        if ((i % 1024) == 0) {
            if (!progress.report(i, numGames)) break;
        }
        const gNumListT& head = hash[i];
        const IndexEntry* ieHead = dbase->getIndexEntry(head.gNumber);

        for (size_t comp=i+1; comp < n_hash; comp++) {
            const gNumListT& compare = hash[comp];
            if (compare.hash != head.hash) break;

            const IndexEntry* ieComp = dbase->getIndexEntry(compare.gNumber);

            if (checkDuplicate(dbase, ieHead, ieComp, &criteria)) {
                duplicates[head.gNumber] = compare.gNumber + 1;
                duplicates[compare.gNumber] = head.gNumber + 1;

                auto isImmune = [&](const IndexEntry* ie) {
                    if (keepAllCommentedGames && ie->GetCommentsFlag())
                        return true;
                    return keepAllGamesWithVars && ie->GetVariationsFlag();
                };

                // Decide which game should get deleted:
                bool deleteHead = false;
                if (deleteStrategy == DELETE_OLDER) {
                    deleteHead = (head.gNumber < compare.gNumber);
                } else if (deleteStrategy == DELETE_NEWER) {
                    deleteHead = (head.gNumber > compare.gNumber);
                } else {
                    ASSERT(deleteStrategy == DELETE_SHORTER);
                    uint a = ieHead->GetNumHalfMoves();
                    uint b = ieComp->GetNumHalfMoves();
                    deleteHead = (a <= b);
                    if (a == b && isImmune(ieHead))
                        deleteHead = false;
                }

                gamenumT gnumDelete = compare.gNumber;
                const IndexEntry* ieDelete = ieComp;
                if (deleteHead) {
                    gnumDelete = head.gNumber;
                    ieDelete = ieHead;
                }
                // Delete whichever game is to be deleted:
                if (!isImmune(ieDelete)) {
                    filter->set(gnumDelete, 1);
                }
            }
        }
    }
    auto[err, nDel] = dbase->transformIndex(filter, {}, [](IndexEntry& ie) {
        ie.SetDeleteFlag(true);
        return true;
    });
    dbase->setDuplicates(std::move(duplicates));
    progress.report(1, 1);
    return (err == OK) ? UI_Result(ti, OK, nDel) : UI_Result(ti, err);
}

//////////////////////////////////////////////////////////////////////
/// CLIPBASE functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_clipbase:
//    Game clipbase functions.
//    Copies a game to, or pastes from, the clipbase database.
int
sc_clipbase (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    scidBaseT* clipbase = DBasePool::getBase(DBasePool::getClipBase());
    ASSERT(clipbase);

    static const char * options [] = {
        "clear", "paste", NULL
    };
    enum {
        CLIP_CLEAR, CLIP_PASTE
    };
    int index = -1;

    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case CLIP_CLEAR:
        clipbase->Close();
        DBasePool::clearClipBase();
        break;

    case CLIP_PASTE: // Paste the active clipbase game
        if (db != clipbase) {
            delete db->game;
            db->game = clipbase->game->clone();
            db->gameNumber = -1;
            db->gameAltered = true;
        }
        break;

    default:
        return InvalidCommand(ti, "sc_clipbase", options);
    }

    return UI_Result(ti, OK);
}

//////////////////////////////////////////////////////////////////////
/// ECO Classification functions

// ecoTranslateT:
//    Structure for a linked list of ECO opening name translations.
//
struct ecoTranslateT {
    char   language;
    char * from;
    char * to;
    ecoTranslateT * next;
};

static ecoTranslateT * ecoTranslations = NULL;
void translateECO (Tcl_Interp * ti, const char * strFrom, DString * dstrTo);

int
sc_eco (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    int index = -1;
    static const char * options [] = {
        "base", "game", "read", "reset", "summary",
        "translate", NULL
    };
    enum {
        ECO_BASE, ECO_GAME, ECO_READ, ECO_RESET, ECO_SUMMARY,
        ECO_TRANSLATE
    };

    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case ECO_BASE:
        return sc_eco_base (cd, ti, argc, argv);

    case ECO_GAME:
        return sc_eco_game (cd, ti, argc, argv);

    case ECO_READ:
        return sc_eco_read (cd, ti, argc, argv);

    case ECO_RESET:
        if (ecoBook) { ecoBook = NULL; }
        break;

    case ECO_SUMMARY:
        return sc_eco_summary (cd, ti, argc, argv);

    case ECO_TRANSLATE:
        return sc_eco_translate (cd, ti, argc, argv);

    default:
        return InvalidCommand (ti, "sc_eco", options);
    }

    return TCL_OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_eco_base:
//    Reclassifies all games in the current base by ECO code.
//
//    The first parameter indicates if all games (not only those
//    with no existing ECO code) should be classified.
//       "0" or "nocode": only games with no ECO code.
//       "1" or "all": classify all games.
//       "date:yyyy.mm.dd": only games since date.
//    The second boolean parameter indicates if Scid-specific ECO
//    extensions (e.g. "B12j" instead of just "B12") should be used.
//
//    If the database is read-only, games can still be classified but
//    the results will not be stored to the database file.
int
sc_eco_base (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 4) {
        return errorResult (ti, "Usage: sc_eco base <bool:all_games> <bool:extensions>");
    }
    if (!ecoBook) { return errorResult (ti, "No ECO Book is loaded."); }
    if (! db->inUse) return errorResult (ti, ERROR_FileNotOpen);

    int option = -1;
    enum {ECO_NOCODE, ECO_ALL, ECO_DATE, ECO_FILTER};

    switch (argv[2][0]) {
    case '0':
    case 'n':
        option = ECO_NOCODE; break;
    case 'd':
        option = ECO_DATE; break;
    case 'f':
        option = ECO_FILTER; break;
    default:
        option = ECO_ALL; break;
    }

    bool extendedCodes = strGetBoolean(argv[3]);
    dateT startDate = ZERO_DATE;
    if (option == ECO_DATE) {
        startDate = date_EncodeFromString (&(argv[2][5]));
    }

    scidBaseT& dbase = *db;
    auto entry_op = [&](IndexEntry& ie) {
        if (ie.GetLength() == 0)
            return false;

        // Ignore games with existing ECO code if directed:
        if (option == ECO_NOCODE && ie.GetEcoCode() != 0)
            return false;

        // Ignore games before starting date if directed:
        if (option == ECO_DATE && ie.GetDate() < startDate)
            return false;

        auto bbuf = dbase.getGame(ie);
        Game* g = scratchGame;
        if (g->DecodeSkipTags(&bbuf) != OK)
            return false;

        ecoT ecoCode = ECO_None;
        for (;;) {
            auto pos = g->GetCurrentPos();
            if (pos->TotalMaterial() < ecoBook->FewestPieces())
                break;

            const auto eco = ecoBook->findECO(pos);
            if (eco != ECO_None) {
                ecoCode = eco;
            }

            simpleMoveT sm;
            if (g->DecodeNextMove(&bbuf, sm) != OK)
                break;

            g->GetCurrentPos()->DoSimpleMove(&sm);
        }

        if (!extendedCodes) {
            ecoCode = eco_BasicCode(ecoCode);
        }

        if (ie.GetEcoCode() != ecoCode) {
            ie.SetEcoCode(ecoCode);
            return true;
        }
        return false;
    };

    std::string filter =
        (option == ECO_FILTER) ? "dbfilter" : dbase.newFilter();
    auto hf = dbase.getFilter(filter);
    auto changes = dbase.transformIndex(hf, UI_CreateProgress(ti), entry_op);
    if (option == ECO_FILTER)
        dbase.deleteFilter(filter.c_str());

    return UI_Result(ti, changes.first, changes.second);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_eco_game:
//    Returns ECO code for the current game. If the optional
//    parameter <ply> is passed, it returns the ply depth of the
//    deepest match instead of the ECO code.
int
sc_eco_game (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    uint returnPly = 0;
    if (argc > 2) {
        if (argc == 3  &&  strIsPrefix (argv[2], "ply")) {
            returnPly = 1;
        } else {
            return errorResult (ti, "Usage: sc_game eco [ply]");
        }
    }
    if (!ecoBook) { return TCL_OK; }

    auto location = db->game->currentLocation();
    db->game->MoveToEnd();
    ecoT ecoCode = ECO_None;
    do {
        ecoCode = ecoBook->findECO(db->game->GetCurrentPos());
    } while (ecoCode == ECO_None && db->game->MoveBackup() == OK);

    auto ply = db->game->GetCurrentPly();
    db->game->restoreLocation(location);

    if (ecoCode == ECO_None)
        return UI_Result(ti, OK);

    if (returnPly)
        return UI_Result(ti, OK, ply);

    ecoStringT extEco;
    eco_ToExtendedString(ecoCode, extEco);
    return UI_Result(ti, OK, extEco);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_eco_read:
//    Reads a book file for ECO classification.
//    Returns the book size (number of positions).
int
sc_eco_read (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc < 3) { return TCL_OK; }
    ecoBook = nullptr;
    auto book = PBook::ReadEcoFile(argv[2]);
    if (book.first != OK) {
        if (book.first == ERROR_FileOpen) {
            Tcl_AppendResult (ti, "Unable to open the ECO file:\n",
                              argv[2], NULL);
        } else {
            Tcl_AppendResult (ti, "Unable to load the ECO file:\n",
                              argv[2], NULL);
        }
        return book.first;
    }
    ecoBook = std::move(book.second);
    return UI_Result(ti, OK, ecoBook->Size());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_eco_summary:
//    Returns a listing of positions for the specified ECO prefix,
//    in plain text or color (Scid hypertext) format.
int
sc_eco_summary (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    bool color = true;
    if (argc != 3  &&  argc != 4) {
        return errorResult (ti, "Usage: sc_eco summary <ECO-prefix> [<bool:color>]");
    }
    if (argc == 4) { color = strGetBoolean (argv[3]); }
    if (!ecoBook) { return TCL_OK; }
    DString * dstr = new DString;
    DString * temp = new DString;
    bool inMoveList = false;
    translateECO(ti, ecoBook->EcoSummary(argv[2]).c_str(), dstr);
    if (color) {
        DString * oldstr = dstr;
        dstr = new DString;
        const char * s = oldstr->Data();
        while (*s) {
            char ch = *s;
            switch (ch) {
            case '[':
                dstr->Append ("<tab>");
                dstr->AddChar (ch);
                break;
            case ']':
                dstr->AddChar (ch);
                dstr->Append ("<blue><run importMoveList {");
                inMoveList = true;
                temp->Clear();
                break;
            case '\n':
                if (inMoveList) {
                    dstr->Append ("}>", temp->Data());
                    inMoveList = false;
                }
                dstr->Append ("</run></blue></tab><br>");
                break;
            default:
                dstr->AddChar (ch);
                if (inMoveList) { temp->AddChar (transPiecesChar(ch)); }//{ temp->AddChar (ch); }
            }
            s++;
        }
        delete oldstr;
    }
    Tcl_AppendResult (ti, dstr->Data(), NULL);
    delete temp;
    delete dstr;
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_eco_translate:
//    Adds a new ECO openings phrase translation.
int
sc_eco_translate (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 5) {
        return errorResult (ti, "Usage: sc_eco translate <lang> <from> <to>");
    }

#ifdef WINCE
    ecoTranslateT * trans = (ecoTranslateT * )my_Tcl_Alloc( sizeof(ecoTranslateT));
#else
    ecoTranslateT * trans = new ecoTranslateT;
#endif
    trans->next = ecoTranslations;
    trans->language = argv[2][0];
    trans->from = strDuplicate (argv[3]);
    trans->to = strDuplicate (argv[4]);
    ecoTranslations = trans;
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// translateECO:
//    Translates an ECO opening name into the current language.
//
void
translateECO (Tcl_Interp * ti, const char * strFrom, DString * dstrTo)
{
    ecoTranslateT * trans = ecoTranslations;
    dstrTo->Clear();
    dstrTo->Append (strFrom);
    const char * language = Tcl_GetVar (ti, "language", TCL_GLOBAL_ONLY);
    if (language == NULL) { return; }
    char lang = language[0];
    while (trans != NULL) {
        if (trans->language == lang
            &&  strContains (dstrTo->Data(), trans->from)) {
            // Translate this phrase in the string:
            char * temp = strDuplicate (dstrTo->Data());
            dstrTo->Clear();
            char * in = temp;
            while (*in != 0) {
                if (strIsPrefix (trans->from, in)) {
                    dstrTo->Append (trans->to);
                    in += strLength (trans->from);
                } else {
                    dstrTo->AddChar (*in);
                    in++;
                }
            }
#ifdef WINCE
            my_Tcl_Free((char*) temp);
#else
            delete[] temp;
#endif
        }
        trans = trans->next;
    }
}

//////////////////////////////////////////////////////////////////////
///  FILTER functions

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter: filter commands.  Valid minor commands:
//    count:     returns the number of games in the filter.
//    reset:     resets the filter so all games are included.
//    remove:    removes game number <x> from the filter.
//    stats:     prints filter statistics.
int
sc_filter_old(ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    int index = -1;
    static const char * options [] = {
        "count", "first", "frequency",
        "last", "negate", "next",
        "previous", "stats",
        "search", "release",
        "treestats", "export", "copy", "and", "or", "new", NULL
    };
    enum {
        FILTER_COUNT, FILTER_FIRST, FILTER_FREQ,
        FILTER_LAST, FILTER_NEGATE, FILTER_NEXT,
        FILTER_PREV, FILTER_STATS,
        FILTER_SEARCH, FILTER_RELEASE,
        FILTER_TREESTATS, FILTER_EXPORT, FILTER_COPY, FILTER_AND, FILTER_OR, FILTER_NEW
    };

    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case FILTER_COUNT:
        if (argc == 2) {
            size_t res = db->getFilter("dbfilter")->size();
            return UI_Result(ti, OK, res);
        }
        break;

    case FILTER_NEW:
        if (argc == 3) {
            if (auto dbase = DBasePool::getBase(strGetUnsigned(argv[2])))
                return UI_Result(ti, OK, dbase->newFilter());

            return UI_Result(ti, ERROR_BadArg, "sc_filter: invalid baseId");
        }
        return UI_Result(ti, ERROR_BadArg, "Usage: sc_filter new baseId");

    case FILTER_FIRST:
        return sc_filter_first (cd, ti, argc, argv);

    case FILTER_LAST:
        return sc_filter_last (cd, ti, argc, argv);

    case FILTER_NEXT:
        return sc_filter_next (cd, ti, argc, argv);

    case FILTER_PREV:
        return sc_filter_prev (cd, ti, argc, argv);

    case FILTER_STATS:
        return sc_filter_stats (cd, ti, argc, argv);

    }

    if (argc < 4) return errorResult (ti, "Usage: sc_filter <cmd> baseId filterName");
    scidBaseT* dbase = DBasePool::getBase(strGetUnsigned(argv[2]));
    if (dbase == NULL) return errorResult (ti, "sc_filter: invalid baseId");
    HFilter filter = dbase->getFilter(argv[3]);
    if (filter == 0) return errorResult (ti, "sc_filter: invalid filterName");
    switch (index) {
    case FILTER_AND:
        if (argc == 5) {
            const HFilter f = dbase->getFilter(argv[4]);
            if (f != 0) {
                for (uint i=0, n = dbase->numGames(); i < n; i++) {
                    if (filter.get(i) != 0 && f.get(i) == 0) filter.set(i, 0);
                }
                return UI_Result(ti, OK);
            }
        }
        return errorResult (ti, "Usage: sc_filter and baseId filterName filterAnd");

    case FILTER_OR:
        if (argc == 5) {
            const HFilter f = dbase->getFilter(argv[4]);
            if (f != 0) {
                for (uint i=0, n = dbase->numGames(); i < n; i++) {
                    if (filter.get(i) == 0) filter.set(i, f.get(i));
                }
                return UI_Result(ti, OK);
            }
        }
        return errorResult (ti, "Usage: sc_filter or baseId filterName filterOr");

    case FILTER_COPY:
        if (argc == 5) {
            const HFilter f = dbase->getFilter(argv[4]);
            if (f != 0) {
                for (uint i=0, n = dbase->numGames(); i < n; i++) {
                    filter.set(i, f.get(i));
                }
                return UI_Result(ti, OK);
            }
        }
        return errorResult (ti, "Usage: sc_filter copy baseId filterTo filterFrom");

    case FILTER_FREQ:
        return sc_filter_freq (dbase, filter, ti, argc, argv);

    case FILTER_NEGATE:
        for (uint i=0, n = dbase->numGames(); i < n; i++) {
            filter.set(i, ! filter.get(i) );
        }
        return UI_Result(ti, OK);

    case FILTER_COUNT:
        return UI_Result(ti, OK, filter->size());

    case FILTER_RELEASE:
        dbase->deleteFilter(argv[3]);
        return TCL_OK;

    case FILTER_SEARCH:
        if (argc >= 5) {
            std::string_view subcmd = argv[4];
            if (subcmd == "header")
                return sc_search_header (cd, ti, dbase, filter, argc -3, argv +3);

            if (subcmd == "board") {
                if (argc == 5 || argc == 6) {
                    // sc_filter search baseId filterTo board [filterFrom]
                    bool useCache = (argc == 5);

                    auto const& pos = *db->game->GetCurrentPos();
                    if (useCache &&
                        dbase->treeCache.cacheRestore(pos, *dbase->treeFilter))
                        return UI_Result(ti, OK);

                    if (!SearchPos(&pos).setFilter(
                            *dbase, filter, UI_CreateProgress(ti)))
                        return UI_Result(ti, ERROR_UserCancel);

                    if (useCache)
                        dbase->treeCache.cacheAdd(pos, *dbase->treeFilter);

                    return UI_Result(ti, OK);
                }

                if (argc == 10) {
                    const char* args[6] = {argv[3], argv[4], argv[9],
                                           argv[5], argv[6], argv[7]};
                    return sc_search_board(ti, dbase, filter, 6, args);
                }
            }
        }
        return errorResult (ti, "Usage: sc_filter search baseId filterName <header|board> [args]");

    case FILTER_TREESTATS: {
            const auto stats = dbase->getTreeStat(filter);
            UI_List res (stats.size());
            UI_List ginfo(9);
            for (auto const& node : stats) {
                ginfo.clear();
                ginfo.push_back(node.move ? node.move.getSAN() : "[end]");
                ginfo.push_back(node.freq[0]);
                ginfo.push_back(node.freq[RESULT_White]);
                ginfo.push_back(node.freq[RESULT_Draw]);
                ginfo.push_back(node.freq[RESULT_Black]);
                ginfo.push_back(node.avgElo());
                ginfo.push_back(node.eloPerformance());
                ginfo.push_back(node.eloCount);
                ginfo.push_back(node.move.getColor() == WHITE ? "W" : "B");
                res.push_back(ginfo);
            }
            return UI_Result(ti, OK, res);
        }

    case FILTER_EXPORT:
        if (argc >= 7 && argc <=9) {
            FILE* exportFile = fopen(argv[5], "wb");
            if (exportFile == NULL) return errorResult (ti, "Error opening file for exporting games.");
            Game g;
            if (strCompare("LaTeX", argv[6]) == 0) {
                g.SetPgnFormat (PGN_FORMAT_LaTeX);
                g.ResetPgnStyle (PGN_STYLE_TAGS | PGN_STYLE_COMMENTS | PGN_STYLE_VARS | PGN_STYLE_SHORT_HEADER | PGN_STYLE_SYMBOLS | PGN_STYLE_INDENT_VARS);
            } else { //Default to PGN
                g.SetPgnFormat (PGN_FORMAT_Plain);
                g.ResetPgnStyle (PGN_STYLE_TAGS | PGN_STYLE_COMMENTS | PGN_STYLE_VARS);
            }
            if (argc > 7) fprintf(exportFile, "%s", argv[7]);
            Progress progress = UI_CreateProgress(ti);
            size_t count = filter->size();
            gamenumT* idxList = new gamenumT[count];
            count = dbase->listGames(argv[4], 0, count, filter, idxList);
            errorT err = OK;
                for (size_t i = 0; i < count; ++i) {
                    const IndexEntry* ie = dbase->getIndexEntry(idxList[i]);
                    // Skip any corrupt games:
                    if (dbase->getGame(*ie, g) != OK) continue;

                    std::pair<const char*, unsigned> pgn = g.WriteToPGN(75, true);
                    if (pgn.second != fwrite(pgn.first, 1, pgn.second, exportFile)) {
                        err = ERROR_FileWrite;
                        break;
                    }
                    if ((i % 1024 == 0) && !progress.report(i, count)) {
                        err = ERROR_UserCancel;
                        break;
                    }
                }
            if (err == OK && argc > 8)
                fprintf(exportFile, "%s", argv[8]);
            fclose (exportFile);
            delete[] idxList;
            return UI_Result(ti, err);
        }
        return errorResult (ti, "Usage: sc_filter export baseId filterName sortCrit filename <PGN|LaTeX> [header] [footer]");

	}
    return InvalidCommand (ti, "sc_filter", options);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter_freq:
//    Returns a two-integer list showing how many filter games,
//    and how many total database games, meet the specified
//    date or mean rating range criteria.
//    Usage:
//        sc_filter freq baseId filterName date <startdate> [<endDate>]
//    or  sc_filter freq baseId filterName elo <lowerMeanElo> [<upperMeanElo>]
//Klimmek: or sc_filter freq baseId filterName moves <lowerhalfMove> <higherhalfMove>
//         add mode to count games with specified movenumber
//    where the final parameter defaults to the maximum allowed
//    date or Elo rating.
//    Note for rating queries: only the rating values in the game
//    are used; estimates from other games or the spelling file
//    will be ignored. Also, if one player has an Elo rating but
//    the other does not, the other rating will be assumed to be
//    same as the nonzero rating, up to a maximum of 2200.
int
sc_filter_freq (scidBaseT* dbase, const HFilter& filter, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage =
        "Usage: sc_filter freq baseId filterName date|elo|move <startDate|minElo|lowerhalfMove> [<endDate|maxElo|higherhalfMove>] [GuessElo]";

    bool eloMode = false;
    bool moveMode = false;
    bool guessElo = true;
    const char * options[] = { "date", "elo", "move", NULL };
    enum { OPT_DATE, OPT_ELO, OPT_MOVE };
    int option = -1;

    if (argc >= 6  &&  argc <= 8) {
        option = strUniqueMatch (argv[4], options);
    }
    switch (option) {
        case OPT_DATE: eloMode = false; break;
        case OPT_ELO: eloMode = true; break;
        case OPT_MOVE: moveMode = true; break;
        default: return errorResult (ti, usage);
    }

    dateT startDate = date_EncodeFromString (argv[5]);
    dateT endDate = DATE_MAKE (YEAR_MAX, 12, 31);
    uint minElo = strGetUnsigned (argv[5]);
    uint maxElo = MAX_ELO;
    uint maxMove, minMove;

    minMove = minElo;
    maxMove = minMove + 1;
    if (argc >= 7) {
        endDate = date_EncodeFromString (argv[6]);
        maxMove = maxElo = strGetUnsigned (argv[6]);
    }
    if (argc == 8) {
        guessElo = strGetUnsigned (argv[7]);
    }
    //Klimmek: define _NoEloGuess_: Do not guess Elo, else old behavior    
    if ( guessElo ) {
    // Double min/max Elos to avoid halving every mean Elo:
        minElo = minElo + minElo;
        maxElo = maxElo + maxElo + 1;
    }
    // Calculate frequencies in the specified date or rating range:
    uint filteredCount = 0;
    uint allCount = 0;

    if (eloMode) {
        for (uint gnum=0, n = dbase->numGames(); gnum < n; gnum++) {
            const IndexEntry* ie = dbase->getIndexEntry(gnum);
            if ( guessElo ) {
                uint wElo = ie->GetWhiteElo();
                uint bElo = ie->GetBlackElo();
                uint bothElo = wElo + bElo;
                if (wElo == 0  &&  bElo != 0) {
                    bothElo += (bElo > 2200 ? 2200 : bElo);
                } else if (bElo == 0  &&  wElo != 0) {
                    bothElo += (wElo > 2200 ? 2200 : wElo);
                }
                if (bothElo >= minElo  &&  bothElo <= maxElo) {
                    allCount++;
                    if (filter.get(gnum) != 0) {
                        filteredCount++;
                    }
                }
            } else {
                //Klimmek: if lowest Elo in the Range: count them
                uint mini = ie->GetWhiteElo();
                if ( mini > ie->GetBlackElo() ) mini = ie->GetBlackElo();
                if (mini < minElo  ||  mini >= maxElo)
                    continue;
                allCount++;
                if (filter.get(gnum) != 0) {
                    filteredCount++;
                }
            }
        }
    } else if ( moveMode ) {
        //Klimmek: count games with x Moves minMove=NumberHalfmove and maxMove Numberhalfmove+1
        for (uint gnum=0, n = dbase->numGames(); gnum < n; gnum++) {
            const IndexEntry* ie = dbase->getIndexEntry(gnum);
            uint move = ie->GetNumHalfMoves();
            if (move >= minMove  &&  move <= maxMove) {
                allCount++;
                if (filter.get(gnum) != 0) {
                    filteredCount++;
                }
            }
        }
    }
    else { // datemode
        for (uint gnum=0, n = dbase->numGames(); gnum < n; gnum++) {
            const IndexEntry* ie = dbase->getIndexEntry(gnum);
            dateT date = ie->GetDate();
            if (date >= startDate  &&  date <= endDate) {
                allCount++;
                if (filter.get(gnum) != 0) {
                    filteredCount++;
                }
            }
        }
    }
    appendUintElement (ti, filteredCount);
    appendUintElement (ti, allCount);
    return TCL_OK;
}

//TODO:
//This functions do not works because they do not specify the base, the filter and the sort criteria
//So for the moment we assume base=db, filter=dbFilter and sort=N+

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter_first:
//    Returns the game number of the first game in the filter,
//    or 0 if the filter is empty.
int
sc_filter_first(ClientData, Tcl_Interp * ti, int, const char**)
{
	for (uint gnum=0; gnum < db->numGames(); gnum++) {
		if (db->dbFilter->Get(gnum) == 0) continue;
		return setUintResult (ti, gnum +1);
	}
	return setUintResult (ti, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter_last:
//    Returns the game number of the last game in the filter,
//    or 0 if the filter is empty.
int
sc_filter_last(ClientData, Tcl_Interp * ti, int, const char**)
{
	long gnum = db->numGames();
	for (gnum--; gnum >= 0; gnum--) {
		if (db->dbFilter->Get(gnum) == 0) continue;
		return setUintResult (ti, gnum +1);
	}
	return setUintResult (ti, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter_next:
//    Returns number of next game in the filter.
int
sc_filter_next(ClientData, Tcl_Interp * ti, int, const char**)
{
    if (db->inUse) {
        uint nextNumber = db->gameNumber + 1;
        while (nextNumber < db->numGames()) {
            if (db->dbFilter->Get(nextNumber) > 0) {
                return setUintResult (ti, nextNumber + 1);
            }
            nextNumber++;
        }
    }
    return setUintResult (ti, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter_prev:
//    Returns number of previous game in the filter.
int
sc_filter_prev(ClientData, Tcl_Interp * ti, int, const char**)
{
    if (db->inUse) {
        int prevNumber = db->gameNumber - 1;
        while (prevNumber >= 0) {
            if (db->dbFilter->Get(prevNumber) > 0) {
                return setIntResult (ti, prevNumber + 1);
            }
            prevNumber--;
        }
    }
    return setUintResult (ti, 0);
}

//END TODO

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_filter_stats:
//    Returns statistics about the filter.
int
sc_filter_stats (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    enum {STATS_ALL, STATS_ELO, STATS_YEAR};

    if (argc < 2 || argc > 5) {
        return errorResult (ti, "Usage: sc_filter stats [all | elo <xx> | year <xx>]");
    }
    int statType = STATS_ALL;
    uint min = 0;
    uint max = 0;
    uint inv_max = 0;
    if (argc > 2) {
        if (argv[2][0] == 'e') { statType = STATS_ELO; }
        if (argv[2][0] == 'y') { statType = STATS_YEAR; }
    }
    if (statType == STATS_ELO  ||  statType == STATS_YEAR) {
        if (argc < 4) {
            return errorResult (ti, "Incorrect number of parameters.");
        }
        min = strGetUnsigned (argv[3]);
        max = strGetUnsigned (argv[4]);
        //Klimmek: +10000 workaround to trigger max elo in filter function
        if ( max > 10000 ) {
            max -= 10000;
            inv_max = 1;
        }
    }
    uint results[4] = {0, 0, 0, 0};
    uint total = 0;
    const HFilter filter = db->getFilter("dbfilter");
    for (uint i=0, n = db->numGames(); i < n; i++) {
        const IndexEntry* ie = db->getIndexEntry(i);
        if (filter.get(i)) {
            if ( max == 0 ) { //Old Statistic : 
                if (statType == STATS_ELO  &&
                    (ie->GetWhiteElo() < min  ||  ie->GetBlackElo() < min)) {
                    continue;
                }
                if (statType == STATS_YEAR
                    &&  date_GetYear(ie->GetDate()) < min) {
                    continue;
                }
            } else { //Klimmek:  new statistic: evaluation in intervals
                //count all games where player with highest Elo is in the specific range      
                if (statType == STATS_ELO ) {
                    if (inv_max) {
                        uint maxi = ie->GetWhiteElo();
                        if ( maxi < ie->GetBlackElo() ) maxi = ie->GetBlackElo();
                        if (maxi < min  ||  maxi >= max)
                            continue;
                    }
                    else {
                //count all games where player with lowest Elo is in the specific range      
                        uint mini = ie->GetWhiteElo();
                        if ( mini > ie->GetBlackElo() ) mini = ie->GetBlackElo();
                        if (mini < min  ||  mini >= max)
                            continue;
                    }
                }
                if (statType == STATS_YEAR
                    &&  ( date_GetYear(ie->GetDate()) < min || date_GetYear(ie->GetDate()) >= max) ) {
                    continue;
                }
            }
            results[ie->GetResult()]++;
            total++;
        }
    }
    char temp[80];
    uint percentScore = results[RESULT_White] * 2 + results[RESULT_Draw] +
        results[RESULT_None];
    percentScore = total ? percentScore * 500 / total : 0;
    sprintf (temp, "%7u %7u %7u %7u   %3u%c%u%%",
             total,
             results[RESULT_White],
             results[RESULT_Draw],
             results[RESULT_Black],
             percentScore / 10, decimalPointChar, percentScore % 10);
    Tcl_AppendResult (ti, temp, NULL);
    return TCL_OK;
}

//////////////////////////////////////////////////////////////////////
///  GAME functions

int
sc_game (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "altered",    "setaltered", "crosstable", "eco",
        "find",       "firstMoves", "import",
        "info",        "load",      "merge",      "moves",
        "new",        "novelty",    "number",     "pgn",
        "pop",        "push",       "SANtoUCI",   "save",
        "startBoard", "strip",
        "tags",       "truncate",   "UCI_currentPos",
        "undo",       "undoAll",    "undoPoint",  "redo",       NULL
    };
    enum {
        GAME_ALTERED,    GAME_SET_ALTERED, GAME_CROSSTABLE, GAME_ECO,
        GAME_FIND,       GAME_FIRSTMOVES, GAME_IMPORT,
        GAME_INFO,       GAME_LOAD,       GAME_MERGE,      GAME_MOVES,
        GAME_NEW,        GAME_NOVELTY,    GAME_NUMBER,     GAME_PGN,
        GAME_POP,        GAME_PUSH,       GAME_SANTOUCI,   GAME_SAVE,
        GAME_STARTBOARD, GAME_STRIP,
        GAME_TAGS,       GAME_TRUNCATE,   GAME_UCI_CURRENTPOS,
        GAME_UNDO,       GAME_UNDO_ALL,   GAME_UNDO_POINT, GAME_REDO
    };
    int index = -1;
    char old_language = 0;

    if (argc > 1) { index = strUniqueMatch (argv[1], options);}

    switch (index) {
    case GAME_ALTERED:
        return UI_Result(ti, OK, db->gameAltered);

    case GAME_SET_ALTERED:
        if (argc != 3 ) {
          return errorResult (ti, "Usage: sc_game setaltered [0|1]");
        }
        db->gameAltered = strGetUnsigned (argv[2]);
        break;
    case GAME_CROSSTABLE:
        return sc_game_crosstable (cd, ti, argc, argv);

    case GAME_ECO:  // "sc_game eco" is equivalent to "sc_eco game"
        return sc_eco_game (cd, ti, argc, argv);

    case GAME_FIND:
        return sc_game_find (cd, ti, argc, argv);

    case GAME_FIRSTMOVES:
        return sc_game_firstMoves (cd, ti, argc, argv);

    case GAME_IMPORT:
        return sc_game_import (cd, ti, argc, argv);

    case GAME_INFO:
        return sc_game_info (cd, ti, argc, argv);

    case GAME_LOAD:
        return sc_game_load (cd, ti, argc, argv);

    case GAME_MERGE:
        return sc_game_merge (cd, ti, argc, argv);

    case GAME_MOVES:
        return sc_game_moves (cd, ti, argc, argv);

    case GAME_NEW:
        db->gameAlterations.clear();
        return sc_game_new (cd, ti, argc, argv);

    case GAME_NOVELTY:
        return sc_game_novelty (cd, ti, argc, argv);

    case GAME_NUMBER:
        return setUintResult (ti, db->gameNumber + 1);

    case GAME_PGN:
        return sc_game_pgn (cd, ti, argc, argv);

    case GAME_POP:
        return sc_game_pop (cd, ti, argc, argv);

    case GAME_PUSH:
        return sc_game_push (cd, ti, argc, argv);

    case GAME_SANTOUCI:
        if (argc == 3) {
            auto pos = db->game->GetCurrentPos();
            simpleMoveT sm;
            errorT err = pos->ParseMove(&sm, argv[2]);
            if (err != OK)
                return UI_Result(ti, err);

            char buf[16] = {};
            sm.toLongNotation(buf);
            return UI_Result(ti, OK, buf);
        }
        return errorResult(ti, "usage sc_game SANtoUCI move");

    case GAME_SAVE:
        return sc_game_save (cd, ti, argc, argv);

    case GAME_STARTBOARD:
        return sc_game_startBoard (cd, ti, argc, argv);

    case GAME_STRIP:
        return sc_game_strip (cd, ti, argc, argv);

    case GAME_TAGS:
        return sc_game_tags (cd, ti, argc, argv);

    case GAME_TRUNCATE:
        old_language = language;
        language = 0;
        if (argc > 2 && strIsPrefix (argv[2], "-start")) {
            // "sc_game truncate -start" truncates the moves up to the
            // current position:
            db->game->TruncateStart();
        } else {
            // Truncate from the current position to the end of the game
            db->game->Truncate();
        }
        db->gameAltered = true;
        language = old_language;
        break;

    case GAME_UCI_CURRENTPOS:
        return UI_Result(ti, OK, db->game->currentPosUCI());

    case GAME_UNDO:
        if (argc > 2 && strCompare("size", argv[2]) == 0) {
            return UI_Result(ti, OK, (uint) db->gameAlterations.undoSize());
        }
        db->game = db->gameAlterations.undo(db->game);
        return UI_Result(ti, OK);

    case GAME_UNDO_ALL:
        db->gameAltered = false;
        db->gameAlterations.clear();
        if (db->gameNumber < 0) {
            db->game->Clear();
        } else {
            const IndexEntry* ie = db->getIndexEntry(db->gameNumber);
            errorT err = db->getGame(*ie, *db->game);
            if (err != OK) return UI_Result(ti, err);
            db->game->MoveToStart();
        }
        return UI_Result(ti, OK);

    case GAME_UNDO_POINT:
        db->gameAlterations.store(db->game);
        break;

    case GAME_REDO:
        if (argc > 2 && strCompare("size", argv[2]) == 0) {
            return UI_Result(ti, OK, (uint) db->gameAlterations.redoSize());
        }
        db->game = db->gameAlterations.redo(db->game);
        return UI_Result(ti, OK);

    default:
        return InvalidCommand (ti, "sc_game", options);
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// isCrosstableGame:
//    Returns true if the game with the specified index entry
//    is considered a crosstable game. It must have the specified
//    Event and Site, and a Date within the specified range or
//    have the specified non-zero EventDate.
static inline bool
isCrosstableGame (const IndexEntry* ie, idNumberT siteID, idNumberT eventID,
                  dateT eventDate)
{
    if (ie->GetSite() != siteID  ||  ie->GetEvent() != eventID) {
        return false;
    }
    dateT EventDate = ie->GetEventDate();
    if (eventDate != 0  && EventDate != 0 && EventDate != eventDate) {
        return false;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_crosstable:
//    Returns the crosstable for the current game.
int
sc_game_crosstable (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
#ifndef WINCE
    static const char * options [] = {
        "plain", "html", "hypertext", "latex", "filter", "count", NULL
    };
    enum {
        OPT_PLAIN, OPT_HTML, OPT_HYPERTEXT, OPT_LATEX, OPT_FILTER, OPT_COUNT
    };
    int option = -1;

    const char * usageMsg =
        "Usage: sc_game crosstable plain|html|hypertext|filter|count [name|rating|score|country] [allplay|swiss] [(+|-)(colors|countries|tallies|ratings|titles|groups|breaks|numcolumns)]";

    static const char * extraOptions [] = {
        "allplay", "knockout", "swiss", "auto",
        "name", "rating", "score", "country",
        "-ages", "+ages",               // Show player ages
        "-breaks", "+breaks",           // Show tiebreak scores
        "-colors", "+colors",           // Show game colors in Swiss table
        "-countries", "+countries",     // Show current countries
        "-flags", "+flags",             // Show flags
        "-tallies", "+tallies",
        "-ratings", "+ratings",         // Show Elo ratings
        "-titles", "+titles",           // Show FIDE titles
        "-groups", "+groups",           // Separate players into score groups
        "-deleted", "+deleted",         // Include deleted games in table
        "-numcolumns", "+numcolumns",   // All-play-all numbered columns
        "-gameNumber",
        "-threewin", "+threewin",       // Give 3 points for win, 1 for draw
        NULL
    };
    enum {
        EOPT_ALLPLAY, EOPT_KNOCKOUT, EOPT_SWISS, EOPT_AUTO,
        EOPT_SORT_NAME, EOPT_SORT_RATING, EOPT_SORT_SCORE, EOPT_SORT_COUNTRY,
        EOPT_AGES_OFF, EOPT_AGES_ON,
        EOPT_BREAKS_OFF, EOPT_BREAKS_ON,
        EOPT_COLORS_OFF, EOPT_COLORS_ON,
        EOPT_COUNTRIES_OFF, EOPT_COUNTRIES_ON,
        EOPT_FLAGS_OFF, EOPT_FLAGS_ON,
        EOPT_TALLIES_OFF, EOPT_TALLIES_ON,
        EOPT_RATINGS_OFF, EOPT_RATINGS_ON,
        EOPT_TITLES_OFF, EOPT_TITLES_ON,
        EOPT_GROUPS_OFF, EOPT_GROUPS_ON,
        EOPT_DELETED_OFF, EOPT_DELETED_ON,
        EOPT_NUMCOLUMNS_OFF, EOPT_NUMCOLUMNS_ON,
        EOPT_GNUMBER,
        EOPT_THREEWIN_OFF, EOPT_THREEWIN_ON
    };

    int sort = EOPT_SORT_SCORE;
    crosstableModeT mode = CROSSTABLE_AllPlayAll;
    bool showAges = true;
    bool showColors = true;
    bool showCountries = true;
    bool showFlags = true;
    bool showTallies = true;
    bool showRatings = true;
    bool showTitles = true;
    bool showBreaks = false;
    bool scoreGroups = false;
    bool useDeletedGames = false;
    bool numColumns = false;  // Numbers for columns in all-play-all table
    uint numTableGames = 0;
    uint gameNumber = 0;
    bool threewin = false;

    if (argc >= 3) { option = strUniqueMatch (argv[2], options); }
    if (option < 0) { return errorResult (ti, usageMsg); }

    for (int arg=3; arg < argc; arg++) {
        int extraOption = strUniqueMatch (argv[arg], extraOptions);
        switch (extraOption) {
            case EOPT_ALLPLAY:        mode = CROSSTABLE_AllPlayAll; break;
            case EOPT_KNOCKOUT:       mode = CROSSTABLE_Knockout;   break;
            case EOPT_SWISS:          mode = CROSSTABLE_Swiss;      break;
            case EOPT_AUTO:           mode = CROSSTABLE_Auto;       break;
            case EOPT_SORT_NAME:      sort = EOPT_SORT_NAME;   break;
            case EOPT_SORT_RATING:    sort = EOPT_SORT_RATING; break;
            case EOPT_SORT_SCORE:     sort = EOPT_SORT_SCORE;  break;
            case EOPT_SORT_COUNTRY:   sort = EOPT_SORT_COUNTRY;  break;
            case EOPT_AGES_OFF:       showAges = false;        break;
            case EOPT_AGES_ON:        showAges = true;         break;
            case EOPT_BREAKS_OFF:     showBreaks = false;      break;
            case EOPT_BREAKS_ON:      showBreaks = true;       break;
            case EOPT_COLORS_OFF:     showColors = false;      break;
            case EOPT_COLORS_ON:      showColors = true;       break;
            case EOPT_COUNTRIES_OFF:  showCountries = false;   break;
            case EOPT_COUNTRIES_ON:   showCountries = true;    break;
            case EOPT_FLAGS_OFF:      showFlags = false;       break;
            case EOPT_FLAGS_ON:       showFlags = true;        break;
            case EOPT_TALLIES_OFF:    showTallies = false;     break;
            case EOPT_TALLIES_ON:     showTallies = true;      break;
            case EOPT_RATINGS_OFF:    showRatings = false;     break;
            case EOPT_RATINGS_ON:     showRatings = true;      break;
            case EOPT_TITLES_OFF:     showTitles = false;      break;
            case EOPT_TITLES_ON:      showTitles = true;       break;
            case EOPT_GROUPS_OFF:     scoreGroups = false;     break;
            case EOPT_GROUPS_ON:      scoreGroups = true;      break;
            case EOPT_DELETED_OFF:    useDeletedGames = false; break;
            case EOPT_DELETED_ON:     useDeletedGames = true;  break;
            case EOPT_NUMCOLUMNS_OFF: numColumns = false;      break;
            case EOPT_NUMCOLUMNS_ON:  numColumns = true;       break;
            case EOPT_GNUMBER:
                // Game number to print the crosstable for is
                // given in the next argument:
                if (arg+1 >= argc) { return errorResult (ti, usageMsg); }
                gameNumber = strGetUnsigned (argv[arg+1]);
                arg++;
                break;
            case EOPT_THREEWIN_OFF:  threewin = false ; break;
            case EOPT_THREEWIN_ON:   threewin = true  ; break;
            default: return errorResult (ti, usageMsg);
        }
    }
    if (!db->inUse) { return TCL_OK; }

    const char * newlineStr = "";
    switch (option) {
        case OPT_PLAIN:     newlineStr = "\n";     break;
        case OPT_HTML:      newlineStr = "<br>\n"; break;
        case OPT_HYPERTEXT: newlineStr = "<br>";   break;
        case OPT_LATEX:     newlineStr = "\\\\\n"; break;
    }

    // Load crosstable game if necessary:
    Game * g = db->game;
    if (gameNumber > 0) {
        g = scratchGame;
        g->Clear();
        if (gameNumber > db->numGames()) {
            return setResult (ti, "Invalid game number");
        }
        const IndexEntry* ie = db->getIndexEntry(gameNumber - 1);
        if (ie->GetLength() == 0) {
            return errorResult (ti, "Error: empty game file record.");
        }
        if (db->getGame(*ie, *g) != OK) {
            return errorResult (ti, "Error reading game file.");
        }
    }

    idNumberT eventId = 0, siteId = 0;
    if (db->getNameBase()->FindExactName (NAME_EVENT, g->GetEventStr(), &eventId) != OK) {
        return TCL_OK;
    }
    if (db->getNameBase()->FindExactName (NAME_SITE, g->GetSiteStr(), &siteId) != OK) {
        return TCL_OK;
    }

    dateT eventDate = g->GetEventDate();
    dateT firstSeenDate = g->GetDate();
    dateT lastSeenDate = g->GetDate();

    Crosstable * ctable = new Crosstable;
    if (sort == EOPT_SORT_NAME) { ctable->SortByName(); }
    if (sort == EOPT_SORT_RATING) { ctable->SortByElo(); }
    if (sort == EOPT_SORT_COUNTRY) { ctable->SortByCountry(); }

    ctable->SetThreeWin(threewin);
    ctable->SetSwissColors (showColors);
    ctable->SetAges (showAges);
    ctable->SetCountries (showCountries);
    ctable->SetFlags (showFlags);
    ctable->SetTallies (showTallies);
    ctable->SetElos (showRatings);
    ctable->SetTitles (showTitles);
    ctable->SetTiebreaks (showBreaks);
    ctable->SetSeparateScoreGroups (scoreGroups);
    ctable->SetDecimalPointChar (decimalPointChar);
    ctable->SetNumberedColumns (numColumns);

    switch (option) {
        case OPT_PLAIN:     ctable->SetPlainOutput();     break;
        case OPT_HTML:      ctable->SetHtmlOutput();      break;
        case OPT_HYPERTEXT: ctable->SetHypertextOutput(); break;
        case OPT_LATEX:     ctable->SetLaTeXOutput();     break;
    }

    // Find all games that should be listed in the crosstable:
    const SpellChecker* spell = spellChk;
    bool tableFullMessage = false;
    for (uint i=0, n = db->numGames(); i < n; i++) {
        const IndexEntry* ie = db->getIndexEntry(i);
        if (ie->GetDeleteFlag()  &&  !useDeletedGames) { continue; }
        if (! isCrosstableGame (ie, siteId, eventId, eventDate)) {
            continue;
        }
        idNumberT whiteId = ie->GetWhite();
        const char * whiteName = db->getNameBase()->GetName (NAME_PLAYER, whiteId);
        idNumberT blackId = ie->GetBlack();
        const char * blackName = db->getNameBase()->GetName (NAME_PLAYER, blackId);

        // Ensure we have two different players:
        if (whiteId == blackId) { continue; }

        // If option is OPT_FILTER, adjust the filter and continue &&&
        if (option == OPT_FILTER) {
            db->dbFilter->Set (i, 1);
            continue;
        }

        // If option is OPT_COUNT, increment game count and continue:
        if (option == OPT_COUNT) {
            numTableGames++;
            continue;
        }

        // Add the two players to the crosstable:
        if (ctable->AddPlayer (whiteId, whiteName, ie->GetWhiteElo(), spell) != OK  ||
            ctable->AddPlayer (blackId, blackName, ie->GetBlackElo(), spell) != OK)
        {
            if (! tableFullMessage) {
                tableFullMessage = true;
                Tcl_AppendResult (ti, "Warning: Player limit reached; table is incomplete\n\n", NULL);
            }
            continue;
        }

        uint round = strGetUnsigned (db->getNameBase()->GetName (NAME_ROUND, ie->GetRound()));
        dateT date = ie->GetDate();
        resultT result = ie->GetResult();
        ctable->AddResult (i+1, whiteId, blackId, result, round, date);
        if (date < firstSeenDate) { firstSeenDate = date; }
        if (date > lastSeenDate) { lastSeenDate = date; }
    }

    if (option == OPT_COUNT) {
        // Just return a count of the number of tournament games:
        delete ctable;
        return setUintResult (ti, numTableGames);
    }
    if (option == OPT_FILTER) {
        delete ctable;
        return TCL_OK;
    }
    if (ctable->NumPlayers() < 2) {
        delete ctable;
        return setResult (ti, "No crosstable for this game.");
    }

    if (option == OPT_LATEX) {
        Tcl_AppendResult (ti, "\\documentclass[10pt,a4paper]{article}\n\n",
                          "\\usepackage{a4wide}\n\n",
                          "\\begin{document}\n\n",
                          "\\setlength{\\parindent}{0cm}\n",
                          "\\setlength{\\parskip}{0.5ex}\n",
                          "\\small\n", NULL);
    }

    if (mode == CROSSTABLE_Auto) { mode = ctable->BestMode(); }

    // Limit all-play-all tables to 300 players:
    uint apaLimit = 300;
    if (mode == CROSSTABLE_AllPlayAll  &&
            ctable->NumPlayers() > apaLimit  &&
            !tableFullMessage) {
        Tcl_AppendResult (ti, "Warning: Too many players for all-play-all; try displaying as a swiss tournament.\n\n", NULL);
    }

    char stemp[1000];
    sprintf (stemp, "%s%s%s, ", g->GetEventStr(), newlineStr, g->GetSiteStr());
    Tcl_AppendResult (ti, stemp, NULL);
    date_DecodeToString (firstSeenDate, stemp);
    strTrimDate (stemp);
    Tcl_AppendResult (ti, stemp, NULL);
    if (lastSeenDate != firstSeenDate) {
        date_DecodeToString (lastSeenDate, stemp);
        strTrimDate (stemp);
        Tcl_AppendResult (ti, " - ", stemp, NULL);
    }
    Tcl_AppendResult (ti, newlineStr, NULL);

    eloT avgElo = ctable->AvgRating();
    if (avgElo > 0  &&  showRatings) {
        Tcl_AppendResult (ti, translate (ti, "AverageRating", "Average Rating"),
                          ": ", NULL);
        appendUintResult (ti, avgElo);
        uint category = ctable->FideCategory (avgElo);
        if (category > 0  &&  mode == CROSSTABLE_AllPlayAll) {
            sprintf (stemp, "  (%s %u)",
                     translate (ti, "Category", "Category"), category);
            Tcl_AppendResult (ti, stemp, NULL);
        }
        Tcl_AppendResult (ti, newlineStr, NULL);
    }

    DString * dstr = new DString;
    if (mode != CROSSTABLE_AllPlayAll) { apaLimit = 0; }
    ctable->PrintTable (dstr, mode, apaLimit, db->gameNumber+1);

    Tcl_AppendResult (ti, dstr->Data(), NULL);
    if (option == OPT_LATEX) {
        Tcl_AppendResult (ti, "\n\\end{document}\n", NULL);
    }
    delete ctable;
    delete dstr;
#endif
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_find:
//    Returns the game number of the game in that current database
//    that best matches the specified number, player names, site,
//    round, year and result.
//    This command is used primarily to locate a bookmarked game in
//    a database where the number may be inaccurate due to database
//    sorting or compaction.
int
sc_game_find (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 9) {
        return errorResult (ti, "sc_game_find: Incorrect parameters");
    }

    uint gnum = strGetUnsigned (argv[2]);
    if (gnum == 0) { return setUintResult (ti, 0); }
    gnum--;
    const char * whiteStr = argv[3];
    const char * blackStr = argv[4];
    const char * siteStr = argv[5];
    const char * roundStr = argv[6];
    uint year = strGetUnsigned(argv[7]);
    resultT result = strGetResult (argv[8]);

    idNumberT white, black, site, round;
    white = black = site = round = 0;
    db->getNameBase()->FindExactName (NAME_PLAYER, whiteStr, &white);
    db->getNameBase()->FindExactName (NAME_PLAYER, blackStr, &black);
    db->getNameBase()->FindExactName (NAME_SITE, siteStr, &site);
    db->getNameBase()->FindExactName (NAME_ROUND, roundStr, &round);

    // We give each game a "score" which is 1 for each matching field.
    // So the best possible score is 6.

    // First, check if the specified game number matches all fields:
    if (db->numGames() > gnum) {
        uint score = 0;
        const IndexEntry* ie = db->getIndexEntry(gnum);
        if (ie->GetWhite() == white) { score++; }
        if (ie->GetBlack() == black) { score++; }
        if (ie->GetSite() == site) { score++; }
        if (ie->GetRound() == round) { score++; }
        if (ie->GetYear() == year) { score++; }
        if (ie->GetResult() == result) { score++; }
        if (score == 6) { return setUintResult (ti, gnum+1); }
    }

    // Now look for the best matching game:
    uint bestNum = 0;
    uint bestScore = 0;

    for (uint i=0, n = db->numGames(); i < n; i++) {
        uint score = 0;
        const IndexEntry* ie = db->getIndexEntry(i);
        if (ie->GetWhite() == white) { score++; }
        if (ie->GetBlack() == black) { score++; }
        if (ie->GetSite() == site) { score++; }
        if (ie->GetRound() == round) { score++; }
        if (ie->GetYear() == year) { score++; }
        if (ie->GetResult() == result) { score++; }
        // Update if the best score, favouring the specified game number
        // in the case of a tie:
        if (score > bestScore  ||  (score == bestScore  &&  gnum == i)) {
            bestScore = score;
            bestNum = i;
        }
        // Stop now if the best possible match is found:
        if (score == 6) { break; }
    }
    return setUintResult (ti, bestNum + 1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_firstMoves:
//    get the first few moves of the specified game as  a text line.
//    E.g., "sc_game firstMoves 4" might return "1.e4 e5 2.Nf3 Nf6"
int
sc_game_firstMoves (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_game firstMoves <numMoves>");
    }
    if (!db->inUse) {
        return errorResult (ti, errMsgNotOpen(ti));
    }

    int plyCount = strGetInteger (argv[2]);
    // Check plyCount is a reasonable value, or set it to current plycount.
    if (plyCount < 0)  plyCount = db->game->GetCurrentPly();
    if (plyCount == 0) plyCount = 1;

    DString dstr;
    db->game->GetPartialMoveList (&dstr, plyCount);
    return UI_Result(ti, OK, std::string(dstr.Data()));
}

int sc_game_import(ClientData, Tcl_Interp* ti, int argc, const char** argv) {
	if (argc != 3)
		return errorResult(ti, "Usage: sc_game import <pgn-text>");

	db->game->Clear();
	db->gameAltered = true;

	PgnParseLog pgn;
	if (!pgnParseGame(argv[2], std::strlen(argv[2]), *db->game, pgn) &&
	    pgn.log.empty())
		return UI_Result(ti, OK, "No PGN text found.");

	if (pgn.log.empty())
		return UI_Result(ti, OK,
		                 "PGN text imported with no errors or warnings.");

	return UI_Result(ti, OK,
	                 "Errors/warnings importing PGN text:\n\n" + pgn.log);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// probe_tablebase:
//    Probes the tablebases for the current position, and returns
//    the score, a descriptive score with optimal moves, or just a
//    (random) optimal move.
bool
probe_tablebase (Tcl_Interp * ti, int mode, DString * dstr)
{
    int score = 0;
    bool showResult = false;
    bool showSummary = false;
    bool fullReport = false;
    bool optimalMoves = false;
    colorT toMove = db->game->GetCurrentPos()->GetToMove();

    switch (mode) {
    case PROBE_RESULT:
        showResult = true;
        break;
    case PROBE_SUMMARY:
        showResult = true;
        showSummary = true;
        break;
    case PROBE_REPORT:
        fullReport = true;
        break;
    case PROBE_OPTIMAL:
        optimalMoves = true;
        break;
    default:
        return false;
    }

    if (scid_TB_Probe (db->game->GetCurrentPos(), &score) != OK) {
        if (! fullReport) { return false; }
    }

    Position * gamePos = NULL;
    bool moveFound [MAX_LEGAL_MOVES] = {0};
    int moveScore [MAX_LEGAL_MOVES] = {0};
    bool movePrinted [MAX_LEGAL_MOVES] = {0};
    uint winCount = 0;
    uint drawCount = 0;
    uint lossCount = 0;
    uint unknownCount = 0;
    
    MoveList moveList;
    sanListT sanList;
    gamePos = db->game->GetCurrentPos();
    gamePos->GenerateMoves (&moveList);
    gamePos->CalcSANStrings (&sanList, SAN_CHECKTEST);

    if (showSummary  ||  fullReport  ||  optimalMoves) {
        Position scratchPos = *gamePos;

        for (uint i=0; i < moveList.Size(); i++) {
            simpleMoveT * smPtr = moveList.Get(i);
            scratchPos.DoSimpleMove (smPtr);
            moveFound[i] = false;
            movePrinted[i] = false;
            int newScore = 0;
            if (scid_TB_Probe (&scratchPos, &newScore) == OK) {
                moveFound[i] = true;
                moveScore[i] = newScore;
                if (newScore < 0) {
                    winCount++;
                } else if (newScore == 0) {
                    drawCount++;
                } else {
                    lossCount++;
                }
            } else {
                unknownCount++;
            }
            scratchPos.UndoSimpleMove (smPtr);
        }
    }

    // Optimal moves mode: return only the optimal moves, nothing else.
    if (optimalMoves) {
        uint count = 0;
        for (uint i=0; i < moveList.Size(); i++) {
            if ((score >= 0  &&  moveScore[i] == -score)  ||
                (score < 0  &&  moveScore[i] == -score - 1)) {
                if (count > 0) { dstr->Append (" "); }
                dstr->Append (sanList.list[i]);
                count++;
            }
        }
        return true;
    }

    if (fullReport) {
        char tempStr [80];
        sprintf (tempStr, "+:%u  =:%u  -:%u  ?:%u",
                 winCount, drawCount, lossCount, unknownCount);
        dstr->Append (tempStr);
        int prevScore = -9999999;   // Lower than any possible TB score
        bool first = true;

        while (1) {
            bool found = false;
            uint index = 0;
            int bestScore = 0;
            const char * bestMove = "";
            for (uint i=0; i < moveList.Size(); i++) {
                if (movePrinted[i]) { continue; }
                if (! moveFound[i]) { continue; }
                int newScore = - moveScore[i];
                if (!found  ||
                    (newScore > 0 && bestScore <= 0)  ||
                    (newScore > 0 && newScore < bestScore)  ||
                    (newScore == 0 && bestScore < 0)  ||
                    (newScore < 0 && bestScore < 0 && newScore < bestScore)  ||
                    (newScore == bestScore &&
                     strCompare (bestMove, sanList.list[i]) > 0) ) {
                    found = true;
                    index = i;
                    bestScore = newScore;
                    bestMove = sanList.list[i];
                }
            }
            if (!found) { break; }
            movePrinted[index] = true;
            if (first ||
                (bestScore > 0  && prevScore < 0)  ||
                (bestScore == 0 && prevScore != 0)  ||
                (bestScore < 0  && prevScore >= 0)) {
                dstr->Append ("\n");
                first = false;
                const char * tag = NULL;
                const char * msg = NULL;
                if (bestScore > 0) {
                    tag = "WinningMoves"; msg = "Winning moves";
                } else if (bestScore < 0) {
                    tag = "LosingMoves"; msg = "Losing moves";
                } else {
                    tag = "DrawingMoves"; msg = "Drawing moves";
                }
                dstr->Append ("\n", translate(ti, tag, msg), ":");
            }
            if (bestScore != prevScore) {
                if (bestScore > 0) {
                    sprintf (tempStr, " +%3d   ", bestScore);
                } else if (bestScore == 0) {
                    strCopy (tempStr, " =      ");
                } else {
                    sprintf (tempStr, " -%3d   ", -bestScore);
                }
                dstr->Append ("\n", tempStr);
            } else {
                dstr->Append (", ");
            }
            prevScore = bestScore;
            dstr->Append (bestMove);
        }
        if (unknownCount > 0) {
            dstr->Append ("\n\n");
            dstr->Append (translate (ti, "UnknownMoves", "Unknown-result moves"));
            dstr->Append (":\n ?      ");
            bool firstUnknown = true;
            while (1) {
                bool found = false;
                const char * bestMove = "";
                uint index = 0;
                for (uint i=0; i < moveList.Size(); i++) {
                    if (!moveFound[i]  && !movePrinted[i]) {
                        if (!found  ||
                            strCompare (bestMove, sanList.list[i]) > 0) {
                            found = true;
                            bestMove = sanList.list[i];
                            index = i;
                        }
                    }
                }
                if (!found) { break; }
                movePrinted[index] = true;
                if (!firstUnknown) {
                    dstr->Append (", ");
                }
                firstUnknown = false;
                dstr->Append (bestMove);
            }
        }
        dstr->Append ("\n");
        return true;
    }

    if (score == 0) {
        // Print drawn tablebase position info:
        if (showResult) {
            dstr->Append ("= [", translate (ti, "Draw"));
        }
        if (showSummary) {
            uint drawcount = 0;
            uint losscount = 0;
            const char * drawlist [MAX_LEGAL_MOVES];
            const char * losslist [MAX_LEGAL_MOVES];

            for (uint i=0; i < moveList.Size(); i++) {
                if (moveFound[i]) {
                    if (moveScore[i] == 0) {
                        drawlist[drawcount] = sanList.list[i];
                        drawcount++;
                    } else {
                        losslist[losscount] = sanList.list[i];
                        losscount++;
                    }
                }
            }
            if (moveList.Size() == 0) {
                dstr->Append (" (", translate (ti, "stalemate"), ")");
            } else if (drawcount == moveList.Size()) {
                dstr->Append (" ", translate (ti, "withAllMoves"));
            } else if (drawcount == 1) {
                dstr->Append (" ", translate (ti, "with"));
                dstr->Append (" ", drawlist[0]);
            } else if (drawcount+1 == moveList.Size() && losscount==1) {
                dstr->Append (" ", translate (ti, "withAllButOneMove"));
            } else if (drawcount > 0) {
                dstr->Append (" ", translate (ti, "with"), " ");
                dstr->Append (drawcount);
                dstr->Append (" ");
                if (drawcount == 1) {
                    dstr->Append (translate (ti, "move"));
                } else {
                    dstr->Append (translate (ti, "moves"));
                }
                dstr->Append (": ");
                for (uint m=0; m < drawcount; m++) {
                    if (m < 3) {
                        if (m > 0) { dstr->Append (", "); }
                        dstr->Append (drawlist[m]);
                    }
                }
                if (drawcount > 3) { dstr->Append (", ..."); }
            }
            if (losscount > 0) {
                dstr->Append (" (");
                if (losscount == 1) {
                    if (losscount+drawcount == moveList.Size()) {
                        dstr->Append (translate (ti, "only"), " ");
                    }
                    dstr->Append (losslist[0], " ", translate (ti, "loses"));
                } else if (drawcount < 4  &&
                           drawcount+losscount == moveList.Size()) {
                    dstr->Append (translate (ti, "allOthersLose"));
                } else {
                    dstr->Append (losscount);
                    dstr->Append (" ", translate (ti, "lose"), ": ");
                    for (uint m=0; m < losscount; m++) {
                        if (m < 3) {
                            if (m > 0) { dstr->Append (", "); }
                            dstr->Append (losslist[m]);
                        }
                    }
                    if (losscount > 3) { dstr->Append (", ..."); }
                }
                dstr->Append (")");
            }
        }
        if (showResult) { dstr->Append ("]"); }

    } else if (score > 0) {
        // Print side-to-move-mates tablebase info:
        if (showResult) {
            char temp[200];
            sprintf (temp, "%s:%d [%s %s %d",
                     toMove == WHITE ? "+-" : "-+", score,
                     translate (ti, toMove == WHITE ? "White" : "Black"),
                     translate (ti, "matesIn"), score);
            dstr->Append (temp);
        }

        // Now show all moves that mate optimally.
        // This requires generating all legal moves, and trying each
        // to find its tablebase score; optimal moves will have
        // the condition (new_score == -old_score).

        if (showSummary) {
            uint count = 0;

            for (uint i=0; i < moveList.Size(); i++) {
                if (moveFound[i]  &&  moveScore[i] == -score) {
                    count++;
                    if (count == 1) {
                        dstr->Append (" ", translate (ti, "with"), ": ");
                    } else {
                        dstr->Append (", ");
                    }
                    dstr->Append (sanList.list[i]);
                }
            }
        }
        if (showResult) { dstr->Append ("]"); }

    } else {
        // Score is negative so side to move is LOST:
        if (showResult) {
            char tempStr [80];
            if (score == -1) {
                sprintf (tempStr, "# [%s %s %s",
                         translate (ti, toMove == WHITE ? "Black" : "White"),
                         translate (ti, "hasCheckmated"),
                         translate (ti, toMove == WHITE ? "White" : "Black"));
            } else {
                sprintf (tempStr, "%s:%d [%s %s %d",
                         toMove == WHITE ? "-+" : "+-", -1 - score,
                         translate (ti, toMove == WHITE ? "Black" : "White"),
                         translate (ti, "matesIn"),
                         -1 - score);
            }
            dstr->Append (tempStr);
        }

        // Now show all moves that last optimally.
        // This requires generating all legal moves, and trying
        // each to find its tablebase score; optimal moves will
        // have the condition (new_score == (-old_score - 1)).

        if (showSummary) {
            uint count = 0;
            for (uint i=0; i < moveList.Size(); i++) {
                if (moveFound[i]  &&  moveScore[i] == (-score - 1)) {
                    count++;
                    dstr->Append (", ");
                    if (count == 1) {
                        dstr->Append (translate (ti, "longest"), ": ");
                    }
                    dstr->Append (sanList.list[i]);
                }
            }
        }
        if (showResult) { dstr->Append ("]"); }
    }

    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_info:
//    Return the Game Info string for the active game.
//    The returned text includes color codes.
int
sc_game_info (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    bool hideNextMove = false;
    bool showMaterialValue = false;
    bool showFEN = false;
    uint commentWidth = 50;
    uint commentHeight = 1;
    bool fullComment = false;
    uint showTB = 2;  // 0 = no TB output, 1 = score only, 2 = best moves.
    char temp[1024];

    int arg = 2;
    while (arg < argc) {
        if  (strIsPrefix (argv[arg], "-hideNextMove")) {
            if (arg+1 < argc) {
                arg++;
                hideNextMove = strGetBoolean(argv[arg]);
            }
        } else if  (strIsPrefix (argv[arg], "-materialValue")) {
            if (arg+1 < argc) {
                arg++;
                showMaterialValue = strGetBoolean(argv[arg]);
            }
        } else if  (strIsPrefix (argv[arg], "-tb")) {
            if (arg+1 < argc) {
                arg++;
                showTB = strGetUnsigned(argv[arg]);
            }
        } else if  (strIsPrefix (argv[arg], "-fen")) {
            if (arg+1 < argc) {
                arg++;
                showFEN = strGetBoolean(argv[arg]);
            }
        } else if  (strIsPrefix (argv[arg], "-cfull")) {
            // Show full comment:
            if (arg+1 < argc) {
                arg++;
                fullComment = strGetBoolean(argv[arg]);
                if (fullComment) {
                    commentWidth = 99999;
                    commentHeight = 99999;
                }
            }
        } else if  (strIsPrefix (argv[arg], "-cwidth")) {
            if (arg+1 < argc) {
                arg++;
                commentWidth = strGetBoolean(argv[arg]);
            }
        } else if  (strIsPrefix (argv[arg], "-cheight")) {
            if (arg+1 < argc) {
                arg++;
                commentHeight = strGetBoolean(argv[arg]);
            }
        } else if (strIsPrefix (argv[arg], "white")) {
            Tcl_AppendResult (ti, db->game->GetWhiteStr(), NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "welo")) {
            return setIntResult (ti, db->game->GetWhiteElo() );
        } else if (strIsPrefix (argv[arg], "black")) {
            Tcl_AppendResult (ti, db->game->GetBlackStr(), NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "belo")) {
            return setIntResult (ti, db->game->GetBlackElo() );
        } else if (strIsPrefix (argv[arg], "event")) {
            Tcl_AppendResult (ti, db->game->GetEventStr(), NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "site")) {
            Tcl_AppendResult (ti, db->game->GetSiteStr(), NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "round")) {
            Tcl_AppendResult (ti, db->game->GetRoundStr(), NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "date")) {
            char dateStr [12];
            date_DecodeToString (db->game->GetDate(), dateStr);
            Tcl_AppendResult (ti, dateStr, NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "year")) {
            return setUintResult (ti, date_GetYear (db->game->GetDate()));
        } else if (strIsPrefix (argv[arg], "result")) {
            return setResult (ti, RESULT_STR[db->game->GetResult()]);
        } else if (strIsPrefix (argv[arg], "nextMove")) {
            db->game->GetSAN (temp);
            transPieces(temp);
            Tcl_AppendResult (ti, temp, NULL);
            return TCL_OK;
// nextMoveNT is the same as nextMove, except that the move is not translated
        } else if (strIsPrefix (argv[arg], "nextMoveNT")) {
            db->game->GetSAN (temp);
            Tcl_AppendResult (ti, temp, NULL);
            return TCL_OK;
// returns next move played in UCI format
        } else if (strIsPrefix (argv[arg], "nextMoveUCI")) {
          db->game->GetNextMoveUCI (temp);
          Tcl_AppendResult (ti, temp, NULL);
          return TCL_OK;
        } else if (strIsPrefix (argv[arg], "previousMove")) {
            db->game->GetPrevSAN (temp);
            transPieces(temp);
            Tcl_AppendResult (ti, temp, NULL);
            return TCL_OK;
// previousMoveNT is the same as previousMove, except that the move is not translated
        } else if (strIsPrefix (argv[arg], "previousMoveNT")) {
            db->game->GetPrevSAN (temp);
            Tcl_AppendResult (ti, temp, NULL);
            return TCL_OK;
// returns previous move played in UCI format
        } else if (strIsPrefix (argv[arg], "previousMoveUCI")) {
            db->game->GetPrevMoveUCI (temp);
            Tcl_AppendResult (ti, temp, NULL);
            return TCL_OK;
        } else if (strIsPrefix (argv[arg], "duplicate")) {
            uint dupGameNum = db->getDuplicates(db->gameNumber);
            return setUintResult (ti, dupGameNum);
        } else if (strIsPrefix(argv[arg], "ECO")) {
            std::string str;
            if (ecoBook) {
                auto ecoStr = ecoBook->findECOstr(db->game->GetCurrentPos());
                if (!ecoStr.empty())
                    str.append(ecoStr);
            }
            return UI_Result(ti, OK, str);
        }
        arg++;
    }

    const char * gameStr = translate (ti, "game");
    sprintf (temp, "%c%s %u:  <pi %s>%s</pi>", toupper(gameStr[0]),
             gameStr + 1, db->gameNumber + 1,
             db->game->GetWhiteStr(), db->game->GetWhiteStr());
    if (auto whCountry = db->game->FindExtraTag("WhiteCountry"))
        sprintf(temp + std::strlen(temp), " (%s)", whCountry);

    Tcl_AppendResult (ti, temp, NULL);
    eloT elo = db->game->GetWhiteElo();
    bool eloEstimated = false;
    if (elo == 0) {
        elo = db->game->GetWhiteEstimateElo();
        eloEstimated = true;
    }
    if (elo != 0) {
        sprintf (temp, " <red>%u%s</red>", elo, eloEstimated ? "*" : "");
        Tcl_AppendResult (ti, temp, NULL);
    }
    sprintf (temp, "  --  <pi %s>%s</pi>",
             db->game->GetBlackStr(), db->game->GetBlackStr());
    if (auto blCountry = db->game->FindExtraTag("BlackCountry"))
        sprintf(temp + std::strlen(temp), " (%s)", blCountry);

    Tcl_AppendResult (ti, temp, NULL);
    elo = db->game->GetBlackElo();
    eloEstimated = false;
    if (elo == 0) {
        elo = db->game->GetBlackEstimateElo();
        eloEstimated = true;
    }
    if (elo != 0) {
        sprintf (temp, " <red>%u%s</red>", elo, eloEstimated ? "*" : "");
        Tcl_AppendResult (ti, temp, NULL);
    }

    if (hideNextMove) {
        sprintf (temp, "<br>(%s: %s)",
                 translate (ti, "Result"), translate (ti, "hidden"));
    } else {
        sprintf (temp, "<br>%s <red>(%u)</red>",
                 RESULT_LONGSTR[db->game->GetResult()],
                 (db->game->GetNumHalfMoves() + 1) / 2);
    }
    Tcl_AppendResult (ti, temp, NULL);

    if (db->game->GetEco() != 0) {
        ecoStringT fullEcoStr;
        eco_ToExtendedString (db->game->GetEco(), fullEcoStr);
        ecoStringT basicEcoStr;
        strCopy (basicEcoStr, fullEcoStr);
        if (strLength(basicEcoStr) >= 4) { basicEcoStr[3] = 0; }
        Tcl_AppendResult (ti, "   <blue><run ::windows::eco::Refresh ",
                          basicEcoStr, ">", fullEcoStr,
                          "</run></blue>", NULL);
    }
    char dateStr[20];
    date_DecodeToString (db->game->GetDate(), dateStr);
    strTrimDate (dateStr);
    Tcl_AppendResult (ti, "   <red>", dateStr, "</red>", NULL);

    if (db->gameNumber >= 0) {
        // Check if this game is deleted or has other user-settable flags:
        const IndexEntry* ie = db->getIndexEntry(db->gameNumber);
        if (ie->GetDeleteFlag()) {
            Tcl_AppendResult (ti, "   <gray>(",
                              translate (ti, "deleted"), ")</gray>", NULL);
        }
        char userFlags[16];
        if (ie->GetFlagStr (userFlags, NULL) != 0) {
            // Print other flags set for this game:
            const char * flagStr = userFlags;
            // Skip over "D" for Deleted, as it is indicated above:
            if (*flagStr == 'D') { flagStr++; }
            if (*flagStr != 0) {
                Tcl_AppendResult (ti, "   <gray>(",
                                  translate (ti, "flags", "flags"),
                                  ": ", flagStr, NULL);
                int flagCount = 0;
                while (*flagStr != 0) {
                    const char * flagName = NULL;
                    switch (*flagStr) {
                        case 'W': flagName = "WhiteOpFlag"; break;
                        case 'B': flagName = "BlackOpFlag"; break;
                        case 'M': flagName = "MiddlegameFlag"; break;
                        case 'E': flagName = "EndgameFlag"; break;
                        case 'N': flagName = "NoveltyFlag"; break;
                        case 'P': flagName = "PawnFlag"; break;
                        case 'T': flagName = "TacticsFlag"; break;
                        case 'Q': flagName = "QsideFlag"; break;
                        case 'K': flagName = "KsideFlag"; break;
                        case '!': flagName = "BrilliancyFlag"; break;
                        case '?': flagName = "BlunderFlag"; break;
                        case 'U': flagName = "UserFlag"; break;
                    }
                    if (flagName != NULL) {
                        Tcl_AppendResult (ti, (flagCount > 0 ? ", " : " - "),
                                          translate (ti, flagName), NULL);
                    }
                    flagCount++;
                    flagStr++;
                }
                Tcl_AppendResult (ti, ")</gray>", NULL);
            }
        }

        if (db->game->FindExtraTag("Bib") != NULL) {
           Tcl_AppendResult (ti, "  <red><run ::Bibliography::ShowRef>Bib</run></red>", NULL);
        }

        // Check if this game has a twin (duplicate):
        if (db->getDuplicates(db->gameNumber) != 0) {
            Tcl_AppendResult (ti, "   <blue><run updateTwinChecker>(",
                              translate (ti, "twin"), ")</run></blue>", NULL);
        }
    }
    sprintf (temp, "<br><gray><run ::crosstab::Open>%s:  %s</run> (%s)</gray><br>",
             db->game->GetSiteStr(),
             db->game->GetEventStr(),
             db->game->GetRoundStr());
    Tcl_AppendResult (ti, temp, NULL);

    char san [20];
    char tempTrans[20];
    byte * nags;
    colorT toMove = db->game->GetCurrentPos()->GetToMove();
    uint moveCount = db->game->GetCurrentPos()->GetFullMoveCount();
    uint prevMoveCount = moveCount;
    if (toMove == WHITE) { prevMoveCount--; }

    db->game->GetPrevSAN (san);
    strcpy(tempTrans, san);
    transPieces(tempTrans);
    bool printNags = true;
    if (san[0] == 0) {
        strCopy (temp, "(");
        strAppend (temp, db->game->GetVarLevel() == 0 ?
                   translate (ti, "GameStart", "Start of game") :
                   translate (ti, "LineStart", "Start of line"));
        strAppend (temp, ")");
        printNags = false;
    } else {
        sprintf (temp, "<run ::move::Back>%u.%s%s</run>",
                 prevMoveCount, toMove==WHITE ? ".." : "", tempTrans);//san);
        printNags = true;
    }
    Tcl_AppendResult (ti, translate (ti, "LastMove", "Last move"), NULL);
    Tcl_AppendResult (ti, ": <darkblue>", temp, "</darkblue>", NULL);
    nags = db->game->GetNags();
    if (printNags  &&  *nags != 0  &&  !hideNextMove) {
        Tcl_AppendResult (ti, "<red>", NULL);
        for (uint nagCount = 0 ; nags[nagCount] != 0; nagCount++) {
            char nagstr[20];
            game_printNag (nags[nagCount], nagstr, true, PGN_FORMAT_Plain);
            if (nagCount > 0  ||  (nagstr[0] != '!' && nagstr[0] != '?')) {
                Tcl_AppendResult (ti, " ", NULL);
            }
            Tcl_AppendResult (ti, nagstr, NULL);
        }
        Tcl_AppendResult (ti, "</red>", NULL);
    }

    // Now print next move:

    db->game->GetSAN (san);
    strcpy(tempTrans, san);
    transPieces(tempTrans);
    if (san[0] == 0) {
        strCopy (temp, "(");
        strAppend (temp, db->game->GetVarLevel() == 0 ?
                   translate (ti, "GameEnd", "End of game") :
                   translate (ti, "LineEnd", "End of line"));
        strAppend (temp, ")");
        printNags = false;
    } else if (hideNextMove) {
        sprintf (temp, "%u.%s(", moveCount, toMove==WHITE ? "" : "..");
        strAppend (temp, translate (ti, "hidden"));
        strAppend (temp, ")");
        printNags = false;
    } else {
        sprintf (temp, "<run ::move::Forward>%u.%s%s</run>",
                 moveCount, toMove==WHITE ? "" : "..", tempTrans);//san);
        printNags = true;
    }
    Tcl_AppendResult (ti, "   ", translate (ti, "NextMove", "Next"), NULL);
    Tcl_AppendResult (ti, ": <darkblue>", temp, "</darkblue>", NULL);
    nags = db->game->GetNextNags();
    if (printNags  &&  !hideNextMove  &&  *nags != 0) {
        Tcl_AppendResult (ti, "<red>", NULL);
        for (uint nagCount = 0 ; nags[nagCount] != 0; nagCount++) {
            char nagstr[20];
            game_printNag (nags[nagCount], nagstr, true, PGN_FORMAT_Plain);
            if (nagCount > 0  ||  (nagstr[0] != '!' && nagstr[0] != '?')) {
                Tcl_AppendResult (ti, " ", NULL);
            }
            Tcl_AppendResult (ti, nagstr, NULL);
        }
        Tcl_AppendResult (ti, "</red>", NULL);
    }

    if (db->game->GetVarLevel() > 0) {
        Tcl_AppendResult (ti, "   <green><run sc_var exit; updateBoard -animate>",
                          "(<lt>-Var)", "</run></green>", NULL);
    }

    if (showMaterialValue) {
        uint mWhite = db->game->GetCurrentPos()->MaterialValue (WHITE);
        uint mBlack = db->game->GetCurrentPos()->MaterialValue (BLACK);
        sprintf (temp, "    <gray>(%u-%u", mWhite, mBlack);
        Tcl_AppendResult (ti, temp, NULL);
        if (mWhite > mBlack) {
            sprintf (temp, ":+%u", mWhite - mBlack);
            Tcl_AppendResult (ti, temp, NULL);
        } else if (mBlack > mWhite) {
            sprintf (temp, ":-%u", mBlack - mWhite);
            Tcl_AppendResult (ti, temp, NULL);
        }
        Tcl_AppendResult (ti, ")</gray>", NULL);
    }

    // Print first few variations if there are any:

    uint varCount = db->game->GetNumVariations();
    if (!hideNextMove  &&  varCount > 0) {
        Tcl_AppendResult (ti, "<br>", translate (ti, "Variations"), ":", NULL);
        for (uint vnum = 0; vnum < varCount && vnum < 5; vnum++) {
            char s[20];
            db->game->MoveIntoVariation (vnum);
            db->game->GetSAN (s);
            strcpy(tempTrans, s);
            transPieces(tempTrans);
            sprintf (temp, "   <run sc_var enter %u; updateBoard -animate>v%u",
                     vnum, vnum+1);
            Tcl_AppendResult (ti, "<green>", temp, "</green>: ", NULL);
            if (s[0] == 0) {
                sprintf (temp, "<darkblue>(empty)</darkblue>");
            } else {
                sprintf (temp, "<darkblue>%u.%s%s</darkblue>",
                         moveCount, toMove == WHITE ? "" : "..", tempTrans);//s);
            }
            Tcl_AppendResult (ti, temp, NULL);
            byte * firstNag = db->game->GetNextNags();
            if (*firstNag >= NAG_GoodMove  &&  *firstNag <= NAG_DubiousMove) {
                game_printNag (*firstNag, s, true, PGN_FORMAT_Plain);
                Tcl_AppendResult (ti, "<red>", s, "</red>", NULL);
            }
            Tcl_AppendResult (ti, "</run>", NULL);
            db->game->MoveExitVariation ();
        }
    }

    // Check if this move has a comment:

    if (db->game->GetMoveComment() != NULL) {
        Tcl_AppendResult (ti, "<br>", translate(ti, "Comment"),
                          " <green><run makeCommentWin>", NULL);
        char * str = strDuplicate(db->game->GetMoveComment());
        strTrimMarkCodes (str);
        const char * s = str;
        uint len;
        uint lines = 0;
        // Add the first commentWidth characters of the comment, up to
        // the first commentHeight lines:
        for (len = 0; len < commentWidth; len++, s++) {
            char ch = *s;
            if (ch == 0) { break; }
            if (ch == '\n') {
                lines++;
                if (lines >= commentHeight) { break; }
                Tcl_AppendResult (ti, "<br>", NULL);
            } else if (ch == '<') {
                Tcl_AppendResult (ti, "<lt>", NULL);
            } else if (ch == '>') {
                Tcl_AppendResult (ti, "<gt>", NULL);
            } else {
                appendCharResult (ti, ch);
            }
        }
        // Complete the current comment word and add "..." if necessary:
        if (len == commentWidth) {
            char ch = *s;
            while (ch != ' '  &&  ch != '\n'  &&  ch != 0) {
                appendCharResult (ti, ch);
                s++;
                ch = *s;
            }
            if (ch != 0) {
                Tcl_AppendResult (ti, "...", NULL);
            }
        }
        Tcl_AppendResult (ti, "</run></green>", NULL);
#ifdef WINCE
        my_Tcl_Free((char*) str);
#else
        delete[] str;
#endif
    }

    // Probe tablebases:

    if (!hideNextMove) {
        DString * tbStr = new DString;
        if (probe_tablebase (ti, showTB, tbStr)) {
            Tcl_AppendResult (ti, "<br>TB: <blue><run ::tb::open>",
                              tbStr->Data(), "</run></blue>", NULL);
        }
        delete tbStr;
    }

    // Now check ECO book for the current position:
    if (ecoBook) {
        auto ecoStr = ecoBook->findECOstr(db->game->GetCurrentPos());
        if (!ecoStr.empty()) {
            std::string ecoComment(ecoStr);
            ecoT eco = eco_FromString(ecoComment.c_str());
            ecoStringT estr;
            eco_ToExtendedString (eco, estr);
            uint len = strLength (estr);
            if (len >= 4) { estr[3] = 0; }
            DString tempDStr;
            translateECO (ti, ecoComment.c_str(), &tempDStr);
            Tcl_AppendResult (ti, "<br>ECO:  <blue><run ::windows::eco::Refresh ",
                              estr, ">", tempDStr.Data(),
                              "</run></blue>", NULL);
        }
    }
    if (showFEN) {
        char boardStr [200];
        db->game->GetCurrentPos()->PrintFEN (boardStr, FEN_ALL_FIELDS);
        Tcl_AppendResult (ti, "<br><gray>", boardStr, "</gray>", NULL);
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_load:
//    Takes a game number and loads the game
int
sc_game_load (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (!db->inUse) {
        return errorResult (ti, errMsgNotOpen(ti));
    }
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_game load <gameNumber>");
    }

    db->gameAlterations.clear();
    
    uint gnum = strGetUnsigned (argv[2]);

    // Check the game number is valid::
    if (gnum < 1  ||  gnum > db->numGames()) {
        return errorResult (ti, "Invalid game number.");
    }

    // We number games from 0 internally, so subtract one:
    gnum--;
    const char * corruptMsg = "Sorry, this game appears to be corrupt.";

    const IndexEntry* ie = db->getIndexEntry(gnum);

    if (db->getGame(*ie, *db->game) != OK) {
        return errorResult (ti, corruptMsg);
    }

    if (db->dbFilter->Get(gnum) > 0) {
        db->game->MoveToPly(db->dbFilter->Get(gnum) - 1);
    } else {
        db->game->MoveToStart();
    }

    db->gameNumber = gnum;
    db->gameAltered = false;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_merge:
//    Merge the specified game into a variation from the current
//    game position.
int
sc_game_merge (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_game merge <baseNum> <gameNum> [<endPly>]";
    if (argc < 4  ||  argc > 5) { return errorResult (ti, usage); }

    const scidBaseT* base = DBasePool::getBase(strGetUnsigned(argv[2]));
    if (base == 0) return UI_Result(ti, ERROR_FileNotOpen);

    uint gnum = strGetUnsigned (argv[3]);
    uint endPly = 9999;     // Defaults to huge number for all moves.
    if (argc == 5) { endPly = strGetUnsigned (argv[4]); }

    if (gnum < 1  ||  gnum > base->numGames()) {
        return errorResult (ti, "Invalid game number.");
    }
    // Number games from 0 internally:
    gnum--;

    // Check that the specified game can be merged:
    if (base == db  &&  (int)gnum == db->gameNumber) {
        return errorResult (ti, "This game cannot be merged into itself.");
    }
    if (db->game->AtStart()  &&  db->game->AtEnd()) {
        return errorResult (ti, "The current game has no moves.");
    }
    if (db->game->HasNonStandardStart()) {
        return errorResult (ti, "The current game has a non-standard start position.");
    }

    // Load the merge game:

    const IndexEntry* ie = base->getIndexEntry(gnum);
    auto bbuf = base->getGame(*ie);
    Game * merge = scratchGame;
    if (merge->DecodeMovesOnly(bbuf) != OK) {
        return errorResult (ti, "Error decoding game.");
    }
    merge->LoadStandardTags (ie, base->getNameBase());
    if (merge->HasNonStandardStart()) {
        return errorResult (ti, "The merge game has a non-standard start position.");
    }

    // Set up an array of all the game positions in the merge game:
    uint nMergePos = merge->GetNumHalfMoves() + 1;
    typedef char compactBoardStr [36];
    compactBoardStr * mergeBoards = new compactBoardStr [nMergePos];
    merge->MoveToStart();
    for (uint i=0; i < nMergePos; i++) {
        merge->GetCurrentPos()->PrintCompactStr (mergeBoards[i]);
        merge->MoveForward();
    }

    // Now find the deepest position in the current game that occurs
    // in the merge game:
    db->game->MoveToStart();
    uint matchPly = 0;
    uint mergePly = 0;
    uint ply = 0;
    bool done = false;
    while (!done) {
        if (db->game->MoveForward() != OK) { done = true; }
        ply++;
        compactBoardStr currentBoard;
        db->game->GetCurrentPos()->PrintCompactStr (currentBoard);
        for (uint n=0; n < nMergePos; n++) {
            if (strEqual (currentBoard, mergeBoards[n])) {
                matchPly = ply;
                mergePly = n;
            }
        }
    }

    delete [] mergeBoards;

    // Now the games match at the locations matchPly in the current
    // game and mergePly in the merge game.
    // Create a new variation and add merge-game moves to it:
    db->game->MoveToPly (matchPly);
    bool atLastMove = db->game->AtEnd();
    simpleMoveT * sm = NULL;
    if (atLastMove) {
        // At end of game, so remember final game move for replicating
        // at the start of the variation:
        db->game->MoveBackup();
        sm = db->game->GetCurrentMove();
        ASSERT(sm);
        db->game->MoveForward();
    }
    db->game->MoveForward();
    db->game->AddVariation();
    db->gameAltered = true;
    if (sm) {
        // We need to replicate the last move of the current game.
        db->game->AddMove(sm);
    }
    merge->MoveToPly (mergePly);
    ply = mergePly;
    while (ply < endPly) {
        simpleMoveT * mergeMove = merge->GetCurrentMove();
        if (merge->MoveForward() != OK) { break; }
        if (mergeMove == NULL) { break; }
        if (db->game->AddMove(mergeMove) != OK) { break; }
        ply++;
    }

    // Finally, add a comment describing the merge-game details:
    DString * dstr = new DString;
    dstr->Append (RESULT_LONGSTR[ie->GetResult()]);
    if (ply < merge->GetNumHalfMoves()) {
        dstr->Append ("(", (merge->GetNumHalfMoves()+1) / 2, ")");
    }
    dstr->Append (" ", ie->GetWhiteName (base->getNameBase()));
    eloT elo = ie->GetWhiteElo();
    if (elo > 0) { dstr->Append (" (", elo, ")"); }
    dstr->Append (" - ");
    dstr->Append (ie->GetBlackName (base->getNameBase()));
    elo = ie->GetBlackElo();
    if (elo > 0) { dstr->Append (" (", elo, ")"); }
    dstr->Append (" / ", ie->GetEventName (base->getNameBase()));
    dstr->Append (" (", ie->GetRoundName (base->getNameBase()), ")");
    dstr->Append (", ", ie->GetSiteName (base->getNameBase()));
    dstr->Append (" ", ie->GetYear());
    db->game->SetMoveComment ((char *) dstr->Data());
    delete dstr;

    // And exit the new variation:
    db->game->MoveExitVariation();
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_moves:
//    Return a string of the moves reaching the current game position.
//    Optional arguments: "coord" for coordinate notation (1 move per line);
//    "nomoves" for standard algebraic without move numbers.
//    Default output is standard algebraic with move numbers.
int
sc_game_moves (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    bool sanFormat = true;
    bool printMoves = true;
    bool listFormat = false;
    const uint MAXMOVES = 500;
    sanStringT * moveStrings = new sanStringT [MAXMOVES];
    uint plyCount = 0;
    Game * g = db->game;
    for (int arg = 2; arg < argc; arg++) {
        if (argv[arg][0] == 'c') { sanFormat = false; }
        if (argv[arg][0] == 'n') { printMoves = false; }
        if (argv[arg][0] == 'l') { printMoves = false; }
    }

    auto location = g->currentLocation();
    while (! g->AtStart()) {
        if (g->AtVarStart()) {
            g->MoveExitVariation();
            continue;
        }
        g->MoveBackup();
        simpleMoveT * sm = g->GetCurrentMove();
        if (sm == NULL) { break; }
        char * s = moveStrings[plyCount];
        if (sanFormat) {
            g->GetSAN (s);
        } else {
            *s++ = square_FyleChar(sm->from);
            *s++ = square_RankChar(sm->from);
            *s++ = square_FyleChar(sm->to);
            *s++ = square_RankChar(sm->to);
            if (sm->promote != EMPTY) {
                *s++ = piece_Char (piece_Type (sm->promote));
            }
            *s = 0;
        }
        plyCount++;
        if (plyCount == MAXMOVES) {
            // Too many moves, just give up:
            g->restoreLocation(location);
            delete[] moveStrings;
            return TCL_OK;
        }
    }
    g->restoreLocation(location);
    uint count = 0;
    for (uint i = plyCount; i > 0; i--, count++) {
        char move [20];
        if (sanFormat) {
            move[0] = 0;
            if (printMoves  &&  (count % 2 == 0)) {
                sprintf (move, "%u.", (count / 2) + 1);
            }
            strAppend (move, moveStrings[i - 1]);
        } else {
            strCopy (move, moveStrings [i - 1]);
        }
        if (listFormat) {
            Tcl_AppendElement (ti, move);
        } else {
            Tcl_AppendResult (ti, (count == 0 ? "" : " "), move, NULL);
        }
    }
    delete[] moveStrings;
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_new:
//    Clears the current game.
int
sc_game_new(ClientData, Tcl_Interp*, int, const char**)
{
    db->game->Clear();
    db->gameNumber = -1;
    db->gameAltered = false;
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_novelty:
//    Finds the first move in the current game (after the deepest
//    position found in the ECO book) that reaches a position not
//    found in the selected database. It then moves to that point
//    in the game and returns a text string of the move.
int
sc_game_novelty (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage =
        "Usage: sc_game novelty [-older] base";

    bool olderGamesOnly = false;

    int baseArg = 2;
    if (argc >= baseArg
        &&  argv[baseArg][0] == '-'  &&  argv[baseArg][1] == 'o'
        &&  strIsPrefix (argv[baseArg], "-older")) {
        olderGamesOnly = true;
        baseArg++;
    }
    if (argc < baseArg  ||  argc > baseArg+1) return errorResult(ti, usage);
    scidBaseT* base = DBasePool::getBase(strGetInteger (argv[baseArg]));
    if (base == 0) return UI_Result(ti, ERROR_BadArg);

    // First, move to the deepest ECO position in the game.
    // This code is adapted from sc_eco_game().
    Game* g = base->game;
    if (ecoBook) {
        while (g->MoveForward() == OK) {}
        while (ecoBook->findECOstr(g->GetCurrentPos()).empty()) {
            if (g->MoveBackup() != OK) break;
        }
    }

    // Now keep doing an exact position search (ignoring the current
    // game) and skipping to the next game position whenever a match
    // is found, until a position not in any database game is reached:
    Progress progress = UI_CreateProgress(ti);
    std::string filtername = base->newFilter();
    HFilter filter = base->getFilter(filtername);
    dateT currentDate = g->GetDate();
    while (g->MoveForward() == OK) {
        SearchPos(g->GetCurrentPos()).setFilter(*base, filter, Progress());
        int count = 0;
        for (uint i=0, n = base->numGames(); i < n; i++) {
            if (filter.get(i) == 0) continue;

            // Ignore newer games if requested:
            if (olderGamesOnly) {
                if (base->getIndexEntry(i)->GetDate() >= currentDate) continue;
            }
            if (count++ != 0) break;
        }

        if (count <= 1) { // Novelty found
            base->deleteFilter(filtername.c_str());
            return UI_Result(ti, OK, g->GetCurrentPly());
        }

        auto work_done = g->GetCurrentPly() + 1;
        if (!progress.report(work_done, g->GetNumHalfMoves())) {
            base->deleteFilter(filtername.c_str());
            return UI_Result(ti, ERROR_UserCancel);
        }
    }

    base->deleteFilter(filtername.c_str());
    return UI_Result(ti, OK, -1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_pgn:
//    Returns the PGN representation of the game.
//    Optional args:
//      -format (plain|html|latex): output format. Default=plain.
//      -shortHeader (0|1): short, 3-line (non-PGN) header. Default=0.
//      -space (0|1): printing a space after move numbers. Default=0.
//      -tags (0|1): printing (nonstandard) tags. Default=1.
//      -comments (0|1): printing nags/comments. Default=1.
//      -variations (0|1): printing variations. Default=1.
//      -indentVars (0|1): indenting variations. Default=0.
//      -indentComments (0|1): indenting comments. Default=0.
//      -width (number): line length for wordwrap. Default=huge (99999),
//        to let a Tk text widget do its own line-breaking.
//      -base (number): Print the game from the numbered base.
//      -gameNumber (number): Print the numbered game instead of the
//        active game.
//      -unicode (0|1): use unicocde characters (e.g. U+2654 for king). Default=0.
int
sc_game_pgn (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "-column", "-comments", "-base", "-gameNumber", "-format",
        "-shortHeader", "-indentComments", "-indentVariations",
        "-symbols", "-tags", "-variations", "-width", "-space",
        "-markCodes", "-unicode",
        NULL
    };
    enum {
        OPT_COLUMN, OPT_COMMENTS, OPT_BASE, OPT_GAME_NUMBER, OPT_FORMAT,
        OPT_SHORT_HDR, OPT_INDENT_COMMENTS, OPT_INDENT_VARS,
        OPT_SYMBOLS, OPT_TAGS, OPT_VARS, OPT_WIDTH, OPT_SPACE,
        OPT_NOMARKS, OPT_UNICODE,
    };

    const scidBaseT* base = db;
    Game * g = db->game;
    uint lineWidth = 99999;
    g->ResetPgnStyle();
    g->SetPgnFormat (PGN_FORMAT_Plain);
    g->AddPgnStyle (PGN_STYLE_TAGS | PGN_STYLE_COMMENTS | PGN_STYLE_VARS);

    // Parse all the command options:
    // Note that every option takes a value so options/values always occur
    // in pairs, which simplifies the code.

    int thisArg = 2;
    while (thisArg < argc) {
        int index = strUniqueMatch (argv[thisArg], options);
        if (index == -1) {
            Tcl_AppendResult (ti, "Invalid option to sc_game pgn: ",
                              argv[thisArg], "; valid options are: ", NULL);
            for (const char ** s = options; *s != NULL; s++) {
                Tcl_AppendResult (ti, *s, " ", NULL);
            }
            return TCL_ERROR;
        }

        // Check that our option has a value:
        if (thisArg+1 == argc) {
            Tcl_AppendResult (ti, "Invalid option value: sc_game pgn ",
                              options[index], " requires a value.", NULL);
            return TCL_ERROR;
        }

        uint value = strGetUnsigned (argv[thisArg+1]);

        if (index == OPT_WIDTH) {
            lineWidth = value;

        } else if (index == OPT_BASE) {
            base = DBasePool::getBase(value);
            if (base == 0) return UI_Result(ti, ERROR_FileNotOpen);
            g = base->game;

        } else if (index == OPT_GAME_NUMBER) {
            // Print the numbered game instead of the active game:

            g = scratchGame;
            g->Clear();
            if (value < 1  ||  value > base->numGames()) {
                return setResult (ti, "Invalid game number");
            }
            const IndexEntry* ie = base->getIndexEntry(value - 1);
            if (ie->GetLength() == 0) {
                return errorResult (ti, "Error: empty game file record.");
            }
            if (base->getGame(*ie, *g) != OK) {
                return errorResult (ti, "Error reading game file.");
            }

        } else if (index == OPT_FORMAT) {
            // The option value should be "plain", "html" or "latex".
            if (! g->SetPgnFormatFromString (argv[thisArg+1])) {
                return errorResult (ti, "Invalid -format option.");
            }

        } else {
            // The option is a boolean affecting pgn style:
            uint bitmask = 0;
            switch (index) {
                case OPT_COLUMN:
                    bitmask = PGN_STYLE_COLUMN;        break;
                case OPT_COMMENTS:
                    bitmask = PGN_STYLE_COMMENTS;        break;
                case OPT_SYMBOLS:
                    bitmask = PGN_STYLE_SYMBOLS;         break;
                case OPT_TAGS:
                    bitmask = PGN_STYLE_TAGS;            break;
                case OPT_VARS:
                    bitmask = PGN_STYLE_VARS;            break;
                case OPT_SHORT_HDR:
                    bitmask = PGN_STYLE_SHORT_HEADER;    break;
                case OPT_SPACE:
                    bitmask = PGN_STYLE_MOVENUM_SPACE;   break;
                case OPT_INDENT_VARS:
                    bitmask = PGN_STYLE_INDENT_VARS;     break;
                case OPT_INDENT_COMMENTS:
                    bitmask = PGN_STYLE_INDENT_COMMENTS; break;
                case OPT_NOMARKS:
                    bitmask = PGN_STYLE_STRIP_MARKS;     break;
                case OPT_UNICODE:
                    bitmask = PGN_STYLE_UNICODE;         break;
                default: // unreachable!
                    return errorResult (ti, "Invalid option.");
            };
            if (bitmask > 0) {
                if (value) {
                    g->AddPgnStyle (bitmask);
                } else {
                    g->RemovePgnStyle (bitmask);
                }
            }
        }
        thisArg += 2;
    }

    std::pair<const char*, unsigned> pgnBuf = g->WriteToPGN(lineWidth);
    Tcl_AppendResult (ti, pgnBuf.first, NULL);
    return TCL_OK;
}

//~~~~~~~ DEPRECATED ~~~~~~
// sc_game_pop:
//    Restores the last game saved with sc_game_push.
int
sc_game_pop(ClientData, Tcl_Interp*, int, const char**)
{
    if (db->deprecated_push_pop.first) {
        delete db->game;
        db->game = db->deprecated_push_pop.first;
        db->gameAltered = db->deprecated_push_pop.second;
        db->deprecated_push_pop.first = nullptr;
    } else {
        ASSERT(0);
    }
    return TCL_OK;
}

//~~~~~~~ DEPRECATED ~~~~~~
// sc_game_push:
//    Saves the current game and pushes a new empty game onto
//    the game state stack.
//    If the optional argument "copy" is present, the new game will be
//    a copy of the current game.
int
sc_game_push (ClientData, Tcl_Interp*, int argc, const char ** argv)
{
    bool copy = false;
    
    if ( argc > 2 && !strcmp( argv[2], "copy" ) ) {
        copy = true;
    }
    else if ( argc > 2 && !strcmp( argv[2], "copyfast" ) ) {
        copy = true;
    }

    Game* g = (copy) ? db->game->clone() : new Game;
    if (db->deprecated_push_pop.first) {
        ASSERT(0);
        delete db->deprecated_push_pop.first;
    }
    db->deprecated_push_pop = {db->game, db->gameAltered};
    db->game = g;
    db->gameAltered = false;

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_save:
//    Saves the current game. If the parameter is 0, a NEW
//    game is added; otherwise, that game number is REPLACED.
int
sc_game_save (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    scidBaseT * dbase = db;
    Game* currGame = db->game;
    if (argc == 4) {
        dbase = DBasePool::getBase(strGetUnsigned(argv[3]));
        if (dbase == 0) return errorResult (ti, "Invalid database number.");
    } else if (argc != 3) {
        return errorResult (ti, "Usage: sc_game save <gameNumber> [targetbaseId]");
    }

    gamenumT gnum = strGetUnsigned(argv[2]);
    if (gnum == 0) {
        gnum = INVALID_GAMEID;
    } else {
        gnum -= 1;
        const IndexEntry* ieOld = dbase->getIndexEntry_bounds(gnum);
        if (ieOld == 0) return ERROR_BadArg;
        // User-settable flags were stored in currGame when the game
        // was loaded, but the user may have changed them.
        char buf[IndexEntry::IDX_NUM_FLAGS + 1];
        ieOld->GetFlagStr(buf, "WBMENPTKQ!?U123456");
        currGame->SetScidFlags(buf);
    }
    auto location = currGame->currentLocation();
    errorT res = dbase->saveGame(currGame, gnum);
    currGame->restoreLocation(location);
    if (res == OK) {
        if (gnum == INVALID_GAMEID && db == dbase) {
            // Saved new game, so set gameNumber to the saved game number:
            db->gameNumber = db->numGames() - 1;
        }
        db->gameAltered = false;
    }

    return UI_Result(ti, res);;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_startBoard:
//    Sets the starting position from a FEN string.
//    If there is no FEN string argument, a boolean value is
//    returned indicating whether the current game starts with
//    a setup position.
int
sc_game_startBoard (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc == 2) {
        return UI_Result(ti, OK, db->game->HasNonStandardStart());
    } else if (argc != 3) {
        return errorResult (ti, "Usage: sc_game startBoard <fenString>");
    }
    const char * str = argv[2];
    char buf[256];
    if (strIsPrefix ("random:", str)) {
        // A "FEN" string that starts with "random:" is interpreted as a
        // material configuration, and a random position with this
        // set of material is generated. For example, "random:krpkr"
        // generates a random legal Rook+Pawn-vs-Rook position.
        Position scratchPos;
        if (scratchPos.Random (str+7) != OK) {
            return errorResult (ti, "Invalid material string.");
        }
        scratchPos.PrintFEN(buf, FEN_ALL_FIELDS);
        str = buf;
    }
    auto err = db->game->SetStartFen(str);
    if (err != OK)
        return errorResult(ti, "Invalid FEN string.");

    db->gameAltered = true;
    return UI_Result(ti, OK);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_strip:
//    Strips all comments, variations or annotations from a game.
int sc_game_strip(ClientData, Tcl_Interp* ti, int argc, const char** argv) {
	if (argc == 3 && !strcmp("variations", argv[2])) {
		db->game->strip(true, false, false);
	} else if (argc == 3 && !strcmp("comments", argv[2])) {
		db->game->strip(false, true, true);
	} else {
		return errorResult(ti, "Usage: sc_game strip [comments|variations]");
	}
	db->gameAltered = true;
	return UI_Result(ti, OK);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    Returns summary information of the specified game:
//    its players, site, etc; or its moves; or all its boards
//    positions.
UI_res_t sc_base_gamesummary(const scidBaseT& base, UI_handle_t ti, int argc,
                             const char** argv) {
	const char* usage = "Usage: sc_base gamesummary baseId gameNum";
	if (argc != 4)
		return UI_Result(ti, ERROR_BadArg, usage);

	Game* g = scratchGame;
	gamenumT gnum = strGetUnsigned(argv[3]);
	if (gnum > 0) {
		auto ie = base.getIndexEntry_bounds(gnum - 1);
		if (!ie || base.getGame(*ie, *scratchGame) != OK) {
			return UI_Result(ti, ERROR_BadArg, usage);
		}
	} else {
		g = base.game;
	}

    UI_List res(3);

    // Return header summary if requested:
        DString dstr;
        dstr.Append (g->GetWhiteStr());
        eloT elo = g->GetWhiteElo();
        if (elo > 0) { dstr.Append (" (", elo, ")"); }
        dstr.Append ("  --  ", g->GetBlackStr());
        elo = g->GetBlackElo();
        if (elo > 0) { dstr.Append (" (", elo, ")"); }
        dstr.Append ("\n", g->GetEventStr());
        const char * round = g->GetRoundStr();
        if (! strIsUnknownName(round)) {
            dstr.Append (" (", round, ")");
        }
        dstr.Append ("  ", g->GetSiteStr(), "\n");
        char dateStr [20];
        date_DecodeToString (g->GetDate(), dateStr);
        // Remove ".??" or ".??.??" from end of date:
        if (dateStr[4] == '.'  &&  dateStr[5] == '?') { dateStr[4] = 0; }
        if (dateStr[7] == '.'  &&  dateStr[8] == '?') { dateStr[7] = 0; }
        dstr.Append (dateStr, "  ");
        dstr.Append (RESULT_LONGSTR[g->GetResult()]);
        ecoT eco = g->GetEco();
        if (eco != 0) {
            ecoStringT ecoStr;
            eco_ToExtendedString (eco, ecoStr);
            dstr.Append ("  ", ecoStr);
        }
        res.push_back(dstr.Data());

    // Here, a list of the boards or moves is requested:
    const auto n_moves = g->GetNumHalfMoves() + 1;
    UI_List boards(n_moves);
    UI_List moves(n_moves);
    auto location = g->currentLocation();
    g->MoveToStart();
    do {
            char boardStr[100];
            g->GetCurrentPos()->MakeLongStr (boardStr);
            boards.push_back(boardStr);

            colorT toMove = g->GetCurrentPos()->GetToMove();
            uint moveCount = g->GetCurrentPos()->GetFullMoveCount();
            char san [20];
            g->GetSAN (san);
            if (san[0] != 0) {
                char temp[40];
                if (toMove == WHITE) {
                    sprintf (temp, "%u.%s", moveCount, san);
                } else {
                    strCopy (temp, san);
                }
                byte * nags = g->GetNextNags();
                if (*nags != 0) {
                    for (uint nagCount = 0 ; nags[nagCount] != 0; nagCount++) {
                        char nagstr[20];
                        game_printNag (nags[nagCount], nagstr, true,
                                       PGN_FORMAT_Plain);
                        if (nagCount > 0  ||
                              (nagstr[0] != '!' && nagstr[0] != '?')) {
                            strAppend (temp, " ");
                        }
                        strAppend (temp, nagstr);
                    }
                }
                moves.push_back(temp);
            } else {
                moves.push_back(RESULT_LONGSTR[g->GetResult()]);
            }

    } while (g->MoveForward() == OK);
    g->restoreLocation(location);

    res.push_back(boards);
    res.push_back(moves);
    return UI_Result(ti, OK, res);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_tags:
//   Get, set or reload the current game tags, or share them
//   with another game.
int
sc_game_tags (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * options[] = {
        "get", "set", "reload", "share", NULL
    };
    enum { OPT_GET, OPT_SET, OPT_RELOAD, OPT_SHARE };

    int index = -1;
    if (argc >= 3) { index = strUniqueMatch (argv[2], options); }

    switch (index) {
        case OPT_GET:    return sc_game_tags_get (cd, ti, argc, argv);
        case OPT_SET:
          return sc_game_tags_set (cd, ti, argc, argv);
        case OPT_RELOAD: return sc_game_tags_reload (cd, ti, argc, argv);
        case OPT_SHARE:  return sc_game_tags_share (cd, ti, argc, argv);
        default:         return InvalidCommand (ti, "sc_game tags", options);
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_tags_get:
//    Gets a tag for the active game given its name.
//    Valid names are:  Event, Site, Date, Round, White, Black,
//       WhiteElo, BlackElo, ECO, Extra.
//    All except the last (Extra) return the tag value as a string.
//    For "Extra", the function returns all the extra tags as one long
//    string, in PGN format, one tag per line.
int
sc_game_tags_get (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{

    static const char * options [] = {
        "Event", "Site", "Date", "Year", "Month", "Day",
        "Round", "White", "Black", "Result", "WhiteElo",
        "BlackElo", "WhiteRType", "BlackRType", "ECO",
        "EDate", "EYear", "EMonth", "EDay", "Extra",
        NULL
    };
    enum {
        T_Event, T_Site, T_Date, T_Year, T_Month, T_Day,
        T_Round, T_White, T_Black, T_Result, T_WhiteElo,
        T_BlackElo, T_WhiteRType, T_BlackRType, T_ECO,
        T_EDate, T_EYear, T_EMonth, T_EDay, T_Extra
    };

    const char * usage = "Usage: sc_game tags get [-last] <tagName>";
    const char * tagName;
    Game * g = db->game;

    if (argc < 4  ||  argc > 5) {
        return errorResult (ti, usage);
    }
    tagName = argv[3];
    if (argc == 5) {
        if (!strEqual (argv[3], "-last")) { return errorResult (ti, usage); }
        tagName = argv[4];
        if (db->numGames() > 0) {
            g = scratchGame;
            const IndexEntry* ie = db->getIndexEntry(db->numGames() - 1);
            if (db->getGame(*ie, *g) != OK) {
                return errorResult (ti, "Error reading game file.");
            }
        }
    }
    const char * s;
    int index = strExactMatch (tagName, options);

    switch (index) {
    case T_Event:
        s = g->GetEventStr();  if (!s) { s = "?"; }
        Tcl_AppendResult (ti, s, NULL);
        break;

    case T_Site:
        s = g->GetSiteStr();  if (!s) { s = "?"; }
        Tcl_AppendResult (ti, s, NULL);
        break;

    case T_Date:
        {
            char dateStr[20];
            date_DecodeToString (g->GetDate(), dateStr);
            Tcl_AppendResult (ti, dateStr, NULL);
        }
        break;

    case T_Year:
        return setUintResult (ti, date_GetYear (g->GetDate()));

    case T_Month:
        return setUintWidthResult (ti, date_GetMonth (g->GetDate()), 2);

    case T_Day:
        return setUintWidthResult (ti, date_GetDay (g->GetDate()), 2);

    case T_Round:
        s = g->GetRoundStr();  if (!s) { s = "?"; }
        Tcl_AppendResult (ti, s, NULL);
        break;

    case T_White:
        s = g->GetWhiteStr();  if (!s) { s = "?"; }
        Tcl_AppendResult (ti, s, NULL);
        break;

    case T_Black:
        s = g->GetBlackStr();  if (!s) { s = "?"; }
        Tcl_AppendResult (ti, s, NULL);
        break;

    case T_Result:
        return UI_Result(ti, OK, std::string(1, RESULT_CHAR[g->GetResult()]));

    case T_WhiteElo:
        return setUintResult (ti, g->GetWhiteElo());

    case T_BlackElo:
        return setUintResult (ti, g->GetBlackElo());

    case T_WhiteRType:
        return setResult (ti, ratingTypeNames[g->GetWhiteRatingType()]);

    case T_BlackRType:
        return setResult (ti, ratingTypeNames[g->GetBlackRatingType()]);

    case T_ECO:
        {
            ecoStringT ecoStr;
            eco_ToExtendedString (g->GetEco(), ecoStr);
            Tcl_AppendResult (ti, ecoStr, NULL);
            break;
        }

    case T_EDate:
        {
            char dateStr[20];
            date_DecodeToString (g->GetEventDate(), dateStr);
            Tcl_AppendResult (ti, dateStr, NULL);
        }
        break;

    case T_EYear:
        return setUintResult (ti, date_GetYear (g->GetEventDate()));

    case T_EMonth:
        return setUintWidthResult (ti, date_GetMonth (g->GetEventDate()), 2);

    case T_EDay:
        return setUintWidthResult (ti, date_GetDay (g->GetEventDate()), 2);

    case T_Extra:
        for (auto& tag : g->GetExtraTags()) {
            Tcl_AppendResult(ti, tag.first.c_str(), " \"", tag.second.c_str(), "\"\n", NULL);
        }
        break;

    default:  // Not a valid tag name.
        return InvalidCommand (ti, "sc_game tags get", options);
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_tags_set:
//    Set the standard tags for this game.
//    Args are: event, site, date, round, white, black, result,
//              whiteElo, whiteRatingType, blackElo, blackRatingType, Eco,
//              eventdate.
//    Last arg is the non-standard tags, a string of lines in the format:
//        [TagName "TagValue"]
int
sc_game_tags_set (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * options[] = {
        "-event", "-site", "-date", "-round", "-white", "-black", "-result",
        "-whiteElo", "-whiteRatingType", "-blackElo", "-blackRatingType",
        "-eco", "-eventdate", "-extra",
        NULL
    };
    enum {
        T_EVENT, T_SITE, T_DATE, T_ROUND, T_WHITE, T_BLACK, T_RESULT,
        T_WHITE_ELO, T_WHITE_RTYPE, T_BLACK_ELO, T_BLACK_RTYPE,
        T_ECO, T_EVENTDATE, T_EXTRA
    };

    int arg = 3;
    if (((argc-arg) % 2) != 0) {
        return errorResult (ti, "Odd number of parameters.");
    }

    // Process each pair of parameters:
    while (arg+1 < argc) {
        int index = strUniqueMatch (argv[arg], options);
        const char * value = argv[arg+1];
        arg += 2;

        switch (index) {
            case T_EVENT: db->game->SetEventStr (value); break;
            case T_SITE: db->game->SetSiteStr (value); break;
            case T_DATE:
                db->game->SetDate (date_EncodeFromString(value));
                break;
            case T_ROUND: db->game->SetRoundStr (value); break;
            case T_WHITE: db->game->SetWhiteStr (value); break;
            case T_BLACK: db->game->SetBlackStr (value); break;
            case T_RESULT: db->game->SetResult (strGetResult(value)); break;
            case T_WHITE_ELO:
                db->game->SetWhiteElo (strGetUnsigned(value)); break;
            case T_WHITE_RTYPE:
                db->game->SetWhiteRatingType (strGetRatingType (value)); break;
            case T_BLACK_ELO:
                db->game->SetBlackElo (strGetUnsigned(value)); break;
            case T_BLACK_RTYPE:
                db->game->SetBlackRatingType (strGetRatingType (value)); break;
            case T_ECO:
                db->game->SetEco (eco_FromString (value)); break;
            case T_EVENTDATE:
                db->game->SetEventDate (date_EncodeFromString(value));
                break;
            case T_EXTRA:
                {
                    // Add all the nonstandard tags:
                    db->game->ClearExtraTags ();
                    int largc;
                    const char ** largv;
                    if (Tcl_SplitList (ti, value, &largc,
                                       (CONST84 char ***) &largv) != TCL_OK) {
                        // Error from Tcl_SplitList!
                        return errorResult (ti, "Error parsing extra tags.");
                    }

                    // Extract each tag-value pair and add it to the game:
                    for (int i=0; i < largc; i++) {
                        char tagStr [1024];
                        char valueStr [1024];
                        //if ( 	sscanf (largv[i], "%s", tagStr ) == 1 &&  
                        //			sscanf (largv[i+1], "%s", valueStr) == 1) {
                        // Usage :: sc_game tags set -extra [ list "Annotator \"boob [sc_pos moveNumber]\"\n" ]
                        if (sscanf (largv[i], "%s \"%[^\"]\"\n", tagStr, valueStr) == 2) {
                            db->game->AddPgnTag (tagStr, valueStr);
                        } else {
                            // Invalid line in the list; just ignore it.
                        }
                    }
                    Tcl_Free ((char *) largv);
                }
                break;
            default:
                return InvalidCommand (ti, "sc_game tags set", options);
        }
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_tags_reload:
//    Reloads the tags (White, Black, Event,Site, etc) for a game.
//    Useful when a name that may occur in the current game has been
//    edited.
int
sc_game_tags_reload(ClientData, Tcl_Interp*, int, const char**)
{
    if (!db->inUse  ||   db->gameNumber < 0) { return TCL_OK; }
    const IndexEntry* ie = db->getIndexEntry(db->gameNumber);
    db->game->LoadStandardTags (ie, db->getNameBase());
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_game_tags_share:
//    Shares tags between two games, updating one where the other
//    has more complete or better information.
//
//    This is mainly useful for combining the header information
//    of a pair of twins before deleting one of them. For example,
//    one may have a less complete date while the other may have
//    no ratings or an unknown ("?") round value.
//
//    If the subcommand parameter is "check", a list is returned
//    with a multiple of four elements, each set of four indicating
//    a game number, the tag that will be changed, the old value,
//    and the new value. If the parameter is "update", the changes
//    will be made and the empty string is returned.
int
sc_game_tags_share (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage =
        "Usage: sc_game tags share [check|update] <gameNumber1> <gameNumber2>";
    if (argc != 6) { return errorResult (ti, usage); }
    bool updateMode = false;
    if (strIsPrefix (argv[3], "check")) {
        updateMode = false;
    } else if (strIsPrefix (argv[3], "update")) {
        updateMode = true;
    } else {
        return errorResult (ti, usage);
    }
    // Get the two game numbers, which should be different and non-zero.
    uint gn1 = strGetUnsigned (argv[4]);
    uint gn2 = strGetUnsigned (argv[5]);
    if (gn1 == 0) { return TCL_OK; }
    if (gn2 == 0) { return TCL_OK; }
    if (gn1 == gn2) { return TCL_OK; }
    if (gn1 > db->numGames()) { return TCL_OK; }
    if (gn2 > db->numGames()) { return TCL_OK; }

    // Do nothing if the base is not writable:
    if (!db->inUse  ||  db->isReadOnly()) { return TCL_OK; }

    // Make a local copy of each index entry:
    IndexEntry ie1 = *(db->getIndexEntry(gn1 - 1));
    IndexEntry ie2 = *(db->getIndexEntry(gn2 - 1));
    bool updated1 = false;
    bool updated2 = false;

    // Share dates if appropriate:
    char dateStr1 [16];
    char dateStr2 [16];
    dateT date1 = ie1.GetDate();
    dateT date2 = ie2.GetDate();
    date_DecodeToString (date1, dateStr1);
    date_DecodeToString (date2, dateStr2);
    strTrimDate (dateStr1);
    strTrimDate (dateStr2);
    if (date1 == 0) { *dateStr1 = 0; }
    if (date2 == 0) { *dateStr2 = 0; }
    // Check if one date is a prefix of the other:
    if (!strEqual (dateStr1, dateStr2)  &&  strIsPrefix (dateStr1, dateStr2)) {
        // Copy date grom game 2 to game 1:
        if (updateMode) {
            ie1.SetDate (date2);
            updated1 = true;
        } else {
            appendUintElement (ti, gn1);
            Tcl_AppendElement (ti, "Date");
            Tcl_AppendElement (ti, dateStr1);
            Tcl_AppendElement (ti, dateStr2);
        }
    }
    if (!strEqual (dateStr1, dateStr2)  &&  strIsPrefix (dateStr2, dateStr1)) {
        // Copy date grom game 1 to game 2:
        if (updateMode) {
            ie2.SetDate (date1);
            updated2 = true;
        } else {
            appendUintElement (ti, gn2);
            Tcl_AppendElement (ti, "Date");
            Tcl_AppendElement (ti, dateStr2);
            Tcl_AppendElement (ti, dateStr1);
        }
    }

    // Check if an event name can be updated:
    idNumberT event1 = ie1.GetEvent();
    idNumberT event2 = ie2.GetEvent();
    const char * eventStr1 = ie1.GetEventName (db->getNameBase());
    const char * eventStr2 = ie2.GetEventName (db->getNameBase());
    bool event1empty = strEqual (eventStr1, "")  ||  strEqual (eventStr1, "?");
    bool event2empty = strEqual (eventStr2, "")  ||  strEqual (eventStr2, "?");
    if (event1empty  && !event2empty) {
        // Copy event from event 2 to game 1:
        if (updateMode) {
            ie1.SetEvent (event2);
            updated1 = true;
        } else {
            appendUintElement (ti, gn1);
            Tcl_AppendElement (ti, "Event");
            Tcl_AppendElement (ti, eventStr1);
            Tcl_AppendElement (ti, eventStr2);
        }
    }
    if (event2empty  && !event1empty) {
        // Copy event from game 1 to game 2:
        if (updateMode) {
            ie2.SetEvent (event1);
            updated2 = true;
        } else {
            appendUintElement (ti, gn2);
            Tcl_AppendElement (ti, "Event");
            Tcl_AppendElement (ti, eventStr2);
            Tcl_AppendElement (ti, eventStr1);
        }
    }

    // Check if a round name can be updated:
    idNumberT round1 = ie1.GetRound();
    idNumberT round2 = ie2.GetRound();
    const char * roundStr1 = ie1.GetRoundName (db->getNameBase());
    const char * roundStr2 = ie2.GetRoundName (db->getNameBase());
    bool round1empty = strEqual (roundStr1, "")  ||  strEqual (roundStr1, "?");
    bool round2empty = strEqual (roundStr2, "")  ||  strEqual (roundStr2, "?");
    if (round1empty  && !round2empty) {
        // Copy round from game 2 to game 1:
        if (updateMode) {
            ie1.SetRound (round2);
            updated1 = true;
        } else {
            appendUintElement (ti, gn1);
            Tcl_AppendElement (ti, "Round");
            Tcl_AppendElement (ti, roundStr1);
            Tcl_AppendElement (ti, roundStr2);
        }
    }
    if (round2empty  && !round1empty) {
        // Copy round from game 1 to game 2:
        if (updateMode) {
            ie2.SetRound (round1);
            updated2 = true;
        } else {
            appendUintElement (ti, gn2);
            Tcl_AppendElement (ti, "Round");
            Tcl_AppendElement (ti, roundStr2);
            Tcl_AppendElement (ti, roundStr1);
        }
    }

    // Check if Elo ratings can be shared:
    eloT welo1 = ie1.GetWhiteElo();
    eloT belo1 = ie1.GetBlackElo();
    eloT welo2 = ie2.GetWhiteElo();
    eloT belo2 = ie2.GetBlackElo();
    if (welo1 == 0  &&  welo2 != 0) {
        // Copy White rating from game 2 to game 1:
        if (updateMode) {
            ie1.SetWhiteElo (welo2);
            updated1 = true;
        } else {
            appendUintElement (ti, gn1);
            Tcl_AppendElement (ti, "WhiteElo");
            appendUintElement (ti, welo1);
            appendUintElement (ti, welo2);
        }
    }
    if (welo2 == 0  &&  welo1 != 0) {
        // Copy White rating from game 1 to game 2:
        if (updateMode) {
            ie2.SetWhiteElo (welo1);
            updated2 = true;
        } else {
            appendUintElement (ti, gn2);
            Tcl_AppendElement (ti, "WhiteElo");
            appendUintElement (ti, welo2);
            appendUintElement (ti, welo1);
        }
    }
    if (belo1 == 0  &&  belo2 != 0) {
        // Copy Black rating from game 2 to game 1:
        if (updateMode) {
            ie1.SetBlackElo (belo2);
            updated1 = true;
        } else {
            appendUintElement (ti, gn1);
            Tcl_AppendElement (ti, "BlackElo");
            appendUintElement (ti, belo1);
            appendUintElement (ti, belo2);
        }
    }
    if (belo2 == 0  &&  belo1 != 0) {
        // Copy Black rating from game 1 to game 2:
        if (updateMode) {
            ie2.SetBlackElo (belo1);
            updated2 = true;
        } else {
            appendUintElement (ti, gn2);
            Tcl_AppendElement (ti, "BlackElo");
            appendUintElement (ti, belo2);
            appendUintElement (ti, belo1);
        }
    }

    if (!updateMode)
        return TCL_OK;

    auto duplicates = db->extractDuplicates();
    errorT err1 = OK;
    errorT err2 = OK;
    if (updated1) {
        Game game;
        err1 = db->getGame(ie1, game);
        if (err1 == OK) {
            err1 = db->saveGame(&game, gn1 - 1);
        }
    }
    if (updated2) {
        Game game;
        err2 = db->getGame(ie2, game);
        if (err2 == OK) {
            err2 = db->saveGame(&game, gn2 - 1);
        }
    }
    db->setDuplicates(std::move(duplicates));
    return UI_Result(ti, err1 != OK ? err1 : err2);
}

//////////////////////////////////////////////////////////////////////
///  INFO functions
static bool
date_ValidString (const char * str)
{
    uint maxValues[3] = { YEAR_MAX, 12, 31 };

    // Check year, then month, then day:
    for (uint i=0; i < 3; i++) {
        uint maxValue = maxValues[i];
        bool seenQuestion, seenDigit, seenOther;
        seenQuestion = seenDigit = seenOther = false;
        const char * start = str;
        while (*str != 0  &&  *str != '.') {
            char ch = *str;
            if (ch >= '0'  &&  ch <= '9') {
                seenDigit = true;
            } else if (ch == '?') {
                seenQuestion = true;
            } else {
                seenOther = true;
            }
            str++;
        }
        // Here, we should have seen question marks or digits, not both:
        if (seenOther) { return false; }
        if (seenQuestion  &&  seenDigit) { return false; }
        if (seenDigit) {
            // Check that the value is not too large:
            uint value = strGetUnsigned (start);
            if (value > maxValue) { return false; }
        }
        if (*str == 0) { return true; } else { str++; }
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_info:
//    General Scid Information commands.
int
sc_info (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "clipbase", "decimal", "priority",
        "html", "limit", "ratings",
        "suffix", "tb", "validDate", "version", "language", NULL
    };
    enum {
        INFO_CLIPBASE, INFO_DECIMAL, INFO_PRIORITY,
        INFO_HTML, INFO_LIMIT, INFO_RATINGS,
        INFO_SUFFIX, INFO_TB, INFO_VALIDDATE, INFO_VERSION, INFO_LANGUAGE
    };
    int index = -1;

    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case INFO_CLIPBASE:
        return UI_Result(ti, OK, DBasePool::getClipBase());

    case INFO_DECIMAL:
        if (argc >= 3) {
            decimalPointChar = argv[2][0];
        } else {
            return UI_Result(ti, OK, std::string(1, decimalPointChar));
        }
        break;

    case INFO_PRIORITY:
        return sc_info_priority(cd, ti, argc, argv);

    case INFO_HTML:
        if (argc >= 3) {
            htmlDiagStyle = strGetUnsigned (argv[2]);
        } else {
            return setUintResult (ti, htmlDiagStyle);
        }
        break;

    case INFO_LIMIT:
        return sc_info_limit (cd, ti, argc, argv);

    case INFO_RATINGS:   // List of all recognised rating types.
        {
            uint i = 0;
            while (ratingTypeNames[i] != NULL) {
                Tcl_AppendElement (ti, (char *) ratingTypeNames[i]);
                i++;
            }
        }
        break;

    case INFO_SUFFIX:
        return sc_info_suffix (cd, ti, argc, argv);

    case INFO_TB:
        return sc_info_tb (cd, ti, argc, argv);

    case INFO_VALIDDATE:
        if (argc != 3) {
            return errorResult (ti, "Usage: sc_info validDate <datestring>");
        }
        return UI_Result(ti, OK, date_ValidString (argv[2]));

    case INFO_VERSION:
        if (argc >= 3  &&  strIsPrefix (argv[2], "date")) {
            setResult (ti, __DATE__);
        } else {
            setResult (ti, SCID_VERSION_STRING);
        }
        break;
    case INFO_LANGUAGE:
      if (argc != 3) {
        return errorResult (ti, "Usage: sc_info language <lang>");
      }
      if ( strcmp(argv[2], "en") == 0) {language = 0;}
      if ( strcmp(argv[2], "fr") == 0) {language = 1;}
      if ( strcmp(argv[2], "es") == 0) {language = 2;}
      if ( strcmp(argv[2], "de") == 0) {language = 3;}
      if ( strcmp(argv[2], "it") == 0) {language = 4;}
      if ( strcmp(argv[2], "ne") == 0) {language = 5;}
      if ( strcmp(argv[2], "cz") == 0) {language = 6;}
      if ( strcmp(argv[2], "hu") == 0) {language = 7;}
      if ( strcmp(argv[2], "no") == 0) {language = 8;}
      if ( strcmp(argv[2], "sw") == 0) {language = 9;}
      if ( strcmp(argv[2], "ca") == 0) {language = 10;}
      if ( strcmp(argv[2], "fi") == 0) {language = 11;}
      if ( strcmp(argv[2], "gr") == 0) {language = 12;}

    break;
    default:
        return InvalidCommand (ti, "sc_info", options);
    };

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_info limit:
//    Limits that Scid imposes.
int
sc_info_limit (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "elo", "year", "bases", NULL
    };
    enum {
        LIM_ELO, LIM_YEAR, LIM_BASES
    };
    int index = -1;
    int result = 0;

    if (argc == 3) { index = strUniqueMatch (argv[2], options); }

    switch (index) {
    case LIM_ELO:
        result = MAX_ELO;
        break;

    case LIM_YEAR:
        result = YEAR_MAX;
        break;

    case LIM_BASES:
        result = MAX_BASES;
        break;

    default:
        return UI_Result(ti, ERROR_BadArg, "Usage: sc_info limit <elo|year|bases>");
    }

    return UI_Result(ti, OK, result);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_info suffix:
//    Returns a Scid file suffix for a database file type.
//    The suffix is returned with the leading dot.
int
sc_info_suffix (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "index", NULL
    };
    enum {
        SUFFIX_OPT_INDEX
    };
    int index = -1;

    if (argc == 3) { index = strUniqueMatch (argv[2], options); }

    const char * suffix = "";

    switch (index) {
        case SUFFIX_OPT_INDEX: suffix = INDEX_SUFFIX;    break;
        default: return InvalidCommand (ti, "sc_info suffix", options);
    }

    return setResult (ti, suffix);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_info_tb:
//   Set up a tablebase directory, or check if a certain
//   tablebase is available.
int
sc_info_tb (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage =
        "Usage: sc_info tb [<directory>|available <material>|cache <size-kb>]";

    if (argc == 2) {
        // Command: sc_info tb
        // Returns whether tablebase support is complied.
        return UI_Result(ti, OK, scid_TB_compiled());

    } else if (argc == 3) {
        // Command: sc_info_tb <directories>
        // Clears tablebases and registers all tablebases in the
        // specified directories string, which can have more than
        // one directory separated by commas or semicolons.
        return setUintResult (ti, scid_TB_Init (argv[2]));

    } else if (argc == 4  &&  argv[2][0] == 'a') {
        // Command: sc_probe available <material>
        // The material is specified as "KRKN", "kr-kn", etc.
        // Set up the required material:
        matSigT ms = MATSIG_Empty;
        const char * material = argv[3];
        if (toupper(*material) != 'K') { return UI_Result(ti, OK, false); }
        material++;
        colorT side = WHITE;
        while (1) {
            char ch = toupper(*material);
            material++;
            if (ch == 0) { break; }
            if (ch == 'K') { side = BLACK; continue; }
            pieceT p = piece_Make (side, piece_FromChar (ch));
            if (ch == 'P') { p = piece_Make (side, PAWN); }
            if (p == EMPTY) { continue; }
            ms = matsig_setCount (ms, p, matsig_getCount (ms, p) + 1);
        }
        // Check if a tablebase for this material is available:
        return UI_Result(ti, OK, scid_TB_Available (ms));
    } else if (argc == 4  &&  argv[2][0] == 'c') {
        // Set the preferred tablebase cache size, to take effect
        // at the next tablebase initialisation.
        uint cachesize = strGetUnsigned (argv[3]);
        scid_TB_SetCacheSize (cachesize * 1024);
        return TCL_OK;
    } else {
        return errorResult (ti, usage);
    }
}

//////////////////////////////////////////////////////////////////////
//  MOVE functions

int
sc_move (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "add", "addSan", "addUCI", "back", "end", "endVar", "forward",
        "pgn", "ply", "start", NULL
    };
    enum {
        MOVE_ADD, MOVE_ADDSAN, MOVE_ADDUCI, MOVE_BACK, MOVE_END, MOVE_ENDVAR,
        MOVE_FORWARD, MOVE_PGN, MOVE_PLY, MOVE_START
    };
    int index = -1;

    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case MOVE_ADD:
        return sc_move_add (cd, ti, argc, argv);

    case MOVE_ADDSAN:
        return sc_move_addSan (cd, ti, argc, argv);

    case MOVE_ADDUCI:
        return sc_move_addUCI (cd, ti, argc, argv);

    case MOVE_BACK:
        return sc_move_back (cd, ti, argc, argv);

    case MOVE_END:
        db->game->MoveToEnd();
        break;

    case MOVE_ENDVAR:
        while (db->game->MoveForward() == OK) {
        }
        break;

    case MOVE_FORWARD:
        return sc_move_forward (cd, ti, argc, argv);

    case MOVE_PGN:
        return sc_move_pgn (cd, ti, argc, argv);

    case MOVE_PLY:
        if (argc == 3) {
            db->game->MoveToPly(strGetUnsigned(argv[2]));
            return UI_Result(ti, OK);
        }
        return errorResult (ti, "Usage: sc_move ply <plynumber>");

    case MOVE_START:
        db->game->MoveToStart();
        break;

    default:
        return InvalidCommand (ti, "sc_move", options);
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_move_add: takes a move specified by three parameters
//      (square square promo) and adds it to the game.
int
sc_move_add (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{

    if (argc != 5) {
        return errorResult (ti, "Usage: sc_move add <sq> <sq> <promo>");
    }

    uint sq1 = strGetUnsigned (argv[2]);
    uint sq2 = strGetUnsigned (argv[3]);
    uint promo = strGetUnsigned (argv[4]);
    if (promo == 0) { promo = EMPTY; }

    char s[8];
    s[0] = square_FyleChar (sq1);
    s[1] = square_RankChar (sq1);
    s[2] = square_FyleChar (sq2);
    s[3] = square_RankChar (sq2);
    if (promo == EMPTY) {
        s[4] = 0;
    } else {
        s[4] = piece_Char(promo);
        s[5] = 0;
    }
    simpleMoveT sm;
    Position * pos = db->game->GetCurrentPos();
    errorT err = pos->ReadCoordMove(&sm, s, s[4] == 0 ? 4 : 5, true);
    if (err == OK) {
        err = db->game->AddMove(&sm);
        if (err == OK) {
            db->gameAltered = true;
            return TCL_OK;
        }
    }
    return errorResult (ti, "Error adding move.");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_move_addSan:
//    Takes moves in regular SAN (e.g. "e4" or "Nbd2") and adds them
//    to the game. The moves can be in one large string, separate
//    list elements, or a mixture of both. Move numbers are ignored
//    but variations/comments/annotations are parsed and added.
int sc_move_addSan(ClientData, Tcl_Interp* ti, int argc, const char** argv) {
	PgnParseLog parser;
	for (int i = 2; i < argc; ++i) {
		db->gameAltered = true;
		if (!pgnParseGame(argv[i], std::strlen(argv[i]), *db->game, parser))
			return UI_Result(ti, ERROR_InvalidMove, argv[i]);
	}
	return UI_Result(ti, OK);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_move_addUCI:
//    Takes moves in engine UCI format (e.g. "g1f3") and adds them
//    to the game. The result is translated.
//    In case of an error, return the moves that could be converted.
int
sc_move_addUCI (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    char s[8], tmp[10];
    if (argc < 3) { return TCL_OK; }
    char * ptr = (char *) argv[2];

    while (*ptr != 0) {
      s[0] = ptr[0];
      s[1] = ptr[1];
      s[2] = ptr[2];
      s[3] = ptr[3];
      if (ptr[4] == ' ') {
        s[4] = 0;
        ptr += 5;
      } else if (ptr[4] == 0) {
        s[4] = 0;
        ptr += 4;        
      } else {
        s[4] = ptr[4];
        s[5] = 0;
        ptr += 6;
      }
      simpleMoveT sm;
      Position * pos = db->game->GetCurrentPos();
      errorT err = pos->ReadCoordMove(&sm, s, s[4] == 0 ? 4 : 5, true);
      if (err == OK) {
        err = db->game->AddMove(&sm);
        if (err == OK) {
            db->gameAltered = true;
            db->game->GetPrevSAN (tmp);
            transPieces(tmp);
            Tcl_AppendResult (ti, tmp, " ", NULL);
        } else {
            //Tcl_AppendResult (ti, "Error reading move(s): ", ptr, NULL);
            break;
        }
      } else {
        //Tcl_AppendResult (ti, "Error reading move(s): ", ptr, NULL);
        break;
      }
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_move_back:
//    Moves back a specified number of moves (default = 1 move).
int
sc_move_back (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    int numMovesTakenBack = 0;
    int count = 1;
    if (argc > 2) {
        count = strGetInteger (argv[2]);
        // if (count < 1) { count = 1; }
    }

    for (int i = 0; i < count; i++) {
        if (db->game->MoveBackup() != OK) { break; }
        numMovesTakenBack++;
    }

    setUintResult (ti, numMovesTakenBack);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_move_forward:
//    Moves forward a specified number of moves (default = 1 move).
int
sc_move_forward (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    int numMovesMade = 0;
    int count = 1;
    if (argc > 2) {
        count = strGetInteger (argv[2]);
        // Do we want to allow moving forward 0 moves? Yes, I think so.
        // if (count < 1) { count = 1; }
    }

    for (int i = 0; i < count; i++) {
        if (db->game->MoveForward() != OK) { break; }
        numMovesMade++;
    }

    setUintResult (ti, numMovesMade);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_move_pgn:
//    Set the current board to the position closest to
//    the specified place in the PGN output (given as a byte count
//    from the start of the output).
int
sc_move_pgn (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_move pgn <offset>");
    }

    uint offset = strGetUnsigned (argv[2]);
    db->game->MoveToLocationInPGN (offset);
    return TCL_OK;
}



//////////////////////////////////////////////////////////////////////
//  POSITION functions

int
sc_pos (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "addNag", "analyze", "bestSquare", "board", "clearNags",
        "fen", "getComment", "getNags", "hash", "html",
        "isAt", "isCheck", "isLegal", "isPromotion",
        "matchMoves", "moveNumber", "pgnOffset",
        "probe", "setComment", "side", "tex", "moves", "location",
        "attacks", "getPrevComment", NULL
    };
    enum {
        POS_ADDNAG, POS_ANALYZE, POS_BESTSQ, POS_BOARD, POS_CLEARNAGS,
        POS_FEN, POS_GETCOMMENT, POS_GETNAGS, POS_HASH, POS_HTML,
        POS_ISAT, POS_ISCHECK, POS_ISLEGAL, POS_ISPROMO,
        POS_MATCHMOVES, POS_MOVENUM, POS_PGNOFFSET,
        POS_PROBE, POS_SETCOMMENT, POS_SIDE, POS_TEX, POS_MOVES, LOCATION,
        POS_ATTACKS, POS_GETPREVCOMMENT
    };

    char boardStr[200];
    int index = -1;
    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case POS_ADDNAG:
        return sc_pos_addNag (cd, ti, argc, argv);

    case POS_ANALYZE:
        return sc_pos_analyze (cd, ti, argc, argv);

    case POS_BESTSQ:
        return sc_pos_bestSquare (cd, ti, argc, argv);

    case POS_BOARD:
        db->game->GetCurrentPos()->MakeLongStr (boardStr);
        Tcl_AppendResult (ti, boardStr, NULL);
        break;

    case POS_CLEARNAGS:
        db->game->ClearNags();
        db->gameAltered = true;
        break;

    case POS_FEN:
        db->game->GetCurrentPos()->PrintFEN (boardStr, FEN_ALL_FIELDS);
        Tcl_AppendResult (ti, boardStr, NULL);
        break;

    case POS_GETCOMMENT:
        const char * tempStr;
        tempStr = db->game->GetMoveComment();
        if (tempStr) {
            Tcl_AppendResult (ti, tempStr, NULL);
        }
        break;

    case POS_GETPREVCOMMENT:
        return UI_Result(ti, OK, db->game->GetPreviousMoveComment());

    case POS_GETNAGS:
        return sc_pos_getNags (cd, ti, argc, argv);

    case POS_HASH:
        return sc_pos_hash (cd, ti, argc, argv);

    case POS_HTML:
        return sc_pos_html (cd, ti, argc, argv);

    case POS_ISAT:
        return sc_pos_isAt (cd, ti, argc, argv);

    case POS_ISCHECK:
        return UI_Result(ti, OK, db->game->GetCurrentPos()->IsKingInCheck());

    case POS_ISLEGAL:
        return sc_pos_isLegal (cd, ti, argc, argv);

    case POS_ISPROMO:
        return sc_pos_isPromo (cd, ti, argc, argv);

    case POS_MATCHMOVES:
        return sc_pos_matchMoves (cd, ti, argc, argv);

    case POS_MOVES:
        return sc_pos_moves (cd, ti, argc, argv);

    case POS_MOVENUM:
        // This used to return:
        //     (db->game->GetCurrentPly() + 2) / 2
        // but that value is wrong for games with non-standard
        // start positions. The correct value to return is:
        //     db->game->GetCurrentPos()->GetFullMoveCount()
        return setUintResult (ti, db->game->GetCurrentPos()->GetFullMoveCount());

    case POS_PGNOFFSET:
        setUintResult (ti, db->game->GetPgnOffset());
        break;

    case POS_PROBE:
        return sc_pos_probe (cd, ti, argc, argv);

    case POS_SETCOMMENT:
        return sc_pos_setComment (cd, ti, argc, argv);

    case POS_SIDE:
        setResult (ti, (db->game->GetCurrentPos()->GetToMove() == WHITE)
                   ? "white" : "black");
        break;

    case POS_TEX:
        {
            bool flip = false;
            if (argc > 2  &&  strIsPrefix (argv[2], "flip")) { flip = true; }
            DString * dstr = new DString;
            db->game->GetCurrentPos()->DumpLatexBoard (dstr, flip);
            Tcl_AppendResult (ti, dstr->Data(), NULL);
            delete dstr;
        }
        break;

    case LOCATION:
        return UI_Result(ti, OK, db->game->GetCurrentPly());

    case POS_ATTACKS:
        {
            Position pos(*db->game->GetCurrentPos());
            for (colorT c = WHITE; c <= BLACK; c++) {
                for (uint i = 0; i < pos.GetCount(c); i++) {
                    squareT sq = pos.GetList(c)[i];
                    pos.SetToMove(color_Flip(c));
                    int att = pos.TreeCalcAttacks(color_Flip(c), sq);
                    if (att) {
                      appendUintElement(ti, sq);
                      if (att > 1) Tcl_AppendElement(ti, "green");
                      else if (att > 0) Tcl_AppendElement(ti, "yellow");
                      else Tcl_AppendElement(ti, "red");
                    }
                }
            }
        }
        break;

    default:
        return InvalidCommand (ti, "sc_pos", options);
    }

    return TCL_OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_addNag:
//    Adds a NAG (annotation symbol) for the current move.
int
sc_pos_addNag (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_pos addNag <nagvalue>");
    }
    const char * nagStr = argv[2];
	if( strcmp(nagStr, "X") == 0)
		db->game->RemoveNag( true);
	else if( strcmp(nagStr, "Y") == 0)
		db->game->RemoveNag( false);
	else
	{
		byte nag = game_parseNag({nagStr, nagStr + std::strlen(nagStr)});
		if (nag != 0) {
			db->game->AddNag ((byte) nag);
		}
		db->gameAltered = true;
	}
    return TCL_OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_analyze:
//    Analyzes the current position for the specified number of
//    milliseconds.
//    Returns a two-element list containing the score in centipawns
//    (from the perspective of the side to move) and the best move.
//    If there are no legal moves, the second element is the empty string.
int
sc_pos_analyze (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_pos analyze [<option> <value> ...]";

    uint searchTime = 1000;   // Default = 1000 milliseconds
    uint hashTableKB = 1024;  // Default: one-megabyte hash table.
    uint pawnTableKB = 32;
    bool postMode = false;
    bool pruning = false;
    uint mindepth = 4; // will not check time until this depth is reached
    uint searchdepth = 0;

    static const char * options [] = {
        "-time", "-hashkb", "-pawnkb", "-post", "-pruning", "-mindepth", "-searchdepth", NULL
    };
    enum {
        OPT_TIME, OPT_HASH, OPT_PAWN, OPT_POST, OPT_PRUNING, OPT_MINDEPTH, OPT_SEARCHDEPTH
    };
    int arg = 2;
    while (arg+1 < argc) {
        const char * option = argv[arg];
        const char * value = argv[arg+1];
        arg += 2;
        int index = strUniqueMatch (option, options);
        switch (index) {
            case OPT_TIME:     searchTime = strGetUnsigned(value);  break;
            case OPT_HASH:     hashTableKB = strGetUnsigned(value); break;
            case OPT_PAWN:     pawnTableKB = strGetUnsigned(value); break;
            case OPT_POST:     postMode = strGetBoolean(value);     break;
            case OPT_PRUNING:  pruning = strGetBoolean(value);      break;
            case OPT_MINDEPTH: mindepth = strGetUnsigned(value);    break;
            case OPT_SEARCHDEPTH: searchdepth = strGetUnsigned(value);    break;
            default:
                return InvalidCommand (ti, "sc_pos analyze", options);
        }
    }
    if (arg != argc) { return errorResult (ti, usage); }

    // Generate all legal moves:
    Position * pos = db->game->GetCurrentPos();
    MoveList mlist;
    pos->GenerateMoves(&mlist);

    // Start the engine:
    Engine * engine = new Engine();
    engine->SetSearchTime (searchTime);
    engine->SetHashTableKilobytes (hashTableKB);
    engine->SetPawnTableKilobytes (pawnTableKB);
    engine->SetMinDepthCheckTime(mindepth);
    if (searchdepth > 0)
      engine->SetSearchDepth(searchdepth);
    engine->SetPosition (pos);
    engine->SetPostMode (postMode);
    engine->SetPruning (pruning);
    int score = engine->Think (&mlist);
    delete engine;

    char moveStr[20];
    moveStr[0] = 0;
    if (mlist.Size() > 0) {
        pos->MakeSANString (mlist.Get(0), moveStr, SAN_MATETEST);
    }
    UI_List res(2);
    res.push_back(score);
    res.push_back(moveStr);
    return UI_Result(ti, OK, res);
}

// Lambda class used by sc_pos_bestSquare() and sc_pos_isLegal()
namespace {
class SelectBySquare {
	squareT sq_;

public:
	explicit SelectBySquare(squareT sq) : sq_(sq) {}
	bool operator()(const simpleMoveT& sm) {
		return sm.from == sq_ || sm.to == sq_;
	}
};
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_bestSquare:
//    Takes a square and returns the best square that makes a move
//    with the given square. The square can be the from or to part of
//    a move. Used for smart move completion.
//    Returns -1 if no legal moves go to or from the square.
int
sc_pos_bestSquare (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_pos bestSquare <square>");
    }

    Position * pos = db->game->GetCurrentPos();

    // Try to read the square parameter as algebraic ("h8") or numeric (63):
    squareT sq = strGetSquare (argv[2]);
    if (sq == NULL_SQUARE) {
      int sqInt = strGetInteger (argv[2]);
      if (sqInt >= 0  &&  sqInt <= 63) { sq = sqInt; }
    }
    if (sq == NULL_SQUARE) {
        return errorResult (ti, "Usage: sc_pos bestSquare <square>");
    }

    // Generate all legal moves:
    MoveList mlist;
    pos->GenerateMoves(&mlist);

    // Restrict the list of legal moves to contain only those that
    // move to or from the specified square:
    mlist.resize(std::distance(mlist.begin(),
                 std::partition(mlist.begin(), mlist.end(), SelectBySquare(sq))
    ));

    // If no matching legal moves, return -1:
    if (mlist.Size() == 0) {
        return setResult (ti, "-1");
    }

    if (mlist.Size() > 1) {
        // We have more than one move to choose from, so first check
        // the ECO openings book (if it is loaded) to see if any move
        // in the list reaches an ECO position. If so, select the move
        // reaching the largest ECO code as the best move. If no ECO
        // position is found, do a small chess engine search to find
        // the best move.

        ecoT bestEco = ECO_None;
        ecoT secondBestEco = ECO_None;
        if (ecoBook != NULL) {
            for (uint i=0; i < mlist.Size(); i++) {
                pos->DoSimpleMove (mlist.Get(i));
                ecoT eco = ecoBook->findECO(pos);
                pos->UndoSimpleMove (mlist.Get(i));
                if (eco >= bestEco) {
                    secondBestEco = bestEco;
                    bestEco = eco;
                    std::rotate(mlist.begin(), mlist.begin() + i,
                                mlist.begin() + i + 1);
                }
            }
        }

        if (bestEco == ECO_None  ||  bestEco == secondBestEco) {
            // No matching ECO position found, or a tie. So do a short
            // engine search to find the best move; 25 ms (= 1/40 s)
            // is enough to reach a few ply and select reasonable
            // moves but fast enough to seem almost instant. The
            // search promotes the best move to be first in the list.
            Engine * engine = new Engine();
            engine->SetSearchTime (25);    // Do a 25 millisecond search
            engine->SetPosition (pos);
            engine->Think (&mlist);
            delete engine;
        }
    }

    // Now the best move is the first in the list, either because it
    // is the only move, or it reaches the largest ECO code, or because
    // the chess engine search selected it.
    // Find the other square in the best move and return it:

    simpleMoveT * sm = mlist.Get(0);
    ASSERT (sq == sm->from  ||  sq == sm->to);
    squareT bestSq = sm->from;
    if (sm->from == sq) { bestSq = sm->to; }
    setUintResult (ti, bestSq);

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_getNags:
//    Get the NAGs for the current move.
int
sc_pos_getNags(ClientData, Tcl_Interp* ti, int, const char**)
{
    byte * nag = db->game->GetNags();
    if (nag[0] == 0) {
        return setResult (ti, "0");
    }
    while (*nag) {
        char temp[20];
        game_printNag (*nag, temp, true, PGN_FORMAT_Plain);
        Tcl_AppendResult (ti, temp, " ", NULL);
        nag++;
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_hash:
//   Returns the 32-bit hash value of the current position.
int
sc_pos_hash (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_pos hash [full|pawn]";
    bool pawnHashOnly = false;
    if (argc > 3) { return errorResult (ti, usage); }
    if (argc == 3) {
        switch (argv[2][0]) {
            case 'f': pawnHashOnly = false; break;
            case 'p': pawnHashOnly = true;  break;
            default:  return errorResult (ti, usage);
        }
    }
    Position * pos = db->game->GetCurrentPos();
    uint hash = pos->HashValue();
    if (pawnHashOnly) {
        hash = pos->PawnHashValue();
    }
    return setUintResult (ti, hash);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_html:
//    Returns an HTML table representation of the board.
//    There are two styles: 0 (the default), which has
//    40x40 squares and images in a "bitmaps" subdirectory;
//    and style 1 which has 36x35 squares and images in
//    a "bitmaps2" directory.
//    The directory can be overridden with the "-path" command.
int
sc_pos_html (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_pos html [-flip <boolean>] [-path <path>] [<style:0|1>]";
    uint style = htmlDiagStyle;
    bool flip = false;
    int arg = 2;
    const char * path = NULL;

    if (argc > arg+1  && strEqual (argv[arg], "-flip")) {
        flip = strGetBoolean(argv[arg+1]);
        arg += 2;
    }
    if (argc > arg+1  && strEqual (argv[arg], "-path")) {
        path = argv[arg+1];
        arg += 2;
    }
    if (argc < arg ||  argc > arg+1) {
        return errorResult (ti, usage);
    }
    if (argc == arg+1) { style = strGetUnsigned (argv[arg]); }

    DString * dstr = new DString;
    db->game->GetCurrentPos()->DumpHtmlBoard (dstr, style, path, flip);
    Tcl_AppendResult (ti, dstr->Data(), NULL);
    delete dstr;
    return TCL_OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_isAt: returns whether the position is at the
//   start or end of a move list, according to the arg value.
//   Valid arguments are: start, end, vstart and vend (or unique
//   abbreviations thereof).
int
sc_pos_isAt (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "start", "end", "vstart", "vend", NULL
    };
    enum {
        OPT_START, OPT_END, OPT_VSTART, OPT_VEND
    };
    int index = -1;
    if (argc == 3) { index = strUniqueMatch(argv[2], options); }

    switch (index) {
    case OPT_START:
        return UI_Result(ti, OK, db->game->AtStart());

    case OPT_END:
        return UI_Result(ti, OK, db->game->AtEnd());

    case OPT_VSTART:
        return UI_Result(ti, OK, db->game->AtVarStart());

    case OPT_VEND:
        return UI_Result(ti, OK, db->game->AtVarEnd());

    default:
        return errorResult (ti, "Usage: sc_pos isAt start|end|vstart|vend");
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_isPromo:
//    Takes two squares (from and to, in either order) and
//    returns true if they represent a pawn promotion move.
int
sc_pos_isPromo (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 4) {
        return errorResult (ti, "Usage: sc_pos isPromotion <square> <square>");
    }

    Position * pos = db->game->GetCurrentPos();
    int fromSq = strGetInteger (argv[2]);
    int toSq = strGetInteger (argv[3]);

    if (fromSq < A1  ||  fromSq > H8  ||  toSq < A1  ||  toSq > H8) {
        return errorResult (ti, "Usage: sc_pos isPromotion <square> <square>");
    }

    return UI_Result(ti, OK, pos->IsPromoMove ((squareT) fromSq, (squareT) toSq));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_isLegal: returns true if the move between the two provided
//    squares (either could be the from square) is legal.
int
sc_pos_isLegal (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 4) {
        return errorResult (ti, "Usage: sc_pos isLegal <square> <square>");
    }

    Position * pos = db->game->GetCurrentPos();
    int sq1 = strGetInteger (argv[2]);
    int sq2 = strGetInteger (argv[3]);
    if (sq1 < 0  ||  sq1 > 63  ||  sq2 < 0  ||  sq2 > 63) {
        return UI_Result(ti, OK, false);
    }

    // Compute all legal moves, then restrict the list to only
    // contain moves that include sq1 and sq2 as to/from squares:
    MoveList mlist;
    pos->GenerateMoves(&mlist);
    simpleMoveT* end1 = std::partition(mlist.begin(), mlist.end(), SelectBySquare(sq1));
    bool found = mlist.begin() !=
                 std::partition(mlist.begin(), end1, SelectBySquare(sq2));
    return UI_Result(ti, OK, found);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_matchMoves: Return the list of legal moves matching
//     a specified prefix. Note that any occurrence of "x", "=", "+",
//     or "#" is removed from the move text of each move, and the
//     castling moves are "OK" and "OQ" for king and queen-side
//     castling respectively, so that no move is a prefix of
//     any other move.
int
sc_pos_matchMoves (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
	// copies original to target, filtering out any characters in excludeChars.
	auto strCopyExclude = [](char* target, const char* original,
	                         const char* excludeChars) {
		while (*original != 0) {
			int exclude = 0;
			for (auto s = excludeChars; *s; s++) {
				if (*original == *s) {
					exclude = 1;
					break;
				}
			}
			if (!exclude) {
				*target = *original;
				target++;
			}
			original++;
		}
		*target = 0;
	};

    if (argc != 3  &&  argc != 4) {
        return errorResult (ti, "Usage: sc_pos matchMoves <movetext-prefix>");
    }
    bool coordMoves = false;
    const char * prefix = argv[2];
    if (argc == 4) { coordMoves = strGetBoolean (argv[3]); }
    char str[20];
    Position * p = db->game->GetCurrentPos();
    sanListT sanList;
    p->CalcSANStrings (&sanList, SAN_NO_CHECKTEST);

    for (uint i=0; i < sanList.num; i++) {
        strCopyExclude (str, sanList.list[i], "x=+#");
        if (strEqual (str, "O-O")) { strCopy (str, "OK"); }
        if (strEqual (str, "O-O-O")) { strCopy (str, "OQ"); }
        if (strIsPrefix (prefix, str)) {
            Tcl_AppendElement (ti, str);
        }
    }

    // If the prefix string is for b-pawn moves, also look for any
    // Bishop moves that could match, and add them provided they do not
    // clash with a pawn move.
    if (prefix[0] >= 'a'  &&  prefix[0] <= 'h') {
        char * newPrefix = strDuplicate (prefix);
        newPrefix[0] = toupper(newPrefix[0]);
        for (uint i=0; i < sanList.num; i++) {
            strCopyExclude (str, sanList.list[i], "x=+#");
            if (strIsPrefix (newPrefix, str)) {
                Tcl_AppendElement (ti, str);
            }
        }
#ifdef WINCE
        my_Tcl_Free((char*) newPrefix);
#else
        delete[] newPrefix;
#endif
    }

    // If the prefix string starts with a file (a-h), also add coordinate
    // moves if coordMoves is true:
    if (coordMoves  &&  prefix[0] >= 'a'  &&  prefix[0] <= 'h') {
        MoveList mList;
        p->GenerateMoves(&mList);
        for (uint i=0; i < mList.Size(); i++) {
            simpleMoveT * sm = mList.Get(i);
            str[0] = square_FyleChar (sm->from);
            str[1] = square_RankChar (sm->from);
            str[2] = square_FyleChar (sm->to);
            str[3] = square_RankChar (sm->to);
            if (sm->promote == EMPTY) {
                str[4] = 0;
            } else {
                str[4] = piece_Char (sm->promote);
                str[5] = 0;
            }
            if (strIsPrefix (prefix, str)) {
                Tcl_AppendElement (ti, str);
            }
        }
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_moves: Return the list of legal moves in SAN notation
//
int
sc_pos_moves(ClientData, Tcl_Interp * ti, int argc, const char**)
{
    if (argc != 2) {
        return errorResult (ti, "Usage: sc_pos moves");
    }
    Position * p = db->game->GetCurrentPos();
    sanListT sanList; 
    p->CalcSANStrings (&sanList, SAN_CHECKTEST);

    for (uint i=0; i < sanList.num; i++) {
            Tcl_AppendElement (ti, sanList.list[i]);
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_probe:
//    Probes tablebases for the current move.
int
sc_pos_probe (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_pos probe [score|report|optimal|board <sq>]";
    static const char * options[] = {
        "score", "report", "optimal", "board", NULL
    };
    enum { OPT_SCORE, OPT_REPORT, OPT_OPTIMAL, OPT_BOARD };

    int option = OPT_SCORE;  // Default option is to return the score.
    if (argc >= 3) {
        option = strUniqueMatch(argv[2], options);
    }

    if (option == OPT_REPORT) {
        if (argc != 3) { return errorResult (ti, usage); }
        // Command: sc_probe report
        // Tablebase report:
        DString * tbReport = new DString;
        if (probe_tablebase (ti, PROBE_REPORT, tbReport)) {
            Tcl_AppendResult (ti, tbReport->Data(), NULL);
        }
        delete tbReport;
    } else if (option == OPT_OPTIMAL) {
        if (argc != 3) { return errorResult (ti, usage); }
        // Command: sc_probe optimal
        // Optimal moves from tablebase:
        DString * tbOptimal = new DString;
        if (probe_tablebase (ti, PROBE_OPTIMAL, tbOptimal)) {
            Tcl_AppendResult (ti, tbOptimal->Data(), NULL);
        }
        delete tbOptimal;
    } else if (option == OPT_SCORE) {
        int score = 0;
        if (scid_TB_Probe (db->game->GetCurrentPos(), &score) == OK) {
            setIntResult (ti, score);
        }
    } else if (option == OPT_BOARD) {
        return sc_pos_probe_board (cd, ti, argc, argv);
    } else {
        return errorResult (ti, usage);
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_probe_board:
//    Probes tablebases for the current position with one piece
//    (specified by its square) relocated to each empty board square.
int
sc_pos_probe_board (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_pos probe board <square>";

    if (argc != 4) { return errorResult (ti, usage); }

    // Try to read the square parameter as algebraic ("h8") or numeric (63):
    squareT sq = strGetSquare (argv[3]);
    if (sq == NULL_SQUARE) {
      int sqInt = strGetInteger (argv[3]);
      if (sqInt >= 0  &&  sqInt <= 63) { sq = sqInt; }
    }

    if (sq == NULL_SQUARE) {
         return errorResult (ti, usage);
    }

    Position pos = *(db->game->GetCurrentPos());
    const pieceT * board = pos.GetBoard();
    if (board[sq] == EMPTY) { return TCL_OK; }

    for (squareT toSq = A1; toSq <= H8; toSq++) {
        const char * result = "";
        if (pos.RelocatePiece (sq, toSq) != OK) {
            result = "X";
        } else {
            int score = 0;
            if (scid_TB_Probe (&pos, &score) != OK) {
                result = "?";
            } else {
                if (score > 0) {
                    result = "+";
                } else if (score < 0) {
                    result = "-";
                } else {
                    result = "=";
                }
            }
            pos.RelocatePiece (toSq, sq);
        }
        Tcl_AppendResult (ti, result, NULL);
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_pos_setComment:
//    Set the comment for the current move.
int
sc_pos_setComment (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_pos setComment <comment-text>");
    }
    const char * str = argv[2];
    const char * oldComment = db->game->GetMoveComment();

    if (str[0] == 0  || (isspace((char)str[0]) && str[1] == 0)) {
        // No comment: nullify comment if necessary:
        if (oldComment != NULL) {
            db->game->SetMoveComment (NULL);
            db->gameAltered = true;
        }
    } else {
        // Only set the comment if it has actually changed:
        if (oldComment == NULL  ||  !strEqual (str, oldComment)) {
            db->game->SetMoveComment (str);
            db->gameAltered = true;
        }
    }
    return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
//   NAME commands

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_correct:
//    Corrects specified names in the database.
int
sc_name_correct (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    nameT nt = NAME_INVALID;
    if (argc == 4) { nt = NameBase::NameTypeFromString (argv[2]); }

    if (! NameBase::IsValidNameType(nt)) {
        return errorResult (ti,
                "Usage: sc_name correct p|e|s|r <corrections>");
    }

    const char * str = argv[3];
    char oldName [512];
    char newName [512];
    char birth[128];
    char death[128];
    char line [512];
    uint errorCount = 0;
    uint correctionCount = 0;
    uint badDateCount = 0;

    scidBaseT* dbase = db;
    const NameBase* nb = dbase->getNameBase();
    std::vector<idNumberT> oldIDs;
    std::vector<std::string> newNames;
    std::vector<std::pair<dateT, dateT> > dates(nb->GetNumNames(nt),
                                                {ZERO_DATE, ZERO_DATE});
    while (*str != 0) {
        uint length = 0;
        while (*str != 0  &&  *str != '\n') {
            if (length < 511) { line[length++] = *str; }
            str++;
        }
        line[length] = 0;
        if (*str == '\n') { str++; }
        // Now parse oldName and newName out of line, if the
        // line starts with a double-quote:
        char * s = line;
        if (*s != '"') { continue; }
        birth[0] = 0;
        death[0] = 0;
        int dummyCount = 0;
        if (sscanf (line, "\"%[^\"]\" >> \"%[^\"]\" (%d)  %[0-9.?]--%[0-9.?]",
                    oldName, newName, &dummyCount, birth, death) < 2) {
            continue;
        }

        correctionCount++;

        // Find oldName in the NameBase:
        idNumberT oldID = 0;
        if (nb->FindExactName (nt, oldName, &oldID) != OK) {
            errorCount++;
            continue;
        }

        oldIDs.emplace_back(oldID);
        newNames.emplace_back(newName);
        dates[oldID] = {date_EncodeFromString(birth),
                        date_EncodeFromString(death)};
    }

    std::vector<idNumberT> newIDs;
    auto initNewIDs = [&](const std::vector<idNumberT>& v_ids) {
        newIDs.resize(nb->GetNumNames(nt));
        std::iota(newIDs.begin(), newIDs.end(), idNumberT(0));
        auto it = v_ids.begin();
        for (auto& id : oldIDs) {
            newIDs[id] = *it++;
        }
        dates.resize(newIDs.size(), {ZERO_DATE, ZERO_DATE});
    };

    auto entry_op = [&](idNumberT oldID, const IndexEntry& ie) {
        auto newID = newIDs[oldID];
        if (oldID == newID)
            return oldID;

        dateT date = ie.GetDate();
        if (date != ZERO_DATE) {
            auto range = dates[oldID];
            if (date < range.first ||
                (range.second != ZERO_DATE && date > range.second)) {
                ++badDateCount;
                return oldID;
            }
        }
        return newID;
    };

    Progress progress = UI_CreateProgress(ti);
    auto filter = dbase->newFilter();
    auto changes = dbase->transformNames(nt, dbase->getFilter(filter), progress,
                                         newNames, initNewIDs, entry_op);
    dbase->deleteFilter(filter.c_str());

    UI_List res(4);
    res.push_back(correctionCount);
    res.push_back(errorCount);
    res.push_back(changes.second);
    res.push_back(badDateCount);
    return UI_Result(ti, changes.first, res);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_edit: edits a name in the NameBase. This requires
//    writing the entire index file, since the ID number of
//    the edited name will change.
//    A rating, date or eventdate can also be edited.
//
//    1st arg: player|event|site|round|rating|date|edate
//    2nd arg: "all" / "filter" / "crosstable" (which games to edit)
//    3rd arg: name to edit.
//    4th arg: new name -- it might already exist in the namebase.
int
sc_name_edit (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_name edit <type> <oldName> <newName>";
    const char * options[] = {
        "player", "event", "site", "round", "rating",
        "date", "edate", NULL
    };
    enum {
        OPT_PLAYER, OPT_EVENT, OPT_SITE, OPT_ROUND, OPT_RATING,
        OPT_DATE, OPT_EVENTDATE
    };

    scidBaseT* dbase = db;
    int option = -1;
    if (argc > 2) { option = strUniqueMatch (argv[2], options); }

    nameT nt = NAME_PLAYER;
    switch (option) {
    case OPT_PLAYER:  nt = NAME_PLAYER;  break;
    case OPT_EVENT:   nt = NAME_EVENT;   break;
    case OPT_SITE:    nt = NAME_SITE;    break;
    case OPT_ROUND:   nt = NAME_ROUND;   break;
    case OPT_RATING:  break;
    case OPT_DATE:    break;
    case OPT_EVENTDATE:   break;
    default:
        return errorResult (ti, usage);
    }

    if (option == OPT_RATING) {
        if (argc != 7) { return errorResult (ti, usage); }
    } else {
        if (argc != 6) { return errorResult (ti, usage); }
    }

    enum { EDIT_ALL, EDIT_FILTER, EDIT_CTABLE };
    int editSelection = EDIT_ALL;
    switch (argv[3][0]) {
    case 'a': editSelection = EDIT_ALL; break;
    case 'f': editSelection = EDIT_FILTER; break;
    case 'c': editSelection = EDIT_CTABLE; break;
    default:
        return errorResult (ti, usage);
    }

    const char * oldName = argv[4];
    const char * newName = argv[5];
    dateT oldDate = ZERO_DATE;
    dateT newDate = ZERO_DATE;
    eloT newRating = 0;
    byte newRatingType = 0;
    if (option == OPT_RATING) {
        newRating = strGetUnsigned (argv[5]);
        newRatingType = strGetRatingType (argv[6]);
    }
    if (option == OPT_DATE  ||  option == OPT_EVENTDATE) {
        oldDate = date_EncodeFromString (argv[4]);
        newDate = date_EncodeFromString (argv[5]);
    }

    // Find the existing name in the namebase:
    idNumberT newID = 0;
    idNumberT oldID = 0;
    if (option != OPT_DATE  &&  option != OPT_EVENTDATE) {
        if (dbase->getNameBase()->FindExactName (nt, oldName, &oldID) != OK)
            return UI_Result(ti, OK, 0);
    }

    // Set up crosstable game criteria if necessary:
    idNumberT eventId = 0, siteId = 0;
    dateT eventDate = 0;
    if (editSelection == EDIT_CTABLE) {
        Game* g = dbase->game;
        auto nb = dbase->getNameBase();
        if (nb->FindExactName(NAME_EVENT, g->GetEventStr(), &eventId) != OK)
            return UI_Result(ti, OK, 0);
        if (nb->FindExactName(NAME_SITE, g->GetSiteStr(), &siteId) != OK)
            return UI_Result(ti, OK, 0);
        eventDate = g->GetEventDate();
    }
    auto inCTable = [&](const IndexEntry& ie) {
        if (editSelection != EDIT_CTABLE)
            return true;
        return isCrosstableGame(&ie, siteId, eventId, eventDate);
    };

    std::string filter =
        (editSelection == EDIT_FILTER) ? "dbfilter" : dbase->newFilter();
    auto hf = dbase->getFilter(filter);
    auto prg = UI_CreateProgress(ti);
    std::pair<errorT, size_t> changes;
    switch (option) {
    case OPT_DATE:
        changes = dbase->transformIndex(hf, prg, [&](IndexEntry& ie) {
            if (ie.GetDate() == oldDate && inCTable(ie)) {
                ie.SetDate(newDate);
                return true;
            }
            return false;
        });
        break;
    case OPT_EVENTDATE:
        changes = dbase->transformIndex(hf, prg, [&](IndexEntry& ie) {
            if (ie.GetEventDate() == oldDate && inCTable(ie)) {
                ie.SetEventDate(newDate);
                return true;
            }
            return false;
        });
        break;

    case OPT_RATING:
        changes = dbase->transformIndex(hf, prg, [&](IndexEntry& ie) {
            bool bTB = inCTable(ie);
            bool bWH = (ie.GetWhite() == oldID);
            bool bBK = (ie.GetBlack() == oldID);
            if (bTB && (bWH || bBK)) {
                if (bWH) {
                    ie.SetWhiteElo(newRating);
                    ie.SetWhiteRatingType(newRatingType);
                }
                if (bBK) {
                    ie.SetBlackElo(newRating);
                    ie.SetBlackRatingType(newRatingType);
                }
                return true;
            }
            return false;
        });
        break;

    default:
        changes = dbase->transformNames(
            nt, hf, prg, std::vector<std::string>(1, newName),
            [&](const std::vector<idNumberT>& v) {
                newID = v.front(); // store the newID
            },
            [&](idNumberT id, const IndexEntry& ie) {
                return (id == oldID && inCTable(ie)) ? newID : id;
            });
    }

    if (editSelection != EDIT_FILTER)
        dbase->deleteFilter(filter.c_str());

    return UI_Result(ti, changes.first, changes.second);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_retrievename:
//    Check for the right name in spellcheck and return it.
UI_res_t sc_name_retrievename (UI_handle_t ti, const SpellChecker& sp, int argc, const char ** argv)
{
    const char * usageStr = "Usage: sc_name retrievename <player>";
    if (argc != 3 ) { return errorResult (ti, usageStr); }
    std::vector<const char*> res = sp.find(NAME_PLAYER, argv[argc-1]);
    return UI_Result(ti, OK, (res.size() == 1) ? res[0] : "");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_elo:
//    Search for Elo-Ratings in spellcheck file.
static UI_res_t sc_name_elo(UI_handle_t ti, const SpellChecker& sp, int argc,
                            const char** argv) {
	auto usage = "Usage: sc_name elo [year] <player>";
	if (argc < 3 || argc > 4)
		return UI_Result(ti, ERROR_BadArg, usage);

	const char* playerName = argv[argc - 1];
	uint startYear = (argc == 4) ? strGetUnsigned(argv[2]) : 1900;

	UI_List res(YEAR_MAX * 12 * 2);
	if (auto vElo = sp.getPlayerElo(playerName)) {
		for (uint year = startYear; year < YEAR_MAX; year++) {
			for (uint month = 1; month < 13; month++) {
				if (eloT elo = vElo->getElo(DATE_MAKE(year, month, 15))) {
					char temp[500];
					sprintf(temp, "%4u.%02u", year, (month - 1) * 100 / 12);
					res.push_back(temp);
					sprintf(temp, "%4u", elo);
					res.push_back(temp);
				}
			}
		}
	}
	return UI_Result(ti, OK, res);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_info:
//    Prints information given a player name. Reports on the players
//    success rate with white and black, common openings by ECO code,
int
sc_name_info (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static char * lastPlayerName = NULL;
    const char * usageStr = "Usage: sc_name info [-htext] <player>";

    if (argc != 3  &&  argc != 4) { return errorResult (ti, usageStr); }

    bool htextOutput = false;
    bool setFilter = false;   // Set filter to games by this player
    bool setOpponent = false; // Set filter to games vs this opponent
    bool filter [NUM_COLOR_TYPES][NUM_RESULT_TYPES] =
    {
        { false, false, false, false },
        { false, false, false, false }
    };

    if (argc == 4) {
        const char * opt = argv[2];
        if (strIsPrefix ("-h", opt)  &&  strIsPrefix (opt, "-htext")) {
            htextOutput = true;
        } else if (opt[0] == '-'  &&  (opt[1] == 'f' || opt[1] == 'o')) {
            if (opt[1] == 'f') {
                setFilter = true;
            } else {
                setOpponent = true;
            }
            // Parse filter options: a = all, w = wins, d = draws, l = losses
            // for White, and capitalise those for Black.
            const char * fopt = opt + 2;
            while (*fopt != 0) {
                switch (*fopt) {
                case 'a':  // All White games:
                    filter [WHITE][RESULT_White] = true;
                    filter [WHITE][RESULT_Draw] = true;
                    filter [WHITE][RESULT_Black] = true;
                    filter [WHITE][RESULT_None] = true;
                    break;
                case 'A':  // All Black games:
                    filter [BLACK][RESULT_White] = true;
                    filter [BLACK][RESULT_Draw] = true;
                    filter [BLACK][RESULT_Black] = true;
                    filter [BLACK][RESULT_None] = true;
                    break;
                case 'w':  // White wins:
                    filter [WHITE][RESULT_White] = true;
                    break;
                case 'W':  // Black wins:
                    filter [BLACK][RESULT_White] = true;
                    break;
                case 'd':  // White draws:
                    filter [WHITE][RESULT_Draw] = true;
                    break;
                case 'D':  // Black draws:
                    filter [BLACK][RESULT_Draw] = true;
                    break;
                case 'l':  // White losses:
                    filter [WHITE][RESULT_Black] = true;
                    break;
                case 'L':  // Black losses:
                    filter [BLACK][RESULT_Black] = true;
                    break;
                default:
                    return errorResult (ti, usageStr);
                }
                fopt++;
            }
        } else {
            return errorResult (ti, usageStr);
        }
    }

    // Set up player name:
    const char * playerName = argv[argc-1];
    if (strEqual (playerName, "")) {
        if (lastPlayerName != NULL) { playerName = lastPlayerName; }
    } else {
        if (lastPlayerName != NULL) { delete[] lastPlayerName; }
        lastPlayerName = strDuplicate (playerName);
    }

    // Try to find player name in this database:
    idNumberT id = 0;
    if (db->getNameBase()->FindExactName (NAME_PLAYER, playerName, &id) != OK) {
        Tcl_AppendResult (ti, "The name \"", playerName,
                          "\" does not exist in this database.", NULL);
        return TCL_OK;
    }

    // Try to find opponent in this database:
    idNumberT opponentId = 0;
    const char * opponent = NULL;
    if (strEqual (playerName, db->game->GetWhiteStr())) {
        opponent = db->game->GetBlackStr();
    } else if (strEqual (playerName, db->game->GetBlackStr())) {
        opponent = db->game->GetWhiteStr();
    }

    if (opponent != NULL) {
        if (db->getNameBase()->FindExactName (NAME_PLAYER, opponent, &opponentId) != OK) {
            opponent = NULL;
        }
    }

    enum {STATS_ALL = 0, STATS_FILTER, STATS_OPP};

    uint whitescore [3][NUM_RESULT_TYPES];
    uint blackscore [3][NUM_RESULT_TYPES];
    uint bothscore [3][NUM_RESULT_TYPES];
    uint whitecount[3] = {0};
    uint blackcount[3] = {0};
    uint totalcount[3] = {0};
    for (uint stat=STATS_ALL; stat <= STATS_OPP; stat++) {
        for (resultT r = 0; r < NUM_RESULT_TYPES; r++) {
            whitescore[stat][r] = 0;
            blackscore[stat][r] = 0;
            bothscore[stat][r] = 0;
        }
    }

    uint ecoCount [NUM_COLOR_TYPES] [50];
    uint ecoScore [NUM_COLOR_TYPES] [50];
    for (uint ecoGroup=0; ecoGroup < 50; ecoGroup++) {
        ecoCount[WHITE][ecoGroup] = ecoCount[BLACK][ecoGroup] = 0;
        ecoScore[WHITE][ecoGroup] = ecoScore[BLACK][ecoGroup] = 0;
    }

    dateT firstGameDate = ZERO_DATE;
    dateT lastGameDate = ZERO_DATE;

    if (setFilter || setOpponent) db->dbFilter->Fill(0);

    for (uint i=0, n = db->numGames(); i < n; i++) {
        const IndexEntry* ie = db->getIndexEntry(i);
        ecoT ecoCode = ie->GetEcoCode();
        int ecoClass = -1;
        if (ecoCode != ECO_None) {
            ecoStringT ecoStr;
            eco_ToBasicString (ecoCode, ecoStr);
            if (ecoStr[0] != 0) {
                ecoClass = ((ecoStr[0] - 'A') * 10) + (ecoStr[1] - '0');
                if (ecoClass < 0  ||  ecoClass >= 50) { ecoClass = -1; }
            }
        }

        resultT result = ie->GetResult();
        idNumberT whiteId = ie->GetWhite();
        idNumberT blackId = ie->GetBlack();
        dateT date = ZERO_DATE;

        // Track statistics as white and black:
        if (whiteId == id) {
            date = ie->GetDate();
            if (ecoClass >= 0) {
                ecoCount[WHITE][ecoClass]++;
                ecoScore[WHITE][ecoClass] += RESULT_SCORE[result];
            }
            whitescore[STATS_ALL][result]++;
            bothscore[STATS_ALL][result]++;
            whitecount[STATS_ALL]++;
            totalcount[STATS_ALL]++;
            if (db->dbFilter->Get(i) > 0) {
                whitescore[STATS_FILTER][result]++;
                bothscore[STATS_FILTER][result]++;
                whitecount[STATS_FILTER]++;
                totalcount[STATS_FILTER]++;
            }
            if (opponent != NULL  &&  blackId == opponentId) {
                whitescore[STATS_OPP][result]++;
                bothscore[STATS_OPP][result]++;
                whitecount[STATS_OPP]++;
                totalcount[STATS_OPP]++;
                if (setOpponent  &&  filter[WHITE][result]) {
                    db->dbFilter->Set (i, 1);
                }
            }
            if (setFilter  &&  filter[WHITE][result]) {
                db->dbFilter->Set (i, 1);
            }
        } else if (blackId == id) {
            date = ie->GetDate();
            result = RESULT_OPPOSITE[result];
            if (ecoClass >= 0) {
                ecoCount[BLACK][ecoClass]++;
                ecoScore[BLACK][ecoClass] += RESULT_SCORE[result];
            }
            blackscore[STATS_ALL][result]++;
            bothscore[STATS_ALL][result]++;
            blackcount[STATS_ALL]++;
            totalcount[STATS_ALL]++;
            if (db->dbFilter->Get(i) > 0) {
                blackscore[STATS_FILTER][result]++;
                bothscore[STATS_FILTER][result]++;
                blackcount[STATS_FILTER]++;
                totalcount[STATS_FILTER]++;
            }
            if (opponent != NULL  &&  whiteId == opponentId) {
                blackscore[STATS_OPP][result]++;
                bothscore[STATS_OPP][result]++;
                blackcount[STATS_OPP]++;
                totalcount[STATS_OPP]++;
                if (setOpponent  &&  filter[BLACK][result]) {
                    db->dbFilter->Set (i, 1);
                }
            }
            if (setFilter  &&  filter[BLACK][result]) {
                db->dbFilter->Set (i, 1);
            }
        }

        // Keep track of first and last games by this player:
        if (date != ZERO_DATE) {
            if (firstGameDate == ZERO_DATE  ||  date < firstGameDate) {
                firstGameDate = date;
            }
            if (lastGameDate == ZERO_DATE  ||  date > lastGameDate) {
                lastGameDate = date;
            }
        }
    }

    char temp [500];
    uint score, percent;
    colorT color;
    const char * newline = (htextOutput ? "<br>" : "\n");
    const char * startHeading = (htextOutput ? "<darkblue>" : "");
    const char * endHeading = (htextOutput ? "</darkblue>" : "");
    const char * startBold = (htextOutput ? "<b>" : "");
    const char * endBold = (htextOutput ? "</b>" : "");
    uint wWidth = strLength (translate (ti, "White:"));
    uint bWidth = strLength (translate (ti, "Black:"));
    uint tWidth = strLength (translate (ti, "Total:"));
    uint wbtWidth = wWidth;
    if (bWidth > wbtWidth) { wbtWidth = bWidth; }
    if (tWidth > wbtWidth) { wbtWidth = tWidth; }
    const char * fmt = \
     "%s  %-*s %3u%c%02u%%   +%s%3u%s  =%s%3u%s  -%s%3u%s  %4u%c%c /%s%4u%s";
    SpellChecker* spChecker = spellChk;

    Tcl_AppendResult (ti, startBold, playerName, endBold, newline, NULL);

    // Show title, country, etc if listed in player spellcheck file:
    if (spChecker != NULL) {
        const PlayerInfo* pInfo = spChecker->getPlayerInfo(playerName);
        if (pInfo) { Tcl_AppendResult (ti, "  ", pInfo->GetComment(), newline, NULL); }
    }
    sprintf (temp, "  %s%u%s %s (%s: %u)",
             htextOutput ? "<red><run sc_name info -faA {}; ::windows::stats::Refresh>" : "",
             totalcount[STATS_ALL],
             htextOutput ? "</run></red>" : "",
             (totalcount[STATS_ALL] == 1 ?
              translate (ti, "game") : translate (ti, "games")),
             translate (ti, "Filter"),
             totalcount[STATS_FILTER]);
    Tcl_AppendResult (ti, temp, NULL);
    if (firstGameDate != ZERO_DATE) {
        date_DecodeToString (firstGameDate, temp);
        strTrimDate (temp);
        Tcl_AppendResult (ti, ", ", temp, NULL);
    }
    if (lastGameDate > firstGameDate) {
        date_DecodeToString (lastGameDate, temp);
        strTrimDate (temp);
        Tcl_AppendResult (ti, "--", temp, NULL);
    }
    Tcl_AppendResult (ti, newline, NULL);

    // Print biography if applicable:
    if (spChecker != NULL) {
        std::vector<const char*> bio;
        const PlayerInfo* pInfo = spChecker->getPlayerInfo(playerName, &bio);
        if (pInfo != 0) {
            for (size_t i=0, n=bio.size(); i < n; i++) {
                if (i == 0) {
                    Tcl_AppendResult (ti, newline, startHeading,
                              translate (ti, "Biography"), ":",
                              endHeading, newline, NULL);
                }
                Tcl_AppendResult (ti, "  ", bio[i], newline, NULL);
            }
        }
    }
    // Print stats for all games:

    strCopy (temp, translate (ti, "PInfoAll"));
    if (! htextOutput) { strTrimMarkup (temp); }
    Tcl_AppendResult (ti, newline, startHeading, temp, ":",
                      endHeading, newline, NULL);

    score = percent = 0;
    if (whitecount[STATS_ALL] > 0) {
        score = whitescore[STATS_ALL][RESULT_White] * 2
            + whitescore[STATS_ALL][RESULT_Draw]
            + whitescore[STATS_ALL][RESULT_None];
        percent = score * 5000 / whitecount[STATS_ALL];
    }
    sprintf (temp, fmt,
             htextOutput ? "<tt>" : "",
             wbtWidth,
             translate (ti, "White:"),
             percent / 100, decimalPointChar, percent % 100,
             htextOutput ? "<red><run sc_name info -fw {}; ::windows::stats::Refresh>" : "",
             whitescore[STATS_ALL][RESULT_White],
             htextOutput ? "</run></red>" : "",
             htextOutput ? "<red><run sc_name info -fd {}; ::windows::stats::Refresh>" : "",
             whitescore[STATS_ALL][RESULT_Draw],
             htextOutput ? "</run></red>" : "",
             htextOutput ? "<red><run sc_name info -fl {}; ::windows::stats::Refresh>" : "",
             whitescore[STATS_ALL][RESULT_Black],
             htextOutput ? "</run></red>" : "",
             score / 2, decimalPointChar, score % 2 ? '5' : '0',
             htextOutput ? "<red><run sc_name info -fa {}; ::windows::stats::Refresh>" : "",
             whitecount[STATS_ALL],
             htextOutput ? "</run></red></tt>" : "");
    Tcl_AppendResult (ti, temp, newline, NULL);

    score = percent = 0;
    if (blackcount[STATS_ALL] > 0) {
        score = blackscore[STATS_ALL][RESULT_White] * 2
            + blackscore[STATS_ALL][RESULT_Draw]
            + blackscore[STATS_ALL][RESULT_None];
        percent = score * 5000 / blackcount[STATS_ALL];
    }
    sprintf (temp, fmt,
             htextOutput ? "<tt>" : "",
             wbtWidth,
             translate (ti, "Black:"),
             percent / 100, decimalPointChar, percent % 100,
             htextOutput ? "<red><run sc_name info -fW {}; ::windows::stats::Refresh>" : "",
             blackscore[STATS_ALL][RESULT_White],
             htextOutput ? "</run></red>" : "",
             htextOutput ? "<red><run sc_name info -fD {}; ::windows::stats::Refresh>" : "",
             blackscore[STATS_ALL][RESULT_Draw],
             htextOutput ? "</run></red>" : "",
             htextOutput ? "<red><run sc_name info -fL {}; ::windows::stats::Refresh>" : "",
             blackscore[STATS_ALL][RESULT_Black],
             htextOutput ? "</run></red>" : "",
             score / 2, decimalPointChar, score % 2 ? '5' : '0',
             htextOutput ? "<red><run sc_name info -fA {}; ::windows::stats::Refresh>" : "",
             blackcount[STATS_ALL],
             htextOutput ? "</run></red></tt>" : "");
    Tcl_AppendResult (ti, temp, newline, NULL);

    score = percent = 0;
    if (totalcount[STATS_ALL] > 0) {
        score = bothscore[STATS_ALL][RESULT_White] * 2
            + bothscore[STATS_ALL][RESULT_Draw]
            + bothscore[STATS_ALL][RESULT_None];
        percent = score * 5000 / totalcount[STATS_ALL];
    }
    sprintf (temp, fmt,
             htextOutput ? "<tt>" : "",
             wbtWidth,
             translate (ti, "Total:"),
             percent / 100, decimalPointChar, percent % 100,
             htextOutput ? "<red><run sc_name info -fwW {}; ::windows::stats::Refresh>" : "",
             bothscore[STATS_ALL][RESULT_White],
             htextOutput ? "</run></red>" : "",
             htextOutput ? "<red><run sc_name info -fdD {}; ::windows::stats::Refresh>" : "",
             bothscore[STATS_ALL][RESULT_Draw],
             htextOutput ? "</run></red>" : "",
             htextOutput ? "<red><run sc_name info -flL {}; ::windows::stats::Refresh>" : "",
             bothscore[STATS_ALL][RESULT_Black],
             htextOutput ? "</run></red>" : "",
             score / 2, decimalPointChar, score % 2 ? '5' : '0',
             htextOutput ? "<red><run sc_name info -faA {}; ::windows::stats::Refresh>" : "",
             totalcount[STATS_ALL],
             htextOutput ? "</run></red></tt>" : "");
    Tcl_AppendResult (ti, temp, newline, NULL);

    // Now print stats for games in the filter:

    strCopy (temp, translate (ti, "PInfoFilter"));
    if (! htextOutput) { strTrimMarkup (temp); }
    Tcl_AppendResult (ti, newline, startHeading, temp, ":",
                      endHeading, newline, NULL);
    score = percent = 0;
    if (whitecount[STATS_FILTER] > 0) {
        score = whitescore[STATS_FILTER][RESULT_White] * 2
            + whitescore[STATS_FILTER][RESULT_Draw]
            + whitescore[STATS_FILTER][RESULT_None];
        percent = score * 5000 / whitecount[STATS_FILTER];
    }
    sprintf (temp, fmt,
             htextOutput ? "<tt>" : "",
             wbtWidth,
             translate (ti, "White:"),
             percent / 100, decimalPointChar, percent % 100,
             "", whitescore[STATS_FILTER][RESULT_White], "",
             "", whitescore[STATS_FILTER][RESULT_Draw], "",
             "", whitescore[STATS_FILTER][RESULT_Black], "",
             score / 2, decimalPointChar, score % 2 ? '5' : '0',
             "", whitecount[STATS_FILTER],
             htextOutput ? "</tt>" : "");
    Tcl_AppendResult (ti, temp, newline, NULL);

    score = percent = 0;
    if (blackcount[STATS_FILTER] > 0) {
        score = blackscore[STATS_FILTER][RESULT_White] * 2
            + blackscore[STATS_FILTER][RESULT_Draw]
            + blackscore[STATS_FILTER][RESULT_None];
        percent = score * 5000 / blackcount[STATS_FILTER];
    }
    sprintf (temp, fmt,
             htextOutput ? "<tt>" : "",
             wbtWidth,
             translate (ti, "Black:"),
             percent / 100, decimalPointChar, percent % 100,
             "", blackscore[STATS_FILTER][RESULT_White], "",
             "", blackscore[STATS_FILTER][RESULT_Draw], "",
             "", blackscore[STATS_FILTER][RESULT_Black], "",
             score / 2, decimalPointChar, score % 2 ? '5' : '0',
             "", blackcount[STATS_FILTER],
             htextOutput ? "</tt>" : "");
    Tcl_AppendResult (ti, temp, newline, NULL);

    score = percent = 0;
    if (totalcount[STATS_FILTER] > 0) {
        score = bothscore[STATS_FILTER][RESULT_White] * 2
            + bothscore[STATS_FILTER][RESULT_Draw]
            + bothscore[STATS_FILTER][RESULT_None];
        percent = score * 5000 / totalcount[STATS_FILTER];
    }
    sprintf (temp, fmt,
             htextOutput ? "<tt>" : "",
             wbtWidth,
             translate (ti, "Total:"),
             percent / 100, decimalPointChar, percent % 100,
             "", bothscore[STATS_FILTER][RESULT_White], "",
             "", bothscore[STATS_FILTER][RESULT_Draw], "",
             "", bothscore[STATS_FILTER][RESULT_Black], "",
             score / 2, decimalPointChar, score % 2 ? '5' : '0',
             "", totalcount[STATS_FILTER],
             htextOutput ? "</tt>" : "");
    Tcl_AppendResult (ti, temp, newline, NULL);

    // Now print stats for games against the current opponent:

    if (opponent != NULL) {
        Tcl_AppendResult (ti, newline, startHeading,
                          translate (ti, "PInfoAgainst"), " ",
                          startBold, opponent, endBold, ":",
                          endHeading, newline, NULL);

        score = percent = 0;
        if (whitecount[STATS_OPP] > 0) {
            score = whitescore[STATS_OPP][RESULT_White] * 2
            + whitescore[STATS_OPP][RESULT_Draw]
            + whitescore[STATS_OPP][RESULT_None];
            percent = score * 5000 / whitecount[STATS_OPP];
        }
        sprintf (temp, fmt,
                 htextOutput ? "<tt>" : "",
                 wbtWidth,
                 translate (ti, "White:"),
                 percent / 100, decimalPointChar, percent % 100,
                 htextOutput ? "<red><run sc_name info -ow {}; ::windows::stats::Refresh>" : "",
                 whitescore[STATS_OPP][RESULT_White],
                 htextOutput ? "</run></red>" : "",
                 htextOutput ? "<red><run sc_name info -od {}; ::windows::stats::Refresh>" : "",
                 whitescore[STATS_OPP][RESULT_Draw],
                 htextOutput ? "</run></red>" : "",
                 htextOutput ? "<red><run sc_name info -ol {}; ::windows::stats::Refresh>" : "",
                 whitescore[STATS_OPP][RESULT_Black],
                 htextOutput ? "</run></red>" : "",
                 score / 2, decimalPointChar, score % 2 ? '5' : '0',
                 htextOutput ? "<red><run sc_name info -oa {}; ::windows::stats::Refresh>" : "",
                 whitecount[STATS_OPP],
                 htextOutput ? "</run></red></tt>" : "");
        Tcl_AppendResult (ti, temp, newline, NULL);

        score = percent = 0;
        if (blackcount[STATS_OPP] > 0) {
            score = blackscore[STATS_OPP][RESULT_White] * 2
                + blackscore[STATS_OPP][RESULT_Draw]
                + blackscore[STATS_OPP][RESULT_None];
            percent = score * 5000 / blackcount[STATS_OPP];
        }
        sprintf (temp, fmt,
                 htextOutput ? "<tt>" : "",
                 wbtWidth,
                 translate (ti, "Black:"),
                 percent / 100, decimalPointChar, percent % 100,
                 htextOutput ? "<red><run sc_name info -oW {}; ::windows::stats::Refresh>" : "",
                 blackscore[STATS_OPP][RESULT_White],
                 htextOutput ? "</run></red>" : "",
                 htextOutput ? "<red><run sc_name info -oD {}; ::windows::stats::Refresh>" : "",
                 blackscore[STATS_OPP][RESULT_Draw],
                 htextOutput ? "</run></red>" : "",
                 htextOutput ? "<red><run sc_name info -oL {}; ::windows::stats::Refresh>" : "",
                 blackscore[STATS_OPP][RESULT_Black],
                 htextOutput ? "</run></red>" : "",
                 score / 2, decimalPointChar, score % 2 ? '5' : '0',
                 htextOutput ? "<red><run sc_name info -oA {}; ::windows::stats::Refresh>" : "",
                 blackcount[STATS_OPP],
                 htextOutput ? "</run></red></tt>" : "");
        Tcl_AppendResult (ti, temp, newline, NULL);

        score = percent = 0;
        if (totalcount[STATS_OPP] > 0) {
            score = bothscore[STATS_OPP][RESULT_White] * 2
                + bothscore[STATS_OPP][RESULT_Draw]
                + bothscore[STATS_OPP][RESULT_None];
            percent = score * 5000 / totalcount[STATS_OPP];
    }
        sprintf (temp, fmt,
                 htextOutput ? "<tt>" : "",
                 wbtWidth,
                 translate (ti, "Total:"),
                 percent / 100, decimalPointChar, percent % 100,
                 htextOutput ? "<red><run sc_name info -owW {}; ::windows::stats::Refresh>" : "",
                 bothscore[STATS_OPP][RESULT_White],
                 htextOutput ? "</run></red>" : "",
                 htextOutput ? "<red><run sc_name info -odD {}; ::windows::stats::Refresh>" : "",
                 bothscore[STATS_OPP][RESULT_Draw],
                 htextOutput ? "</run></red>" : "",
                 htextOutput ? "<red><run sc_name info -olL {}; ::windows::stats::Refresh>" : "",
                 bothscore[STATS_OPP][RESULT_Black],
                 htextOutput ? "</run></red>" : "",
                 score / 2, decimalPointChar, score % 2 ? '5' : '0',
                 htextOutput ? "<red><run sc_name info -oaA {}; ::windows::stats::Refresh>" : "",
                 totalcount[STATS_OPP],
                 htextOutput ? "</run></red></tt>" : "");
        Tcl_AppendResult (ti, temp, newline, NULL);
    }

    // Now print common openings played:

    for (color = WHITE; color <= BLACK; color++) {
        for (uint count = 0; count < 6; count++) {
            int mostPlayedIdx = -1;
            uint mostPlayed = 0;
            for (uint i=0; i < 50; i++) {
                if (ecoCount[color][i] > mostPlayed) {
                    mostPlayedIdx = i;
                    mostPlayed = ecoCount[color][i];
                }
            }
            if (mostPlayed > 0) {
                if (count == 0) {
                    const char * s = (color == WHITE ? "PInfoMostWhite" :
                                      "PInfoMostBlack");
                    Tcl_AppendResult (ti, newline, startHeading,
                                      translate (ti, s), ":",
                                      endHeading, newline, NULL);
                } else if (count == 3) {
                    Tcl_AppendResult (ti, newline, NULL);
                }
                Tcl_AppendResult (ti, "   ", NULL);

                temp[0] = mostPlayedIdx / 10 + 'A';
                temp[1] = mostPlayedIdx % 10 + '0';
                temp[2] = 0;
                if (htextOutput) {
                    Tcl_AppendResult (ti, "<blue><run ::windows::eco::Refresh ",
                                      temp, ">", NULL);
                }
                Tcl_AppendResult (ti, temp, NULL);
                if (htextOutput) {
                    Tcl_AppendResult (ti, "</run></blue>", NULL);
                }
                sprintf (temp, ":%3u (%u%%)", mostPlayed,
                         ecoScore[color][mostPlayedIdx] * 50 / mostPlayed);
                Tcl_AppendResult (ti, temp, NULL);
                ecoCount[color][mostPlayedIdx] = 0;
            }
        }
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_match: returns the first N matching names,
//    or fewer if there are not N matches, given a substring
//    to search for.
//    Output is a Tcl list, to be read in pairs: the first element of
//    each pair is the frequency, the second contains the name.
//
//    1st arg: "p" (player) / "e" (event) / "s" (site) / "r" (round)
//    2nd arg: prefix string to search for.
//    3rd arg: maximum number of matches to return.
//    Example: sc_nameMatch player "Speel" 10
int
sc_name_match (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = \
        "Usage: sc_name match [-elo] <nameType> <prefix> <maxMatches>";

    int arg = 2;
    int argsleft = argc - 2;
    bool eloMode = false;  // In elo mode, return player peak ratings.
    if (argsleft < 3) { return errorResult (ti, usage); }
    if (argv[arg][0] == '-'  &&  strIsPrefix (argv[arg], "-elo")) {
        eloMode = true;
        arg++;
        argsleft--;
    }
    if (argsleft != 3) {
        return errorResult (ti, usage);
    }

    nameT nt = NameBase::NameTypeFromString (argv[arg++]);
    if (nt == NAME_INVALID) {
        return errorResult (ti, usage);
    }

    const char * prefix = argv[arg++];
    uint maxMatches = strGetUnsigned (argv[arg++]);
    if (maxMatches == 0) { return TCL_OK; }
    auto matches = db->getNameBase()->getFirstMatches(nt, prefix, maxMatches);
    for (auto nameID : matches) {
        uint freq = db->getNameFreq(nt, nameID);
        const char * str = db->getNameBase()->GetName (nt, nameID);
        appendUintElement (ti, freq);
        Tcl_AppendElement (ti, str);
        if (nt == NAME_PLAYER  &&  eloMode) {
            appendUintElement (ti, db->getNameBase()->GetElo (nameID));
        }
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_plist:
//   Returns a list of play data matching selected criteria.
struct PlayerActivity {
    dateT firstDate;
    dateT lastDate;

    PlayerActivity() : firstDate(ZERO_DATE), lastDate(ZERO_DATE) {}
    void addDate(dateT date) {
        if (firstDate == ZERO_DATE || date < firstDate) firstDate = date;
        if (date > lastDate) lastDate = date;
    }
};

class PListSort{
    scidBaseT* dbase_;
    int sort_;
    const std::vector<PlayerActivity>& activity_;
    enum { SORT_ELO, SORT_GAMES, SORT_OLDEST, SORT_NEWEST, SORT_NAME };

public:
    PListSort(scidBaseT* dbase, const std::vector<PlayerActivity>& activity, int sortOrder)
    : dbase_(dbase), sort_(sortOrder), activity_(activity) {
	}
    bool operator() (idNumberT p1, idNumberT p2)
    {
        const NameBase* nb = dbase_->getNameBase();
        int compare = 0;
        switch (sort_) {
        case SORT_ELO:
            compare = nb->GetElo(p2) - nb->GetElo(p1);
            break;
        case SORT_GAMES:
            compare = dbase_->getNameFreq(NAME_PLAYER, p2) - dbase_->getNameFreq(NAME_PLAYER, p1);
            break;
        case SORT_OLDEST:
             // Sort by oldest game year in ascending order:
            compare = date_GetYear(activity_[p1].firstDate) - date_GetYear(activity_[p2].firstDate);
            break;
        case SORT_NEWEST:
             // Sort by newest game date in descending order:
            compare = date_GetYear(activity_[p2].lastDate) - date_GetYear(activity_[p1].lastDate);
            break;
        }

        // If equal, resolve by comparing names, first case-insensitive and
        // then case-sensitively if still tied:
        if (compare == 0) {
            const char* name1 = nb->GetName (NAME_PLAYER, p1);
            const char* name2 = nb->GetName (NAME_PLAYER, p2);
            compare = strCaseCompare (name1, name2);
            if (compare == 0) { compare = strCompare (name1, name2); }
        }
        return compare < 0;
    }
};

int
sc_name_plist (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    const char * usage = "Usage: sc_name plist [-<option> <value> ...]";

    scidBaseT* dbase = db;
    const char * namePrefix = "";
    uint minGames = 0;
    uint maxGames = dbase->numGames();
    uint minElo = 0;
    uint maxElo = MAX_ELO;
    size_t count = 10;

    static const char * options [] = {
        "-name", "-minElo", "-maxElo", "-minGames", "-maxGames",
        "-size", "-sort", NULL
    };
    enum {
        OPT_NAME, OPT_MINELO, OPT_MAXELO, OPT_MINGAMES, OPT_MAXGAMES,
        OPT_SIZE, OPT_SORT
    };

    // Valid sort types:
    static const char * sortModes [] = {
        "elo", "games", "oldest", "newest", "name", NULL
    };
    enum {
        SORT_ELO, SORT_GAMES, SORT_OLDEST, SORT_NEWEST, SORT_NAME
    };

    int sortMode = SORT_NAME;

    // Read parameters in pairs:
    int arg = 2;
    while (arg+1 < argc) {
        const char * option = argv[arg];
        const char * value = argv[arg+1];
        arg += 2;
        int index = strUniqueMatch (option, options);
        switch (index) {
            case OPT_NAME:     namePrefix = value;                   break;
            case OPT_MINELO:   minElo = strGetUnsigned (value);      break;
            case OPT_MAXELO:   maxElo = strGetUnsigned (value);      break;
            case OPT_MINGAMES: minGames = strGetUnsigned (value);    break;
            case OPT_MAXGAMES: maxGames = strGetUnsigned (value);    break;
            case OPT_SIZE:     count = strGetUnsigned (value); break;
            case OPT_SORT:
               sortMode = strUniqueMatch (value, sortModes);
               break;
            default:
                return InvalidCommand (ti, "sc_name plist", options);
        }
    }

    if (arg != argc) { return errorResult (ti, usage); }
    if (sortMode == -1) return InvalidCommand (ti, "sc_name plist -sort", sortModes);


    const NameBase* nb = dbase->getNameBase();
    idNumberT nPlayers = nb->GetNumNames(NAME_PLAYER);

    std::vector<idNumberT> plist;
    for (idNumberT id = 0; id < nPlayers; id++) {
        const char * name = nb->GetName (NAME_PLAYER, id);
        uint nGames = db->getNameFreq (NAME_PLAYER, id);
        eloT elo = nb->GetElo (id);
        if (nGames < minGames  ||  nGames > maxGames) { continue; }
        if (elo < minElo  ||  elo > maxElo) { continue; }
        if (! strIsCasePrefix (namePrefix, name)) { continue; }
        plist.push_back(id);
    }

    std::vector<PlayerActivity> activity(nPlayers);
    for (gamenumT gnum=0, n = dbase->numGames(); gnum < n; gnum++) {
        const IndexEntry* ie = dbase->getIndexEntry(gnum);
        dateT date = ie->GetDate();
        if (date_GetYear(date) > 0) {
            activity[ie->GetWhite()].addDate(date);
            activity[ie->GetBlack()].addDate(date);
        }
    }

    count = std::min(count, plist.size());
    std::partial_sort(plist.begin(), plist.begin() + count, plist.end(), PListSort(dbase, activity, sortMode));

    UI_List res(count);
    UI_List info(5);
    for (size_t i=0; i < count; i++) {
        idNumberT id = plist[i];
        info.clear();
        info.push_back(dbase->getNameFreq(NAME_PLAYER, id));
        info.push_back(date_GetYear(activity[id].firstDate));
        info.push_back(date_GetYear(activity[id].lastDate));
        info.push_back(nb->GetElo(id));
        info.push_back(nb->GetName(NAME_PLAYER, id));
        res.push_back(info);
    }

    return UI_Result(ti, OK, res);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_ratings:
//   Scan the current database for games with unrated players who
//   have Elo rating information in the spellcheck file, and fill
//   in the missing Elo ratings.
//
//   Boolean options:
//      -nomonth (default=true): indicates whether games with no month
//           should still have ratings allocated, assuming the month
//           to be January.
//      -update (default=true): indicates whether the database should
//           be updated; if it is false, no actual changes are made.
//      -debug (default=false): for debugging; it dumps all detected
//           rating changes, one per line, to stdout.
//      -test (default=false): tests whether a sspelling file with
//           rating info is loaded and can be used on this database.
//      -overwrite (default=false): if true, existing ratings can be
//           changed.
//
//   Returns a two-integer list: the number of changed ratings, and
//   the number of changed games.
UI_res_t sc_name_ratings (UI_handle_t ti, scidBaseT& dbase, const SpellChecker& sp, int argc, const char ** argv)
{
    const char * options[] = {
        "-update", "-test", "-change", "-filter" };
    enum {
        OPT_UPDATE, OPT_TEST, OPT_CHANGE, OPT_FILTER
    };

    bool updateIndexFile = true;
    bool testOnly = false;
    bool overwrite = false;
    bool filterOnly = false;

    int arg = 2;
    while (arg+1 < argc) {
        int option = strUniqueMatch (argv[arg], options);
        bool value = strGetBoolean (argv[arg+1]);
        switch (option) {
            case OPT_UPDATE:    updateIndexFile = value;    break;
            case OPT_TEST:      testOnly = value;           break;
            case OPT_CHANGE:    overwrite = value;          break;
            case OPT_FILTER:    filterOnly = value;         break;
            default: return InvalidCommand (ti, "sc_name ratings", options);
        }
        arg += 2;
    }

    if (! sp.hasEloData()) {
        return UI_Result(ti, ERROR, "The current spellcheck file does not have "
                          "Elo rating information.\n\n"
                          "To use this function, you should load "
                          "\"ratings.ssp\" (available from the Scid website) "
                          "as your spellcheck file first.");
    }

    if (testOnly) { return UI_Result(ti, OK); }

    uint numChangedRatings = 0;
    uint numChangedGames = 0;
    const NameBase* nb = dbase.getNameBase();
    std::vector<bool> cached(nb->GetNumNames(NAME_PLAYER), false);
    std::vector<const PlayerElo*> vElo(nb->GetNumNames(NAME_PLAYER), NULL);

    auto getElo = [&](idNumberT id, dateT date) {
        if (!cached[id]) {
            cached[id] = true;
            vElo[id] = sp.getPlayerElo(nb->GetName(NAME_PLAYER, id));
        }
        return (vElo[id]) ? vElo[id]->getElo(date) : 0;
    };

    auto entry_op = [&](IndexEntry& ie) {
        dateT date = ie.GetDate();
        eloT eloWhite = (!overwrite && ie.GetWhiteElo() != 0)
                            ? 0
                            : getElo(ie.GetWhite(), date);
        eloT eloBlack = (!overwrite && ie.GetBlackElo() != 0)
                            ? 0
                            : getElo(ie.GetBlack(), date);
        unsigned nChanges = (eloWhite != 0) ? 1 : 0;
        nChanges += (eloBlack != 0) ? 1 : 0;
        if (nChanges) {
            numChangedRatings += nChanges;
            numChangedGames++;
            if (updateIndexFile) {
                if (eloWhite != 0) {
                    ie.SetWhiteElo(eloWhite);
                    ie.SetWhiteRatingType(RATING_Elo);
                }
                if (eloBlack != 0) {
                    ie.SetBlackElo(eloBlack);
                    ie.SetBlackRatingType(RATING_Elo);
                }
                return true;
            }
        }
        return false;
    };

    std::string filter = (filterOnly) ? "dbfilter" : dbase.newFilter();
    auto hf = dbase.getFilter(filter);
    auto changes = dbase.transformIndex(hf, UI_CreateProgress(ti), entry_op);
    if (!filterOnly)
        dbase.deleteFilter(filter.c_str());

    UI_List res(2);
    res.push_back(numChangedRatings);
    res.push_back(numChangedGames);
    return UI_Result(ti, changes.first, res);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_read:
//   Reads a Scid name spelling file into memory, and returns a list of
//   four integers: the number of player, event, site and round names in
//   the file.
//   If there is no filename argument, sc_name_read just returns the same
//   list for the current spellchecker status without reading a new file.
int
sc_name_read (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc > 5) {
        return UI_Result(ti, ERROR_BadArg, "Usage: sc_name read <spellcheck-file>");
    }

    if (argc > 2) {
        const char * filename = argv[2];
        Progress progress = UI_CreateProgress(ti);
        std::pair<errorT, SpellChecker*> newSpell = SpellChecker::Create(filename, progress);
        if (newSpell.first != OK) {
            return UI_Result(ti, newSpell.first, "Error reading name spellcheck file.");
        }
        if (spellChk != NULL) { delete spellChk; }
        spellChk = newSpell.second;
        progress.report(1, 1);
    }

    UI_List res(NUM_NAME_TYPES);
    for (nameT i = 0; i < NUM_NAME_TYPES; i++) {
        size_t n = (spellChk == NULL) ? 0 : spellChk->numCorrectNames(i);
        res.push_back(n);
    }
    return UI_Result(ti, OK, res);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_name_spellcheck:
//   Scan the current database for spelling corrections.
UI_res_t sc_name_spellcheck (UI_handle_t ti, scidBaseT& dbase, const SpellChecker& sp, int argc, const char ** argv)
{
    nameT nt = NAME_INVALID;
    uint maxCorrections = 20000;
    bool doSurnames = false;
    bool ambiguous = true;
    const char * usage = "Usage: sc_name spellcheck [-max <integer>] [-surnames <boolean>] [-ambiguous <boolean>] players|events|sites|rounds";

    const char * options[] = {
        "-max", "-surnames", "-ambiguous", NULL
    };
    enum {
        OPT_MAX, OPT_SURNAMES, OPT_AMBIGUOUS
    };
    
    int arg = 2;
    while (arg+1 < argc) {
        const char * option = argv[arg];
        const char * value = argv[arg+1];
        arg += 2;
        int index = -1;
        if (option[0] == '-') { index = strUniqueMatch (option, options); }

        switch (index) {
        case OPT_MAX:
            maxCorrections = strGetUnsigned (value);
            if ( maxCorrections == 0 ) {
                maxCorrections = (uint)-1;
            }
            break;
        case OPT_SURNAMES:
            doSurnames = strGetBoolean (value);
            break;
        case OPT_AMBIGUOUS:
            ambiguous = strGetBoolean (value);
            break;
        default:
            return InvalidCommand (ti, "sc_name spellcheck", options);
        }
    }
    if (arg+1 != argc) { return errorResult (ti, usage); }
    nt = NameBase::NameTypeFromString (argv[arg]);

    if (! NameBase::IsValidNameType (nt)) {
        return errorResult (ti, usage);
    }

    const NameBase* nb = dbase.getNameBase();
    std::vector<std::string> tmpRes;
    char tempStr[1024];
    uint correctionCount = 0;
    
    Progress progress = UI_CreateProgress(ti);
    // Check every name of the specified type:
    for (idNumberT id=0, n=nb->GetNumNames(nt); id < n; id++) {
        if (correctionCount >= maxCorrections) break;

        if ((id % 1000) == 0) {  // Update the percentage done bar:
            if (!progress.report(id, n)) break;
        }

        uint frequency = db->getNameFreq(nt, id);
        // Do not bother trying to correct unused names:
        if (frequency == 0) continue;

        const char* origName = nb->GetName(nt, id);
        // If requested ignore surnames
        if (nt == NAME_PLAYER  &&  !doSurnames  && strIsSurnameOnly (origName)) continue;

        // First, check for a general prefix or suffix correction:
        std::string name = origName;
        size_t nGenCorrections = sp.getGeneralCorrections(nt).normalize(&name);

        // If spellchecking names, remove any country code like " (USA)"
        // in parentheses at the end of the name:
        if (nt == NAME_PLAYER) {
            size_t country = name.rfind(" (");
            if (country != std::string::npos && (country + 6) == name.length()) {
                if (*(name.rbegin()) == ')') name.erase(country);
            }
        }

        std::vector<const char*> corrections = sp.find(nt, name.c_str());
        // If requested ignore ambiguous corrections
        if (!ambiguous && corrections.size() > 1) continue;

        if (nGenCorrections != 0 && corrections.size() == 0) {
            corrections.push_back(name.c_str());
        }

        std::string correctCmd;
        const char* strAmbiguous = corrections.size() > 1 ? "Ambiguous: " : "";
        for (size_t i=0; i < corrections.size(); i++) {
            if (strcmp(origName, corrections[i]) == 0) {
                if (corrections.size() != 1) {
                    correctCmd.append("ERROR: " + name);
                }
                continue;
            }
            if (i==0) correctionCount++;

            sprintf (tempStr, "%s\"%s\"\t>> \"%s\" (%u)",
                              strAmbiguous,
                              origName,
                              corrections[i],
                              frequency);
            correctCmd += tempStr;

            if (nt == NAME_PLAYER) { // Look for a player birthdate:
                const PlayerInfo* pInfo = sp.getPlayerInfo(corrections[i]);
                dateT birthdate = pInfo->getBirthdate();
                dateT deathdate = pInfo->getDeathdate();
                if (birthdate != ZERO_DATE  ||  deathdate != ZERO_DATE) {
                    correctCmd += "  ";
                    if (birthdate != ZERO_DATE) {
                        date_DecodeToString (birthdate, tempStr);
                        correctCmd += tempStr;
                    }
                    correctCmd += "--";
                    if (deathdate != ZERO_DATE) {
                        date_DecodeToString (deathdate, tempStr);
                        correctCmd += tempStr;
                    }
                }
            }
            correctCmd += "\n";
        }

        if (!correctCmd.empty()) tmpRes.push_back(correctCmd);
    }

    std::sort(tmpRes.begin(), tmpRes.end());

    progress.report(1,1);

    // Now generate the return message:
    static const char* NAME_TYPE_STRING[] = {"player", "event", "site", "round"};
    sprintf (tempStr, "Scid found %u %s name correction%s.\n",
             correctionCount, NAME_TYPE_STRING[nt],
             strPlural (correctionCount));
    std::string res = tempStr;
    res +=
        "Edit the list to remove any corrections you do not want.\n"
        "Only lines of the form:\n"
        "   \"Old Name\" >> \"New Name\"\n"
        "(with no spaces before the \"Old Name\") are processed.\n"
        "You can discard a correction you do not want by deleting\n"
        "its line, or simply by adding a space or any other character\n"
        "at the start of the line.\n";
    if (nt == NAME_PLAYER  &&  ! doSurnames) {
        res +=
            "Note: player names with a surname only, such as \"Kramnik\",\n"
            "have not been corrected, since such corrections are often\n"
            "wrong. You can choose to also show surname-only corrections\n"
            "using the button below.\n";
    }
    res += "\n";
    std::vector<std::string>::const_iterator it = tmpRes.begin();
    std::vector<std::string>::const_iterator it_Ambiguous =
        std::lower_bound(tmpRes.begin(), tmpRes.end(), "Ambig");
    for (;it != it_Ambiguous; it++) {
        res += *it;
    }
    for (;it != tmpRes.end(); it++) {
        res += "\n";
        res += *it;
    }
    return UI_Result(ti, OK, res);
}

UI_res_t sc_name(UI_extra_t cd, UI_handle_t ti, int argc, const char** argv) {
	static const char * options [] = {
        "correct", "edit", "info", "match", "plist",
        "ratings", "read", "spellcheck", "retrievename", "elo",
        NULL
    };
    enum {
        OPT_CORRECT, OPT_EDIT, OPT_INFO, OPT_MATCH, OPT_PLIST,
        OPT_RATINGS, OPT_READ, OPT_SPELLCHECK, OPT_RETRIEVENAME, OPT_ELO
    };

    int index = -1;
    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    if (!db->inUse) {
        return errorResult (ti, ERROR_FileNotOpen, errMsgNotOpen(ti));
    }

    switch (index) {
    case OPT_INFO:
        return sc_name_info (cd, ti, argc, argv);

    case OPT_MATCH:
        return sc_name_match (cd, ti, argc, argv);

    case OPT_PLIST:
        return sc_name_plist (cd, ti, argc, argv);

    case OPT_READ:
        return sc_name_read (cd, ti, argc, argv);
    }

    if (db->isReadOnly() && index != OPT_RETRIEVENAME) {
        return errorResult (ti, ERROR_FileReadOnly);
    }

    switch (index) {
    case OPT_CORRECT:
        return sc_name_correct (cd, ti, argc, argv);

    case OPT_EDIT:
        return sc_name_edit (cd, ti, argc, argv);
    };

    if (spellChk == NULL) {
        return UI_Result(ti, ERROR,
            "A spellcheck file has not been loaded.\n\n"
            "You can load one from the Options menu.");
    }

    switch (index) {
    case OPT_RATINGS:
        return sc_name_ratings(ti, *db, *spellChk, argc, argv);

    case OPT_RETRIEVENAME:
        return sc_name_retrievename(ti, *spellChk, argc, argv);

    case OPT_SPELLCHECK:
        return sc_name_spellcheck(ti, *db, *spellChk, argc, argv);

    case OPT_ELO:
        return sc_name_elo (ti, *spellChk, argc, argv);

    default:
        return InvalidCommand (ti, "sc_name", options);
    }

    return TCL_OK;
}

//////////////////////////////////////////////////////////////////////
//  OPENING/PLAYER REPORT functions

static uint
avgGameLength (resultT result)
{
    uint sum = 0;
    uint count = 0;
    for (gamenumT i=0, n = db->numGames(); i < n; i++) {
        const IndexEntry* ie = db->getIndexEntry(i);
        if (result == ie->GetResult()) {
            count++;
            sum += ((ie->GetNumHalfMoves() + 1) / 2);
        }
    }
    if (count == 0) { return 0; }
    return (sum + (count/2)) / count;
}

int
sc_report (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "avgLength", "best", "counts", "create", "eco", "elo",
        "endmaterial", "format", "frequency", "line", "max", "moveOrders",
        "notes", "players", "print", "score", "select", "themes", NULL
    };
    enum {
        OPT_AVGLENGTH, OPT_BEST, OPT_COUNTS, OPT_CREATE, OPT_ECO, OPT_ELO,
        OPT_ENDMAT, OPT_FORMAT, OPT_FREQ, OPT_LINE, OPT_MAX, OPT_MOVEORDERS,
        OPT_NOTES, OPT_PLAYERS, OPT_PRINT, OPT_SCORE, OPT_SELECT, OPT_THEMES
    };

    static const char * usage =
        "Usage: sc_report opening|player <command> [<options...>]";
    OpTable * report = NULL;
    if (argc < 2) {
        return errorResult (ti, usage);
    }
    switch (argv[1][0]) {
        case 'O': case 'o':  report = reports[REPORT_OPENING]; break;
        case 'P': case 'p':  report = reports[REPORT_PLAYER]; break;
        default:
            return errorResult (ti, usage);
    }

    DString * dstr = NULL;
    int index = strUniqueMatch (argv[2], options);

    if (! db->inUse) {
        return errorResult (ti, errMsgNotOpen(ti));
    }
    if (index >= 0  &&  index != OPT_CREATE  &&  report == NULL) {
        return errorResult (ti, "No report has been created yet.");
    }

    const scidBaseT::Stats& stats = db->getStats();
    switch (index) {
    case OPT_AVGLENGTH:
        if (argc != 4) {
            return errorResult (ti, "Usage: sc_report player|opening avgLength 1|=|0|*");
        } else {
            resultT result = strGetResult (argv[3]);
            appendUintElement (ti, report->AvgLength (result));
            appendUintElement (ti, avgGameLength (result));
        }
        break;

    case OPT_BEST:
        if (argc != 5) {
            return errorResult (ti, "Usage: sc_report opening|player best w|b|a|o|n <count>");
        }
        dstr = new DString;
        report->BestGames (dstr, strGetUnsigned(argv[4]), argv[3]);
        Tcl_AppendResult (ti, dstr->Data(), NULL);
        break;

    case OPT_COUNTS:
        appendUintElement (ti, report->GetTotalCount());
        appendUintElement (ti, report->GetTheoryCount());
        break;

    case OPT_CREATE:
        return sc_report_create (cd, ti, argc, argv);

    case OPT_ECO:
        if (argc > 3) {
            dstr = new DString();
            report->TopEcoCodes (dstr, strGetUnsigned(argv[3]));
            Tcl_AppendResult (ti, dstr->Data(), NULL);
        } else {
            Tcl_AppendResult (ti, report->GetEco(), NULL);
        }
        break;

    case OPT_ELO:
        if (argc != 4) {
            return errorResult (ti, "Usage: sc_report opening|player elo white|black");
        } else {
            colorT color = WHITE;
            uint count = 0;
            uint pct = 0;
            uint perf = 0;
            if (argv[3][0] == 'B'  ||  argv[3][0] == 'b') { color = BLACK; }
            uint avg = report->AvgElo (color, &count, &pct, &perf);
            appendUintElement (ti, avg);
            appendUintElement (ti, count);
            appendUintElement (ti, pct);
            appendUintElement (ti, perf);
        }
        break;

    case OPT_ENDMAT:
        dstr = new DString;
        report->EndMaterialReport (dstr,
                       translate (ti, "OprepReportGames", "Report games"),
                       translate (ti, "OprepAllGames", "All games"));
        Tcl_AppendResult (ti, dstr->Data(), NULL);
        break;

    case OPT_FORMAT:
        if (argc != 4) {
            return errorResult (ti, "Usage: sc_report opening|player format latex|html|text|ctext");
        }
        report->SetFormat (argv[3]);
        break;

    case OPT_FREQ:
        if (argc != 4) {
            return errorResult (ti, "Usage: sc_report opening|player frequency 1|=|0|*");
        } else {
            resultT result = strGetResult (argv[3]);
            appendUintElement (ti, report->PercentFreq (result));
            uint freq = stats.nResults[result] * 1000;
            freq = freq / db->numGames();
            appendUintElement (ti, freq);
        }
        break;

    case OPT_LINE:
        dstr = new DString;
        report->PrintStemLine (dstr);
        Tcl_AppendResult (ti, dstr->Data(), NULL);
        break;

    case OPT_MAX:
        if (argc == 4  &&  argv[3][0] == 'g') {
            return setUintResult (ti, OPTABLE_MAX_TABLE_LINES);
        } else if (argc == 4  &&  argv[3][0] == 'r') {
            return setUintResult (ti, OPTABLE_MAX_ROWS);
        }
        return errorResult (ti, "Usage: sc_report opening|player max games|rows");

    case OPT_MOVEORDERS:
        if (argc != 4) {
            return errorResult (ti, "Usage: sc_report opening|player moveOrders <count>");
        }
        dstr = new DString;
        report->PopularMoveOrders (dstr, strGetUnsigned(argv[3]));
        Tcl_AppendResult (ti, dstr->Data(), NULL);
        break;

    case OPT_NOTES:
        if (argc < 4  ||  argc > 5) {
            return errorResult (ti, "Usage: sc_report opening|player notes <0|1> [numrows]");
        }
        report->ClearNotes ();
        if (strGetBoolean (argv[3])  &&  report->GetNumLines() > 0) {
            report->GuessNumRows ();
            if (argc > 4) {
                uint nrows = strGetUnsigned (argv[4]);
                if (nrows > 0) { report->SetNumRows (nrows); }
            }
            dstr = new DString;
            // Print the table just to set up notes, but there is
            // no need to return the result:
            report->PrintTable (dstr, "", "");
        }
        break;

    case OPT_PLAYERS:
        if (argc != 5) {
            return errorResult (ti, "Usage: sc_report opening|player players w|b <count>");
        } else {
            colorT color = WHITE;
            if (argv[3][0] == 'B'  ||  argv[3][0] == 'b') { color = BLACK; }
            dstr = new DString;
            report->TopPlayers (dstr, color, strGetUnsigned(argv[4]));
            Tcl_AppendResult (ti, dstr->Data(), NULL);
        }
        break;

    case OPT_PRINT:
        if (argc < 3  ||  argc > 6) {
            return errorResult (ti, "Usage: sc_report opening|players print [numrows] [title] [comment]");
        }
        report->GuessNumRows ();
        if (argc > 3) {
            uint nrows = strGetUnsigned (argv[3]);
            if (nrows > 0) { report->SetNumRows (nrows); }
        }
        dstr = new DString;
        report->PrintTable (dstr, argc > 4 ? argv[4] : "",
                             argc > 5 ? argv[5] : "");
        Tcl_AppendResult (ti, dstr->Data(), NULL);
        break;

    case OPT_SCORE:
        appendUintElement (ti, report->PercentScore());
        {
            uint percent = stats.nResults[RESULT_White] * 2;
            percent += stats.nResults[RESULT_Draw];
            percent = percent * 500;
            uint sum = (stats.nResults[RESULT_White] +
                                 stats.nResults[RESULT_Draw] +
                                 stats.nResults[RESULT_Black]);
            if (sum != 0)
            	percent = percent / sum;
            	else
            	percent = 0;
            appendUintElement (ti, percent);
        }
        break;

    case OPT_SELECT:
        return sc_report_select (cd, ti, argc, argv);

    case OPT_THEMES:
        dstr = new DString;
        report->ThemeReport (dstr, argc - 3, (const char **) argv + 3);
        Tcl_AppendResult (ti, dstr->Data(), NULL);
        break;

    default:
        return InvalidCommand (ti, "sc_report", options);
    }

    if (dstr != NULL) { delete dstr; }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_report_create:
//    Creates a new opening table.
//    NOTE: It assumes the filter contains the results
//    of a tree search for the current position, so
//    the Tcl code that calls this need to ensure that
//    is done first.
int
sc_report_create (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    uint maxThemeMoveNumber = 20;
    uint maxExtraMoves = 1;
    uint maxLines = OPTABLE_MAX_TABLE_LINES;
    static const char * usage =
        "Usage: sc_report opening|player create [maxExtraMoves] [maxLines] [excludeMove]";

    uint reportType = 0;
    if (argc < 2) {
        return errorResult (ti, usage);
    }

    switch (argv[1][0]) {
        case 'O': case 'o':  reportType = REPORT_OPENING; break;
        case 'P': case 'p':  reportType = REPORT_PLAYER; break;
        default:
            return errorResult (ti, usage);
    }

    if (argc > 3) {
        maxExtraMoves = strGetUnsigned (argv[3]);
    }
    if (argc > 4) {
        maxLines = strGetUnsigned (argv[4]);
        if (maxLines > OPTABLE_MAX_TABLE_LINES) {
            maxLines = OPTABLE_MAX_TABLE_LINES;
        }
        if (maxLines == 0) { maxLines = 1; }
    }
    const char * excludeMove = "";
    if (argc > 5) { excludeMove = argv[5]; }
    if (excludeMove[0] == '-') { excludeMove = ""; }

    if (reports[reportType] != NULL) {
        delete reports[reportType];
    }
    OpTable* report = new OpTable(reportTypeName[reportType], db->game, ecoBook.get());
    reports[reportType] = report;
    report->SetMaxTableLines (maxLines);
    report->SetExcludeMove (excludeMove);
    report->SetDecimalChar (decimalPointChar);
    report->SetMaxThemeMoveNumber (maxThemeMoveNumber);

    Progress progress = UI_CreateProgress(ti);

    for (uint gnum=0, n = db->numGames(); gnum < n; gnum++) {
        if ((gnum % 2000) == 0) {  // Update the percentage done bar:
            if (!progress.report(gnum, n)) break;
        }

        byte ply = db->dbFilter->Get(gnum);
        const IndexEntry* ie = db->getIndexEntry(gnum);
        if (ply != 0) {
            if (db->getGame(*ie, *scratchGame) != OK) {
                return errorResult (ti, "Error reading game file.");
            }
            scratchGame->MoveToPly (ply - 1);
            if (scratchGame->AtEnd()) ply = 0;
            if (ply != 0) {
                uint moveOrderID = report->AddMoveOrder (scratchGame);
                OpLine * line = new OpLine (scratchGame, ie, gnum+1,
                                            maxExtraMoves, maxThemeMoveNumber);
                if (report->Add (line)) {
                    line->SetMoveOrderID (moveOrderID);
                } else {
                    delete line;
                }
            }
        }
        report->AddEndMaterial (ie->GetFinalMatSig(), (ply != 0));
    }
    progress.report(1,1);

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_report_select:
//    Restricts the filter to only contain games
//    in the opening report matching the specified
//    opening/endgame theme or note number.
int
sc_report_select (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * usage =
        "Usage: sc_report opening|player select <op|eg|note> <number>";
    if (argc != 5) {
        return errorResult (ti, usage);
    }
    OpTable * report = NULL;
    switch (argv[1][0]) {
        case 'O': case 'o':  report = reports[REPORT_OPENING]; break;
        case 'P': case 'p':  report = reports[REPORT_PLAYER]; break;
        default:
            return errorResult (ti, usage);
    }

    char type = tolower (argv[3][0]);
    uint number = strGetUnsigned (argv[4]);

    uint * matches = report->SelectGames (type, number);
    uint * match = matches;
    db->dbFilter->Fill(0);
    while (*match != 0) {
        uint gnum = *match - 1;
        match++;
        uint ply = *match + 1;
        match++;
        db->dbFilter->Set (gnum, ply);
    }
    delete[] matches;

    return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
//  SEARCH and TREE functions

int
sc_tree (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "stats", "cachesize", "cacheinfo", NULL
    };
    enum {
        TREE_STATS, TREE_CACHESIZE, TREE_CACHEINFO
    };

    int index = -1;
    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case TREE_STATS:
        return sc_tree_stats(cd, ti, argc, argv);

    case TREE_CACHESIZE:
        return sc_tree_cachesize (cd, ti, argc, argv);

    case TREE_CACHEINFO:
        return sc_tree_cacheinfo (cd, ti, argc, argv);

    default:
        return InvalidCommand (ti, "sc_tree", options);
    }

    return TCL_OK;
}

// @returns the tree stats of the specified filter
int
sc_tree_stats (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * usage =
      "Usage: sc_tree stats baseId filterId [<0|1>] [alpha|eco|frequency|score]";

    // Sort options: these should match the moveSortE enumerated type.
    static const char * sortOptions[] = {
        "alpha", "eco", "frequency", "score", NULL
    };
    // Enumeration of possible move-sorting methods for tree mode:
    enum moveSortE { SORT_ALPHA, SORT_ECO, SORT_FREQUENCY, SORT_SCORE };

    if (argc < 4)
        return UI_Result(ti, ERROR_BadArg, usage);

    scidBaseT* base = DBasePool::getBase(strGetUnsigned(argv[2]));
    if (!base)
        return UI_Result(ti, ERROR_BadArg, usage);

    HFilter filter = base->getFilter(argv[3]);
    if (filter == nullptr)
        return UI_Result(ti, ERROR_BadArg, usage);

    bool hideMoves = (argc > 4) ? strGetBoolean(argv[4]) : false;
    int sortMethod = (argc > 5) ? strUniqueMatch(argv[5], sortOptions)
                                : SORT_FREQUENCY;
    if (sortMethod < 0)
        return UI_Result(ti, ERROR_BadArg, usage);

    Position searchPos = *(db->game->GetCurrentPos());
    auto tree = base->getTreeStat(filter);

    auto calc_eco = [&](auto const& move) {
        ecoT eco = ECO_None;
        if (ecoBook && move) {
            simpleMoveT sm;
            sm.from = move.getFrom();
            sm.to = move.getTo();
            sm.movingPiece = piece_Make(move.getColor(), move.getPiece());
            sm.promote = move.isPromo() ? move.getPromo() : EMPTY;

            searchPos.DoSimpleMove(&sm);
            eco = ecoBook->findECO(&searchPos);
            searchPos.UndoSimpleMove(&sm);
        }
        return eco;
    };

    char tempTrans[10];
    auto calc_san = [&](auto const& move) {
        strcpy(tempTrans, move ? move.getSAN().c_str() : "[end]");
        transPieces(tempTrans);
    };

    if (sortMethod == SORT_ALPHA) { // icase alphabetical order
        std::sort(tree.begin(), tree.end(), [&](auto const& a, auto const& b) {
            calc_san(a.move);
            std::string temp = tempTrans;
            calc_san(b.move);
            return temp < std::string_view(tempTrans);
        });

    } else if (sortMethod == SORT_ECO) { // Order by eco code
        std::sort(tree.begin(), tree.end(), [&](auto const& a, auto const& b) {
            return calc_eco(a.move) < calc_eco(b.move);
        });

    } else if (sortMethod == SORT_SCORE) { // Order by success
        if (searchPos.GetToMove() == WHITE) {
            std::sort(tree.begin(), tree.end(),
                      [&](auto const& a, auto const& b) {
                          return a.score() > b.score();
                      });
        } else {
            std::sort(tree.begin(), tree.end(),
                      [&](auto const& a, auto const& b) {
                          return a.score() < b.score();
                      });
        }
    }

    char temp[256];
    std::string output;
    const char * titleRow =
        "    Move   ECO       Frequency    Score  AvElo Perf AvYear %Draws";
    titleRow = translate (ti, "TreeTitleRow", titleRow);
    output.append(titleRow);

    auto format_output = [&](auto const& node, auto& dest) {
        const auto avgElo = static_cast<int>(node.avgElo());
        const auto avgYear = static_cast<int>(node.avgYear());
        const auto pctDraws = static_cast<int>(node.percDraws());
        const auto score = node.score();
        const auto perf = node.eloCount < 10
                              ? 0
                              : static_cast<int>(node.eloPerformance());

        sprintf(temp, "  %3d%c%1d%%", score / 10, decimalPointChar, score % 10);
        dest.append(temp);
        if (avgElo == 0) {
            dest.append("      ");
        } else {
            sprintf(temp, "  %4d", avgElo);
            dest.append(temp);
        }
        if (perf == 0) {
            dest.append("      ");
        } else {
            sprintf(temp, "  %4d", perf);
            dest.append(temp);
        }
        if (avgYear == 0) {
            dest.append("      ");
        } else {
            sprintf(temp, "  %4d", avgYear);
            dest.append(temp);
        }
        sprintf(temp, "  %3d%%", pctDraws);
        dest.append(temp);
    };

    if (tree.size() > 0) {
        TreeNode totals({});
        for (auto const& node : tree) { // reduce
            totals.freq[0] += node.freq[0];
            totals.freq[RESULT_White] += node.freq[RESULT_White];
            totals.freq[RESULT_Black] += node.freq[RESULT_Black];
            totals.freq[RESULT_Draw] += node.freq[RESULT_Draw];
            totals.eloWhiteSum += node.eloWhiteSum;
            totals.eloBlackSum += node.eloBlackSum;
            totals.eloCount += node.eloCount;
            totals.yearSum += node.yearSum;
            totals.yearCount += node.yearCount;
        }

        // Now we print the list into the return string:
        unsigned count = 0;
        for (auto const& node : tree) {
            calc_san(node.move);
            ecoT eco = calc_eco(node.move);
            ecoStringT ecoStr;
            eco_ToExtendedString(eco, ecoStr);
            auto freq = long(1000ll * node.freq[0] / totals.freq[0]);
            sprintf (temp, "\n%2u: %-6s %-5s %7u:%3ld%c%1ld%%",
                     ++count,
                     hideMoves ? "---" : tempTrans,//node->san,
                     hideMoves ? "" : ecoStr,
                     node.freq[0],
                     freq / 10,
                     decimalPointChar,
                     freq % 10);
            output.append(temp);
            format_output(node, output);
        }

        // Print a totals line as well, if there are any moves in the tree:
        const char * totalString = translate (ti, "TreeTotal:", "TOTAL:");
        output.append("\n_______________________________________________________________\n");
        sprintf (temp, "%-12s     %7u:100%c0%%",
                 totalString, totals.freq[0], decimalPointChar);
        output.append(temp);
        format_output(totals, output);
        output.append("\n");
    }
    return UI_Result(ti, OK, output);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_tree_cachesize:
//    set cache size
int
sc_tree_cachesize (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
  if (argc != 4) {
    return errorResult (ti, "Usage: sc_tree cachesize <base> <size>");
  }
  scidBaseT* base = DBasePool::getBase(strGetInteger(argv[2]));
  if (base) base->treeCache.CacheResize(strGetUnsigned(argv[3]));
  return TCL_OK;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_tree_cacheinfo:
//    returns a list of 2 values : used slots and max cache size
int
sc_tree_cacheinfo (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
  if (argc != 3) {
    return errorResult (ti, "Usage: sc_tree cacheinfo <base>");
  }
  scidBaseT* base = DBasePool::getBase(strGetInteger(argv[2]));
  if (base)
      return UI_Result(ti, OK, base->treeCache.Size());

  return UI_Result(ti, OK, 0);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_search:
//    Search function interface.
int
sc_search (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "board", "header", "material", NULL
    };
    enum { OPT_BOARD, OPT_HEADER, OPT_MATERIAL };

    int index = -1;
    if (argc > 1) { index = strUniqueMatch (argv[1], options); }
    int ret = TCL_OK;

    if (!db->inUse) {
        return errorResult (ti, errMsgNotOpen(ti));
    }

    switch (index) {
    case OPT_BOARD:
        ret = sc_search_board (ti, db, db->getFilter("dbfilter"), argc, argv);
        break;

    case OPT_HEADER: {
        HFilter filter = db->getFilter("dbfilter");
        ret = sc_search_header (cd, ti, db, filter, argc, argv);
        break;
    }

    case OPT_MATERIAL:
        ret = sc_search_material (cd, ti, argc, argv);
        break;

    default:
        return InvalidCommand (ti, "sc_search", options);
    }

    return ret;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_search_board:
//    Searches for exact match for the current position.
//    if <base> is present, search for current position in base <base>,
//    and sets <base> filter accordingly
int sc_search_board(Tcl_Interp* ti, const scidBaseT* dbase, HFilter filter,
                    int argc, const char** argv) {
	ASSERT(dbase != nullptr && filter != nullptr);

	const char* usageStr =
	    "Usage: sc_search board <filterOp> <searchType> <searchInVars> <flip>";

	if (argc != 6)
		return errorResult(ti, usageStr);

    filterOpT filterOp = strGetFilterOp (argv[2]);

    bool useHpSigSpeedup = false;
    gameExactMatchT searchType = GAME_EXACT_MATCH_Exact;

    switch (argv[3][0]) {
    case 'E':
        searchType = GAME_EXACT_MATCH_Exact;
        useHpSigSpeedup = true;
        break;
    case 'P':
        searchType = GAME_EXACT_MATCH_Pawns;
        useHpSigSpeedup = true;
        break;
    case 'F':
        searchType = GAME_EXACT_MATCH_Fyles;
        break;
    case 'M':
        searchType = GAME_EXACT_MATCH_Material;
        break;
    default:
        return errorResult (ti, usageStr);
    }

    bool searchInVars = strGetBoolean (argv[4]);
    bool flip = false;
    flip = strGetBoolean (argv[5]);

    Position * pos = db->game->GetCurrentPos();
    
    Progress progress = UI_CreateProgress(ti);
    Timer timer;  // Start timing this search.
    Position * posFlip =  NULL;
    matSigT msig = matsig_Make (pos->GetMaterial());
    matSigT msigFlip = 0;
    uint hpSig = pos->GetHPSig();
    uint hpSigFlip = 0;

    if (flip) {
        posFlip = new Position;
        char cboard [40];
        pos->PrintCompactStrFlipped (cboard);
        posFlip->ReadFromCompactStr ((byte *) cboard);
        hpSigFlip = posFlip->GetHPSig();
        msigFlip = matsig_Make (posFlip->GetMaterial());
    }

    // If filter operation is to reset the filter, reset it:
    if (filterOp == FILTEROP_RESET) {
        filter->includeAll();
        filterOp = FILTEROP_AND;
    }
    size_t startFilterCount = filter->size();

    // Here is the loop that searches on each game:
    Game tmpGame;
    Game* g = &tmpGame;
    gamenumT gameNum = 0;
    for (gamenumT n = dbase->numGames(); gameNum < n; gameNum++) {
        if ((gameNum % 5000) == 0) {  // Update the percentage done bar:
            if (!progress.report(gameNum, n)) break;
        }
        // First, apply the filter operation:
        if (filterOp == FILTEROP_AND) {  // Skip any games not in the filter:
            if (filter.get(gameNum) == 0) {
                continue;
            }
        } else /* filterOp==FILTEROP_OR*/ { // Skip any games in the filter:
            if (filter.get(gameNum) != 0) {
                continue;
            } else {
                // OK, this game is NOT in the filter.
                // Add it so filterCounts are kept up to date:
                filter.set (gameNum, 1);
            }
        }

        const IndexEntry* ie = dbase->getIndexEntry(gameNum);
        if (ie->GetLength() == 0) {
            // Skip games with no gamefile record:
            filter.set (gameNum, 0);
            continue;
        }

        // Set "useVars" to true only if the search specified searching
        // in variations, AND this game has variations:
        bool useVars = searchInVars && ie->GetVariationsFlag();

        bool possibleMatch = true;
        bool possibleFlippedMatch = flip;

        // Apply speedups if we are not searching in variations:
        if (! useVars) {
            if (! ie->GetStartFlag()) {
                // Speedups that only apply to standard start games:
                if (useHpSigSpeedup  &&  hpSig != 0xFFFF) {
                    const byte * hpData = ie->GetHomePawnData();
                    if (! hpSig_PossibleMatch (hpSig, hpData)) {
                        possibleMatch = false;
                    }
                    if (possibleFlippedMatch) {
                        if (! hpSig_PossibleMatch (hpSigFlip, hpData)) {
                            possibleFlippedMatch = false;
                        }
                    }
                }
            }

            // If this game has no promotions, check the material of its final
            // position, since the searched position might be unreachable:
            if (possibleMatch) {
                if (!matsig_isReachable (msig, ie->GetFinalMatSig(),
                                         ie->GetPromotionsFlag(),
                                         ie->GetUnderPromoFlag())) {
                        possibleMatch = false;
                    }
            }
            if (possibleFlippedMatch) {
                if (!matsig_isReachable (msigFlip, ie->GetFinalMatSig(),
                                         ie->GetPromotionsFlag(),
                                         ie->GetUnderPromoFlag())) {
                        possibleFlippedMatch = false;
                    }
            }
        }

        if (!possibleMatch  &&  !possibleFlippedMatch) {
            filter.set (gameNum, 0);
            continue;
        }

        // At this point, the game needs to be loaded:
        auto bbuf = dbase->getGame(*ie);
        if (!bbuf) {
            return errorResult (ti, "Error reading game file.");
        }
        uint ply = 0;
        if (useVars) {
            g->DecodeMovesOnly(bbuf);
            // Try matching the game without variations first:
            if (ply == 0  &&  possibleMatch) {
                if (g->ExactMatch (pos, NULL, NULL, searchType)) {
                    ply = g->GetCurrentPly() + 1;
                }
            }
            if (ply == 0  &&  possibleFlippedMatch) {
                if (g->ExactMatch (posFlip, NULL, NULL, searchType)) {
                    ply = g->GetCurrentPly() + 1;
                }
            }
            if (ply == 0  &&  possibleMatch) {
                g->MoveToStart();
                if (g->VarExactMatch (pos, searchType)) {
                    ply = g->GetCurrentPly() + 1;
                }
            }
            if (ply == 0  &&  possibleFlippedMatch) {
                g->MoveToStart();
                if (g->VarExactMatch (posFlip, searchType)) {
                    ply = g->GetCurrentPly() + 1;
                }
            }
        } else {
            // No searching in variations:
            if (possibleMatch) {
                auto bbuf_clone = bbuf;
                if (g->ExactMatch(pos, &bbuf_clone, NULL, searchType)) {
                    // Set its auto-load move number to the matching move:
                    ply = g->GetCurrentPly() + 1;
                }
            }
            if (ply == 0  &&  possibleFlippedMatch) {
                if (g->ExactMatch (posFlip, &bbuf, NULL, searchType)) {
                    ply = g->GetCurrentPly() + 1;
                }
            }
        }
        if (ply > 255) { ply = 255; }
        filter.set (gameNum, ply);
    }

    progress.report(1,1);
    if (flip) { delete posFlip; }

    // Now print statistics and time for the search:
    char temp[200];
    int centisecs = timer.CentiSecs();
    if (gameNum != dbase->numGames()) {
        Tcl_AppendResult (ti, errMsgSearchInterrupted(ti), "  ", NULL);
    }
    sprintf (temp, "%lu / %lu  (%d%c%02d s)",
             static_cast<unsigned long>(filter->size()),
             static_cast<unsigned long>(startFilterCount),
             centisecs / 100, decimalPointChar, centisecs % 100);
    Tcl_AppendResult (ti, temp, NULL);
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// addPattern():
//    Called by the parameter parsing section of sc_search_material()
//    to add a pattern to a pattern list.
//    Returns the new head of the pattern list.
patternT *
addPattern (patternT * pattHead, patternT * addPatt)
{
    // Create a new pattern structure:
#ifdef WINCE
    patternT * newPatt = (patternT *) my_Tcl_Alloc(sizeof(new patternT));
#else
    patternT * newPatt = new patternT;
#endif

    // Initialise it:
    newPatt->flag = addPatt->flag;
    newPatt->pieceMatch = addPatt->pieceMatch;
    newPatt->fyleMatch = addPatt->fyleMatch;
    newPatt->rankMatch = addPatt->rankMatch;

    // Add to the head of the list of patterns, and return:
    newPatt->next = pattHead;
    return newPatt;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// freePatternList():
//    Frees the memory used by a list of patterns.
void
freePatternList (patternT * patt)
{
    patternT * nextPatt;
    while (patt) {
        nextPatt = patt->next;
#ifdef WINCE
        my_Tcl_Free((char*)patt);
#else
        delete patt;
#endif
        patt = nextPatt;
    }
}

void
flipPattern (patternT * patt)
{
    if (patt->rankMatch != NO_RANK) {
        patt->rankMatch = (RANK_1 + RANK_8) - patt->rankMatch;
    }
    patt->pieceMatch = PIECE_FLIP[patt->pieceMatch];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parsePattern:
//    Called by sc_search_material to extract the details of
//    a pattern parameter (e.g "no wp c ?" for no White pawn on
//    the d file).
errorT
parsePattern (const char * str, patternT * patt)
{
    ASSERT (str != NULL  &&  patt != NULL);

    // Set up pointers to the four whitespace-separated pattern
    // parameter values in the string:
    str = strFirstWord (str);
    const char * flagStr = str;
    str = strNextWord (str);
    const char * colorPieceStr = str;
    str = strNextWord (str);
    const char * fyleStr = str;
    str = strNextWord (str);
    const char * rankStr = str;

    // Parse the color parameter: "w", "b", or "?" for no pattern.
    if (*colorPieceStr == '?') {
        // Empty pattern:
        patt->pieceMatch = EMPTY;
        return OK;
    }

    colorT color = WHITE;
    switch (tolower(*colorPieceStr)) {
        case 'w': color = WHITE; break;
        case 'b': color = BLACK; break;
        default: return ERROR;
    }

    // Parse the piece type parameter for this pattern:
    pieceT p = EMPTY;
    switch (tolower(colorPieceStr[1])) {
        case 'k': p = KING; break;
        case 'q': p = QUEEN; break;
        case 'r': p = ROOK; break;
        case 'b': p = BISHOP; break;
        case 'n': p = KNIGHT; break;
        case 'p': p = PAWN; break;
        default: return ERROR;
    }

    patt->pieceMatch = piece_Make (color, p);
    patt->flag = strGetBoolean (flagStr);

    // Parse the fyle parameter for this pattern:
    char ch = *fyleStr;
    if (ch == '?') {
        patt->fyleMatch = NO_FYLE;
    } else if (ch >= 'a'  &&  ch <= 'h') {
        patt->fyleMatch = A_FYLE + (ch - 'a');
    } else {
        return ERROR;
    }

    // Parse the rank parameter for this pattern:
    ch = *rankStr;
    if (ch == '?') {
        patt->rankMatch = NO_RANK;
    } else if (ch >= '1'  &&  ch <= '8') {
        patt->rankMatch = RANK_1 + (ch - '1');
    } else {
        return ERROR;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_search_material:
//    Searches by material and/or pattern.
int
sc_search_material (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (! db->inUse) {
        return errorResult (ti, "Not an open database.");
    }

    uint minMoves = 0;
    uint minPly = 0;
    uint maxPly = 999;
    uint matchLength = 1;
    byte min[MAX_PIECE_TYPES] = {0};
    byte minFlipped[MAX_PIECE_TYPES] = {0};
    byte max[MAX_PIECE_TYPES] = {0};
    byte maxFlipped[MAX_PIECE_TYPES] = {0};
    max[WM] = max[BM] = 9;
    int matDiff[2];
    matDiff[0] = -40;
    matDiff[1] = 40;
    filterOpT filterOp = FILTEROP_RESET;
    bool flip = false;
    bool oppBishops = true;
    bool sameBishops = true;
    uint hpExcludeMask = HPSIG_Empty;
    uint hpExMaskFlip = HPSIG_Empty;
    patternT * patt = NULL;
    patternT * flippedPatt = NULL;
    patternT tempPatt;

    const char * options[] = {
        "wq", "bq", "wr", "br", "wb", "bb", "wn", "bn",
        "wp", "bp", "wm", "bm", "flip", "filter", "range",
        "length", "bishops", "diff", "pattern", NULL
    };
    enum {
        OPT_WQ, OPT_BQ, OPT_WR, OPT_BR, OPT_WB, OPT_BB, OPT_WN, OPT_BN,
        OPT_WP, OPT_BP, OPT_WM, OPT_BM, OPT_FLIP, OPT_FILTER, OPT_RANGE,
        OPT_LENGTH, OPT_BISHOPS, OPT_DIFF, OPT_PATTERN
    };

    int arg = 2;
    while (arg+1 < argc) {
        const char * option = argv[arg];
        const char * value = argv[arg+1];
        arg += 2;
        int index = -1;
        if (option[0] == '-') {
            index = strUniqueMatch (&(option[1]), options);
        }
        uint counts [2] = {0, 0};
        if (index >= OPT_WQ  &&  index <= OPT_BM) {
            strGetUnsigneds (value, counts, 2);
        }

        switch (index) {

        case OPT_WQ:  min[WQ] = counts[0];  max[WQ] = counts[1];  break;
        case OPT_BQ:  min[BQ] = counts[0];  max[BQ] = counts[1];  break;
        case OPT_WR:  min[WR] = counts[0];  max[WR] = counts[1];  break;
        case OPT_BR:  min[BR] = counts[0];  max[BR] = counts[1];  break;
        case OPT_WB:  min[WB] = counts[0];  max[WB] = counts[1];  break;
        case OPT_BB:  min[BB] = counts[0];  max[BB] = counts[1];  break;
        case OPT_WN:  min[WN] = counts[0];  max[WN] = counts[1];  break;
        case OPT_BN:  min[BN] = counts[0];  max[BN] = counts[1];  break;
        case OPT_WP:  min[WP] = counts[0];  max[WP] = counts[1];  break;
        case OPT_BP:  min[BP] = counts[0];  max[BP] = counts[1];  break;
        case OPT_WM:  min[WM] = counts[0];  max[WM] = counts[1];  break;
        case OPT_BM:  min[BM] = counts[0];  max[BM] = counts[1];  break;

        case OPT_FLIP:
            flip = strGetBoolean (value);
            break;

        case OPT_FILTER:
            filterOp = strGetFilterOp (value);
            break;

        case OPT_RANGE:
            strGetUnsigneds (value, counts, 2);
            minPly = counts[0];  maxPly = counts[1];
            break;

        case OPT_LENGTH:
            matchLength = strGetUnsigned (value);
            if (matchLength < 1) { matchLength = 1; }
            break;

        case OPT_BISHOPS:
            switch (toupper(value[0])) {
                case 'S': oppBishops = false; sameBishops = true;  break;
                case 'O': oppBishops = true;  sameBishops = false; break;
                default:  oppBishops = true;  sameBishops = true;  break;
            }
            break;

        case OPT_DIFF:
            strGetIntegers (value, matDiff, 2);
            break;

        case OPT_PATTERN:
            if (parsePattern (value, &tempPatt) != OK) {
                Tcl_AppendResult (ti, "Invalid pattern: ", value, NULL);
                return TCL_ERROR;
            }
            // Only add to lists if a pattern was specified:
            if (tempPatt.pieceMatch == EMPTY) { break; }
            // Update home-pawn exclude masks if appropriate:
            if (!tempPatt.flag
                &&  piece_Type(tempPatt.pieceMatch) == PAWN
                &&  tempPatt.rankMatch == NO_RANK
                &&  tempPatt.fyleMatch != NO_FYLE) {
                colorT color = piece_Color (tempPatt.pieceMatch);
                colorT flipColor = (color == WHITE ? BLACK : WHITE);
                fyleT fyle = tempPatt.fyleMatch;
                hpExcludeMask = hpSig_AddPawn (hpExcludeMask, color, fyle);
                hpExMaskFlip = hpSig_AddPawn (hpExMaskFlip, flipColor, fyle);
            }
            // Add the pattern and its flipped equivalent:
            patt = addPattern (patt, &tempPatt);
            flipPattern (&tempPatt);
            flippedPatt = addPattern (flippedPatt, &tempPatt);
            break;

        default:
            return InvalidCommand (ti, "sc_search material", options);
        }
    }
    if (arg != argc) { return errorResult (ti, "Odd number of parameters."); }

    // Sanity check of values:
    if (max[WQ] < min[WQ]) { max[WQ] = min[WQ]; }
    if (max[BQ] < min[BQ]) { max[BQ] = min[BQ]; }
    if (max[WR] < min[WR]) { max[WR] = min[WR]; }
    if (max[BR] < min[BR]) { max[BR] = min[BR]; }
    if (max[WB] < min[WB]) { max[WB] = min[WB]; }
    if (max[BB] < min[BB]) { max[BB] = min[BB]; }
    if (max[WN] < min[WN]) { max[WN] = min[WN]; }
    if (max[BN] < min[BN]) { max[BN] = min[BN]; }
    if (max[WP] < min[WP]) { max[WP] = min[WP]; }
    if (max[BP] < min[BP]) { max[BP] = min[BP]; }
    // Minor piece range should be at least the sum of the Bishop
    // and Knight minimums, and at most the sum of the maximums:
    if (min[WM] < min[WB]+min[WN]) { min[WM] = min[WB] + min[WN]; }
    if (min[BM] < max[BB]+min[BN]) { min[BM] = min[BB] + min[BN]; }
    if (max[WM] > max[WB]+max[WN]) { max[WM] = max[WB] + max[WN]; }
    if (max[BM] > max[BB]+max[BN]) { max[BM] = max[BB] + max[BN]; }

    // Swap material difference range values if necessary:
    if (matDiff[0] > matDiff[1]) {
        int temp = matDiff[0]; matDiff[0] = matDiff[1]; matDiff[1] = temp;
    }

    // Set up flipped piece counts if necessary:
    if (flip) {
        minFlipped[WQ] = min[BQ];  maxFlipped[WQ] = max[BQ];
        minFlipped[WR] = min[BR];  maxFlipped[WR] = max[BR];
        minFlipped[WB] = min[BB];  maxFlipped[WB] = max[BB];
        minFlipped[WN] = min[BN];  maxFlipped[WN] = max[BN];
        minFlipped[WP] = min[BP];  maxFlipped[WP] = max[BP];
        minFlipped[WM] = min[BM];  maxFlipped[WM] = max[BM];
        minFlipped[BQ] = min[WQ];  maxFlipped[BQ] = max[WQ];
        minFlipped[BR] = min[WR];  maxFlipped[BR] = max[WR];
        minFlipped[BB] = min[WB];  maxFlipped[BB] = max[WB];
        minFlipped[BN] = min[WN];  maxFlipped[BN] = max[WN];
        minFlipped[BP] = min[WP];  maxFlipped[BP] = max[WP];
        minFlipped[BM] = min[WM];  maxFlipped[BM] = max[WM];
    }

    // Convert move numbers to halfmoves (ply counts):
    minMoves = minPly;
    minPly = minPly * 2 - 1;
    maxPly = maxPly * 2;

    // Set up the material Sig: it is the signature of the MAXIMUMs.
    matSigT msig, msigFlipped;
    int checkMsig = 1;
    if (max[WQ] > 3  ||  max[BQ] > 3  ||  max[WR] > 3 ||  max[BR] > 3 ||
        max[WB] > 3  ||  max[BB] > 3  ||  max[WN] > 3 ||  max[BN] > 3) {
        // It is an unusual search, we cannot use material sig!
        checkMsig = 0;
    }
    msig = matsig_Make (max);
    msigFlipped = MATSIG_FlipColor(msig);

    Progress progress = UI_CreateProgress(ti);
    Timer timer;  // Start timing this search.

    char temp [250];
    Game * g = scratchGame;
    HFilter filter = db->getFilter("dbfilter");

    // If filter operation is to reset the filter, reset it:
    if (filterOp == FILTEROP_RESET) {
        filter->includeAll();
        filterOp = FILTEROP_AND;
    }
    size_t startFilterCount = filter->size();

    // Here is the loop that searches on each game:
    gamenumT gameNum = 0, n = db->numGames();
    for (; gameNum < n; gameNum++) {
        if ((gameNum % 1000) == 0) {  // Update the percentage done bar:
            if (!progress.report(gameNum, n)) break;
        }
        // First, apply the filter operation:
        if (filterOp == FILTEROP_AND) {  // Skip any games not in the filter:
            if (filter.get(gameNum) == 0) {
                continue;
            }
        } else /* filterOp == FILTEROP_OR*/ { // Skip any games in the filter:
            if (filter.get(gameNum) != 0) {
                continue;
            }
            // OK, this game is NOT in the filter.
            // Add it so filterCounts are kept up to date:
            filter.set (gameNum, 1);
        }

        const IndexEntry* ie = db->getIndexEntry(gameNum);
        if (ie->GetLength() == 0) {  // Skip games with no gamefile record
            filter.set (gameNum, 0);
            continue;
        }

        if (ie->GetNumHalfMoves() < minMoves  &&  ! ie->GetStartFlag()) {
            // Skip games without enough moves to match, if they
            // have the standard starting position:
            filter.set (gameNum, 0);
            continue;
        }

        bool possibleMatch = true;
        bool possibleFlippedMatch = flip;

        // First, eliminate games that cannot match from their final
        // material signature:
        if (checkMsig  &&  !matsig_isReachable (msig, ie->GetFinalMatSig(),
                                                ie->GetPromotionsFlag(),
                                                ie->GetUnderPromoFlag()))
        {
            possibleMatch = false;
        }
        if (flip  &&  checkMsig
                &&  !matsig_isReachable (msigFlipped, ie->GetFinalMatSig(),
                                         ie->GetPromotionsFlag(),
                                         ie->GetUnderPromoFlag()))
        {
            possibleFlippedMatch = false;
        }

        // If the game has a final home pawn that cannot appear in the
        // patterns, exclude it. For example, a White IQP search has no
        // white c or e pawns, so any game that ends with a c2 or e2 pawn
        // at home need not be loaded:

        if (possibleMatch  &&  hpExcludeMask != HPSIG_Empty) {
            uint gameFinalHP = hpSig_Final (ie->GetHomePawnData());
            // If any bit is set in both, this game cannot match:
            if ((gameFinalHP & hpExcludeMask) != 0) {
                possibleMatch = false;
            }
        }
        if (possibleFlippedMatch  &&  hpExMaskFlip != HPSIG_Empty) {
            uint gameFinalHP = hpSig_Final (ie->GetHomePawnData());
            // If any bit is set in both, this game cannot match:
            if ((gameFinalHP & hpExMaskFlip) != 0) {
                possibleFlippedMatch = false;
            }
        }

        if (!possibleMatch  &&  !possibleFlippedMatch) {
            filter.set (gameNum, 0);
            continue;
        }

        // Now, the game must be loaded and searched:
        auto bbuf = db->getGame(*ie);
        if (!bbuf) {
            continue;
        }

        bool result = false;
        if (possibleMatch) {
            auto bbuf_clone = bbuf;
            bool hasPromo = ie->GetPromotionsFlag() || ie->GetUnderPromoFlag();
            result = g->MaterialMatch (hasPromo, bbuf_clone, min, max, patt,
                                       minPly, maxPly, matchLength,
                                       oppBishops, sameBishops,
                                       matDiff[0], matDiff[1]);
        }
        if (result == 0  &&  possibleFlippedMatch) {
            bool hasPromo = ie->GetPromotionsFlag() || ie->GetUnderPromoFlag();
            result = g->MaterialMatch (hasPromo, bbuf, minFlipped, maxFlipped,
                                       flippedPatt, minPly, maxPly,
                                       matchLength, oppBishops, sameBishops,
                                       matDiff[0], matDiff[1]);
        }

        if (result) {
            // update the filter value to the current ply:
            uint plyOfMatch = g->GetCurrentPly() + 1 - matchLength;
            byte b = (byte) (plyOfMatch + 1);
            if (b == 0) { b = 1; }
            filter.set (gameNum, b);
        } else {
            // This game did NOT match:
            filter.set (gameNum, 0);
        }
    }

    freePatternList (patt);
    freePatternList (flippedPatt);
    progress.report(1,1);

    int centisecs = timer.CentiSecs();

    if (gameNum != n) {
        Tcl_AppendResult (ti, errMsgSearchInterrupted(ti), "  ", NULL);
    }
    sprintf (temp, "%lu / %lu  (%d%c%02d s)",
             static_cast<unsigned long>(filter->size()),
             static_cast<unsigned long>(startFilterCount),
             centisecs / 100, decimalPointChar, centisecs % 100);
    Tcl_AppendResult (ti, temp, NULL);
    return TCL_OK;
}


const uint NUM_TITLES = 8;
enum {
    TITLE_GM, TITLE_IM, TITLE_FM,
    TITLE_WGM, TITLE_WIM, TITLE_WFM,
    TITLE_W, TITLE_NONE
};
const char * titleStr [NUM_TITLES] = {
    "gm", "im", "fm", "wgm", "wim", "wfm", "w", "none"
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parseTitles:
//    Called from sc_search_header to parse a list
//    of player titles to be searched for. The provided
//    string should have some subset of the elements
//    gm, im, fm, wgm, wim, w and none, each separated
//    by whitespace. Example: "gm wgm" would indicate
//    to only search for games by a GM or WIM.
bool *
parseTitles (const char * str)
{
    bool * titles = new bool [NUM_TITLES];

    for (uint t=0; t < NUM_TITLES; t++) { titles[t] = false; }

    str = strFirstWord (str);
    while (*str != 0) {
        for (uint i=0; i < NUM_TITLES; i++) {
            if (strIsCasePrefix (titleStr[i], str)) {
                titles[i] = true;
                break;
            }
        }
        str = strNextWord (str);
    }
    return titles;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_search_header:
//    Searches by header information.
int
sc_search_header (ClientData, Tcl_Interp * ti, scidBaseT* base, HFilter& filter, int argc, const char ** argv)
{
    ASSERT(argc >= 2);
    Progress progress = UI_CreateProgress(ti);
    errorT res = search_index(base, filter, argc -2, argv +2, progress);
    if (res != OK) return UI_Result(ti, res);

    //TODO: the old options that follows do not work with FILTEROP_OR
    //      at the moment there is no tcl code that use them with FILTEROP_OR

	std::string sAnnotator;

	bool bAnnotated = false;

	bool * wTitles = NULL;
    bool * bTitles = NULL;

    bool wToMove = true;
    bool bToMove = true;

    int pgnTextCount = 0;
    char ** sPgnText = NULL;

    const char * options[] = {
        "annotator", "annotated",
        "wtitles", "btitles", "toMove",
        "pgn", NULL
    };
    enum {
        OPT_ANNOTATOR, OPT_ANNOTATED,
        OPT_WTITLES, OPT_BTITLES, OPT_TOMOVE,
        OPT_PGN
    };

    int arg = 2;
    while (arg+1 < argc) {
        const char * option = argv[arg];
        const char * value = argv[arg+1];
        arg += 2;
        int index = -1;
        if (option[0] == '-') {
            index = strUniqueMatch (&(option[1]), options);
        }

        switch (index) {
		case OPT_ANNOTATOR:
			sAnnotator = value;
			break;

		case OPT_ANNOTATED:
			bAnnotated = strGetBoolean(value);
			break;

        case OPT_WTITLES:
            delete[] wTitles;
            wTitles = parseTitles (value);
            break;

        case OPT_BTITLES:
            delete[] bTitles;
            bTitles = parseTitles (value);
            break;

        case OPT_TOMOVE:
            wToMove = false;
            if (strFirstChar (value, 'w')  || strFirstChar (value, 'W')) {
                wToMove = true;
            }
            bToMove = false;
            if (strFirstChar (value, 'b')  || strFirstChar (value, 'B')) {
                bToMove = true;
            }
            break;

        case OPT_PGN:
            if (Tcl_SplitList (ti, (char *)value, &pgnTextCount,
                               (CONST84 char ***) &sPgnText) != TCL_OK) {
                delete[] wTitles;
                delete[] bTitles;
                return TCL_ERROR;
            }
            break;

        }
    }

    // Set up White name matches array:
    std::vector<bool> mWhite;
    if (wTitles != NULL  &&  spellChk != NULL) {
        bool allTitlesOn = true;
        for (uint t=0; t < NUM_TITLES; t++) {
            if (! wTitles[t]) { allTitlesOn = false; break; }
        }
        if (! allTitlesOn) {
            idNumberT i;
            idNumberT numNames = base->getNameBase()->GetNumNames(NAME_PLAYER);
            mWhite.resize(numNames, true);
            for (i=0; i < numNames; i++) {
                const char * name = base->getNameBase()->GetName (NAME_PLAYER, i);
                const PlayerInfo* pInfo = spellChk->getPlayerInfo(name);
                const char * title = (pInfo) ? pInfo->getTitle() : "";
                if ((!wTitles[TITLE_GM]  &&  strEqual(title, "gm"))
                    || (!wTitles[TITLE_GM]  &&  strEqual(title, "hgm"))
                    || (!wTitles[TITLE_IM]  &&  strEqual(title, "im"))
                    || (!wTitles[TITLE_FM]  &&  strEqual(title, "fm"))
                    || (!wTitles[TITLE_WGM]  &&  strEqual(title, "wgm"))
                    || (!wTitles[TITLE_WIM]  &&  strEqual(title, "wim"))
                    || (!wTitles[TITLE_WFM]  &&  strEqual(title, "wfm"))
                    || (!wTitles[TITLE_W]  &&  strEqual(title, "w"))
                    || (!wTitles[TITLE_NONE]  &&  strEqual(title, ""))
                    || (!wTitles[TITLE_NONE]  &&  strEqual(title, "cgm"))
                    || (!wTitles[TITLE_NONE]  &&  strEqual(title, "cim"))) {
                    mWhite[i] = false;
                }
            }
        }
    }

    // Set up Black name matches array:
    std::vector<bool> mBlack;
    if (bTitles != NULL  &&  spellChk != NULL) {
        bool allTitlesOn = true;
        for (uint t=0; t < NUM_TITLES; t++) {
            if (!bTitles[t]) { allTitlesOn = false; break; }
        }
        if (! allTitlesOn) {
            idNumberT i;
            idNumberT numNames = base->getNameBase()->GetNumNames(NAME_PLAYER);
            mBlack.resize(numNames, true);
            for (i=0; i < numNames; i++) {
                const char * name = base->getNameBase()->GetName (NAME_PLAYER, i);
                const PlayerInfo* pInfo = spellChk->getPlayerInfo(name);
                const char * title = (pInfo) ? pInfo->getTitle() : "";
                if ((!bTitles[TITLE_GM]  &&  strEqual(title, "gm"))
                    || (!bTitles[TITLE_GM]  &&  strEqual(title, "hgm"))
                    || (!bTitles[TITLE_IM]  &&  strEqual(title, "im"))
                    || (!bTitles[TITLE_FM]  &&  strEqual(title, "fm"))
                    || (!bTitles[TITLE_WGM]  &&  strEqual(title, "wgm"))
                    || (!bTitles[TITLE_WIM]  &&  strEqual(title, "wim"))
                    || (!bTitles[TITLE_WFM]  &&  strEqual(title, "wfm"))
                    || (!bTitles[TITLE_W]  &&  strEqual(title, "w"))
                    || (!bTitles[TITLE_NONE]  &&  strEqual(title, ""))
                    || (!bTitles[TITLE_NONE]  &&  strEqual(title, "cgm"))
                    || (!bTitles[TITLE_NONE]  &&  strEqual(title, "cim"))) {
                    mBlack[i] = false;
                }
            }
        }
    }

    bool skipSearch = false;
    if (sAnnotator.empty() && mWhite.empty() && mBlack.empty() &&
        bAnnotated == false && wToMove == true && bToMove == true &&
        pgnTextCount == 0) {
        skipSearch = true;
    }

    // Here is the loop that searches on each game:
    errorT result = OK;
    if (!skipSearch)
    for (uint i=0, n = base->numGames(); i < n; i++) {
        if ((i % 5000) == 0) {  // Update the percentage done bar:
            if (!progress.report(i,n)) {
                result = ERROR_UserCancel;
                break;
            }
        }
        // Skip any games not in the filter:
        if (filter.get(i) == 0) continue;

        const IndexEntry* ie = base->getIndexEntry(i);

		auto matchGameHeader = [&]() {
			//TODO: this trick does not work if there is a custom start position
			uint halfmoves = ie->GetNumHalfMoves();
			if ((halfmoves % 2) == 0) { // This game ends with White to move
				if (!wToMove) {
					return false;
				}
			} else { // This game ends with Black to move
				if (!bToMove) {
					return false;
				}
			}

			// Last, we check the players
			if (!mWhite.empty() && !mWhite[ie->GetWhite()]) {
				return false;
			}
			if (!mBlack.empty() && !mBlack[ie->GetBlack()]) {
				return false;
			}

			if (bAnnotated && !(ie->GetCommentsFlag() ||
				                ie->GetVariationsFlag() || ie->GetNagsFlag()))
				return false;

			if (!sAnnotator.empty() && !ie->GetCommentsFlag() &&
				!ie->GetVariationsFlag())
				return false;

			// If we reach here, this game matches all criteria.
			return true;
		};
		bool match = matchGameHeader();

        // Now try to match the comment text if applicable:
        // Note that it is not worth using a faster staring search
        // algorithm like Boyer-Moore or Knuth-Morris-Pratt since
        // profiling showed most that most of the time is spent
        // generating the PGN representation of each game.

		if (match && !sAnnotator.empty()) {
			match = false;
			base->getGame(*ie).decodeTags(
				[&](auto const& tag, auto const& value) {
					if (tag == "Annotator") {
						match = strAlphaContains(std::string(value).c_str(),
						                         sAnnotator.c_str());
					}
				});
		}

		if (match && pgnTextCount > 0) {
			if (base->getGame(*ie, *scratchGame) != OK) {
				match = false;
			} else {
				scratchGame->ResetPgnStyle();
				scratchGame->AddPgnStyle(PGN_STYLE_TAGS);
				scratchGame->AddPgnStyle(PGN_STYLE_COMMENTS);
				scratchGame->AddPgnStyle(PGN_STYLE_VARS);
				scratchGame->AddPgnStyle(PGN_STYLE_SYMBOLS);
				scratchGame->SetPgnFormat(PGN_FORMAT_Plain);
				const char* buf = scratchGame->WriteToPGN().first;
				for (int m = 0; m < pgnTextCount; m++) {
					if (match) {
						match = strContains(buf, sPgnText[m]);
					}
				}
			}
		}

        if (match) {
            filter.set (i, 1);
        } else {
            // This game did NOT match:
            filter.set (i, 0);
        }
    }
    if (wTitles != NULL) { delete[] wTitles; }
    if (bTitles != NULL) { delete[] bTitles; }
    Tcl_Free ((char *) sPgnText);

    progress.report(1,1);

    return UI_Result(ti, result);;
}


//////////////////////////////////////////////////////////////////////
//  VARIATION creation/deletion/navigation functions.

int
sc_var (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "count", "number", "create", "delete", "enter", "exit", "first",
        "level", "list", "moveInto", "promote", NULL
    };
    enum {
        VAR_COUNT, VAR_NUMBER, VAR_CREATE, VAR_DELETE, VAR_ENTER, VAR_EXIT, VAR_FIRST,
        VAR_LEVEL, VAR_LIST, VAR_MOVEINTO, VAR_PROMOTE
    };
    int index = -1;

    if (argc > 1) { index = strUniqueMatch (argv[1], options); }

    switch (index) {
    case VAR_COUNT:
        return setUintResult (ti, db->game->GetNumVariations());

    case VAR_NUMBER:
        return setUintResult (ti, db->game->GetVarNumber());

    case VAR_CREATE:
        if (! (db->game->AtVarStart()  &&  db->game->AtVarEnd())) {
            db->game->MoveForward();
            db->game->AddVariation();
            db->gameAltered = true;
        }
        break;

    case VAR_DELETE:
        return sc_var_delete (cd, ti, argc, argv);

    case VAR_ENTER:
        return sc_var_enter (cd, ti, argc, argv);

    case VAR_EXIT:
        db->game->MoveExitVariation();
        break;

    case VAR_FIRST:
        return sc_var_first (cd, ti, argc, argv);

    case VAR_LEVEL:
        return setUintResult (ti, db->game->GetVarLevel());

    case VAR_LIST:
        return sc_var_list (cd, ti, argc, argv);

    case VAR_MOVEINTO:
        return sc_var_enter (cd, ti, argc, argv);

    case VAR_PROMOTE:
        db->gameAltered = true;
        return UI_Result(ti, db->game->MainVariation ());

    default:
        return InvalidCommand (ti, "sc_var", options);
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_var_delete:
//    Deletes a specified variation.
int sc_var_delete(ClientData, Tcl_Interp* ti, int, const char**) {
	auto err = db->game->DeleteVariation();
	if (err != ERROR_NoVariation)
		db->gameAltered = true;
	return UI_Result(ti, err);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_var_first:
//    Promotes the specified variation of the current to be the
//    first in the list.
int sc_var_first(ClientData, Tcl_Interp* ti, int, const char**) {
	auto err = db->game->FirstVariation();
	if (err != ERROR_NoVariation)
		db->gameAltered = true;
	return UI_Result(ti, err);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_var_list:
//    Returns a Tcl list of the variations for the current move.
int
sc_var_list (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    bool uci = (argc > 2) && ! strCompare("UCI", argv[2]);
    uint varCount = db->game->GetNumVariations();
    char s[100];
    for (uint varNumber = 0; varNumber < varCount; varNumber++) {
        db->game->MoveIntoVariation (varNumber);
        if (uci) db->game->GetNextMoveUCI (s);
        else db->game->GetSAN (s);
        // if (s[0] == 0) { strCopy (s, "(empty)"); }
        Tcl_AppendElement (ti, s);
        db->game->MoveExitVariation ();
    }
    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_var_enter:
//    Moves into a specified variation.
int
sc_var_enter (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_var enter <number>");
    }

    uint varNumber = strGetUnsigned (argv[2]);
    if (varNumber >= db->game->GetNumVariations()) {
        return errorResult (ti, "No such variation!");
    }

    db->game->MoveIntoVariation (varNumber);
    // Should moving into a variation also automatically play
    // the first variation move? Maybe it should depend on
    // whether there is a comment before the first move.
    // Uncomment the following line to auto-play the first move:
    db->game->MoveForward();

    return TCL_OK;
}

//////////////////////////////////////////////////////////////////////
///  BOOK functions

int
sc_book (ClientData cd, Tcl_Interp * ti, int argc, const char ** argv)
{
    static const char * options [] = {
        "load", "close", "moves", "positions", "movesupdate", "update", NULL
    };
    enum {
        BOOK_LOAD,    BOOK_CLOSE, BOOK_MOVE, BOOK_POSITIONS, BOOK_MOVES_UPDATE, BOOK_UPDATE,
    };
    int index = -1;

    if (argc > 1) { index = strUniqueMatch (argv[1], options);}

    switch (index) {
    case BOOK_LOAD:
        return sc_book_load (cd, ti, argc, argv);

    case BOOK_CLOSE:
        return sc_book_close (cd, ti, argc, argv);

    case BOOK_MOVE:  
        return sc_book_moves (cd, ti, argc, argv);

    case BOOK_POSITIONS:  
        return sc_book_positions (cd, ti, argc, argv);

    case BOOK_UPDATE:  
        return sc_book_update (cd, ti, argc, argv);

    case BOOK_MOVES_UPDATE:  
        return sc_book_movesupdate (cd, ti, argc, argv);
        
    default:
        return InvalidCommand (ti, "sc_book", options);
    }

    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_book_load:
//    Opens and loads a .bin book (fruit format)
int
sc_book_load (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 4) {
        return errorResult (ti, "Usage: sc_book load bookfile slot");
    }
 
    uint slot = strGetUnsigned (argv[3]);

	 int bookstate = polyglot_open(argv[2], slot);

    if (bookstate == -1 ) {
			return errorResult (ti, "Unable to load book");
		}
    if (bookstate  >  0 ) {
		   // state == 1: book is read only
			return setIntResult (ti, bookstate);
	 }
    return TCL_OK;

//--//    if (polyglot_open(argv[2], slot) == -1 ) {
//--//			return errorResult (ti, "Unable to load book");
//--//		}
//--//    return TCL_OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_book_close:
//    Closes the previously loaded book
int
sc_book_close (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_book close slot");
    }
    uint slot = strGetUnsigned (argv[2]);
    if (polyglot_close(slot) == -1 ) {
			return errorResult (ti, "Error closing book");
		}
    return TCL_OK;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_book_moves:
//    returns a list of all moves contained in opened book and their probability in a TCL list
int
sc_book_moves (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
		char moves[200] = "";
		char boardStr[100];
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_book moves slot");
    }
    uint slot = strGetUnsigned (argv[2]);
		db->game->GetCurrentPos()->PrintFEN (boardStr, FEN_ALL_FIELDS);
		polyglot_moves(moves, (const char *) boardStr, slot);
    Tcl_AppendResult (ti, moves, NULL);
    return TCL_OK;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_positions:
//    returns a TCL list of moves to a position in the book
int
sc_book_positions (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
		char moves[200] = "";
		char boardStr[100];
    if (argc != 3) {
        return errorResult (ti, "Usage: sc_book positions slot");
    }
    uint slot = strGetUnsigned (argv[2]);
		db->game->GetCurrentPos()->PrintFEN (boardStr, FEN_ALL_FIELDS);
		polyglot_positions(moves, (const char *) boardStr, slot);
    Tcl_AppendResult (ti, moves, NULL);
    return TCL_OK;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_book_update:
//    updates the opened book with probability values
int
sc_book_update (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 4) {
        return errorResult (ti, "Usage: sc_book update <probs> slot");
    }
    uint slot = strGetUnsigned (argv[3]);
		scid_book_update( (char*) argv[2], slot );
    return TCL_OK;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sc_book_movesupdate:
//    updates the opened book with moves and probability values
int
sc_book_movesupdate (ClientData, Tcl_Interp * ti, int argc, const char ** argv)
{
    if (argc != 6) {
        return errorResult (ti, "Usage: sc_book movesupdate <moves> <probs> slot tempfile");
    }
    uint slot = strGetUnsigned (argv[4]);
    scid_book_movesupdate( (char*) argv[2], (char*) argv[3], slot, (char*) argv[5] );
    return TCL_OK;
}
//////////////////////////////////////////////////////////////////////
/// END of tkscid.cpp
//////////////////////////////////////////////////////////////////////
