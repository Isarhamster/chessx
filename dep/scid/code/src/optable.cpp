//////////////////////////////////////////////////////////////////////
//
//  FILE:       optable.cpp
//              OpTable and OpLine class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2001-2003  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "optable.h"
#include "crosstab.h"
#include "dstring.h"
#include "pbook.h"
#include <algorithm>

uint
endgameTheme (matSigT msig)
{
    bool queens = MATSIG_HasQueens (msig);
    bool rooks = MATSIG_HasRooks (msig);
    bool minors = MATSIG_HasBishops (msig) || MATSIG_HasKnights (msig);
    uint idx = EGTHEME_P;

    if (queens) {
        if (rooks) {
            if (minors) { idx = EGTHEME_QRM; } else { idx = EGTHEME_QR; }
        } else {
            if (minors) { idx = EGTHEME_QM; } else { idx = EGTHEME_Q; }
        }
    } else {
        if (rooks) {
            if (minors) { idx = EGTHEME_RM; } else { idx = EGTHEME_R; }
        } else {
            if (minors) { idx = EGTHEME_M; } else { idx = EGTHEME_P; }
        }
    }
    return idx;
}

inline bool posHasIQP (Position * pos, colorT c)
{
    pieceT p = piece_Make (c, PAWN);
    return (pos->FyleCount (p,C_FYLE) == 0  &&  pos->FyleCount (p,E_FYLE) == 0
            && pos->FyleCount (p, D_FYLE) > 0);
}

inline bool posHasAdvancedPawn (Position * pos, colorT c)
{
    if (c == WHITE) {
        return (pos->RankCount (WP,RANK_5) > 0  ||
                pos->RankCount (WP,RANK_6) > 0  ||
                pos->RankCount (WP,RANK_7) > 0);
    }
    return (pos->RankCount (BP,RANK_4) > 0  ||
            pos->RankCount (BP,RANK_3) > 0  ||
            pos->RankCount (BP,RANK_2) > 0);
}

inline bool posHasKPawnStorm (Position * pos, colorT c)
{
    const pieceT* bd = pos->GetBoard();
    // A kingside pawn storm by White is defined to be a situation
    // where there is no longer any white pawn on h2, g2, h3 or g3,
    // but there is a white pawn on the g or h file.
    if (c == WHITE) {
        return (bd[G2] != WP && bd[H2] != WP && bd[G3] != WP && bd[H3] != WP
                && (pos->FyleCount (WP, G_FYLE) > 0 ||
                    pos->FyleCount (WP,H_FYLE) > 0));
    }
    return (bd[G7] != BP && bd[H7] != BP && bd[G6] != BP && bd[H6] != BP
            && (pos->FyleCount (BP, G_FYLE) > 0 ||
                pos->FyleCount (BP,H_FYLE) > 0));
}

inline bool posHasOpenFyle (Position * pos, fyleT f)
{
    return (pos->FyleCount (WP, f) == 0  &&  pos->FyleCount (BP, f) == 0);
}

void
OpLine::Init (void)
{
    GameNumber = 0;
    White = strDuplicate("");
    Black = strDuplicate("");
    Site = strDuplicate("");
    WhiteID = BlackID = 0;
    WhiteElo = BlackElo = 0;
    AvgElo = 0;
    Date = ZERO_DATE;
    Result = RESULT_None;
    Length = 0;
    StartPly = 0;
    NumMoves = 0;
    ShortGame = false;
    NoteNumber = NoteMoveNum = 0;
    for (uint i=0; i < OPLINE_MOVES; i++) {
        Move[i][0] = 0;
    }
    for (uint t=0; t < NUM_POSTHEMES; t++) { Theme[t] = 0; }
    EgTheme = NUM_EGTHEMES;
}

void
OpLine::Init (Game * g, const IndexEntry * ie, gamenumT gameNum,
              uint maxExtraMoves, uint maxThemeMoveNumber)
{
    White = strDuplicate (g->GetWhiteStr());
    Black = strDuplicate (g->GetBlackStr());
    Site = strDuplicate (g->GetSiteStr());

    WhiteID = ie->GetWhite();
    BlackID = ie->GetBlack();
    GameNumber = gameNum;

    Date = g->GetDate();
    Result = g->GetResult();
    NumMoves = (g->GetNumHalfMoves() + 1) / 2;
    EcoCode = g->GetEco();
    WhiteElo = g->GetWhiteElo();
    BlackElo = g->GetBlackElo();
    AvgElo = g->GetAverageElo();
    Length = 0;
    StartPly = g->GetCurrentPly();
    auto location = g->currentLocation();
    if (g->GetCurrentPos()->GetToMove() == BLACK) {
        g->MoveBackup();
    }
    NoteNumber = NoteMoveNum = 0;
    uint columnMoves = OPTABLE_COLUMNS * 2;
    uint maxLineMoves = (OPTABLE_COLUMNS + maxExtraMoves) * 2;
    if (maxLineMoves > OPLINE_MOVES) { maxLineMoves = OPLINE_MOVES; }
    EgTheme = endgameTheme (ie->GetFinalMatSig());

    // First read in just the moves that will appear in table columns:
    uint i = 0;
    ShortGame = false;
    while (i < columnMoves) {
        simpleMoveT * sm = g->GetCurrentMove();
        if (sm == NULL) {
            Move[i][0] = 0;
            ShortGame = true;
        } else {
            Length++;
            g->GetCurrentPos()->MakeSANString (sm, Move[i], SAN_CHECKTEST);
            strStrip (Move[i], '-');
            strStrip (Move[i], '=');
            g->MoveForward();
        }
        i++;
    }

    // Now read in all the extra note moves:
    while (i < maxLineMoves) {
        simpleMoveT * sm = g->GetCurrentMove();
        if (sm == NULL) {
            Move[i][0] = 0;
            ShortGame = true;
        } else {
            Length++;
            g->GetCurrentPos()->MakeSANString (sm, Move[i], SAN_CHECKTEST);
            strStrip (Move[i], '-');
            strStrip (Move[i], '=');
            g->MoveForward();
        }
        i++;
    }
    if (g->GetCurrentMove() == NULL) { ShortGame = true; }

    // Now set positional themes:
    uint maxThemePly = maxThemeMoveNumber * 2;
    for (i=0; i < NUM_POSTHEMES; i++) { Theme[i] = 0; }
    g->MoveToStart();
    for (i=0; i < maxThemePly; i++) {
        if (g->MoveForward() != OK) { break; }
        SetPositionalThemes (g->GetCurrentPos());
    }

    g->restoreLocation(location);
}


void
OpLine::Destroy (void)
{
    delete[] White;
    delete[] Black;
    delete[] Site;
}


void
OpLine::SetPositionalThemes (Position * pos)
{
    squareT wk = pos->GetKingSquare (WHITE);
    squareT bk = pos->GetKingSquare (BLACK);
    fyleT wkf = square_Fyle (wk);
    fyleT bkf = square_Fyle (bk);

    if ((wkf <= C_FYLE && bkf <= C_FYLE) || (wkf >= G_FYLE && bkf >= G_FYLE)) {
        Theme[POSTHEME_CastSame]++;
    }
    if ((wkf <= C_FYLE && bkf >= G_FYLE) || (wkf >= G_FYLE && bkf <= C_FYLE)) {
        Theme[POSTHEME_CastOpp]++;
    }
    if (pos->PieceCount(WQ) == 0  &&  pos->PieceCount(BQ) == 0) {
        Theme[POSTHEME_QueenSwap]++;
    }
    bool wBPair = (pos->PieceCount (WB) >= 2);
    bool bBPair = (pos->PieceCount (BB) >= 2);
    if ((wBPair && !bBPair)  ||  (!wBPair && bBPair)) {
        Theme[POSTHEME_OneBPair]++;
    }
    if (posHasKPawnStorm (pos, WHITE)  ||  posHasKPawnStorm (pos, BLACK)) {
        Theme[POSTHEME_Kstorm]++;
    }
    if (posHasIQP (pos, WHITE)) {
        Theme[POSTHEME_WIQP]++;
    }
    if (posHasIQP (pos, BLACK)) {
        Theme[POSTHEME_BIQP]++;
    }
    if (posHasAdvancedPawn (pos, WHITE)) {
        Theme[POSTHEME_WAdvPawn]++;
    }
    if (posHasAdvancedPawn (pos, BLACK)) {
        Theme[POSTHEME_BAdvPawn]++;
    }
    if (posHasOpenFyle (pos, C_FYLE)  ||  posHasOpenFyle (pos, D_FYLE)
          ||  posHasOpenFyle (pos, E_FYLE)) {
        Theme[POSTHEME_OpenFyle]++;
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpLine::Insert:
//    Inserts subline as a note of this line.
//    The final order of notes (linked by the Next field) will be:
//    (a) subline's notes, (b) subline, (c) this line's original notes.
void
OpLine::Insert (OpLine * subline)
{
    OpLine * subsub = subline->Next;
    subline->Next = Next;
    if (subsub != NULL) {
        OpLine * subtail = subsub;
        while (subtail->Next != NULL) {
            subtail = subtail->Next;
        }
        subtail->Next = subline;
        Next = subsub;
    } else {
        Next = subline;
    }
}

uint
OpLine::CommonLength (OpLine * line)
{
    uint length = 0;
    for (length=0; length < OPLINE_MOVES; length++) {
        if (! strEqual (Move[length], line->Move[length])) { break; }
        if (strEqual (Move[length], "")) { break; }
    }
    return length;
}

void
OpLine::PrintMove (DString * dstr, const char * move, uint format)
{
    char tempTrans[5000];

    if (format == OPTABLE_Compact) {
        strcpy(tempTrans, move);
        transPieces(tempTrans);
        char *ptr = tempTrans;
        char ch = *ptr; //*move;
        while (ch != 0) {
            if (ch != '+'  &&  ch != 'x') {
                dstr->AddChar (ch);
            }
            ptr++;//move++;
            ch = *ptr;//*move;
        }
        return;
    }

    if (format != OPTABLE_LaTeX) {
        strcpy(tempTrans, move);
        transPieces(tempTrans);
        dstr->Append(tempTrans);//(move);
        return;
    }

    // LaTeX format: K,Q,R,B,N are translated to {\K} etc.
    char ch = *move;
    while (ch != 0) {
        switch (ch) {
        case 'K': case 'Q': case 'R': case 'B': case 'N':
            dstr->Append ("{\\");
            dstr->AddChar (ch);
            dstr->AddChar ('}');
            break;
        default:
            dstr->AddChar (ch);
            break;
        }
        move++;
        ch = *move;
    }
}

void
OpLine::PrintNote (DString * dstr, uint movenum, uint start, uint format)
{
    bool wtm = true;
    const char * preFirstMove = "";
    const char * postFirstMove = "";
    if (format == OPTABLE_LaTeX) {
        preFirstMove = "\\textbf{";
        postFirstMove = "}";
    } else if (format == OPTABLE_HTML  ||  format == OPTABLE_CText) {
        preFirstMove = "<b>";
        postFirstMove = "</b>";
    }
    for (uint i=0; i < Length; i++) {
        if (i < start) {
            // do nothing
        } else {
            if (i == start) {
                dstr->Append (preFirstMove, movenum, wtm ? "." : "...");
            } else if (wtm) {
                dstr->Append (" ", movenum, ".");
            } else {
                dstr->Append (" ");
            }
            PrintMove (dstr, Move[i], format);
            if (i == start) {
                dstr->Append (postFirstMove);
            }
        }
        if (wtm) {
            wtm = false;
        } else {
            wtm = true;
            movenum++;
        }
    }
    dstr->Append (" ");
    PrintSummary (dstr, format, false, false);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpLine::PrintSummary():
//   Prints the summary for this line. If fullDate is true, the
//   full date is printed (not just the year). If nmoves is true
//   or the game continues after the last stored line move, the
//   number of moves in the game is printed after the result.
void
OpLine::PrintSummary (DString * dstr, uint format, bool fullDate, bool nmoves)
{
    if (format == OPTABLE_CText  &&  GameNumber > 0) {
        dstr->Append ("<g_", GameNumber, ">");
    }

    const char * resultStr = RESULT_STR[Result];
    if (format == OPTABLE_LaTeX) {
        switch (Result) {
            case RESULT_White: resultStr = "{\\win}"; break;
            case RESULT_Black: resultStr = "{\\loss}"; break;
            case RESULT_Draw:  resultStr = "{\\draw}"; break;
            default: resultStr = "*"; break;
        }
    }
    dstr->Append (resultStr);
    if (nmoves  ||  !ShortGame) {
        dstr->Append ("(", NumMoves, ")");
    }
    const char * preName = "";
    const char * postName  = "";
    if (format == OPTABLE_CText) {
        preName = "<darkblue>";
        postName = "</darkblue>";
    } else if (format == OPTABLE_HTML) {
        preName = "<span class=\"player\">";
        postName = "</span>";
    }

    dstr->Append (" ", preName);
    const char * s = White;
    while (*s != 0  &&  *s != ',') {
        if (format == OPTABLE_LaTeX) {
            if (*s == '_'  ||  *s == '$'  || *s == '%') {
                dstr->AddChar ('\\');
            }
        }
        dstr->AddChar (*s);
        s++;
    }
    dstr->Append (postName);

    const char * preElo = " ";
    const char * postElo = "";
    const char * sep = " - ";
    if (format == OPTABLE_LaTeX) {
        preElo = " \\emph{";  postElo = "}"; sep = " -- ";
    } else if (format == OPTABLE_HTML) {
        preElo = " <span class=\"elo\">";  postElo = "</span>";
        sep = " -- ";
    } else if (format == OPTABLE_CText) {
        preElo = " <green>"; postElo = "</green>";
    }

    if (WhiteElo > 0) { dstr->Append (preElo, WhiteElo, postElo); }
    dstr->Append (sep, preName);

    s = Black;
    while (*s != 0  &&  *s != ',') {
        if (format == OPTABLE_LaTeX) {
            switch (*s) {
            case '_':
            case '$':
            case '%':
            case '&':
            case '#':
                dstr->AddChar ('\\');
                break;
            }
        }
        dstr->AddChar (*s);
        s++;
    }
    dstr->Append (postName);
    if (BlackElo > 0) { dstr->Append (preElo, BlackElo, postElo); }
    dstr->Append (", ", Site, " ");
    if (fullDate) {
        char dateStr[16] = {};
        date_DecodeToString (Date, dateStr);
        // Remove any unknown date fields:
        auto s_end = dateStr + 16;
        auto s = std::find(dateStr + 4, s_end, '?');
        if (s != s_end) {
            s--;
            *s = 0;
        }
        dstr->Append (dateStr);
    } else {
        dstr->Append (date_GetYear (Date));
    }

    if (format == OPTABLE_CText  &&  GameNumber > 0) {
        dstr->Append ("</g>");
    }
}


////////////////////////////////////////////////////////////


void
OpTable::Init (const char * type, Game * g, PBook * ebook)
{
    Type = strDuplicate (type);
    TargetRows = OPTABLE_DEFAULT_ROWS;
    MaxTableLines = OPTABLE_MAX_TABLE_LINES;
    NumRows = 0;
    NumLines = NumTableLines = 0;
    SetMaxExtraMoves (1);
    FilterCount = TheoryCount = 0;
    NumMoveOrders = 0;
    Format = OPTABLE_Text;
    StartLength = 0;
    WTM = (g->GetCurrentPos()->GetToMove() == WHITE ? true : false);
    DecimalChar = '.';

    Results[RESULT_White] = Results[RESULT_Black] = 0;
    Results[RESULT_Draw] = Results[RESULT_None] = 0;
    TheoryResults[RESULT_White] = TheoryResults[RESULT_Black] = 0;
    TheoryResults[RESULT_Draw] = TheoryResults[RESULT_None] = 0;
    ExcludeMove[0] = 0;

    uint i;
    for (i=0; i < NUM_POSTHEMES; i++) { ThemeCount[i] = 0; }

    for (i=0; i < NUM_EGTHEMES; i++) {
        EndgameCount[OPTABLE_Line][i] = EndgameCount[OPTABLE_All][i] = 0;
    }

    // Generate the text for each move up to the current position:
    auto location = g->currentLocation();
    while (! g->AtStart()) {
        if (g->AtVarStart()) {
            g->MoveExitVariation();
            continue;
        }
        if (ebook != NULL && ECOstr_.empty()) {
            auto eco = ebook->findECOstr(g->GetCurrentPos());
            if (!eco.empty())
                ECOstr_.append(eco);
        }
        g->MoveBackup();
        simpleMoveT * sm = g->GetCurrentMove();
        if (sm == NULL) { break; }
        g->GetCurrentPos()->MakeSANString (sm, StartLine[StartLength],
                                           SAN_CHECKTEST);
        StartLength++;
        if (StartLength >= OPTABLE_MAX_STARTLINE) { break; }
    }
    g->restoreLocation(location);
    // Now the moves are in the StartLine[] array, in reverse order.
}


void
OpTable::Clear (void)
{
    uint i;
    for (i=0; i < NumLines; i++) {
        delete Line[i];
    }
    for (i=0; i < NumMoveOrders; i++) {
#ifdef WINCE
        my_Tcl_Free((char*) MoveOrder[i].moves);
#else
        delete MoveOrder[i].moves;
#endif
    }
    NumLines = NumTableLines = 0;
    NumMoveOrders = 0;
    FilterCount = TheoryCount = 0;
    Results[RESULT_White] = Results[RESULT_Black] = 0;
    Results[RESULT_Draw] = Results[RESULT_None] = 0;
    TheoryResults[RESULT_White] = TheoryResults[RESULT_Black] = 0;
    TheoryResults[RESULT_Draw] = TheoryResults[RESULT_None] = 0;
    ExcludeMove[0] = 0;
}

void
OpTable::ClearNotes (void)
{
    // Clear all notes:
    for (uint i=0; i < NumTableLines; i++) {
        Row[i] = Line[i];
        NLines[i] = 1;
        RowScore[i] = RESULT_SCORE[Line[i]->Result];
        Line[i]->Next = NULL;
        Line[i]->NoteMoveNum = 0;
        Line[i]->NoteNumber = 0;
    }
    NumNotes = 0;
}

uint
OpTable::FormatFromStr (const char * str)
{
    switch (str[0]) {
    case 'H':
    case 'h':
        return OPTABLE_HTML;
    case 'L':
    case 'l':
        return OPTABLE_LaTeX;
    case 'T':
    case 't':
        return OPTABLE_Text;
    case 'C':
    case 'c':
        return OPTABLE_CText;
    }
    return OPTABLE_Text;
}

void
OpTable::SetFormat (const char * str)
{
    Format = FormatFromStr (str);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::PercentScore():
//    Returns the percentage score for White across all
//    games that have been added to the database.
//    The integer value returned is the percentage * 10,
//    e.g. "573" for 57.3%
uint
OpTable::PercentScore (void)
{
    uint percent = Results[RESULT_White] * 2;
    percent += Results[RESULT_Draw];
    percent = percent * 500;
    uint total = Results[RESULT_White] + Results[RESULT_Draw]
        + Results[RESULT_Black];
    if (total > 0) { percent = percent / total; }
    return percent;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::TheoryPercent():
//    Returns the percentage score for White across all
//    games except those excluded from the theory table.
uint
OpTable::TheoryPercent (void) {
    uint percent = TheoryResults[RESULT_White] * 2;
    percent += TheoryResults[RESULT_Draw];
    percent = percent * 500;
    uint total = TheoryResults[RESULT_White] + TheoryResults[RESULT_Draw]
        + TheoryResults[RESULT_Black];
    if (total > 0) { percent = percent / total; }
    return percent;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::TheoryScore:
//   Returns the score for White (multiplied by 2 to be an
//   integer value) for all games except those excluded
//   from the theory table.
uint
OpTable::TheoryScore (void)
{
    return TheoryResults[RESULT_White] * 2 + TheoryResults[RESULT_Draw];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::PercentFreq():
//    Returns the frequency of a particular result as a percentage.
//    The integer value returned is the percentage * 10.
uint
OpTable::PercentFreq (resultT result)
{
    uint percent = Results[result] * 1000;
    uint total = Results[RESULT_White] + Results[RESULT_Draw] +
        Results[RESULT_Black] + Results[RESULT_None];
    if (total > 0) { percent = percent / total; }
    return percent;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::Add():
//    Adds the line to the table if possible. If the table is not
//    yet full, the line is added and no line is deleted. If the
//    table is full, either the specified line, or an existing
//    line in the table it replaces, will be deleted.
bool
OpTable::Add (OpLine * line)
{
    if (! WTM) {
        strCopy (line->Move[0], StartLine[0]);
        strStrip (line->Move[0], '-');
        strStrip (line->Move[0], '=');
    }
    for (uint theme=0; theme < NUM_POSTHEMES; theme++) {
        if (line->Theme[theme] >= POSTHEME_THRESHOLD) { ThemeCount[theme]++; }
    }
    Results[line->Result]++;
    FilterCount++;

    // Stop here if this line is excluded from the theory table:
    if (WTM  && strEqual (line->Move[0], ExcludeMove)) { return false; }
    if (!WTM  &&  strEqual (line->Move[1], ExcludeMove)) { return false; }

    TheoryCount++;
    TheoryResults[line->Result]++;
    if (NumLines < OPTABLE_MAX_LINES) {
        Line[NumLines] = line;
        NumLines++;
        if (NumTableLines < MaxTableLines) { NumTableLines++; }
        return true;
    }

    // The table is full, so if this line is to be added, it must
    // evict an existing line with a smaller average Elo rating.

    eloT evictElo = line->AvgElo;
    int evictIndex = -1;
    for (uint i=0; i < NumLines; i++) {
        eloT elo = Line[i]->AvgElo;
        if (elo < evictElo) {
            evictElo = elo;
            evictIndex = i;
        }
    }
    if (evictIndex < 0) {
        return false;
    }
    delete Line[evictIndex];
    Line[evictIndex] = line;
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int_sqrt(): Integer square root.
//    Used by OpTable::GuessNumRows().
static uint
int_sqrt (uint val)
{
    uint guess=0;
    uint bit;
    for (bit = 1 << 15; bit != 0; bit >>= 1) {
        guess ^= bit;
        /* Can set this bit without going over sqrt(val)? */
        if (guess * guess > val) {
            guess ^= bit;
        }
    }
    return guess;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::GuessNumRows():
//    Sets the target number of rows according to the
//    number of lines, hopefully choosing a nice value.
//    The formula is sqrt(NumTableLines*0.75) + 3.
void
OpTable::GuessNumRows (void)
{
    SetNumRows (int_sqrt((NumTableLines * 3) / 4) + 3);
}

#ifdef WINCE
void
OpTable::DumpLines (/*FILE * */ Tcl_Channel fp)
{
    MakeRows();
    DString * dstr = new DString;
    char buf[1024];
    for (uint i=0; i < NumRows; i++) {
        bool first = true;
        OpLine * line = Row[i];
        OpLine * prevLine = NULL;
        while (line != NULL) {
            dstr->Clear();
            if (first) {
                first = false;
                line->PrintNote (dstr, (StartLength + 2) / 2, 0, OPTABLE_Text);
                //fprintf (fp, "ROW %u[%u]: ", i+1, NLines[i]);
                sprintf (buf, "ROW %u[%u]: ", i+1, NLines[i]);
                my_Tcl_Write(fp,buf,strlen(buf));
            } else {
                //fprintf (fp, "   %u-NOTE: ", i+1);
                sprintf (buf, "   %u-NOTE: ", i+1);
                my_Tcl_Write(fp,buf,strlen(buf));
                line->PrintNote (dstr, (StartLength + 2) / 2,
                            line->CommonLength(prevLine), OPTABLE_Text);
            }
            //fprintf (fp, "%s\n", dstr->Data());
            sprintf (buf, "%s\n", dstr->Data());
            my_Tcl_Write(fp,buf,strlen(buf));
            prevLine = line;
            line = line->Next;
        }
    }
    delete dstr;
}

#else
void
OpTable::DumpLines (FILE * fp)
{
    MakeRows();
    DString * dstr = new DString;
    for (uint i=0; i < NumRows; i++) {
        bool first = true;
        OpLine * line = Row[i];
        OpLine * prevLine = NULL;
        while (line != NULL) {
            dstr->Clear();
            if (first) {
                first = false;
                line->PrintNote (dstr, (StartLength + 2) / 2, 0, OPTABLE_Text);
                fprintf (fp, "ROW %u[%u]: ", i+1, NLines[i]);
            } else {
                fprintf (fp, "   %u-NOTE: ", i+1);
                line->PrintNote (dstr, (StartLength + 2) / 2,
                            line->CommonLength(prevLine), OPTABLE_Text);
            }
            fprintf (fp, "%s\n", dstr->Data());
            prevLine = line;
            line = line->Next;
        }
    }
    delete dstr;
}
#endif

bool
OpTable::IsRowMergable (uint rownum)
{
    ASSERT (rownum > 0  &&  rownum < NumRows-1);
    uint prevLength = Row[rownum]->CommonLength (Row[rownum-1]);
    uint nextLength = Row[rownum]->CommonLength (Row[rownum+1]);
    return (nextLength > prevLength);
}

void
OpTable::MergeRow (uint rownum)
{
    ASSERT (rownum < NumRows-1);
    Row[rownum+1]->Insert (Row[rownum]);
    NLines[rownum+1] += NLines[rownum];
    RowScore[rownum+1] += RowScore[rownum];
    for (uint i=rownum; i < NumRows-1; i++) {
        NLines[i] = NLines[i+1];
        Row[i] = Row[i+1];
        RowScore[i] = RowScore[i+1];
    }
    NumRows--;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::SelectTableLines():
//    Select the NumTableLines highest-rated lines and
//    move them to the start of the array of lines.
void
OpTable::SelectTableLines (void)
{
    uint i, j;
    if (NumLines == NumTableLines) { return; }
    ASSERT (NumTableLines < NumLines);
    for (i=0; i < NumTableLines; i++) {
        uint bestIndex = i;
        uint bestAvgElo = Line[i]->AvgElo;
        for (j = i+1; j < NumLines; j++) {
            if (Line[j]->AvgElo >= bestAvgElo) {
                bestIndex = j;
                bestAvgElo = Line[j]->AvgElo;
            }
        }
        if (bestIndex != i) {
            OpLine * temp = Line[i];
            Line[i] = Line[bestIndex];
            Line[bestIndex] = temp;
        }
    }
}

void
OpTable::MakeRows (void) {
    uint i;

    if (NumLines == 0) { return; }

    // Select the NumTableLines highest-rated lines and
    // make them the first lines in the list:
    SelectTableLines ();
    // Sort the table lines:
    SortTableLines (Line, NumTableLines, 0);

    // Clear all notes:
    ClearNotes();

    NumRows = NumTableLines;
    if (TargetRows < OPTABLE_MIN_ROWS) { TargetRows = OPTABLE_MIN_ROWS; }
    if (TargetRows > OPTABLE_MAX_ROWS) { TargetRows = OPTABLE_MAX_ROWS; }

    // Need to merge all rows with CommonLength >= 2*OPTABLE_COLUMNS here,
    // and also merge all rows that are complete prefixes of other
    // rows (that is, where row N+1 contains all the moves of row N and
    // perhaps extra moves as well).

    for (i=0; i < NumRows-1;) {
        uint clength = Row[i]->CommonLength (Row[i+1]);
        if (clength >= 2*OPTABLE_COLUMNS) {
            MergeRow (i);
        } else if (clength == Row[i]->Length) {
            MergeRow (i);
        } else {
            i++;
        }
    }

    // While the number of rows left is greater than the requested number,
    // keep picking a row and merging it into its successor:

    while (NumRows > TargetRows) {
        // Find the best row to merge with the row below it.
        // The first row can always be merged, other rows might not be
        // elegible for merging. Given rows N-1, N and N+1, row N can
        // only be merged into N+1 if CommonLength(N,N+1) is greater
        // than CommonLength(N-1,N).

        uint bestcost = NLines[0] + NLines[1];
        uint bestrow = 0;
        for (i=1; i < NumRows-1; i++) {
            if (IsRowMergable (i)) {
                uint cost = NLines[i] + NLines[i+1];
                if (cost < bestcost) {
                    bestcost = cost;
                    bestrow = i;
                }
            }
        }
        MergeRow (bestrow);
    }

    // Now go through all note lines and find the move at which they
    // differ from the main row line:

    for (i=0; i < NumRows; i++) {
        OpLine * rowline = Row[i];
        OpLine * subline = Row[i]->Next;
        while (subline != NULL) {
            subline->NoteMoveNum = subline->CommonLength(rowline);
            subline = subline->Next;
        }
    }
}

// opSortT:
//    Structure used in OpTable::SortTableLines to keep track of
//    the frequency of a particular move, and the list of lines
//    that contain each unique move.
//
struct opSortT {
    const char * move;
    int count;
    OpLine * lineList;
};

void
OpTable::SortTableLines (OpLine ** lines, uint nlines, uint depth)
{
    uint i, j, nUnique = 0;
    if (nlines < 2) { return; }
    if (depth >= OPLINE_MOVES) { return; }

#ifdef WINCE
    opSortT * moves = (opSortT *) my_Tcl_Alloc(sizeof( opSortT [nlines]));
#else
    opSortT * moves = new opSortT [nlines];
#endif

    for (i=0; i < nlines; i++) {
        bool newMove = true;
        for (j=0; j < nUnique; j++) {
            if (strEqual (lines[i]->GetMove(depth), moves[j].move)) {
                newMove = false;
                moves[j].count++;
                lines[i]->Next = moves[j].lineList;
                moves[j].lineList = lines[i];
            }
        }
        if (newMove) {
            moves[nUnique].count = 1;
            moves[nUnique].move = lines[i]->GetMove(depth);
            lines[i]->Next = NULL;
            moves[nUnique].lineList = lines[i];
            nUnique++;
        }
    }

    // Now moves[] stores the number of times each unique next move appears.
    // Sort moves[]:
    for (i=0; i < nUnique-1; i++) {
        for (j=i+1; j < nUnique; j++) {
            int result = moves[i].count - moves[j].count;
            // If both moves have one line, sort by average elo:
            if (result == 0  &&  moves[i].count == 1) {
                result = moves[i].lineList->AvgElo - moves[j].lineList->AvgElo;
            }
            // If still equal, sort by move notation:
            if (result == 0) {
                result = strCompare (moves[i].move, moves[j].move);
            }
            // Ensure short lines (with no more moves) come first:
            if (strEqual (moves[i].move, "")) { result = -1; }
            if (strEqual (moves[j].move, "")) { result = 1; }
            // Swap if necessary:
            if (result > 0) {
                opSortT temp = moves[i];
                moves[i] = moves[j];
                moves[j] = temp;
            }
        }
    }

    // Now rearrange the lines according to the order of moves:
    uint count = 0;
    for (i=0; i < nUnique; i++) {
        OpLine * line = moves[i].lineList;
        while (line != NULL) {
            lines[count] = line;
            count++;
            line = line->Next;
        }
    }

    // Now recursively sort each block of unique moves:
    count = 0;
    for (i=0; i < nUnique; i++) {
        SortTableLines (lines, moves[i].count, depth+1);
        lines += moves[i].count;
    }

    // Delete the moves array:
#ifdef WINCE
    my_Tcl_Free((char*)moves);
#else
    delete[] moves;
#endif
}

void
OpTable::PrintStemLine (DString * dstr, uint format, bool exclude)
{
    for (uint i=0; i < StartLength; i++) {
        dstr->Append (" ");
        if (i % 2 == 0) { dstr->Append ((i+2)/2, "."); }
        OpLine::PrintMove (dstr, StartLine[StartLength-1-i], format);
    }
    if (exclude  &&  ExcludeMove[0] != 0) {
        dstr->Append (" (X ");
        dstr->Append ((StartLength+2)/2, StartLength % 2 == 0 ? "." : "...");
        OpLine::PrintMove (dstr, ExcludeMove, format);
        dstr->Append (")");
    }
}

void
OpTable::PrintTable (DString * dstr, const char * title, const char * comment)
{
    ASSERT (title != NULL  &&  comment != NULL);
    switch (Format) {
    case OPTABLE_LaTeX:
        PrintLaTeX (dstr, title, comment);
        break;
    case OPTABLE_HTML:
        PrintHTML (dstr, title, comment);
        break;
    case OPTABLE_Text:
        PrintText (dstr, title, comment, false);
        break;
    case OPTABLE_CText:
        PrintText (dstr, title, comment, true);
        break;
    }
}


void
OpTable::PrintLaTeX (DString * dstr, const char * title, const char * comment)
{
    uint i;
    MakeRows();
    NumNotes = 0;
    // Increasing arraystretch above 1.0 adds more whitespace between
    // rows making the table more readable:
    dstr->Append ("\\renewcommand{\\arraystretch}{1.15}\n");
    dstr->Append ("\\twocolumn[\n");
    dstr->Append (title);
    dstr->Append ("\\begin{center}\n");
    dstr->Append ("\\begin{tabular}{r*{", OPTABLE_COLUMNS);
    dstr->Append ("}{p{1.15cm}}r@{: }l}\n\\hline\n");
    dstr->Append ("\\multicolumn{11}{p{13cm}}{\\textbf{");
    PrintStemLine (dstr, OPTABLE_LaTeX, true);
    dstr->Append ("}: \\mbox{");
    dstr->Append (" +", TheoryResults[RESULT_White]);
    dstr->Append (" =", TheoryResults[RESULT_Draw]);
    dstr->Append (" --", TheoryResults[RESULT_Black]);
    uint score = TheoryScore();
    dstr->Append (" (", score/2);
    if (score % 2) { dstr->AddChar (DecimalChar); dstr->AddChar ('5'); }
    dstr->Append ("/", TheoryCount, ": ");
    dstr->Append ((TheoryPercent() + 5) / 10, "\\%)}");
    dstr->Append ("} \\\\\n\\hline\n");
    for (i=0; i < OPTABLE_COLUMNS; i++) {
        dstr->Append (" & ", i + ((StartLength + 2) / 2));
    }
    dstr->Append (" & \\multicolumn{2}{c}{} \\\\\n");
    //dstr->Append ("\\hline\n");

    // Print each row:
    for (uint row=0; row < NumRows; row++) {
        dstr->Append ("\\textbf{", row+1, "}");
        uint nSameMoves = 0;
        if (row > 0) { nSameMoves = Row[row]->CommonLength(Row[row-1]); }
        for (uint j=0; j < 2*OPTABLE_COLUMNS; j++) {
            //dstr->Append (j % 2 == 0 ? " & " : " \\newline ");
            if (j % 2 == 0) {
                dstr->Append (" & ");
            } else if (*(Row[row]->Move[j-1]) != '\0') {
                dstr->Append (" \\newline ");
            }
            if (j < nSameMoves) {
                dstr->Append ("...");
            } else {
                OpLine::PrintMove (dstr, Row[row]->Move[j], OPTABLE_LaTeX);
            }

            // Check for a footnote:
            if (HasNotes (Row[row], j)) {
                dstr->Append ("$^{", NumNotes, "}");
                // Compile following section to indicate number of games
                // and score in each note:
#if 0
                uint ncount = NoteCount(NumNotes);
                uint nscore = NoteScore(NumNotes);
                dstr->Append ("_{\\mbox{\\tiny ", ncount, ":");
                if (ncount == 1) {
                    switch (nscore) {
                    case 100: dstr->Append ("+"); break;
                    case  50: dstr->Append ("="); break;
                    case   0: dstr->Append ("--"); break;
                    }
                } else {
                    dstr->Append (nscore, "\\%");
                }
                dstr->Append ("}}");
#endif
                dstr->Append ("$");
            }
        }

        // Print number of games in this row, and White percentage score:
        dstr->Append (" & ", NLines[row], " & ");
        uint score = 0;
        if (NLines[row] > 0) {
            score = (RowScore[row] * 50 + (NLines[row]/2)) / NLines[row];
        }
        dstr->Append (score, "\\% \\\\\n");
    }

    dstr->Append ("\\hline\n");
    if (! strEqual (comment, "")) {
        dstr->Append ("\\multicolumn{11}{r}{\\em ", comment, "}\n");
    }
    dstr->Append ("\\end{tabular}\n\\end{center}\n]\n");
    PrintNotes (dstr, OPTABLE_LaTeX);
}

void
OpTable::PrintHTML (DString * dstr, const char * title, const char * comment)
{
    uint i;
    MakeRows();
    NumNotes = 0;
    dstr->Append (title);
    dstr->Append ("<center>");
    PrintStemLine (dstr, OPTABLE_HTML, true);
    dstr->Append (": ");
    dstr->Append (" +", TheoryResults[RESULT_White]);
    dstr->Append (" =", TheoryResults[RESULT_Draw]);
    dstr->Append (" -", TheoryResults[RESULT_Black]);
    uint score = TheoryScore();
    dstr->Append (" (", score/2);
    if (score % 2) { dstr->AddChar (DecimalChar); dstr->AddChar ('5'); }
    dstr->Append ("/", TheoryCount, ": ");
    dstr->Append ((TheoryPercent() + 5) / 10, "%)\n");
    dstr->Append ("<table border=0 cellspacing=0 cellpadding=4>\n");
    dstr->Append ("<tr><th></th>");
    for (i=0; i < OPTABLE_COLUMNS; i++) {
        dstr->Append ("<th align=\"left\">", i + ((StartLength + 2) / 2), "</th>");
    }
    dstr->Append ("</tr>\n");

    // Print each row:
    for (uint row=0; row < NumRows; row++) {
        dstr->Append ("<tr><th>", row+1, "</th>\n");
        uint nSameMoves = 0;
        if (row > 0) { nSameMoves = Row[row]->CommonLength(Row[row-1]); }
        for (uint j=0; j < 2*OPTABLE_COLUMNS; j++) {
            dstr->Append (j % 2 == 0 ? " <td> " : " <br> ");
            if (j < nSameMoves) {
                dstr->Append ("...");
            } else {
                OpLine::PrintMove (dstr, Row[row]->Move[j], OPTABLE_HTML);
            }

            // Check for a footnote:
            if (HasNotes (Row[row], j)) {
                dstr->Append ("<sup><a href=\"#note", NumNotes);
                dstr->Append ("\">", NumNotes, "</a></sup>");
            }
            if (j % 2 != 0) { dstr->Append (" </td>\n"); }
        }
        // Print number of games in this row, and White percentage score:
        dstr->Append (" <td>", NLines[row], ": ");
        uint score = 0;
        if (NLines[row] > 0) {
            score = (RowScore[row] * 50 + (NLines[row]/2)) / NLines[row];
        }
        dstr->Append (score, "% </td> ");
        dstr->Append ("</tr>\n");
    }
    dstr->Append ("</table>");
    if (! strEqual (comment, "")) { dstr->Append (comment, "\n"); }
    dstr->Append ("</center>");
    PrintNotes (dstr, OPTABLE_HTML);
}

void
OpTable::PrintText (DString * dstr, const char * title, const char * comment,
                    bool ctext)
{
    uint i;
    MakeRows();
    NumNotes = 0;
    const uint cellBytes = 9;
    char cell [18];
    const char * hrule = "-------------------------------------------------------------------------------\n";

    dstr->Append (title, "\n");
    if (ctext) { dstr->Append ("<tt>"); }
    dstr->Append (hrule, " ");
    if (StartLength > 0) {
        if (ctext) {
            dstr->Append ("<darkblue><run importMoveListTrans {");
            PrintStemLine (dstr, OPTABLE_Text, false);
            dstr->Append ("}>");
        }
        PrintStemLine (dstr, OPTABLE_Text, true);
        if (ctext) { dstr->Append ("</run></darkblue>"); }
        dstr->Append (":");
    }
    dstr->Append (" +", TheoryResults[RESULT_White]);
    dstr->Append (" =", TheoryResults[RESULT_Draw]);
    dstr->Append (" -", TheoryResults[RESULT_Black]);
    uint score = TheoryScore();
    dstr->Append (" (", score/2);
    if (score % 2) { dstr->AddChar (DecimalChar); dstr->AddChar ('5'); }
    dstr->Append ("/", TheoryCount, ": ");
    dstr->Append ((TheoryPercent() + 5) / 10, "%)\n");
    dstr->Append (hrule, "  ");
    for (i=0; i < OPTABLE_COLUMNS; i++) {
        sprintf (cell, " %3u     ", i + ((StartLength + 2) / 2));
        dstr->Append (cell);
    }
    dstr->Append ("\n", hrule);

    DString wStr, bStr, dTemp;
    DString* wstr = &wStr;
    DString* bstr = &bStr;
    DString* dtemp = &dTemp;

    // Print each row:
    for (uint row=0; row < NumRows; row++) {
        wstr->Clear();
        bstr->Clear();
        bool wtm = true;
        char tempStr [8];
        sprintf (tempStr, "%2u  ", row+1);
        wstr->Append (tempStr);
        bstr->Append ("    ");
        uint nSameMoves = 0;
        if (row > 0) { nSameMoves = Row[row]->CommonLength(Row[row-1]); }
        for (uint j=0; j < 2*OPTABLE_COLUMNS; j++) {
            size_t width = 0;
            dtemp->Clear();
            if (j < nSameMoves) {
                dtemp->Append ("...");
                width += 3;
            } else {
                if (ctext  &&  j >= nSameMoves) {
                    dtemp->Append ("<darkblue><run importMoveListTrans {");
                    PrintStemLine (dtemp, OPTABLE_Text, false);
                    uint x = 0;
                    if (! WTM) { x = 1; }
                    for (; x <= j; x++) {
                        dtemp->Append (" ");
                        OpLine::PrintMove (dtemp, Row[row]->Move[x],
                                           OPTABLE_Compact);
                    }
                    dtemp->Append ("}>");
                }
                size_t oldwidth = dtemp->Length();
                OpLine::PrintMove (dtemp, Row[row]->Move[j], OPTABLE_Compact);
                width += dtemp->Length() - oldwidth;
                if (ctext  &&  j >= nSameMoves) {
                    dtemp->Append ("</run></darkblue>");
                }
            }

            // Check for a footnote:
            if (HasNotes (Row[row], j)) {
                if (ctext) {
                    dtemp->Append ("<red><go n", NumNotes, ">");
                    dtemp->Append ("<N", NumNotes, ">");
                }
                size_t oldwidth = dtemp->Length();
                dtemp->Append ("[", NumNotes, "]");
                width += dtemp->Length() - oldwidth;
                if (ctext) { dtemp->Append ("</N", NumNotes, "></go></red>"); }
            }
            // Pad width to full cell width:
            while (width < cellBytes) {
                dtemp->AddChar (' ');
                width++;
            }

            if (wtm) {
                wtm = false;
                wstr->Append (dtemp->Data());
            } else {
                wtm = true;
                bstr->Append (dtemp->Data());
            }
        }

        // Print number of games in this row, and White percentage score:
        sprintf (cell, "%2u:", NLines[row]);
        wstr->Append (cell);
        uint score = 0;
        if (NLines[row] > 0) {
            score = (RowScore[row] * 50 + (NLines[row]/2)) / NLines[row];
        }
        sprintf (cell, "%2u%%", score);
        bstr->Append (cell);
        dstr->Append (wstr->Data(), "\n", bstr->Data(), "\n\n");
    }
    dstr->Append (hrule);
    if (! strEqual (comment, "")) { dstr->Append (comment, "\n"); }
    dstr->Append ("\n");
    if (ctext) { dstr->Append ("</tt>"); }
    PrintNotes (dstr, ctext ? OPTABLE_CText : OPTABLE_Text);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::HasNotes():
//    Determines if a particular move number of a main row line has
//    any footnotes. If it does, the NumNotes counter is incremented,
//    all footnote lines for the new footnote have their NoteMoveNum
//    set to the new footnote number, and true is returned.
bool
OpTable::HasNotes (OpLine * line, uint movenum)
{
    // Check for a footnote: there will be one if this is the last
    // move on the row that will appear in the table, or if there
    // is a subline that diverges at this move.

    OpLine * subline = line->Next;
    bool noteSeen = false;
    bool lastMoveInRow = false;
    if (movenum == line->Length-1) { lastMoveInRow = true; }
    if ((movenum == 2*OPTABLE_COLUMNS - 1) && (movenum < line->Length)) {
        lastMoveInRow = true;
    }
    if (lastMoveInRow) {
        noteSeen = true;
        NumNotes++;
        line->NoteMoveNum = movenum+1;
        line->NoteNumber = NumNotes;
    }
    while (subline != NULL) {
        if (subline->NoteMoveNum == movenum) {
            if (! noteSeen) {
                noteSeen = true;
                NumNotes++;
            }
            subline->NoteNumber = NumNotes;
        }
        if (subline->NoteNumber == 0  &&  lastMoveInRow) {
            subline->NoteNumber = NumNotes;
            subline->NoteMoveNum = movenum+1;
        }
        subline = subline->Next;
    }
    return noteSeen;
}

uint
OpTable::NoteCount (uint note)
{
    uint count = 0;
    for (uint n = 0; n < NumTableLines; n++) {
        if (Line[n]->NoteNumber == note) { count++; }
    }
    return count;
}

uint
OpTable::NoteScore (uint note)
{
    uint count = 0;
    uint score = 0;
    for (uint n = 0; n < NumTableLines; n++) {
        if (Line[n]->NoteNumber == note) {
            count++;
            score += RESULT_SCORE[Line[n]->Result];
        }
    }
    if (count > 0) {
        score = (score * 50 + (count/2)) / count;
    }
    return score;
}

void
OpTable::PrintNotes (DString * dstr, uint format)
{
    if (NumNotes == 0) { return; }

    const char * preNotesList = "\n";
    const char * postNotesList = "\n";
    const char * para = "\n     ";
    const char * endNote = "\n\n";
    const char * nextGame = ";\n        ";

    if (format == OPTABLE_LaTeX) {
        para = "\n\n";
        nextGame = ";\n";
    } else if (format == OPTABLE_HTML) {
        preNotesList = "\n\n<ol>\n";
        postNotesList = "</ol>\n";
        para = "<br>\n";  nextGame = ";\n";
        endNote = "</a><p>\n";
    } else if (format == OPTABLE_CText) {
        nextGame = ";  ";
        endNote = "</tab>\n\n";
        para = "\n       ";
    }
    dstr->Append (preNotesList);

    for (uint note=1; note <= NumNotes; note++) {
        if (format == OPTABLE_LaTeX) {
            dstr->Append ("\\notenum{", note, "}\n");
        } else if (format == OPTABLE_HTML) {
            //dstr->Append ("<li><b>", note, "</b> ");
            dstr->Append ("<li><a name=\"note", note, "\"></a> ");
        } else if (format == OPTABLE_CText) {
            dstr->Append ("<tab><red><go N", note, ">");
            dstr->Append ("<n", note, ">");
            dstr->Append ("[", note, "]");
            dstr->Append ("</n", note, "></go></red>  ");
        } else {
            dstr->Append ("[*", note, "*]  ");
        }
        OpLine *  prevLine =  NULL;
        for (int n = NumTableLines-1; n >= 0; n--) {
            if (Line[n]->NoteNumber == note) {
                uint mnum = Line[n]->NoteMoveNum;
                if (prevLine != NULL) {
                    mnum = Line[n]->CommonLength(prevLine);
                    if (mnum <= Line[n]->NoteMoveNum  &&
                        prevLine->Length > mnum) {
                        dstr->Append (".", para);
                    } else {
                        dstr->Append (nextGame);
                    }
                }
                Line[n]->PrintNote (dstr, (StartLength+2) / 2, mnum, format);
                prevLine = Line[n];
            }
        }
        dstr->Append (".", endNote);
    }
    dstr->Append (postNotesList);
}

void
OpTable::BestGames (DString * dstr, uint count, const char * rtype)
{
    enum {
        BEST_White, BEST_Black, BEST_AvgElo, BEST_Oldest, BEST_Newest
    };
    int rt = BEST_AvgElo;
    switch (rtype[0]) {
    case 'W':
    case 'w':  // Fastest White wins, ties resolved by AvgElo
        rt = BEST_White; break;
    case 'B':
    case 'b':  // Fastest Black wins, ties resolved by AvgElo
        rt = BEST_Black; break;
    case 'A':
    case 'a':  // Highest average rating
        rt = BEST_AvgElo; break;
    case 'O':
    case 'o':  // Oldest games
        rt = BEST_Oldest; break;
    case 'N':
    case 'n':  // Newest games
        rt = BEST_Newest; break;
    default:
        return;
    }

    const char * preNum = " ";
    const char * postNum = ":  ";
    const char * endLine = "\n";
    const char * preList = "";
    const char * postList = "";

    if (Format == OPTABLE_LaTeX) {
        preNum = "\\textbf{";
        postNum = ":}  ";
        endLine = "\\\\\n";
    } else if (Format == OPTABLE_HTML) {
        preNum = "";
        postNum = ":  ";
        endLine = "<br>\n";
    }

    uint i;
    for (i=0; i < NumLines; i++) {
        Line[i]->Selected = false;
    }

    dstr->Append (preList);
    bool printFullDate = (rt == BEST_Oldest || rt == BEST_Newest);

    for (uint c=1; c <= count; c++) {
        uint bestValue = 0;
        int bestIndex = -1;
        for (i=0; i < NumLines; i++) {
            if (Line[i]->Selected) { continue; }
            uint v = 0;
            switch (rt) {
            case BEST_White:
                if (Line[i]->Result != RESULT_White) { continue; }
                v = 1000 - Line[i]->NumMoves;
                v = (v << 12) + Line[i]->AvgElo;
                break;
            case BEST_Black:
                if (Line[i]->Result != RESULT_Black) { continue; }
                v = 1000 - Line[i]->NumMoves;
                v = (v << 12) + Line[i]->AvgElo;
                break;
            case BEST_AvgElo:
                v = Line[i]->AvgElo;
                break;
            case BEST_Oldest:
                v = DATE_MAKE(YEAR_MAX,12,31) - Line[i]->Date;
                if (Line[i]->Date == ZERO_DATE) { v = 0; }
                break;
            case BEST_Newest:
                v = Line[i]->Date;
                break;
            default:
                ASSERT(0);
                break;
            }
            if (v >= bestValue) {
                bestValue = v;
                bestIndex = i;
            }
        }
        if (bestIndex < 0) { break; }
        char tempStr [10];
        sprintf (tempStr, "%2u", c);
        dstr->Append (preNum, tempStr, postNum);
        Line[bestIndex]->PrintSummary (dstr, Format, printFullDate, true);
        if (Line[bestIndex]->NoteNumber != 0) {
            if (Format == OPTABLE_LaTeX) {
                dstr->Append (" $^{", Line[bestIndex]->NoteNumber, "}$");
            } else if (Format == OPTABLE_HTML) {
                dstr->Append (" [<a href=\"#note", Line[bestIndex]->NoteNumber);
                dstr->Append ("\">", Line[bestIndex]->NoteNumber, "</a>]");
            } else if (Format == OPTABLE_CText) {
                dstr->Append (" <red><go n", Line[bestIndex]->NoteNumber);
                dstr->Append (">[", Line[bestIndex]->NoteNumber,
                              "]</go></red>");
            } else {
                dstr->Append (" [", Line[bestIndex]->NoteNumber, "]");
            }
        }
        dstr->Append (endLine);
        Line[bestIndex]->Selected = true;
    }

    dstr->Append (postList);
}

// playerFreqT:
//    Used in OpTable::TopPlayers() to find the most
//    frequent players of a particular color.
//
#define PLAYERFREQ_MAXNOTES 8
struct playerFreqT {
    const char * name;
    uint frequency;
    eloT minElo;
    eloT maxElo;
    uint minYear;
    uint maxYear;
    uint score;
    uint oppEloSum;   // Sum of opponent Elos.
    uint oppEloCount; // Number of games where opponent has Elo rating.
    uint oppEloScore; // Score in games where opponent has Elo rating.
    uint noteNumber [PLAYERFREQ_MAXNOTES + 1];
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::TopPlayers:
//    Returns (through dstr) a list of the most frequent players
//    of the report line, as White or Black.
void
OpTable::TopPlayers (DString * dstr, colorT c, uint count)
{
    uint i;

    // Set up zero-filled array of player frequencies:
    uint largestPlayerID = 0;
    for (i=0; i < NumLines; i++) {
        uint id = (c == WHITE ? Line[i]->WhiteID : Line[i]->BlackID);
        if (id > largestPlayerID) { largestPlayerID = id; }
    }
#ifdef WINCE
    playerFreqT * pf = (playerFreqT *) my_Tcl_Alloc(sizeof(playerFreqT [largestPlayerID + 1]));
#else
    playerFreqT * pf = new playerFreqT [largestPlayerID + 1];
#endif
    for (i=0; i <= largestPlayerID; i++) {
        pf[i].name = NULL;
        pf[i].frequency = 0;
        pf[i].minElo = pf[i].maxElo = 0;
        pf[i].score = 0;
        pf[i].minYear = pf[i].maxYear = 0;
        pf[i].oppEloSum = pf[i].oppEloCount = pf[i].oppEloScore = 0;
        for (uint n=0; n <= PLAYERFREQ_MAXNOTES; n++) {
            pf[i].noteNumber[n] = 0;
        }
    }

    // Fill in the player frequencies array:
    for (i=0; i < NumLines; i++) {
        uint id = 0;
        eloT elo = 0;
        eloT oppElo = 0;
        const char * name = NULL;
        uint score = 0;
        uint year = date_GetYear (Line[i]->Date);
        OpLine * line = Line[i];
        if (c == WHITE) {
            id = line->WhiteID;
            elo = line->WhiteElo;
            oppElo = line->BlackElo;
            name = line->White;
            score = RESULT_SCORE[line->Result];
        } else {
            id = line->BlackID;
            elo = line->BlackElo;
            oppElo = line->WhiteElo;
            name = line->Black;
            score = RESULT_SCORE[ RESULT_OPPOSITE[line->Result] ];
        }
        ASSERT (id <= largestPlayerID);
        pf[id].frequency++;
        pf[id].name = name;
        pf[id].score += score;
        if (elo != 0) {
            if (pf[id].minElo == 0) { pf[id].minElo = elo; }
            if (elo < pf[id].minElo) { pf[id].minElo = elo; }
            if (elo > pf[id].maxElo) { pf[id].maxElo = elo; }
        }
        if (oppElo != 0) {
            pf[id].oppEloCount++;
            pf[id].oppEloSum += oppElo;
            pf[id].oppEloScore += score;
        }
        if (year != 0) {
            if (pf[id].minYear == 0) { pf[id].minYear = year; }
            if (year < pf[id].minYear) { pf[id].minYear = year; }
            if (year > pf[id].maxYear) { pf[id].maxYear = year; }
        }
        if (Line[i]->NoteNumber != 0) {
            for (uint n=0; n <= PLAYERFREQ_MAXNOTES; n++) {
                if (pf[id].noteNumber[n] == Line[i]->NoteNumber) { break; }
                if (pf[id].noteNumber[n] == 0) {
                    pf[id].noteNumber[n] = Line[i]->NoteNumber;
                    break;
                }
            }
        }
    }

    const char * preNum = " ";
    const char * postNum = ":";
    const char * preElo = "";
    const char * postElo = "";
    const char * inRange = "-";
    const char * percentStr = "%";
    const char * startTable = "";
    const char * endTable = "";
    const char * startRow = "";
    const char * endRow = "\n";
    const char * startName = "";
    const char * endName = "";
    const char * nextCell = " ";
    const char * startNotes = " [";
    const char * endNotes = "]";

    if (Format == OPTABLE_HTML) {
        startTable = "<table border=0 cellspacing=0 cellpadding=4>\n";
        endTable = "</table>\n";
        startRow = "<tr><td align=\"right\">";  endRow = "</td></tr>\n";
        nextCell = "</td><td>";
        // startTable = "<pre>\n"; endTable = "</pre>\n";
    }
    if (Format == OPTABLE_CText) {
        startRow = "<tt>"; startName = "</tt>";
        startNotes = " <red>["; endNotes = "]</red>";
    }
    if (Format == OPTABLE_LaTeX) {
        startTable = "\n\\begin{tabular}{rrrrrl}\n";
        endTable = "\\end{tabular}\n";
        startRow = "  ";      endRow = " \\\\ \n";
        nextCell = " & ";     percentStr = "\\%";
        preNum = "\\textbf{"; postNum = ":}";
        preElo = " \\emph{";  postElo = "}";  inRange = "--";
        startNotes = " $^{";  endNotes = "}$";
    }

    dstr->Append (startTable);

    // Now find the "count" most frequent players:
    for (uint n=1; n <= count; n++) {
        uint maxFreq = 0;
        uint maxElo = 0;
        int index = 0;
        bool found = false;
        for (uint id=0; id <= largestPlayerID; id++) {
            uint freq = pf[id].frequency;
            uint elo = pf[id].maxElo;
            if (freq == 0) { continue; }
            if ((freq > maxFreq)  ||  (freq == maxFreq  &&  elo > maxElo)) {
                found = true;
                index = id;
                maxFreq = freq;
                maxElo = elo;
            }
        }

        if (found) {
            char tempStr [100];
            sprintf (tempStr, "%2u", n);
            dstr->Append (startRow, preNum, tempStr, postNum);
            uint freq = pf[index].frequency;
            ASSERT (freq > 0);
            ASSERT (pf[index].name != NULL);
            sprintf (tempStr, "%3u", freq);
            dstr->Append (nextCell, tempStr);

            // Print the year range in which the player played this line:
            uint minYear = pf[index].minYear;
            uint maxYear = pf[index].maxYear;
            if (maxYear == 0) {
                strCopy (tempStr, "         ");
            } else if (minYear == maxYear) {
                sprintf (tempStr, "     %4u", minYear);
            } else {
                sprintf (tempStr, "%4u%s%4u",
                         minYear, inRange, maxYear);
            }
            dstr->Append (nextCell, " ", tempStr);

            // Print the score with this line:
            uint score = (50 * pf[index].score + (freq / 2)) / freq;
            sprintf (tempStr, "%3u%s", score, percentStr);
            dstr->Append (nextCell, tempStr);

            // Print peak Elo while playing this line:
            uint maxElo = pf[index].maxElo;
            if (maxElo == 0) {
                sprintf (tempStr, "%s    %s", preElo, postElo);
            } else {
                sprintf (tempStr, "%s%4u%s", preElo, maxElo, postElo);
            }
            dstr->Append (nextCell, " ", tempStr);
            dstr->Append (nextCell, " ", startName);
            if (Format == OPTABLE_CText) {
                dstr->Append ("<pi ", pf[index].name, ">");
            }
            dstr->Append (pf[index].name);
            if (Format == OPTABLE_CText) { dstr->Append ("</pi>"); }
            dstr->Append (endName);

            // Print the note numbers containing games by this player:
            if (pf[index].noteNumber[0] != 0) {
                dstr->Append (startNotes);
                for (uint n=0; n < PLAYERFREQ_MAXNOTES; n++) {
                    if (pf[index].noteNumber[n] == 0) { break; }
                    if (n > 0) { dstr->Append (","); }
                    if (Format == OPTABLE_CText) {
                        dstr->Append ("<go n", pf[index].noteNumber[n]);
                        dstr->Append (">", pf[index].noteNumber[n], "</go>");

                    } else {
                        dstr->Append (pf[index].noteNumber[n]);
                    }
                }
                if (pf[index].noteNumber[PLAYERFREQ_MAXNOTES] != 0) {
                    dstr->Append (",...");
                }
                dstr->Append (endNotes);
            }
            dstr->Append (endRow);
            pf[index].frequency = 0;
        }
    }
    dstr->Append (endTable);

    // Delete temporary player frequency data:
#ifdef WINCE
    my_Tcl_Free((char*)pf);
#else
    delete[] pf;
#endif
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::TopEcoCodes:
//    Returns (through dstr) a list of the most frequent ECO code
//    sections of the report games.
//    This is generally only useful for a Player report.
void
OpTable::TopEcoCodes (DString * dstr, uint count)
{
    uint ecoCount [50];
    uint ecoScore [50];
    uint ecoSubCount[50][10];

    for (uint ecoGroup=0; ecoGroup < 50; ecoGroup++) {
        ecoCount[ecoGroup] = 0;
        ecoScore[ecoGroup] = 0;
        for (uint subCode = 0; subCode < 10; subCode++) {
            ecoSubCount[ecoGroup][subCode] = 0;
        }
    }

    // Fill in the ECO frequencies array:
    for (uint i=0; i < NumLines; i++) {
        int ecoClass = -1;
        int ecoSubCode = -1;
        ecoT ecoCode = Line[i]->EcoCode;
        if (ecoCode != ECO_None) {
            ecoStringT ecoStr;
            eco_ToBasicString (ecoCode, ecoStr);
            if (ecoStr[0] != 0) {
                ecoClass = ((ecoStr[0] - 'A') * 10) + (ecoStr[1] - '0');
                if (ecoClass < 0  ||  ecoClass >= 50) { ecoClass = -1; }
                ecoSubCode = (ecoStr[2] - '0');
            }
        }
        if (ecoClass >= 0) {
            ecoCount[ecoClass]++;
            ecoScore[ecoClass] += RESULT_SCORE[Line[i]->Result];
            ecoSubCount[ecoClass][ecoSubCode]++;
        }
    }

    const char * preNum = " ";
    const char * postNum = ":";
    const char * inRange = "-";
    const char * percentStr = "%";
    const char * startTable = "";
    const char * endTable = "";
    const char * startRow = "";
    const char * endRow = "\n";
    const char * nextCell = " ";

    if (Format == OPTABLE_HTML) {
        startTable = "<table border=0 cellspacing=0 cellpadding=4>\n";
        endTable = "</table>\n";
        startRow = "<tr><td align=\"right\">";  endRow = "</td></tr>\n";
        nextCell = "</td><td align=\"right\">";
        // startTable = "<pre>\n"; endTable = "</pre>\n";
    }
    if (Format == OPTABLE_CText) {
        startTable = "<tt>"; endTable = "</tt>";
    }
    if (Format == OPTABLE_LaTeX) {
        startTable = "\n\\begin{tabular}{rlrr}\n";
        endTable = "\\end{tabular}\n";
        startRow = "  ";      endRow = " \\\\ \n";
        nextCell = " & ";     percentStr = "\\%";
        preNum = "\\textbf{"; postNum = ":}";
        inRange = "--";
    }

    dstr->Append (startTable);

    // Now find the "count" most frequent ECO groups:
    for (uint n=1; n <= count; n++) {
        uint maxFreq = 0;
        uint ecoClass = 0;
        for (uint i=0; i < 50; i++) {
            if (ecoCount[i] > maxFreq) {
                ecoClass = i;
                maxFreq = ecoCount[i];
            }
        }
        if (maxFreq > 0) {
            char ecoStr[8];
            strCopy (ecoStr, "A00-E99");
            ecoStr[0] = (ecoClass / 10) + 'A';
            ecoStr[1] = (ecoClass % 10) + '0';
            ecoStr[2] = '0';
            ecoStr[3] = 0;

            char tempStr [100];
            sprintf (tempStr, "%2u", n);
            dstr->Append (startRow, preNum, tempStr, postNum);
            dstr->Append (nextCell, ecoStr);
            ecoStr[2] = '9';
            dstr->Append (inRange, ecoStr);
            sprintf (tempStr, "%3u", maxFreq);
            dstr->Append (nextCell, tempStr);

            uint score = (50 * ecoScore[ecoClass] + (maxFreq / 2)) / maxFreq;
            sprintf (tempStr, "%3u%s", score, percentStr);
            dstr->Append (nextCell, tempStr);
            dstr->Append (endRow);
            ecoCount[ecoClass] = 0;
        }
    }
    dstr->Append (endTable);
}

uint
OpTable::AvgLength (resultT result)
{
    uint n = 0;
    uint sum = 0;
    for (uint i=0; i < NumLines; i++) {
        if (Line[i]->Result == result) {
            n++;
            sum += Line[i]->NumMoves;
        }
    }
    if (n == 0) { return 0; }
    return (sum / n);
}

uint
OpTable::AvgElo (colorT color, uint * count, uint * oppScore, uint * oppPerf)
{
    uint n = 0;
    uint sum = 0;
    uint score = 0;
    for (uint i=0; i < NumLines; i++) {
        eloT elo = (color == WHITE ? Line[i]->WhiteElo : Line[i]->BlackElo);
        if (elo > 0) {
            n++;
            sum += elo;
            resultT r = Line[i]->Result;
            if (color == WHITE) {
                r = RESULT_OPPOSITE[r];
            }
            if (r == RESULT_White) {
                score += 2;
            } else if (r == RESULT_Draw  ||  r == RESULT_None) {
                score++;
            }
        }
    }
    if (count != NULL) { *count = n; }
    if (n == 0) {
        if (oppScore != NULL) { *oppScore = 0; }
        if (oppPerf != NULL) { *oppPerf = 0; }
        return 0;
    }
    uint avgElo = (sum + (n/2)) / n;
    uint percent = ((score * 50) + (n/2)) / n;
    if (percent > 100) { percent = 100; }
    if (oppScore != NULL) { *oppScore = percent; }
    if (oppPerf != NULL) {
#ifndef WINCE
        *oppPerf = Crosstable::Performance (avgElo, percent);
#endif
    }
    return (avgElo);
}

uint
OpTable::AddMoveOrder (Game * g)
{
    uint id = 0;
    int index = -1;
    DString dstr;
    g->GetPartialMoveList (&dstr, g->GetCurrentPly());

    // Search for this move order in the current list:

    for (uint i=0; i < NumMoveOrders; i++) {
        if (strEqual (dstr.Data(), MoveOrder[i].moves)) {
            index = i;
            MoveOrder[i].count++;
            id = MoveOrder[i].id;
            break;
        }
    }

    // Add as a new move order if it was not found:

    if (index < 0) {
        if (NumMoveOrders == OPTABLE_MAX_LINES) { return 0; }
        MoveOrder[NumMoveOrders].count = 1;
        MoveOrder[NumMoveOrders].moves = strDuplicate (dstr.Data());
        MoveOrder[NumMoveOrders].id = NumMoveOrders + 1;
        id = MoveOrder[NumMoveOrders].id;
        index = NumMoveOrders;
        NumMoveOrders++;
    }

    // Keep the array in sorted order, to avoid needing to sort it later:
    // The list is sorted by count (highest first), with tied counts
    // sorted alphabetically by the move string.

    while (1) {
        ASSERT (index >= 0);
        if (index <= 0) { break; }
        if (MoveOrder[index].count < MoveOrder[index-1].count) { break; }
        if (MoveOrder[index].count == MoveOrder[index-1].count  &&
            strCompare(MoveOrder[index].moves, MoveOrder[index-1].moves) > 0) {
            break;
        }
        // Move the record for the current move order up one slot by
        // swapping it with the record before it:
        char * tempMoves = MoveOrder[index].moves;
        MoveOrder[index].moves = MoveOrder[index-1].moves;
        MoveOrder[index-1].moves = tempMoves;
        uint tempCount = MoveOrder[index].count;
        MoveOrder[index].count = MoveOrder[index-1].count;
        MoveOrder[index-1].count = tempCount;
        uint tempID = MoveOrder[index].id;
        MoveOrder[index].id = MoveOrder[index-1].id;
        MoveOrder[index-1].id = tempID;
        index--;
    }

    return id;
}

void
OpTable::PopularMoveOrders (DString * dstr, uint count)
{

    const char * preNum = " ";
    const char * postNum = ":  ";
    const char * endLine = "\n";
    const char * preList = "";
    const char * postList = "";
    const char * preCount = " (";
    const char * postCount = ")";

    if (Format == OPTABLE_LaTeX) {
        preNum = "\\textbf{"; postNum = ":}  ";
        endLine = "\\\\\n";
        preCount = " \\textbf{("; postCount = ")}";
    } else if (Format == OPTABLE_HTML) {
        preNum = ""; postNum = ":  ";
        endLine = "<br>\n";
    }

    if (count == 0) {
        // Just return number of move orders counted, as a string:
        dstr->Append (NumMoveOrders);
        return;
    }

    dstr->Append (preList);

    for (uint i=0; i < count; i++) {
        if (i == NumMoveOrders) { break; }
        char tempStr [16];
        sprintf (tempStr, "%2u", i+1);
        dstr->Append (preNum, tempStr, postNum);
        if (Format == OPTABLE_CText) {
            dstr->Append ("<tab><darkblue>");
            dstr->Append ("<run sc_report ", Type, " select mo ");
            dstr->Append (MoveOrder[i].id, "; ::windows::stats::Refresh>");
        }
        OpLine::PrintMove (dstr, MoveOrder[i].moves, Format);
        dstr->Append (preCount, MoveOrder[i].count, postCount);
        if (Format == OPTABLE_CText) {
            dstr->Append ("</run></darkblue></tab>");
        }
        dstr->Append (endLine);
    }
    dstr->Append (postList);
}

void
OpTable::ThemeReport (DString * dstr, uint argc, const char ** argv)
{
    const char * endLine = "\n";
    const char * percentStr = "%";
    const char * startTable = "";
    const char * endTable = "";
    const char * startRow = "  ";
    const char * endRow = "\n";
    const char * nextCell = " ";
    const char * nextCellRight = " ";

    if (argc != (NUM_POSTHEMES + 1)) { return; }

    if (Format == OPTABLE_HTML) {
        endLine = "<br>\n";
        startTable = "<table border=0 cellspacing=0 cellpadding=4>\n";
        endTable = "</table>\n";
        startRow = "<tr><td>";  endRow = "</td></tr>\n";
        nextCell = "</td><td>";  nextCellRight = "</td><td align=\"right\">";
        //startTable = "<pre>\n"; endTable = "</pre>\n";
    }
    if (Format == OPTABLE_CText) {
        startTable = "<tt>"; endTable = "</tt>";
    }
    if (Format == OPTABLE_LaTeX) {
        percentStr = "\\%";
        startTable = "\n\\begin{tabular}{lrlr}\n";
        endTable = "\\end{tabular}\n";
        startRow = "";  endRow = " \\\\ \n";
        nextCell = " & ";  nextCellRight = nextCell;
    }

    // const char * themeName [NUM_POSTHEMES] = {NULL};
    // themeName [POSTHEME_CastSame]  = "Same-side castling:         ";
    // themeName [POSTHEME_CastOpp]   = "Opposite castling:          ";
    // themeName [POSTHEME_QueenSwap] = "Queens exchanged:           ";
    // themeName [POSTHEME_OneBPair]  = "Only 1 side has Bishop pair:";
    // themeName [POSTHEME_Kstorm]    = "Kingside pawn storm:        ";
    // themeName [POSTHEME_WIQP]      = "White Isolated Queen Pawn:  ";
    // themeName [POSTHEME_BIQP]      = "Black Isolated Queen Pawn:  ";
    // themeName [POSTHEME_WAdvPawn]  = "White Pawn on 5/6/7th rank: ";
    // themeName [POSTHEME_BAdvPawn]  = "Black Pawn on 2/3/4th rank: ";
    // themeName [POSTHEME_OpenFyle]  = "Open c/d/e file:            ";

    char tempStr [250];
    //sprintf (tempStr, argv[0], (StartLength + (OPTABLE_COLUMNS * 2)) / 2);
    sprintf (tempStr, argv[0], MaxThemeMoveNumber);
    dstr->Append (tempStr, endLine);
    argc--;
    argv++;

    dstr->Append (startTable);
    uint leftcol = (NUM_POSTHEMES + 1) / 2;
    uint longestLength = 0;
    for (uint i=0; i < NUM_POSTHEMES; i++) {
        uint len = strLength (argv[i]);
        if (len > longestLength) { longestLength = len; }
    }

    uint theme = 0;
    while (true) {
        dstr->Append (theme < leftcol ? startRow : nextCell);
        strPad (tempStr, argv[theme], longestLength, ' ');
        if (Format == OPTABLE_CText) {
            dstr->Append ("<darkblue><run sc_report ", Type, " select theme ");
            dstr->Append (theme, "; ::windows::stats::Refresh>");
        }
        dstr->Append (" ", tempStr);
        if (Format == OPTABLE_CText) { dstr->Append ("</run></darkblue>"); }
        dstr->Append (nextCellRight);
        uint percent = 0;
        if (FilterCount > 0) {
            percent = ((100 * ThemeCount[theme]) + (FilterCount/2)) / FilterCount;
        }
        char rstr [16];
        sprintf (rstr, "%3u", percent);
        dstr->Append (rstr, percentStr);
        if (theme < leftcol) {
            dstr->Append ("  ");
        } else {
            dstr->Append (endRow);
        }
        if (theme == NUM_POSTHEMES - 1) { break; }
        if (theme < leftcol) {
            theme += leftcol;
        } else {
            theme -= leftcol;
            theme++;
        }
    }
    if (NUM_POSTHEMES % 2 == 1) { dstr->Append (endRow); }
    dstr->Append (endTable);
}

void
OpTable::AddEndMaterial (matSigT msig, bool inFilter)
{
    uint idx = endgameTheme (msig);
    EndgameCount [OPTABLE_All][idx]++;
    if (inFilter) {
        EndgameCount [OPTABLE_Line][idx]++;
    }
}

void
OpTable::EndMaterialReport (DString * dstr, const char * repGames,
                            const char * allGames)
{
    const char * startTable = "";
    const char * endTable = "";
    const char * startRow = "";
    const char * endRow = "\n";
    const char * nextCell = " ";
    const char * percentStr = "%";
    const char * preNum = "";
    const char * postNum = "";

    if (Format == OPTABLE_HTML) {
        startTable = "<table border=0 cellspacing=0 cellpadding=4>\n";
        endTable = "</table>\n";
        startRow = "<tr><td>";  endRow = "</td></tr>\n";
        nextCell = "</td><td align=\"right\">";
        // startTable = "<pre>\n"; endTable = "</pre>\n";
    }
    if (Format == OPTABLE_CText) {
        startTable = "<tt>"; endTable = "</tt>";
    }
    if (Format == OPTABLE_LaTeX) {
        startTable = "\n\\begin{tabular}{l*{8}{p{0.8cm}}}\n\\hline\n";
        endTable = "\\hline\n\\end{tabular}\n";
        startRow = "";  endRow = " \\\\ \n";
        nextCell = " & ";  percentStr = "\\%";
        preNum = "\\multicolumn{1}{r}{";  postNum = "}";
    }

    char numStr [16];
    dstr->Append (startTable);
    uint length[2];
    length[OPTABLE_Line] = strLength (repGames);
    length[OPTABLE_All] = strLength (allGames);

    if (Format == OPTABLE_LaTeX) {
        const char * q = " & \\hspace*{\\fill}{\\F Q}\\hspace*{\\fill}";
        const char * r = " & \\hspace*{\\fill}{\\F R}\\hspace*{\\fill}";
        const char * qr = " & \\hspace*{\\fill}{\\F QR}\\hspace*{\\fill}";
        const char * bn = " & \\hspace*{\\fill}{\\F BN}\\hspace*{\\fill}";
        const char * p = " & \\hspace*{\\fill}{{\\F p}}\\hspace*{\\fill}";
        const char * x = " & ";
        dstr->Append (startRow);
        dstr->Append (x, x, x, r); dstr->Append (x, q, x, qr);
        dstr->Append (endRow, startRow);
        dstr->Append (p, bn, r, bn); dstr->Append (q, bn, qr, bn);
        dstr->Append (endRow, "\\hline\n");
    } else {
        dstr->Append(startRow);
        uint len = length[OPTABLE_Line];
        if (length[OPTABLE_All] > len) { len = length[OPTABLE_All]; }
        len++;
        for (uint space=0; space < len; space++) { dstr->AddChar (' '); }
        char t1[10]; char t2[10];
        strcpy(t1, "     P"); strcpy(t2, "    BN");
        transPieces(t1); transPieces(t2);
        dstr->Append (nextCell, t1, nextCell, t2);
        strcpy(t1, "     R"); strcpy(t2, "  R,BN");
        transPieces(t1); transPieces(t2);
        dstr->Append (nextCell, t1, nextCell, t2);
        strcpy(t1, "     Q"); strcpy(t2, "  Q,BN");
        transPieces(t1); transPieces(t2);
        dstr->Append (nextCell, t1, nextCell, t2);
        strcpy(t1, "   Q,R"); strcpy(t2, "Q,R,BN");
        transPieces(t1); transPieces(t2);
        dstr->Append (nextCell, t1, nextCell, t2);
//         dstr->Append (nextCell, "     P", nextCell, "    BN");
//         dstr->Append (nextCell, "     R", nextCell, "  R,BN");
//         dstr->Append (nextCell, "     Q", nextCell, "  Q,BN");
//         dstr->Append (nextCell, "   Q,R", nextCell, "Q,R,BN");
        dstr->Append (endRow);
    }
    const char * rowName [2] = { repGames, allGames };
    for (uint t = OPTABLE_Line; t <= OPTABLE_All; t++) {
        dstr->AddChar (' ');
        dstr->Append (startRow, rowName[t]);
        int diff = length[1-t] - length[t];
        while (diff > 0) { dstr->AddChar (' '); diff--; }

        uint i, sum = 0;
        for (i=0; i < NUM_EGTHEMES; i++) { sum += EndgameCount[t][i]; }
        for (i=0; i < NUM_EGTHEMES; i++) {
            uint pc = 0;
            if (sum > 0) { pc = ((100 * EndgameCount[t][i]) + (sum/2)) / sum; }
            sprintf (numStr, "%5u", pc);
            dstr->Append (nextCell);
            if (Format == OPTABLE_CText  &&  t == OPTABLE_Line) {
                dstr->Append ("<darkblue><run sc_report ", Type);
                dstr->Append (" select end ", i, "; ::windows::stats::Refresh>");
            }
            dstr->Append (preNum, numStr, percentStr, postNum);
            if (Format == OPTABLE_CText  &&  t == OPTABLE_Line) {
                dstr->Append ("</run></darkblue>");
            }
        }
        dstr->Append (endRow);
    }
    dstr->Append (endTable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpTable::SelectGames:
//    Given an endgame or opening theme number or note
//    number, returns an array with two integer elements
//    for every game in the opening report that matches the
//    specified theme or note. The type is specified by
//    the character 'e' for endgame theme, 'o' for opening
//    theme, or 'n' for note number.
//    Each pair of elements contains the game number, and
//    its start ply. The caller is responsible for deleting
//    the allocated array, which ends with a (0,0) pair.
uint *
OpTable::SelectGames (char type, uint number)
{
#ifdef WINCE
    uint * matches = (uint *) my_Tcl_Alloc(sizeof( uint [NumLines * 2 + 2]));
#else
    uint * matches = new uint [NumLines * 2 + 2];
#endif
    uint * match = matches;

    for (uint i=0; i < NumLines; i++) {
        OpLine * line = Line[i];
        if (line == NULL) { continue; }
        if (line->GameNumber == 0) { continue; }
        bool selected = false;
        if (type == 'e') {
            // Only games from one endgame classification:
            if (line->EgTheme == number) { selected = true; }
        } else if (type == 't') {
            // Only games from one positional theme:
            if (number < NUM_POSTHEMES
                  &&  line->Theme[number] >= POSTHEME_THRESHOLD) {
                selected = true;
            }
        } else if (type == 'n') {
            // Only games from one note number:
            if (line->NoteNumber == number) { selected = true; }
        } else if (type == 'm') {
            // Only games from one move order:
            if (line->MoveOrderID == number) { selected = true; }
        } else if (type == 'a') {
            // All games:
            selected = true;
        }
        if (selected) {
            *match++ = line->GameNumber;
            *match++ = line->StartPly;
        }
    }
    *match++ = 0;
    *match++ = 0;
    return matches;
}

//////////////////////////////////////////////////////////////////////
// End of: optable.cpp
//////////////////////////////////////////////////////////////////////
