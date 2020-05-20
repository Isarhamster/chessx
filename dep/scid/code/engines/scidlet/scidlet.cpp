//////////////////////////////////////////////////////////////////////
//
//  FILE:       scidlet.cpp
//              Scidlet, a WinBoard chess engine
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.4
//
//  Notice:     Copyright (c) 2002 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

// This program is a WinBoard-compatible wrapper for the simple
// chess engine built into Scid.

#include "../../src/engine.h"
#include "../../src/misc.h"
#include <algorithm>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef WIN32
#  include <unistd.h>
#endif

#ifdef WIN32
#  define WIN32_LEAN_AND_MEAN 1
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
#endif



bool strCaseEqual (const char * s1, const char * s2) {
    return (strCaseCompare (s1, s2) == 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// unfinishedCommand
//    Stores a command input line not yet processed.
//
static char unfinishedCommand[512] = {0};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// inputReady
//   Check if another line of input is available.
bool
inputReady (void)
{
    if (unfinishedCommand[0] != 0) { return true; }

#ifdef WIN32
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;

    if (!init) {
        init = 1;
        inh = GetStdHandle (STD_INPUT_HANDLE);
        pipe = !GetConsoleMode (inh, &dw);
        if (!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer (inh);
        }
    }
    if (pipe) {
        if (!PeekNamedPipe (inh, NULL, 0, NULL, &dw, NULL)) {
            return true;
        }
        return (dw != 0);
    }
    GetNumberOfConsoleInputEvents (inh, &dw);
    return (dw > 1);
#else
    fd_set fd;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fd);
    FD_SET(0, &fd);
    return (select(1, &fd, NULL, NULL, &tv) > 0);
#endif
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Output and logging:

static FILE * logFile = NULL;

bool
isLogging (void)
{
    return (logFile != NULL);
}

void
startLogging (const char * filename)
{
    if (! isLogging()) {
        logFile = fopen (filename, "w");
    }
}

void
stopLogging (void)
{
    if (isLogging()) { fclose (logFile); }
}

void
output (const char * format, ...)
{
    va_list ap;
    va_start (ap, format);
    vprintf (format, ap);
    if (logFile != NULL) {
        vfprintf (logFile, format, ap);
        fflush (logFile);
    }
    va_end (ap);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// timeControlT
//   Used to keep track of time control information.
//
struct timeControlT {
    uint movesPerControl;
    uint minutesPerControl;
    uint centiSecondsLeft;
    uint oppCentiSecondsLeft;
    uint incrementSeconds;
    bool fixedTimeControl;
};

enum stateT {
    waiting, thinking, pondering
};

struct modeT {
    bool force;       // Force mode, do not think or ponder.
    bool ponder;      // Ponder a reply during opponent move.
    bool analyze;     // Analyze mode, not implemented yet.
    stateT state;     // Current engine state.
    bool drawOffered; // Opponent has offered a draw since our last move.
    uint drawnScores; // Number of successive drawn scores.
};

struct ponderT {
    simpleMoveT move;   // Ponder move, best expected opponent move.
    simpleMoveT reply;  // Best move in reply to the ponder move.
    int score;          // Score for the reply to the ponder move.
    bool guessed;       // True if the opponent made the ponder move.
};

timeControlT tc;
modeT mode;
ponderT ponder;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// setSearchTime
//  Computes and sets the amount of time (in milliseconds) to spend on
//  the next move.
void
setSearchTime (Engine * engine)
{
    int msLeft = tc.centiSecondsLeft * 10;
    int msThisMove = msLeft;

    if (tc.fixedTimeControl) {
        msThisMove = msLeft;
    } else if (tc.movesPerControl > 0) {
        // "XX moves in YY minutes" time control:
        int moveNumber = engine->GetPosition()->GetFullMoveCount();
        int movesToMake = tc.movesPerControl
                        - ((moveNumber-1) % tc.movesPerControl);

        // Keep a bit of time spare:
        if (msLeft < 4000) {
            msLeft /= 2;     // Under 4 seconds left? Just use half of it.
        } else if (msLeft < 20000) {
            msLeft -= 2000;  // Under 20 seconds left? Keep two seconds spare.
        } else {
            msLeft -= 5000;  // Keep five seconds spare.
        }

        msThisMove = msLeft / movesToMake;

    } else if (tc.incrementSeconds > 0) {
        // "Whole game in YY minutes" time control (with increment):

        int msInc = tc.incrementSeconds * 1000;

        // Subtract 0.5s for a safety buffer, but assume at least 0.1s:
        msLeft -= 500;
        if (msLeft < 100) { msLeft = 100; }

        // If time remaining is greater than the increment, use the
        // whole increment plus 1/30th of the remaining time.
        if (msLeft > msInc) {
            msThisMove = msLeft / 30 + msInc;
        } else {
            // Time on clock less is than the increment, so just use
            // some of what is available to try and gain time:
            msThisMove = msLeft * 8 / 10;
        }
    } else {
        // No increment; just use 1/30th of remaining time.
        msThisMove = msLeft / 30;
    }

    // Set the recommended, minimum and maximum search times:
    int msMin = msThisMove / 4;
    int msMax = msThisMove * 4;
    // Make sure the maximum search time will not lose on time:
    int msMaxLimit = tc.centiSecondsLeft * 10;
    if (msMax + 100 > msMaxLimit) { msMax = msMaxLimit - 100; }
    if (tc.fixedTimeControl) { msMin = msThisMove; msMax = msThisMove; }
    engine->SetSearchTime (msMin, msThisMove, msMax);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Configuration options:
//
struct configT {
    const char * bookFileName;  // Opening book file name.
    const char * logFileName;   // Log file name.
    uint hashTableMB;     // Transposition table size in megabytes.
    uint pawnTableMB;     // Pawn hash table size in megabytes.
    bool pruning;         // Should the engine do futility pruning?
    bool resign;          // Should the engine consider resigning?
    bool draw;            // Should the engine offer and accept draws?
    bool log;             // Should the engine log output?
};

const configT defaultConfig = {
    "scidlet.sbk",  // bookFileName
    "scidlet.log",  // logFileName
    16,    // hashTableMB
    1,     // pawnTableMB
    false, // pruning flag
    true,  // resign flag
    true,  // draw flag
    false  // log flag
};

configT config = defaultConfig;

static const char * configFileName = "scidlet.ini";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readConfigLine
//   Processes a single command from the command line parameters
//   or the configuration file.
//   Returns true if the command was valid, or false on error.
bool
readConfigLine (const char * line, configT * config)
{
    // Split the line into an option and value with spare whitespace
    // stripped out:
    char temp[512];
    strCopy (temp, line);
    char * option = (char *) strTrimLeft (temp);
    char * value = (char *) strFirstChar(temp, '=');
    if (value == NULL) { return false; }
    *value = 0;
    value++;
    value = (char *) strTrimLeft (value);
    strTrimRight(option);
    strTrimRight(value);

    if (strCaseEqual (option, "bookfile")) {
        config->bookFileName = strDuplicate (value);
    } else if (strCaseEqual (option, "logfile")) {
        config->logFileName = strDuplicate (value);
    } else if (strCaseEqual (option, "hash")) {
        config->hashTableMB = strGetUnsigned(value);
    } else if (strCaseEqual (option, "pawnhash")) {
        config->pawnTableMB = strGetUnsigned(value);
    } else if (strCaseEqual (option, "pruning")) {
        config->pruning = strGetBoolean(value);
    } else if (strCaseEqual (option, "resign")) {
        config->resign = strGetBoolean(value);
    } else if (strCaseEqual (option, "draw")) {
        config->draw = strGetBoolean(value);
    } else if (strCaseEqual (option, "log")) {
        config->log = strGetBoolean(value);
    } else {
        return false;
    }
    return true;
}

bool
readConfigFile (configT * config) {
    FILE * fp = fopen (configFileName, "r");
    if (fp == NULL) { return false; }
    uint lineNum = 0;
    while (true) {
        char line[512];
        if (fgets (line, 512, fp)){}
        if (feof(fp)) { break; }
        lineNum++;
        const char * s = strTrimLeft(line);
        if (s[0] == '#'  ||  s[0] == '['  ||  s[0] == 0) { continue; }
        if (!readConfigLine (s, config)) {
            output ("# Bad line in %s: line %u: %s\n", configFileName,
                    lineNum, line);
        }
    }
    fclose (fp);
    return true;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Opening book globals and data structure:
//
struct bookEntryT {
    uint hash;        // Board hash value
    uint pawnhash;    // Pawn hash value
    uint freq;        // Frequency, with side to move in lowest bit.
    bookEntryT * next;
};

static const uint BOOK_SIZE = 65536;
static uint nBookEntries = 0;
static bookEntryT * book [BOOK_SIZE];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// compareBookEntries
//   Compares two opening book entries, for sorting by pawn hash value
//   to reduce the binary file size.
int
compareBookEntries (const void * v1, const void * v2)
{
    bookEntryT * b1 = (bookEntryT *) v1;
    bookEntryT * b2 = (bookEntryT *) v2;
    if (b1->pawnhash < b2->pawnhash) { return -1; }
    if (b1->pawnhash > b2->pawnhash) { return  1; }
    if (b1->hash < b2->hash) { return -1; }
    if (b1->hash > b2->hash) { return  1; }
    return (int)b1->freq - (int)b2->freq;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readCompactUint, writeCompactUint:
//   Read/write an unsigned int using a variable number
//   of bytes: 1 for 0-127, 2 for 128-16383, etc.

inline errorT
writeCompactUint (FILE * fp, uint value)
{
    ASSERT (fp != NULL);
    int result;
    while (true) {
        if (value < 128) {
            result = putc (value, fp);
            break;
        }
        putc ((value & 127) | 128, fp);
        value = value >> 7;
    }
    return (result == EOF ? ERROR_FileWrite : OK);
}

inline uint
readCompactUint (FILE * fp)
{
    ASSERT (fp != NULL);
    uint v = 0;
    uint bitIndex = 0;
    while (true) {
        uint b = (uint) getc(fp);
        v = v | ((b & 127) << bitIndex);
        if (! (b & 128)) { break; }
        bitIndex += 7;
    }
    return v;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeFourBytes(), readFourBytes()

inline errorT
writeFourBytes (FILE * fp, uint value)
{
    ASSERT(fp != NULL);
    int result;
    uint v = (value >> 24) & 255;   putc(v, fp);
    v = (value >> 16) & 255;        putc(v, fp);
    v = (value >>  8) & 255;        putc(v, fp);
    v = value & 255;                result = putc(v, fp);
    return (result == EOF ? ERROR_FileWrite : OK);
}

inline uint
readFourBytes (FILE * fp)
{
    ASSERT(fp != NULL);
    uint v = getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    v = v << 8;    v += (uint) getc(fp);
    return v;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeString(), readString():
//      Read/write fixed-length strings.
//      Lengths of zero bytes ARE allowed.
errorT
writeString (FILE * fp, const char * str, uint length)
{
    ASSERT (fp != NULL  &&  str != NULL);
    int result = 0;
    while (length > 0) {
        result = putc(*str, fp);
        str++;
        length--;
    }
    return (result == EOF ? ERROR_FileWrite : OK);
}

errorT
readString (FILE * fp, char * str, uint length)
{
    ASSERT (fp != NULL  &&  str != NULL);
    while (length > 0) {
        *str = getc(fp);
        str++;
        length--;
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Position::ReadLine():
//      Parse a sequence of moves separated by whitespace and
//      move numbers, e.g. "1.e4 e5 2.Nf3" or "e4 e5 Nf3".
//
errorT ReadLine(Position& pos, const char* s) {
	while (true) {
		while (!isalpha(static_cast<unsigned char>(*s)) && *s != 0) {
			s++;
		}
		if (*s == '\0')
			return OK;

		const char* begin = s;
		while (!isspace(static_cast<unsigned char>(*s)) && *s != '\0') {
			s++;
		}

		simpleMoveT sm;
		errorT err = pos.ParseMove(&sm, begin, s);
		if (err != OK)
			return err;

		pos.DoSimpleMove(&sm);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// makeBook
//   Creates Scidlet opening book (SBK) file from a text file.
//   Does not affect the current opening book data in memory.
//   The text file should have one position per line, in the
//   format "<frequency> <moves>", e.g. "1234  1.e4 c5 2.Nf3".
void
makeBook (const char * infile)
{
    uint nPositions = 0;
    const char * suffix = strFileSuffix (infile);
    if (suffix != NULL  &&  strCaseEqual (suffix, "sbk")) {
        output ("# The input file is already in \"sbk\" format.");
        return;
    }

    FILE * in = fopen (infile, "r");
    if (in == NULL) {
        output ("# Error opening file: %s\n", infile);
        return;
    }
    const uint maxPositions = 1024 * 1024;
    bookEntryT * positions = new bookEntryT [maxPositions];
    Position * pos = new Position();
    uint lineNumber = 0;

    while (true) {
        char line [512];
        if (fgets (line, 512, in)){}
        lineNumber++;
        if (feof(in)) { break; }
        pos->StdStart();
        const char * str = strTrimLeft(line);
        if (*str == '#'  ||  *str == 0) { continue; }
        uint freq = strGetUnsigned(str);
        if (freq < 1) { continue; }
        while (isdigit(*str)) { str++; }
        if (ReadLine(*pos, str) != OK) {
            output ("# Error parsing line %u: %s\n", lineNumber, line);
            continue;
        }
        bookEntryT * entry = &(positions[nPositions]);
        uint hash = pos->HashValue();
        if (pos->GetToMove() == BLACK) { hash = ~hash; }
        entry->hash = hash;
        entry->pawnhash = pos->PawnHashValue();
        entry->freq = freq;
        nPositions++;
    }
    fclose (in);
    output ("# Read %u positions from %s\n", nPositions, infile);
    delete pos;

    // Sort the positions for a more compact binary encoding:
    qsort (positions, nPositions, sizeof(bookEntryT), compareBookEntries);

    // Write the positions:
    char outfile [512];
    strCopy (outfile, infile);
    strTrimFileSuffix (outfile);
    strAppend (outfile, ".sbk");
    FILE * out = fopen (outfile, "wb");
    if (out == NULL) {
        output ("# Error opening file for writing: %s\n", outfile);
        return;
    }
    writeString (out, "SBK", 4);
    writeFourBytes (out, nPositions);

    uint prevPawnHash = 0;
    for (uint i=0; i < nPositions; i++) {
        bookEntryT * entry = &(positions[i]);
        uint pawnhash = entry->pawnhash - prevPawnHash;
        writeCompactUint (out, pawnhash);
        prevPawnHash = entry->pawnhash;
        writeFourBytes (out, entry->hash);
        writeCompactUint (out, entry->freq);
    }
    fclose (out);

    output ("# Wrote %u positions to %s\n", nPositions, outfile);
    delete[] positions;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readBook
//   Reads the opening book file.
uint
readBook (const char * filename)
{
    for (uint x=0; x < BOOK_SIZE; x++) { book[x] = NULL; }
    nBookEntries = 0;

    FILE * fp = fopen (filename, "rb");
    if (fp == NULL) { return 0; }

    // Check header for correct ID value:
    char header[4];
    readString (fp, header, 4);
    if (!strEqual (header, "SBK")) {
        output ("# Opening book file (%s) has a corrupt header.\n", filename);
        fclose (fp);
        return 0;
    }

    // Read the number of book entries and allocate them in one chunk:
    uint nEntries = readFourBytes (fp);
    bookEntryT * entries = new bookEntryT [nEntries];

    // Read each entry and add it to the hash table:
    uint prevPawnHash = 0;
    for (uint i = 0; i < nEntries; i++) {
        uint pawnhash = readCompactUint (fp);
        pawnhash += prevPawnHash;
        prevPawnHash = pawnhash;
        if (feof(fp)) {
            output ("# Opening book file (%s) has corrupt data.\n", filename);
            break;
        }
        uint hash = readFourBytes (fp);
        uint freq = readCompactUint (fp);

        // Insert this entry in the hash table:
        bookEntryT * bk = &(entries[i]);
        bk->hash = hash;
        bk->pawnhash = pawnhash;
        bk->freq = freq;
        uint slot = hash % BOOK_SIZE;
        bk->next = book[slot];
        book[slot] = bk;
        nBookEntries++;
    }
    fclose (fp);
    return nBookEntries;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// lookupBook
//   Searches the opening book for the provided position
//   and movelist. Returns an index number into the move
//   list if a book move was selected, or -1 if no book
//   move was found.
int
lookupBook (Position * pos, MoveList * mlist)
{
    if (nBookEntries == 0) { return -1; }

    // If past move 30, do not bother checking the opening book:
    if (pos->GetFullMoveCount() > 30) { return -1; }

    // Set up an array of frequencies per move, allocating
    // from the heap if the list is unusually long:
    uint sumfreq = 0;
    const uint maxMoves = 100;
    uint freqs [maxMoves];
    uint * freq = freqs;
    if (mlist->Size() > maxMoves) { freq = new uint [mlist->Size()]; }

    for (uint i=0; i < mlist->Size(); i++) {
        freq[i] = 0;
        pos->DoSimpleMove (mlist->Get(i));
        uint hash = pos->HashValue();
        if (pos->GetToMove() == BLACK) { hash = ~hash; }
        uint pawnhash = pos->PawnHashValue();
        pos->UndoSimpleMove (mlist->Get(i));
        bookEntryT * bk = book[hash % BOOK_SIZE];
        while (bk != NULL) {
            if (hash == bk->hash  &&  pawnhash == bk->pawnhash) {
                freq[i] = bk->freq;
                break;
            }
            bk = bk->next;
        }
        sumfreq += freq[i];
    }

    int bookmove = -1;
    if (sumfreq > 0) {
        // Select a move at random based on its frequency:
        uint r = rand() % sumfreq;
        sumfreq = 0;
        for (uint j=0; j < mlist->Size(); j++) {
            sumfreq += freq[j];
            if (sumfreq > r) { bookmove = j; break; }
        }
    }
    if (mlist->Size() > maxMoves) { delete[] freq; }
    return bookmove;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// readInput
//   Read a line of input and process it.
//   Returns true if the engine should make a reply.
bool
readInput (Engine * engine)
{
    // Print prompt if appropriate, but not to the log file.
    if (! engine->InXBoardMode()) { printf ("scidlet> "); }

    char newCommand [512];

    // Get the next command, checking if there is an unfinished one.
    if (unfinishedCommand[0] == 0) {
        if (fgets (newCommand, 512, stdin)){}
        strTrimRight (newCommand, "\n", 1);
        if (isLogging()) {
            fprintf (logFile, "scidlet> %s\n", newCommand);
        }
    } else {
        strCopy (newCommand, unfinishedCommand);
        unfinishedCommand[0] = 0;
    }

    // Split the input line into a command and parameters:
    char command [512];
    strCopy (command, strTrimLeft (newCommand));
    const char * parameters = NULL;
    char * firstSpace = (char *) strFirstChar (command, ' ');
    if (firstSpace == NULL) {
        parameters = "";
    } else {
        *firstSpace = 0;    // Terminate the command string.
        parameters = firstSpace + 1;
    }

    // Ignore empty command lines
    if (strEqual (command, "")) { return false; }

    // When pondering, there are a few commands we can process on the
    // fly but the others must be handled outside of the search.
    if (mode.state == pondering) {
        if (strEqual (command, ".")) {
            // Ignore
        } else if (strEqual (command, "draw")) {
            mode.drawOffered = true;
        } else if (strEqual (command, "hint")) {
            // Ignore
        } else if (strEqual (command, "otim")) {
            int centiSecs;
            if (sscanf (parameters, "%d", &centiSecs) == 1) {
                tc.oppCentiSecondsLeft = centiSecs;
            }
        } else if (strEqual (command, "ping")) {
            output ("pong %s\n", parameters);
        } else if (strEqual (command, "time")) {
            int centiSecs;
            if (sscanf (parameters, "%d", &centiSecs) == 1) {
                tc.centiSecondsLeft = centiSecs;
            }
        } else {
            // See if the pondered move was made and if so, convert
            // the pondering into a real search.
            Position * pos = engine->GetPosition();
            // Temporarily undo the ponder move to get the right
            // position for parsing this command as an opponent move:
            simpleMoveT * ponderMove = &(ponder.move);
            pos->UndoSimpleMove (ponderMove);
            simpleMoveT sm;
            errorT err = pos->ParseMove (&sm, newCommand);
            pos->DoSimpleMove (ponderMove);
            if (err == OK  &&  sm.from == ponder.move.from
                  &&  sm.to == ponder.move.to
                  &&  sm.promote == ponder.move.promote) {
                // The opponent made the ponder move.
                ponder.guessed = true;
                setSearchTime (engine);
                mode.state = thinking;
                return false;
            }

            // Stop pondering so we can process this command.
            strCopy (unfinishedCommand, newCommand);
            return true;
        }
        return false;
    }

    // We are not pondering but may be thinking. Most commands can
    // be processed while thinking, but a few require interruption
    // of the search.

    if (strEqual (command, "?")) {
        // Move now if thinking, otherwise ignore.
        if (mode.state == thinking) { return true; }
    } else if (strEqual (command, ".")) {
        // Ignore
    } else if (strEqual (command, "accepted")) {
        // Ignore protocol accepted/rejected
    } else if (strEqual (command, "bk")) {
        // XBoard protocol wants indented text terminated by a blank line:
        output ("   No book information.\n\n");
    } else if (strEqual (command, "computer")) {
        // Ignore
    } else if (strEqual (command, "draw")) {
        mode.drawOffered = true;
    } else if (strEqual (command, "easy")) {
        mode.ponder = false;
    } else if (strEqual (command, "force")) {
        mode.force = true;
    } else if (strEqual (command, "go")) {
        mode.force = false;
        return true;
    } else if (strEqual (command, "hard")) {
        mode.ponder = true;
    } else if (strEqual (command, "hint")) {
            // Ignore
    } else if (strEqual (command, "level")) {
        int nmoves, base, inc;
        if (sscanf (parameters, "%d%d%d", &nmoves, &base, &inc) == 3) {
            tc.movesPerControl = nmoves;
            tc.minutesPerControl = base;
            tc.centiSecondsLeft = base * 60 * 100;
            tc.incrementSeconds = inc;
            tc.fixedTimeControl = false;
        }
    } else if (strEqual (command, "makebook")) {
        if (*parameters == 0) {
            output ("Usage: makebook file.txt\n");  
        } else {
            makeBook (parameters);
        }
    } else if (strEqual (command, "modes")) {
        output ("# Force: %d\n", mode.force);
        output ("# Ponder: %d\n", mode.ponder);
    } else if (strEqual (command, "new")) {
        // If we were searching, quit before processing this command:
        if (mode.state == thinking) {
            strCopy (unfinishedCommand, newCommand);
            return true;
        }
        engine->SetPosition(NULL);
        mode.drawOffered = false;
        mode.drawnScores = 0;
    } else if (strEqual (command, "nopost")) {
        engine->SetPostMode(false);
    } else if (strEqual (command, "otim")) {
        int centiSecs;
        if (sscanf (parameters, "%d", &centiSecs) == 1) {
            tc.oppCentiSecondsLeft = centiSecs;
        }
    } else if (strEqual (command, "perft")) {
        // If we were searching, quit before processing this command:
        if (mode.state == thinking) {
            strCopy (unfinishedCommand, newCommand);
            return true;
        }
        uint depth;
        if (sscanf (parameters, "%u", &depth) == 1) {
            Timer timer;
            uint nmoves = engine->PerfTest (depth);
            uint t = timer.CentiSecs();
            output ("Moves: %u  Time: %u.%02u s\n", nmoves, t / 100, t % 100);
        } else {
            output ("Usage: perft <depth>\n");
        }
    } else if (strEqual (command, "ping")) {
        output ("pong %s\n", parameters);
    } else if (strEqual (command, "playother")) {
        mode.force = false;
    } else if (strEqual (command, "post")) {
        engine->SetPostMode(true);
    } else if (strEqual (command, "protover")) {
        output ("feature myname=\"Scidlet %s\"\n", SCID_VERSION_STRING);
        output ("feature san=1 analyze=0 time=1 draw=1\n");
        output ("feature ping=1 setboard=1 playother=1 sigint=0\n");
        output ("feature colors=0 done=1\n");
    } else if (strEqual (command, "quit")) {
        stopLogging();
        exit (0);
    } else if (strEqual (command, "random")) {
        // Ignore
    } else if (strEqual (command, "rejected")) {
        // Ignore protocol accepted/rejected
    } else if (strEqual (command, "remove")) {
        // If we were searching, quit before processing this command:
        if (mode.state == thinking) {
            strCopy (unfinishedCommand, newCommand);
            return true;
        }
        engine->RetractMove();
        engine->RetractMove();
    } else if (strEqual (command, "result")) {
        // Ignore result command
    } else if (strEqual (command, "sd")) {
        uint depth;
        if (sscanf (parameters, "%u", &depth) == 1) {
            engine->SetSearchDepth (depth);
        }
    } else if (strEqual (command, "setboard")) {
        if (mode.state == thinking) {
            strCopy (unfinishedCommand, newCommand);
            return true;
        }
        Position * tmpPos = new Position();
        errorT err = tmpPos->ReadFromFEN (parameters);
        if (err == OK) {
            engine->SetPosition (tmpPos);
        } else {
            if (engine->InXBoardMode()) {
                output ("tellusererror Illegal FEN position.\n");
            } else {
                output ("Illegal FEN position.\n");
            }
        }
        delete tmpPos;
    } else if (strEqual (command, "st")) {
        int seconds;
        if (sscanf (parameters, "%d", &seconds) == 1) {
            tc.centiSecondsLeft = seconds * 100;
            tc.fixedTimeControl = true;
        }
    } else if (strEqual (command, "time")) {
        int centiSecs;
        if (sscanf (parameters, "%d", &centiSecs) == 1) {
            tc.centiSecondsLeft = centiSecs;
        }
    } else if (strEqual (command, "undo")) {
        // If we were searching, quit before processing this command:
        if (mode.state == thinking) {
            strCopy (unfinishedCommand, newCommand);
            return true;
        }
        engine->RetractMove();
    } else if (strEqual (command, "xboard")) {
        engine->SetXBoardMode(true);
        // Print newline to clear the prompt line:
        puts ("");
    } else {
        // Parse a move or unknown command:
        Position * pos = engine->GetPosition();
        simpleMoveT sm;
        errorT err = pos->ParseMove (&sm, newCommand);
        if (err != OK) {
            output ("Error (unknown command): %s\n", newCommand);
            return false;
        }
        // It is a legal move. Do not accept it if the engine is thinking:
        if (mode.state == thinking) {
            output ("Error (move input while thinking): %s\n", newCommand);
            return false;
        }
        engine->PlayMove(&sm);
        return true;
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// callback
//   Called periodically from the chess engine to check
//   for user input. Returns true if the search should
//   terminate early.
bool
callback (Engine * engine, void *)
{
    if (! inputReady()) { return false; }
    return readInput (engine);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// selectPonderMove
//   Selects the best move for the opponent, which will be pondered
//   for a reply while the opponent is thinking.
bool
selectPonderMove (Engine * engine, simpleMoveT * selected)
{
    Position * pos = engine->GetPosition();

    // Do not ponder on the standard starting position:
    if (pos->IsStdStart()) { return false; }

    // Generate the list of opponent moves, make sure it is not empty:
    MoveList mlist;
    pos->GenerateMoves (&mlist);
    if (mlist.Size() == 0) { return false; }

    // Try to find the ponder move from the principal variation:
    bool pvMoveFound = false;
    principalVarT * pv = engine->GetPV();
    if (pv->length >= 2) {
        MoveList::iterator it =
            std::find(mlist.begin(), mlist.end(), cmpMove(pv->move[1]));
        if (it != mlist.end()) {
            std::rotate(mlist.begin(), it, it + 1);
            pvMoveFound = true;
        }
    }

    if (!pvMoveFound  &&  mlist.Size() > 1) {
       // Do a very short search to find the move to ponder:
       engine->SetCallbackFunction (NULL, NULL);      // Disable callback.
       engine->SetSearchTime (10);                    // Do a 10 ms search.
       engine->Think (&mlist);
       engine->SetCallbackFunction (callback, NULL);  // Re-enable callback.
    }

    // Find the first move in the list for which there is no opening
    // book reply, and select it as the move to ponder:
    for (uint i=0; i < mlist.Size(); i++) {
        simpleMoveT * sm = mlist.Get(i);
        pos->DoSimpleMove (sm);
        MoveList replyList;
        pos->GenerateMoves (&replyList);
        int index = lookupBook (pos, &replyList);
        pos->UndoSimpleMove (sm);
        if (index < 0) {
            *selected = *sm;
            return true;
        }
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// startPondering
//   Makes the specified move for the opponent, and ponders a reply
//   until interrupted by input.
void
startPondering (Engine * engine, simpleMoveT * ponderMove)
{
    Position * pos = engine->GetPosition();

    if (engine->InPostMode()) {
        char san[20];
        pos->MakeSANString (ponderMove, san, SAN_MATETEST);
        output ("# Pondering: %s\n", san);
    }

    ponder.guessed = false;
    mode.state = pondering;
    ponder.move = *ponderMove;
    engine->PlayMove (ponderMove);
    engine->SetSearchTime (1 << 30);
    MoveList mlist;
    engine->GetPosition()->GenerateMoves (&mlist);
    ponder.score = engine->Think (&mlist);
    ponder.reply = *(mlist.Get(0));
    engine->RetractMove ();
    mode.state = waiting;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// makeReply
//   Given a move list and score, prints and makes the best move
//   and checks for draws etc.
void
makeReply (Engine * engine, MoveList * mlist, int score)
{
    Position * pos = engine->GetPosition();

    if (mlist->Size() == 0) {
        // It must be stalemate or checkmate:
        if (score == 0) {
            output ("1/2-1/2 {Stalemate}\n");
        } else {
            if (pos->GetToMove() == WHITE) {
                output ("0-1 {Black mates}\n");
            } else {
                output ("1-0 {White mates}\n");
            }
        }
        return;
    }

    // Look for forced draws, unless there is only one move to make:
    if (mlist->Size() != 1) {
        if (config.draw  &&  mode.drawOffered) {
            // The opponent offered a draw. Accept it if we score
            // the position as exactly zero (drawn) and we are not
            // ahead on material, or if the score is bad for us
            // by half a pawn or so.
            int mscore = engine->ScoreMaterial();
            if (score < -50  ||  (score == 0  &&  mscore <= 0)) {
                output ("offer draw\n");
            }
        }

        // Should we offer a draw?
        if (score != 0) {
            mode.drawnScores = 0;
        } else {
            mode.drawnScores++;
            // Offer a draw every 4 moves when the last few scores
            // have all been zero (drawn), but only if the material
            // score is equal or worse for us, since the opponent
            // should prove they can draw if behind on material.
            if (config.draw  &&  mode.drawnScores >= 4) {
                int mscore = engine->ScoreMaterial();
                if (mscore <= 0) {
                    output ("offer draw\n");
                    mode.drawnScores = 0;
                }
            }
        }

        // If we are hopelessly lost and the opponent has at
        // least 20 seconds remaining, resign now.
        if (config.resign  &&  score < -800
              &&  tc.oppCentiSecondsLeft > 2000) {
            if (pos->GetToMove() == WHITE) {
                output ("0-1 {White resigns}\n");
            } else {
                output ("1-0 {Black resigns}\n");
            }
            engine->SetPosition(NULL);
            mode.drawOffered = false;
            mode.drawnScores = 0;
            return;
        }
    }

    // Print and play the best move:
    char san[20];
    pos->MakeSANString (mlist->Get(0), san, SAN_MATETEST);
    engine->PlayMove (mlist->Get(0));
    output ("move %s\n", san);
    pos = engine->GetPosition();
    if (pos->IsKingInMate()) {
        if (pos->GetToMove() == WHITE) {
            output ("0-1 {Black mates}\n");
        } else {
            output ("1-0 {White mates}\n");
        }
    }
    mode.drawOffered = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main
//   The main Scidlet routine.
int
main (int argc, char ** argv)
{
    srand(time(NULL));

    setbuf (stdin, NULL);
    setbuf (stdout, NULL);

    tc.movesPerControl = 0;
    tc.minutesPerControl = 0;
    tc.centiSecondsLeft = 4000;
    tc.oppCentiSecondsLeft = 4000;
    tc.incrementSeconds = 0;
    tc.fixedTimeControl = false;
    mode.force = false;
    mode.ponder = false;
    mode.analyze = false;
    mode.state = waiting;
    mode.drawOffered = false;

    Engine * engine = new Engine();
    engine->SetXBoardMode (false);
    engine->SetCallbackFunction (callback, NULL);

    // Read configuration file:
    bool configLoaded = readConfigFile(&config);

    // Parse command-line override arguments:
    for (int i=1; i < argc; i++) {
        const char * s = argv[i];
        while (s[0] == '/'  ||  s[0] == '-') { s++; }
        if (! readConfigLine (s, &config)) {
            output ("Unrecognised command-line parameter: %s\n", argv[i]);
        }
    }

    engine->SetHashTableKilobytes (config.hashTableMB * 1024);
    engine->SetPawnTableKilobytes (config.pawnTableMB * 1024);
    engine->SetPruning (config.pruning);
    if (config.log) {
        startLogging(config.logFileName);
        engine->SetLogFile (logFile);
    }

    output ("# Scidlet %s   %s\n", SCID_VERSION_STRING, SCID_WEBSITE);
    output ("# Configuration file (%s) was %s.\n", configFileName,
            configLoaded ? "loaded" : "not loaded");
    output ("# Log file (%s): %s.\n", config.logFileName,
            isLogging() ? "on" : "off");
    output ("# Futility pruning: %s.\n", config.pruning ? "on" : "off");
    // output ("# Resign flag: %s.   Draw flag: %s.\n",
    //         config.resign ? "on" : "off", config.draw ? "on" : "off");
    output ("# Main hash table size: %3u MB = %8u entries.\n",
            config.hashTableMB, engine->NumHashTableEntries());
    output ("# Pawn hash table size: %3u MB = %8u entries.\n",
            config.pawnTableMB, engine->NumPawnTableEntries());
    uint booksize = readBook (config.bookFileName);
    output ("# Opening book (%s): %u positions.\n", 
            config.bookFileName, booksize);

    while (true) {

        ponder.guessed = false;
        if (mode.ponder  &&  !mode.force  &&  !inputReady()) {
            // Ponder until interrupted by input:
            simpleMoveT ponderMove;
            if (selectPonderMove (engine, &ponderMove)) {
                startPondering (engine, &ponderMove);
            }
        }
        if (ponder.guessed) {
            engine->PlayMove (&ponder.move);
        } else {
            bool reply = readInput (engine);
            if (!reply) { continue; }
        }
        if (mode.force) { continue; }

        // Check for certain draws:
        if (engine->NoMatingMaterial()) {
            output ("1/2-1/2 {No mating material}\n");
            continue;
        }
        if (engine->FiftyMoveDraw()) {
            output ("1/2-1/2 {Draw by 50 move rule}\n");
            continue;
        }
        if (engine->RepeatedPosition() >= 3) {
            output ("1/2-1/2 {Draw by repetition}\n");
            continue;
        }

        char san[20];
        MoveList mlist;
        Position * pos = engine->GetPosition();
        pos->GenerateMoves (&mlist);
        int score = 0;

        if (ponder.guessed) {
            MoveList::iterator it =
                std::find(mlist.begin(), mlist.end(), cmpMove(ponder.reply));
            if (it != mlist.end()) std::rotate(mlist.begin(), it, it + 1);
            makeReply (engine, &mlist, ponder.score);
            continue;
        }

        if (mlist.Size() != 1) {
            int bookmove = lookupBook (pos, &mlist);
            if (bookmove >= 0) {
                pos->MakeSANString (mlist.Get(bookmove), san, SAN_MATETEST);
                engine->PlayMove (mlist.Get(bookmove));
                if (engine->InXBoardMode()  &&  engine->InPostMode()) {
                    output (" 0  0  0  0  %s\n", san);
                }
                output ("move %s\n", san);
                continue;
            }

            // Set the search time for this move:
            setSearchTime (engine);

            // Search for the best move to play:
            mode.state = thinking;
            score = engine->Think (&mlist);
            mode.state = waiting;
        }
        makeReply (engine, &mlist, score);
    }
    return 0;
}
