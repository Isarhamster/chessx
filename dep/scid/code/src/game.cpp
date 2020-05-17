//////////////////////////////////////////////////////////////////////
//
//  FILE:       game.cpp
//              Game class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2000-2003  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "game.h"
#include "bytebuf.h"
#include "common.h"
#include "dstring.h"
#include "naglatex.h"
#include "nagtext.h"
#include "position.h"
#include "stored.h"
#include "textbuf.h"
#include <algorithm>
#include <cstring>

// Piece letters translation
int language = 0; // default to english
//  0 = en, 
//  1 = fr, 2 = es, 3 = de, 4 = it, 5 = ne, 6 = cz
//  7 = hu, 8 = no, 9 = sw, 10 = ca, 11 = fi, 12 = gr
//  TODO Piece translations for greek
const char * langPieces[] = { "", 
"PPKRQDRTBFNC", "PPKRQDRTBANC", "PBKKQDRTBLNS", 
"PPKRQDRTBANC", "PpKKQDRTBLNP", "PPKKQDRVBSNJ",
"PGKKQVRBBFNH", "PBKKQDRTBLNS", "PBKKQDRTBLNS", "PPKRQDRTBANC", "PSKKQDRTBLNR", "" };

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// transPieces():
// Given a string, will translate pieces from english to another language
void transPieces(char *s) {
  if (language == 0) return;
  char * ptr = s;
  int i;

  while (*ptr) {
    if (*ptr >= 'A' && *ptr <= 'Z') {
      for (i=0; i<12; i+=2) {
        if (*ptr == langPieces[language][i]) {
          *ptr = langPieces[language][i+1];
          break;
        }
      }
    }
    ptr++;
  }
}

char transPiecesChar(char c) {
  char ret = c;
  if (language == 0) return c;
  for (int i=0; i<12; i+=2) {
    if (c == langPieces[language][i]) {
      ret = langPieces[language][i+1];
      break;
      }
    }
  return ret;
}

const char * ratingTypeNames [17] = {
    "Elo", "Rating", "Rapid", "ICCF", "USCF", "DWZ", "ECF",
    // Reserved for future use:
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    // End of array marker:
    NULL
};

uint
strGetRatingType (const char * name) {
    uint i = 0;
    while (ratingTypeNames[i] != NULL) {
        if (strEqual (name, ratingTypeNames[i])) { return i; }
        i++;
    }
    return 0;
}

typedef Game * GamePtr;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// printNag(): converts a numeric NAG to its string equivalent.
//    The parameter <str> should point to a string at least 10 bytes long.
// TODO
//    replace < and > in NAG codes by <lt> and <gt>
void
game_printNag (byte nag, char * str, bool asSymbol, gameFormatT format)
{
    ASSERT (str != NULL);

    if (nag == 0) {
        *str = 0;
        return;
    }

    if (nag >= (sizeof evalNagsRegular / sizeof (const char *))) {
        if (format == PGN_FORMAT_LaTeX) *str = 0;
        else sprintf (str, "$%u", nag);
        return;
    }

    if (asSymbol) {
       if (format == PGN_FORMAT_LaTeX) {
          strcpy (str, evalNagsLatex[nag]);
       } else {
         strcpy (str, evalNagsRegular[nag]);
       }
       if (nag == NAG_Diagram) {
          if (format == PGN_FORMAT_LaTeX) {
              strcpy (str, evalNagsLatex[nag]);
          } else if (format == PGN_FORMAT_HTML) {
              strcpy(str, "<i>(D)</i>");
          } else {
              str[0] = 'D'; str[1] = 0;
          }
       }
       return;
    } else {
    sprintf (str, "%s$%d", format == PGN_FORMAT_LaTeX ? "\\" : "", nag);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// game_parseNag():
//      Parses an annotation symbol into its numeric equivalent.
//      Accepts numeric format ($51) or symbols such as
//      !, ?, +=, -/+, N, etc.
//
byte game_parseNag(std::pair<const char*, const char*> strview) {
    auto slen = std::distance(strview.first, strview.second);
    if (slen == 0 || slen > 7)
        return 0;

    char strbuf[8] = {0};
    std::copy_n(strview.first, slen, strbuf);
    const char* str = strbuf;

    if (*str == '$') {
        str++;
        return (byte) strGetUnsigned(str);
    }
    if ((*str <= '9'  &&  *str >= '0')) {
        return (byte) strGetUnsigned(str);
    }

    if (*str == '!') {
        // Must be "!", "!!", "!?", or invalid:
        str++;
        if (*str == 0) { return NAG_GoodMove; }            // !      $1
        if (*str == '!') { return NAG_ExcellentMove; }     // !!     $3
        if (*str == '?') { return NAG_InterestingMove; }   // !?     $5
        return 0;
    }

    if (*str == '?') {
        // Must be "?", "??", "?!", or invalid:
        str++;
        if (*str == 0) { return NAG_PoorMove; }            // ?      $2
        if (*str == '?') { return NAG_Blunder; }           // ??     $4
        if (*str == '!') { return NAG_DubiousMove; }       // ?!     $6
        return 0;
    }

    if (*str == '+') {
        // Must be "+=", "+/=", "+/-", "+-", "+--", "+>" or invalid:
        str++;
        if (*str == '=') { return NAG_WhiteSlight; }      // +=      $14
        if (*str == '-' && str[1] == 0) {                 // +-      $18
           return NAG_WhiteDecisive; }
        if (*str == '>') { return NAG_WithAttack; }       // +>      $40
        if (*str == '/'  &&  str[1] == '-') {             // +/-     $16
           return NAG_WhiteClear; }
        if (*str == '/'  &&  str[1] == '=') {             // +/=     $14
           return NAG_WhiteSlight; }
        if (*str == '-'  &&  str[1] == '-') {             // +--     $20
           return NAG_WhiteCrushing; }
        return 0;
    }

    if (*str == '=') {
        // Must be "=" (equal), "=+", "=/+", "=/&" or invalid:
        str++;
        if (*str == 0) { return NAG_Equal; }              // =       $10
        if (*str == '+') { return NAG_BlackSlight; }      // =+      $15
        if (*str == '/'  &&  str[1] == '+') {             // =/+     $15
           return NAG_BlackSlight; }
        if (*str == '/'  &&  str[1] == '&') {             // =/&     $44
           return NAG_Compensation; }
        return 0;
    }

    if (*str == '-') {
        // Must be "-+", "-/+" or "--+", "->":
        str++;
        if (*str == '+') { return NAG_BlackDecisive; }     // -+     $19
        if (*str == '>') { return NAG_WithBlackAttack; }   // ->     $41
        if (*str == '/'  &&  str[1] == '+') {              // -/+    $17
           return NAG_BlackClear; }
        if (*str == '-'  &&  str[1] == '+') {              // --+    $21
           return NAG_BlackCrushing; }
        if (*str == '-'  &&  str[1] == 0) {                // --     $210
           return NAG_See; }
        return 0;
    }

    if (*str == '/') {
        // Must be "/\" or "/"
        str++;
        if (*str == 0)    { return NAG_Diagonal; }         // /      $150
        if (*str == '\\') { return NAG_WithIdea; }         // Tri    $140
        return 0;
    }

    if (*str == 'R') {
        // Must be "R", "RR"
        str++;
        if (*str == 0)   { return NAG_VariousMoves; }      // R      $144
        if (*str == 'R') { return NAG_Comment; }           // RR     $145
        return 0;
    }

    if (*str == 'z') {
        // Must be "zz"
        str++;
        if (*str == 'z') { return NAG_BlackZugZwang; }     // zz     $23
        return 0;
    }
    if (*str == 'Z') {
        // Must be "ZZ"
        str++;
        if (*str == 'Z') { return NAG_ZugZwang; }          // ZZ     $22
        return 0;
    }

    if (*str == 'B') {
        // Must be "BB", "Bb"
        str++;
        if (*str == 'B') { return NAG_BishopPair; }        // BB     $151
        if (*str == 'b') { return NAG_OppositeBishops; }   // Bb     $153
        return 0;
    }

    if (*str == 'o') {
        // Must be "BB", "Bb"
        str++;
        if (*str == '-'  &&  str[1] == 'o') {              // o-o    $192
           return NAG_SeparatedPawns; }
        if (*str == 'o'  &&  str[1] == 0)   {              // [+]    $193
           return NAG_UnitedPawns; }
        if (*str == '^'  &&  str[1] == 0)   {              // o^     $212
           return NAG_PassedPawn; }
        return 0;
    }

    if (*str == '(') {
        // Must be (_)
        str++;
        if (*str == '_'  &&  str[1] == ')') {             // (_)     $142
           return NAG_BetterIs; }
        return 0;
    }

    if (*str == '[') {
        // Must be (_)
        str++;
        if (*str == ']'  &&  str[1] == 0) {                // []     $8
           return NAG_OnlyMove; }
        if (*str == '+'  &&  str[1] == ']') {              // [+]    $48
           return NAG_SlightCentre; }
        if (*str   == '+' &&
            str[1] == '+' && str[2] == ']') {              // [++]   $50
           return NAG_Centre; }
        return 0;
    }

    if (*str == '_') {
        // must be _|_ or _|
        str++;
        if (*str == '|'  &&  str[1] == '_') {              // _|_    $148
           return NAG_Ending; }
        if (*str == '|'  &&  str[1] == 0) {                // _|     $215
           return NAG_Without; }
        return 0;
    }

    if (*str == '|') {
        // must be ||, |_
        str++;
        if (*str == '|' ) { return NAG_Etc; }             // ||      $190
        if (*str == '_') { return NAG_With; }             // |_      $214
        return 0;
    }

    if (*str == '>') {
        // must be >, >>, >>>
        str++;
        if (*str == 0) { return NAG_SlightKingSide; }     // >       $54
        if (*str == '>'  &&  str[1] == 0) {               // >>      $56
           return NAG_ModerateKingSide; }
        if (*str == '>'  &&  str[1] == '>') {             // >>>     $58
           return NAG_KingSide; }
        return 0;
    }

    if (*str == '<') {
        // must be <, <<, <<<, <=>
        str++;
        if (*str == 0) { return NAG_SlightQueenSide; }   // <        $60
        if (*str == '<'  &&  str[1] == 0) {              // <<       $62
           return NAG_ModerateQueenSide; }
        if (*str   == '<'  &&                            // <<<      $64
            str[1] == '<' && str[2] == 0) { return NAG_QueenSide; }
        if (*str == '='  &&                              // <=>      $149
            str[1] == '>' && str[2] == 0) { return NAG_File; }
        if (*str == '+' &&                               // <+>      $130
            str[1] == '>' && str[2] == 0) { return NAG_SlightCounterPlay; }
        if (*str == '-' &&                               // <->      $131
              str[1] == '>' && str[2] == 0) { return NAG_BlackSlightCounterPlay; }
        if (*str == '+' &&                               // <++>     $132
              str[1] == '+' && str[2] == '>' && str[3] == 0) { return NAG_CounterPlay; }
        if (*str == '-' &&                               // <-->     $133
              str[1] == '-' && str[2] == '>' && str[3] == 0) { return NAG_BlackCounterPlay; }
        if (*str == '+' &&                               // <+++>    $134
              str[1] == '+' && str[2] == '+' && str[3] == '>') { return NAG_DecisiveCounterPlay; }
        if (*str   == '-' &&                             // <--->    $135
            str[1] == '-' && str[2] == '-' && str[3] == '>') { return NAG_BlackDecisiveCounterPlay; }
        return 0;
    }

    if (*str == '~' && *(str+1) == '=') {                // ~=       $44
       // alternative Compensation symbol:
        return NAG_Compensation;
    }

    if (*str == '~') {                                   // ~        $13
       // Unclear symbol:
        return NAG_Unclear;
    }

    if (*str == 'x') {                                   // x        $147
        return NAG_WeakPoint;
    }

    if (str[0] == 'N'  &&  str[1] == 0) {                // N        $146
       // Novelty symbol:
        return NAG_Novelty;
    }

    if (str[0] == 'D'  &&  str[1] == 0) {                // D        $201
       // Diagram symbol:
        return NAG_Diagram;
    }
    return 0;
}

errorT Game::AddNag (byte nag) {
    moveT * m = CurrentMove->prev;
    if (m->nagCount + 1 >= MAX_NAGS) { return ERROR_GameFull; }
    if (nag == 0) { /* Nags cannot be zero! */ return OK; }
	// If it is a move nag replace an existing
	if( nag >= 1 && nag <= 6)
		for( int i=0; i<m->nagCount; i++)
			if( m->nags[i] >= 1 && m->nags[i] <= 6)
			{
				m->nags[i] = nag;
				return OK;
			}
	// If it is a position nag replace an existing
	if( nag >= 10 && nag <= 21)
		for( int i=0; i<m->nagCount; i++)
			if( m->nags[i] >= 10 && m->nags[i] <= 21)
			{
				m->nags[i] = nag;
				return OK;
			}
	if( nag >= 1 && nag <= 6)
	{
		// Put Move Nags at the beginning
		for( int i=m->nagCount; i>0; i--)  m->nags[i] =  m->nags[i-1];
		m->nags[0] = nag;
	}
	else
		m->nags[m->nagCount] = nag;
	m->nagCount += 1;
	m->nags[m->nagCount] = 0;
    return OK;
}

errorT Game::RemoveNag (bool isMoveNag) {
    moveT * m = CurrentMove->prev;
	if( isMoveNag)
	{
		for( int i=0; i<m->nagCount; i++)
			if( m->nags[i] >= 1 && m->nags[i] <= 6)
			{
				m->nagCount -= 1;
				for( int j=i; j<m->nagCount; j++)  m->nags[j] =  m->nags[j+1];
				m->nags[m->nagCount] = 0;
				return OK;
			}
	}
	else
	{
		for( int i=0; i<m->nagCount; i++)
			if( m->nags[i] >= 10 && m->nags[i] <= 21)
			{
				m->nagCount -= 1;
				for( int j=i; j<m->nagCount; j++)  m->nags[j] =  m->nags[j+1];
				m->nags[m->nagCount] = 0;
				return OK;
			}
	}
    return OK;
}

//////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Move allocation:
//      moves are allocated in chunks to save memory and for faster
//      performance.
//
constexpr int MOVE_CHUNKSIZE = 128;

moveT* Game::allocMove() {
	if (moveChunkUsed_ == MOVE_CHUNKSIZE) {
		moveChunks_.emplace_front(new moveT[MOVE_CHUNKSIZE]);
		moveChunkUsed_ = 0;
	}
	return moveChunks_.front().get() + moveChunkUsed_++;
}

moveT* Game::NewMove(markerT marker) {
	moveT* res = allocMove();
	res->clear();
	res->marker = marker;
	return res;
}

Game::Game(const Game& obj) {
	extraTags_ = obj.extraTags_;
	WhiteStr = obj.WhiteStr;
	BlackStr = obj.BlackStr;
	EventStr = obj.EventStr;
	SiteStr = obj.SiteStr;
	RoundStr = obj.RoundStr;
	Date = obj.Date;
	EventDate = obj.EventDate;
	EcoCode = obj.EcoCode;
	WhiteElo = obj.WhiteElo;
	BlackElo = obj.BlackElo;
	WhiteRatingType = obj.WhiteRatingType;
	BlackRatingType = obj.BlackRatingType;
	Result = obj.Result;
	std::copy_n(obj.ScidFlags, sizeof(obj.ScidFlags), ScidFlags);

	if (obj.StartPos)
		StartPos = std::make_unique<Position>(*obj.StartPos);

	NumHalfMoves = obj.NumHalfMoves;
	WhiteEstimateElo = obj.WhiteEstimateElo;
	BlackEstimateElo = obj.BlackEstimateElo;
	NumMovesPrinted = obj.NumMovesPrinted;
	PgnStyle = obj.PgnStyle;
	PgnFormat = obj.PgnFormat;
	HtmlStyle = obj.HtmlStyle;

	moveChunkUsed_ = MOVE_CHUNKSIZE;
	FirstMove = obj.FirstMove->cloneLine(nullptr,
	                                     [this]() { return allocMove(); });

	MoveToLocationInPGN(obj.GetLocationInPGN());
}
Game* Game::clone() { return new Game(*this); }

void Game::strip(bool variations, bool comments, bool NAGs) {
    while (variations && MoveExitVariation() == OK) { // Go to main line
    }

    for (auto& chunk : moveChunks_) {
        moveT* move = chunk.get();
        moveT* end = (chunk == moveChunks_.front()) ? move + moveChunkUsed_
                                                    : move + MOVE_CHUNKSIZE;
        for (; move != end; ++move) {
            if (variations) {
                move->numVariations = 0;
                move->varChild = nullptr;
            }
            if (comments)
                move->comment.clear();

            if (NAGs) {
                move->nagCount = 0;
                std::fill_n(move->nags, sizeof(move->nags), 0);
            }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::ClearMoves(): clear all moves.
void Game::ClearMoves() {
    // Delete any chunks of moves except the first:
    if (moveChunks_.empty()) {
        moveChunkUsed_ = MOVE_CHUNKSIZE;
    } else {
        moveChunks_.erase_after(moveChunks_.begin(), moveChunks_.end());
        moveChunkUsed_ = 0;
    }
    StartPos = nullptr;
    CurrentPos->StdStart();

    // Initialize FirstMove: start and end of movelist markers
    FirstMove = NewMove(START_MARKER);
    CurrentMove = NewMove(END_MARKER);
    FirstMove->setNext(CurrentMove);

    VarDepth = 0;
    NumHalfMoves = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::Clear():
//      Reset the game to its normal empty state.
//
void Game::Clear() {
	extraTags_.clear();
	WhiteStr.clear();
	BlackStr.clear();
	EventStr.clear();
	SiteStr.clear();
	RoundStr.clear();
	Date = ZERO_DATE;
	EventDate = ZERO_DATE;
	EcoCode = 0;
	WhiteElo = BlackElo = 0;
	WhiteEstimateElo = BlackEstimateElo = 0;
	WhiteRatingType = BlackRatingType = RATING_Elo;
	Result = RESULT_None;
	ScidFlags[0] = 0;

	NumMovesPrinted = 0;
	PgnStyle = PGN_STYLE_TAGS | PGN_STYLE_VARS | PGN_STYLE_COMMENTS;
	PgnFormat = PGN_FORMAT_Plain;
	HtmlStyle = 0;

	ClearMoves();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::PgnFormatFromString():
//      Converts a string to a gameFormatT, returning true on success
//      or false on error.
//      The string should be a case-insensitive unique prefix of
//      "plain" (or "pgn"), "HTML", "LaTeX" or "Color".
bool
Game::PgnFormatFromString (const char * str, gameFormatT * fmt)
{
    if (strIsCasePrefix (str, "Plain")) {
        *fmt = PGN_FORMAT_Plain;
    } else if (strIsCasePrefix (str, "PGN")) {
        *fmt = PGN_FORMAT_Plain;
    } else if (strIsCasePrefix (str, "HTML")) {
        *fmt = PGN_FORMAT_HTML;
    } else if (strIsCasePrefix (str, "LaTeX")) {
        *fmt = PGN_FORMAT_LaTeX;
    } else if (strIsCasePrefix (str, "Color")) {
        *fmt = PGN_FORMAT_Color;
    } else {
        return false;
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::SetPgnFormatFromString():
//      Sets the PgnFormat from the provided string.
//      Returns true if the PgnFormat was successfully set.
bool
Game::SetPgnFormatFromString (const char * str)
{
    return PgnFormatFromString (str, &PgnFormat);
}

errorT
Game::SetStartFen (const char * fenStr)
{
    auto pos = std::make_unique<Position>();
    errorT err = pos->ReadFromFEN (fenStr);
    if (err != OK)
        return err;

    ClearMoves();
    StartPos = std::move(pos);
    *CurrentPos = *StartPos;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::AddPgnTag(): Add a PGN Tag.
//
void Game::AddPgnTag(const char* tag, const char* value) {
    // First, try to replace an existing tag:
    for (auto& e : extraTags_) {
        if (e.first == tag) {
            e.second.assign(value);
            return;
        }
    }
    extraTags_.emplace_back(tag, value);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::FindExtraTag():
//   Finds and returns an extra PGN tag if it
//   exists, or NULL if it does not exist.
const char* Game::FindExtraTag(const char* tag) const {
    for (auto& e : extraTags_) {
        if (e.first == tag)
            return e.second.c_str();
    }
    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::RemoveExtraTag():
//   Remove an extra PGN tag if it exists.
bool Game::RemoveExtraTag(const char* tag) {
    auto it = std::remove_if(extraTags_.begin(), extraTags_.end(),
                             [&](const std::pair<std::string, std::string>& e) {
                                 return e.first == tag;
                             });
    if (it != extraTags_.end()) {
        extraTags_.erase(it, extraTags_.end());
        return true;
    }
    return false;
}

std::string& Game::accessTagValue(const char* tag, size_t tagLen) {
	if (tagLen == 5) {
		if (std::equal(tag, tag + 5, "Event"))
			return EventStr;
		if (std::equal(tag, tag + 5, "Round"))
			return RoundStr;
		if (std::equal(tag, tag + 5, "White"))
			return WhiteStr;
		if (std::equal(tag, tag + 5, "Black"))
			return BlackStr;
	} else if (tagLen == 4) {
		if (std::equal(tag, tag + 4, "Site"))
			return SiteStr;
	}

	for (auto& elem : extraTags_) {
		if (std::equal(tag, tag + tagLen, elem.first.begin(), elem.first.end()))
			return elem.second;
	}
	extraTags_.emplace_back();
	extraTags_.back().first.assign(tag, tagLen);
	return extraTags_.back().second;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::SetMoveComment():
//      Sets the comment for a move. A comment before the game itself
//      is stored as a comment of FirstMove.
//
void
Game::SetMoveComment (const char * comment)
{
    ASSERT (CurrentMove != NULL  &&  CurrentMove->prev != NULL);
    moveT * m = CurrentMove->prev;
    if (comment == NULL) {
        m->comment.clear();
    } else {
        m->comment = comment;
        // CommentsFlag = 1;
    }
}

int Game::setRating(colorT col, const char* ratingType, size_t ratingTypeLen,
                    std::pair<const char*, const char*> rating) {
	auto begin = ratingTypeNames;
	const size_t ratingSz = 7;
	auto it = std::find_if(begin, begin + ratingSz, [&](auto rType) {
		return std::equal(ratingType, ratingType + ratingTypeLen, rType,
		                  rType + std::strlen(rType));
	});
	byte rType = static_cast<byte>(std::distance(begin, it));
	if (rType >= ratingSz)
		return -1;

	int res = 1;
	auto elo = strGetUnsigned(std::string{rating.first, rating.second}.c_str());
	if (elo > MAX_ELO) {
		elo = 0;
		res = 0;
	}
	if (col == WHITE) {
		SetWhiteElo(static_cast<eloT>(elo));
		SetWhiteRatingType(rType);
	} else {
		SetBlackElo(static_cast<eloT>(elo));
		SetBlackRatingType(rType);
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////
// A "location" in the game is represented by a position (Game::CurrentPos), the
// next move to be played (Game::CurrentMove) and the number of parent variations
// (Game::VarDepth). Since CurrentMove is the next move to be played, some
// invariants must hold: it is never nullptr and it never points to a
// START_MARKER (it will point to a END_MARKER if there are no more moves). This
// also means that CurrentMove->prev is always valid: it will point to a
// previous move or to a START_MARKER.
// The following functions modify ONLY the current location of the game.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Move current position forward one move.
// Also update all the necessary fields in the simpleMove structure
// (CurrentMove->moveData) so it can be undone.
//
errorT Game::MoveForward(void) {
	if (CurrentMove->endMarker())
		return ERROR_EndOfMoveList;

	CurrentPos->DoSimpleMove(&CurrentMove->moveData);
	CurrentMove = CurrentMove->next;

	// Invariants
	ASSERT(CurrentMove && CurrentMove->prev);
	ASSERT(!CurrentMove->startMarker());
	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::MoveBackup():
//      Backup one move.
//
errorT Game::MoveBackup(void) {
	if (CurrentMove->prev->startMarker())
		return ERROR_StartOfMoveList;

	CurrentMove = CurrentMove->prev;
	CurrentPos->UndoSimpleMove(&CurrentMove->moveData);

	// Invariants
	ASSERT(CurrentMove && CurrentMove->prev);
	ASSERT(!CurrentMove->startMarker());
	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::MoveIntoVariation():
//      Move into a subvariation. Variations are numbered from 0.
errorT Game::MoveIntoVariation(uint varNumber) {
	for (auto subVar = CurrentMove; subVar->varChild; --varNumber) {
		subVar = subVar->varChild;
		if (varNumber == 0) {
			CurrentMove = subVar->next; // skip the START_MARKER
			++VarDepth;

			// Invariants
			ASSERT(CurrentMove && CurrentMove->prev);
			ASSERT(!CurrentMove->startMarker());
			return OK;
		}
	}
	return ERROR_NoVariation; // there is no such variation
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::MoveExitVariation():
//      Move out of a variation, to the parent.
//
errorT Game::MoveExitVariation(void) {
	if (VarDepth == 0) // not in a variation!
		return ERROR_NoVariation;

	// Algorithm: go back previous moves as far as possible, then
	// go up to the parent of the variation.
	while (MoveBackup() == OK) {
	}
	CurrentMove = CurrentMove->getParent().first;
	--VarDepth;

	// Invariants
	ASSERT(CurrentMove && CurrentMove->prev);
	ASSERT(!CurrentMove->startMarker());
	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Move to the beginning of the game.
//
void Game::MoveToStart() {
	if (StartPos) {
		*CurrentPos = *StartPos;
	} else {
		CurrentPos->StdStart();
	}
	VarDepth = 0;
	CurrentMove = FirstMove->next;

	// Invariants
	ASSERT(CurrentMove && CurrentMove->prev);
	ASSERT(!CurrentMove->startMarker());
}

void Game::MoveToEnd() {
	MoveToStart();
	while (MoveForward() == OK) {
	}
}

errorT Game::MoveForwardInPGN() {
	if (CurrentMove->prev->varChild && MoveBackup() == OK)
		return MoveIntoVariation(0);

	while (MoveForward() != OK) {
		if (VarDepth == 0)
			return ERROR_EndOfMoveList;

		auto varnum = GetVarNumber();
		MoveExitVariation();
		if (MoveIntoVariation(varnum + 1) == OK)
			return OK;

		MoveForward();
	}
	return OK;
}

errorT Game::MoveToLocationInPGN(unsigned stopLocation) {
	MoveToStart();
	for (unsigned loc = 1; loc < stopLocation; ++loc) {
		errorT err = MoveForwardInPGN();
		if (err != OK)
			return err;
	}
	return OK;
}

unsigned Game::GetLocationInPGN() const {
	unsigned res = 1;
	const moveT* last_move = CurrentMove->prev;
	const moveT* move = FirstMove;
	for (; move != last_move; move = move->nextMoveInPGN()) {
		if (!move->endMarker())
			++res;
	}
	return res;
}

unsigned Game::GetPgnOffset() const {
	unsigned res = 1;
	const moveT* last_move = CurrentMove->getPrevMove();
	if (last_move) {
		const moveT* move = FirstMove;
		for (; move != last_move; move = move->nextMoveInPGN()) {
			if (!move->endMarker())
				++res;
		}
	}
	return res;
}

std::string Game::currentPosUCI() const {
	std::string res = "position startpos moves";
	char FEN[256] = {};

	std::vector<const moveT*> moves;
	const moveT* move = CurrentMove;
	while ((move = move->getPrevMove())) {
		if (move->moveData.isNullMove()) {
			Position lastValidPos = *currentPos();
			for (const moveT* m : moves) {
				lastValidPos.UndoSimpleMove(&m->moveData);
			}
			lastValidPos.PrintFEN(FEN, FEN_ALL_FIELDS);
			break;
		}
		moves.emplace_back(move);
	}

	if (*FEN || HasNonStandardStart(FEN)) {
		res.replace(9, 4, "fen ");
		res.replace(13, 4, FEN);
	}

	const auto allocSpeedup = res.size();
	res.resize(allocSpeedup + moves.size() * 6);
	auto it = res.data() + allocSpeedup;
	for (auto m = moves.crbegin(), end = moves.crend(); m != end; ++m) {
		*it++ = ' ';
		it = (*m)->moveData.toLongNotation(it);
	}
	res.resize(std::distance(res.data(), it)); // shrink
	return res;
}

///////////////////////////////////////////////////////////////////////////
// The following functions modify the moves graph in order to add or delete
// moves. Promoting variations also modifies the moves graph.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::AddMove():
//      Add a move at current position and do it.
//
errorT Game::AddMove(const simpleMoveT* sm) {
	ASSERT(sm != NULL);

	// We must be at the end of a game/variation to add a move:
	if (!CurrentMove->endMarker())
		Truncate();

	CurrentMove->setNext(NewMove(END_MARKER));
	CurrentMove->marker = NO_MARKER;
	CurrentMove->moveData.from = sm->from;
	CurrentMove->moveData.to = sm->to;
	CurrentMove->moveData.promote = sm->promote;
	CurrentMove->moveData.movingPiece = sm->movingPiece;
	if (VarDepth == 0)
		++NumHalfMoves;

	return MoveForward();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::AddVariation():
//      Add a variation for the current move.
//      Also moves into the variation.
errorT Game::AddVariation() {
	auto err = MoveBackup();
	if (err != OK)
		return err;

	auto newVar = NewMove(START_MARKER);
	newVar->setNext(NewMove(END_MARKER));
	CurrentMove->appendChild(newVar);

	// Move into variation
	CurrentMove = newVar->next;
	++VarDepth;

	// Invariants
	ASSERT(CurrentMove && CurrentMove->prev);
	ASSERT(!CurrentMove->startMarker());
	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::FirstVariation():
// Promotes the current variation to first variation.
errorT Game::FirstVariation() {
	auto parent = CurrentMove->getParent();
	auto root = parent.first;
	if (!root)
		return ERROR_NoVariation;

	root->detachChild(parent.second);
	root->insertChild(parent.second, 0);
	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::MainVariation():
//    Like FirstVariation, but promotes the variation to the main line,
//    demoting the main line to be the first variation.
errorT Game::MainVariation() {
	auto parent = CurrentMove->getParent();
	auto root = parent.first;
	if (!root)
		return ERROR_NoVariation;
	if (parent.second->next->endMarker()) // Do not promote empty variations
		return OK;

	// Make the current variation the first variation
	root->detachChild(parent.second);
	root->insertChild(parent.second, 0);

	// Swap the mainline with the current variation
	root->swapLine(*parent.second->next);

	ASSERT(VarDepth);
	if (--VarDepth == 0) { // Recalculate NumHalfMoves
		const auto count_moves = [](auto move) {
			int res = 0;
			while (!move->endMarker()) {
				++res;
				move = move->next;
			}
			return res;
		};
		ASSERT(FirstMove->startMarker() && FirstMove->next);
		NumHalfMoves = count_moves(FirstMove->next);
	}

	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::DeleteVariation():
//      Deletes a variation. Variations are numbered from 0.
//      Note that for speed and simplicity, freed moves are not
//      added to the free list. This means that repeatedly adding and
//      deleting variations will waste memory until the game is cleared.
//
errorT Game::DeleteVariation() {
	auto parent = CurrentMove->getParent();
	auto root = parent.first;
	if (!root || MoveExitVariation() != OK)
		return ERROR_NoVariation;

	root->detachChild(parent.second);
	return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::Truncate():
//      Truncate game at the current move.
//      For speed and simplicity, moves and comments are not freed.
//      So repeatedly adding moves and truncating a game will waste
//      memory until the game is cleared.
void Game::Truncate() {
	if (CurrentMove->endMarker())
		return;

	auto endMove = NewMove(END_MARKER);
	CurrentMove->prev->setNext(endMove);

	CurrentMove = endMove;
	if (VarDepth == 0)
		NumHalfMoves = GetCurrentPly();

	// Invariants
	ASSERT(CurrentMove && CurrentMove->prev);
	ASSERT(!CurrentMove->startMarker());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::TruncateStart():
//      Truncate all moves leading to current position.
void Game::TruncateStart() {
    // It is necessary to rebuild the current position using ReadFromFEN()
    // because the order of pieces is important when encoding to SCIDv4 format.
    char tempStr[256];
    CurrentPos->PrintFEN(tempStr, FEN_ALL_FIELDS);
    auto pos = std::make_unique<Position>();
    if (pos->ReadFromFEN(tempStr) != OK)
        return;

    if (VarDepth != 0 && MainVariation() != OK)
		return;

    NumHalfMoves -= GetCurrentPly();
    StartPos = std::move(pos);
    *CurrentPos = *StartPos;
    FirstMove->setNext(CurrentMove);

    // Do all the moves to update moveData.pieceNum to the new StartPos
    while (MoveForwardInPGN() == OK) {
    }
    MoveToStart();
}

namespace {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// calcHomePawnMask():
//      Computes the homePawn mask for a position.
//
int calcHomePawnMask (pieceT pawn, const pieceT* board)
{
    ASSERT (pawn == WP  ||  pawn == BP);
    const pieceT* bd = &(board[ (pawn == WP ? H2 : H7) ]);
    int result = 0;
    if (*bd == pawn) { result |= 128; }  bd--;   // H-fyle pawn
    if (*bd == pawn) { result |=  64; }  bd--;   // G-fyle pawn
    if (*bd == pawn) { result |=  32; }  bd--;   // F-fyle pawn
    if (*bd == pawn) { result |=  16; }  bd--;   // E-fyle pawn
    if (*bd == pawn) { result |=   8; }  bd--;   // D-fyle pawn
    if (*bd == pawn) { result |=   4; }  bd--;   // C-fyle pawn
    if (*bd == pawn) { result |=   2; }  bd--;   // B-fyle pawn
    if (*bd == pawn) { result |=   1; }          // A-fyle pawn
    return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// patternsMatch():
//      Used by Game::MaterialMatch() to test patterns.
//      Returns 1 if all the patterns in the list match, 0 otherwise.
//
int patternsMatch(const Position* pos, patternT* ptn) {
    const pieceT* board = pos->GetBoard();
    while (ptn != NULL) {
        if (ptn->rankMatch == NO_RANK) {

            if (ptn->fyleMatch == NO_FYLE) { // Nothing to test!
            } else {  // Test this fyle:
                squareT sq = square_Make (ptn->fyleMatch, RANK_1);
                int found = 0;
                for (uint i=0; i < 8; i++, sq += 8) {
                    if (board[sq] == ptn->pieceMatch) { found = 1; break; }
                }
                if (found != ptn->flag) { return 0; }
            }

        } else { // rankMatch is a rank from 1 to 8:

            if (ptn->fyleMatch == NO_FYLE) { // Test the whole rank:
                int found = 0;
                squareT sq = square_Make (A_FYLE, ptn->rankMatch);
                for (uint i=0; i < 8; i++, sq++) {
                    if (board[sq] == ptn->pieceMatch) { found = 1; break; }
                }
                if (found != ptn->flag) { return 0; }
            } else {  // Just test one square:
                squareT sq = square_Make(ptn->fyleMatch, ptn->rankMatch);
                int found = 0;
                if (board[sq] == ptn->pieceMatch) { found = 1; }
                if (found != ptn->flag) { return 0; }
            }
        }

        // If we get this far, this pattern matched. Try the next one:
        ptn = ptn->next;
    }

    // If we reach here, all patterns matched:
    return 1;
}
} // end of anonymous namespace

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::MaterialMatch(): Material search test.
//      The parameters min and max should each be an array of 15
//      counts, to specify the maximum and minimum number of counts
//      of each type of piece.
//
bool
Game::MaterialMatch (bool PromotionsFlag, ByteBuffer& buf, byte * min, byte * max,
                     patternT * patterns, int minPly, int maxPly,
                     int matchLength, bool oppBishops, bool sameBishops,
                     int minDiff, int maxDiff)
{
    ASSERT (matchLength >= 1);

    int matchesNeeded = matchLength;
    int matDiff;
    uint plyCount = 0;
    errorT err = DecodeSkipTags(&buf);
    while (err == OK) {
        bool foundMatch = false;
        byte wMinor, bMinor;

        // If current pos has LESS than the minimum of pawns, this
        // game can never match so return false;
        if (CurrentPos->PieceCount(WP) < min[WP]) { return false; }
        if (CurrentPos->PieceCount(BP) < min[BP]) { return false; }

        // If not in the valid move range, go to the next move or return:
        if ((int)plyCount > maxPly) { return false; }
        if ((int)plyCount < minPly) { goto Next_Move; }

// For these comparisons, we really could only do half of them each move,
// according to which side just moved.
        // For non-pawns, the count could be increased by promotions:
        if (CurrentPos->PieceCount(WQ) < min[WQ]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(BQ) < min[BQ]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(WR) < min[WR]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(BR) < min[BR]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(WB) < min[WB]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(BB) < min[BB]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(WN) < min[WN]) { goto Check_Promotions; }
        if (CurrentPos->PieceCount(BN) < min[BN]) { goto Check_Promotions; }
        wMinor = CurrentPos->PieceCount(WB) + CurrentPos->PieceCount(WN);
        bMinor = CurrentPos->PieceCount(BB) + CurrentPos->PieceCount(BN);
        if (wMinor < min[WM]) { goto Check_Promotions; }
        if (bMinor < min[BM]) { goto Check_Promotions; }

        // Now test maximum counts:
        if (CurrentPos->PieceCount(WQ) > max[WQ]) { goto Next_Move; }
        if (CurrentPos->PieceCount(BQ) > max[BQ]) { goto Next_Move; }
        if (CurrentPos->PieceCount(WR) > max[WR]) { goto Next_Move; }
        if (CurrentPos->PieceCount(BR) > max[BR]) { goto Next_Move; }
        if (CurrentPos->PieceCount(WB) > max[WB]) { goto Next_Move; }
        if (CurrentPos->PieceCount(BB) > max[BB]) { goto Next_Move; }
        if (CurrentPos->PieceCount(WN) > max[WN]) { goto Next_Move; }
        if (CurrentPos->PieceCount(BN) > max[BN]) { goto Next_Move; }
        if (CurrentPos->PieceCount(WP) > max[WP]) { goto Next_Move; }
        if (CurrentPos->PieceCount(BP) > max[BP]) { goto Next_Move; }
        if (wMinor > max[WM]) { goto Next_Move; }
        if (bMinor > max[BM]) { goto Next_Move; }

        // If both sides have ONE bishop, we need to check if the search
        // was restricted to same-color or opposite-color bishops:
        if (CurrentPos->PieceCount(WB) == 1
                && CurrentPos->PieceCount(BB) == 1) {
            if (!oppBishops  ||  !sameBishops) { // Check the restriction:
                colorT whiteBishCol = NOCOLOR;
                colorT blackBishCol = NOCOLOR;

                // Search for the white and black bishop, to find their
                // square color:
                const pieceT* bd = CurrentPos->GetBoard();
                for (squareT sq = A1; sq <= H8; sq++) {
                    if (bd[sq] == WB) {
                        whiteBishCol = BOARD_SQUARECOLOR [sq];
                    } else if (bd[sq] == BB) {
                        blackBishCol = BOARD_SQUARECOLOR [sq];
                    }
                }
                // They should be valid colors:
                ASSERT (blackBishCol != NOCOLOR  &&  whiteBishCol != NOCOLOR);

                // If the square colors do not match the restriction,
                // then this game cannot match:
                if (oppBishops  &&  blackBishCol == whiteBishCol) {
                    return false;
                }
                if (sameBishops  &&  blackBishCol != whiteBishCol) {
                    return false;
                }
            }
        }

        // Now check if the material difference is in-range:
        matDiff = (int)CurrentPos->MaterialValue(WHITE) -
                  (int)CurrentPos->MaterialValue(BLACK);
        if (matDiff < minDiff  ||  matDiff > maxDiff) { goto Next_Move; }

        // At this point, the Material matches; do the patterns match?
        if (patterns == NULL || patternsMatch(currentPos(), patterns)) {
            foundMatch = true;
            matchesNeeded--;
            if (matchesNeeded <= 0) { return true; }
        }
        // No? well, keep trying...
        goto Next_Move;

      Check_Promotions:
        // We only continue if this game has promotion moves:
        if (! PromotionsFlag) { return false; }

      Next_Move:
        {
            simpleMoveT sm;
            err = DecodeNextMove(&buf, sm);
            if (err == OK) {
                CurrentPos->DoSimpleMove(&sm);
            }
        }
        plyCount++;
        if (! foundMatch) { matchesNeeded = matchLength; }
    }

    // End of game reached, and no match:
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::ExactMatch():
//      Exact position search test.
//      If sm is not NULL, its from, to, promote etc will be filled with
//      the next move at the matching position, if there is one.
//      If neverMatch is non-NULL, the boolean it points to is set to
//      true if the game could never match even with extra moves.
//
bool
Game::ExactMatch (Position * searchPos, ByteBuffer * buf, simpleMoveT * sm,
                  gameExactMatchT searchType)
{
    // If buf is NULL, the game is in memory. Otherwise, Decode only
    // the necessary moves:
    errorT err = OK;

    if (buf == NULL) {
        MoveToStart();
    } else {
        err = DecodeSkipTags(buf);
    }

    uint plyCount = 0;
    //uint skip = 0;    // Just for statistics on number of moves skipped.
    uint search_whiteHPawns = 0;
    uint search_blackHPawns = 0;
    uint current_whiteHPawns, current_blackHPawns;
    bool check_pawnMaskWhite, check_pawnMaskBlack;
    bool doHomePawnChecks = false;

    uint wpawnFyle [8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint bpawnFyle [8] = {0, 0, 0, 0, 0, 0, 0, 0};;

    if (searchType == GAME_EXACT_MATCH_Fyles) {
        const pieceT* board = searchPos->GetBoard();
        uint fyle = 0;
        for (squareT sq = A1; sq <= H8; sq++, board++) {
            if (*board == WP) {
                wpawnFyle[fyle]++;
            } else if (*board == BP) {
                bpawnFyle[fyle]++;
            }
            fyle = (fyle + 1) & 7;
        }
    }

    if (searchType == GAME_EXACT_MATCH_Exact  ||
        searchType == GAME_EXACT_MATCH_Pawns) {
        doHomePawnChecks = true;
        search_whiteHPawns = calcHomePawnMask (WP, searchPos->GetBoard());
        search_blackHPawns = calcHomePawnMask (BP, searchPos->GetBoard());
    }
    check_pawnMaskWhite = check_pawnMaskBlack = false;

    while (err == OK) {
        const pieceT* currentBoard = CurrentPos->GetBoard();
        const pieceT* board = searchPos->GetBoard();
        const pieceT* b1 = currentBoard;
        const pieceT* b2 = board;
        bool found = true;

        // If NO_SPEEDUPS is defined, a slower search is done without
        // optimisations that detect insufficient material.
#ifndef NO_SPEEDUPS
        // Insufficient material optimisation:
        if (searchPos->GetCount(WHITE) > CurrentPos->GetCount(WHITE)  ||
            searchPos->GetCount(BLACK) > CurrentPos->GetCount(BLACK)) {
            return false;
        }
        // Insufficient pawns optimisation:
        if (searchPos->PieceCount(WP) > CurrentPos->PieceCount(WP)  ||
            searchPos->PieceCount(BP) > CurrentPos->PieceCount(BP)) {
            return false;
        }

        // HomePawn mask optimisation:
        // If current pos doesn't have a pawn on home rank where
        // the search pos has one, it can never match.
        // This happens when (current_xxHPawns & search_xxHPawns) is
        // not equal to search_xxHPawns.
        // We do not do this optimisation for a pawn files search,
        // because the exact pawn squares are not important there.

        if (searchType != GAME_EXACT_MATCH_Fyles) {
            if (check_pawnMaskWhite) {
                current_whiteHPawns = calcHomePawnMask (WP, currentBoard);
                if ((current_whiteHPawns & search_whiteHPawns)
                        != search_whiteHPawns) {
                    return false;
                }
            }
            if (check_pawnMaskBlack) {
                current_blackHPawns = calcHomePawnMask (BP, currentBoard);
                if ((current_blackHPawns & search_blackHPawns)
                        != search_blackHPawns) {
                    return false;
                }
            }
        }
#endif  // #ifndef NO_SPEEDUPS

        // Not correct color: skip to next move
        if (searchPos->GetToMove() != CurrentPos->GetToMove()) {
            //skip++;
            goto Move_Forward;
        }

        // Extra material: skip to next move
        if (searchPos->GetCount(WHITE) < CurrentPos->GetCount(WHITE)  ||
            searchPos->GetCount(BLACK) < CurrentPos->GetCount(BLACK)) {
            //skip++;
            goto Move_Forward;
        }
        // Extra pawns/pieces: skip to next move
        if (searchPos->PieceCount(WP) != CurrentPos->PieceCount(WP)  ||
            searchPos->PieceCount(BP) != CurrentPos->PieceCount(BP)  ||
            searchPos->PieceCount(WN) != CurrentPos->PieceCount(WN)  ||
            searchPos->PieceCount(BN) != CurrentPos->PieceCount(BN)  ||
            searchPos->PieceCount(WB) != CurrentPos->PieceCount(WB)  ||
            searchPos->PieceCount(BB) != CurrentPos->PieceCount(BB)  ||
            searchPos->PieceCount(WR) != CurrentPos->PieceCount(WR)  ||
            searchPos->PieceCount(BR) != CurrentPos->PieceCount(BR)  ||
            searchPos->PieceCount(WQ) != CurrentPos->PieceCount(WQ)  ||
            searchPos->PieceCount(BQ) != CurrentPos->PieceCount(BQ)) {
            //skip++;
            goto Move_Forward;
        }

        // NOW, compare the actual boards piece-by-piece.
        if (searchType == GAME_EXACT_MATCH_Exact) {
            if (searchPos->HashValue() == CurrentPos->HashValue()) {
                for (squareT sq = A1;  sq <= H8;  sq++, b1++, b2++) {
                    if (*b1 != *b2) { found = false; break; }
                }
            } else {
                found = false;
            }
        } else if (searchType == GAME_EXACT_MATCH_Pawns) {
            if (searchPos->PawnHashValue() == CurrentPos->PawnHashValue()) {
                for (squareT sq = A1;  sq <= H8;  sq++, b1++, b2++) {
                    if (*b1 != *b2  &&  (*b1 == WP  ||  *b1 == BP)) {
                        found = false;
                        break;
                    }
                }
            } else {
                found = false;
            }
        } else if (searchType == GAME_EXACT_MATCH_Fyles) {
            for (fyleT f = A_FYLE; f <= H_FYLE; f++) {
                if (searchPos->FyleCount(WP,f) != CurrentPos->FyleCount(WP,f)
                      || searchPos->FyleCount(BP,f) != CurrentPos->FyleCount(BP,f)) {
                    found = false;
                    break;
                }
            }
        } else {
            // searchType == GAME_EXACT_Match_Material, so do nothing.
        }

        if (found) {
            // Found a match! Set the returned next-move:
            if (sm) {  // We need to decode the next move.
                if (buf == NULL) {
                    MoveForward();
                    if (CurrentMove->marker == END_MARKER) {
                        // Position matched at last move in the game.
                        sm->from = sm->to = NULL_SQUARE;
                        sm->promote = EMPTY;
                    } else {
                        *sm = CurrentMove->prev->moveData;
                        MoveBackup();
                    }
                } else {
                    err = DecodeNextMove(buf, *sm);
                    if (err != OK) {
                        // Position matched at last move in the game.
                        sm->from = sm->to = NULL_SQUARE;
                        sm->promote = EMPTY;
                    }
                }
            }
            return true;
        }

    Move_Forward:
#ifndef NO_SPEEDUPS
        if (doHomePawnChecks) {
            check_pawnMaskWhite = false;
            check_pawnMaskBlack = false;
            rankT rTo = square_Rank (CurrentMove->moveData.to);
            rankT rFrom = square_Rank (CurrentMove->moveData.from);
            // We only re-check the home pawn masks when something moves
            // to or from the 2nd/7th rank:
            if (rTo == RANK_2  ||  rFrom == RANK_2) {
                check_pawnMaskWhite = true;
            }
            if (rTo == RANK_7  ||  rFrom == RANK_7) {
                check_pawnMaskBlack = true;
            }
        }
#endif
        if (buf == NULL) {
            MoveForward ();
            if (CurrentMove->marker == END_MARKER) {
                err = ERROR_EndOfMoveList;
            }
        } else {
            simpleMoveT nextMove;
            err = DecodeNextMove(buf, nextMove);
            if (err == OK) {
                CurrentPos->DoSimpleMove(&nextMove);
            }
            if (err != OK  &&  err != ERROR_EndOfMoveList) {
                return false;
            }
        }
        plyCount++;
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::VarExactMatch():
//    Like ExactMatch(), but also searches in variations.
//    This is much slower than ExactMatch(), since it will
//    search every position until a match is found.
bool
Game::VarExactMatch (Position * searchPos, gameExactMatchT searchType)
{
    uint wpawnFyle [8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint bpawnFyle [8] = {0, 0, 0, 0, 0, 0, 0, 0};;

    if (searchType == GAME_EXACT_MATCH_Fyles) {
        const pieceT* board = searchPos->GetBoard();
        uint fyle = 0;
        for (squareT sq = A1; sq <= H8; sq++, board++) {
            if (*board == WP) {
                wpawnFyle[fyle]++;
            } else if (*board == BP) {
                bpawnFyle[fyle]++;
            }
            fyle = (fyle + 1) & 7;
        }
    }

    errorT err = OK;
    while (err == OK) {
        // Check if this position matches:
        bool match = false;
        if (searchPos->GetToMove() == CurrentPos->GetToMove()
            &&  searchPos->GetCount(WHITE) == CurrentPos->GetCount(WHITE)
            &&  searchPos->GetCount(BLACK) == CurrentPos->GetCount(BLACK)
            &&  searchPos->PieceCount(WP) == CurrentPos->PieceCount(WP)
            &&  searchPos->PieceCount(BP) == CurrentPos->PieceCount(BP)
            &&  searchPos->PieceCount(WN) == CurrentPos->PieceCount(WN)
            &&  searchPos->PieceCount(BN) == CurrentPos->PieceCount(BN)
            &&  searchPos->PieceCount(WB) == CurrentPos->PieceCount(WB)
            &&  searchPos->PieceCount(BB) == CurrentPos->PieceCount(BB)
            &&  searchPos->PieceCount(WR) == CurrentPos->PieceCount(WR)
            &&  searchPos->PieceCount(BR) == CurrentPos->PieceCount(BR)
            &&  searchPos->PieceCount(WQ) == CurrentPos->PieceCount(WQ)
            &&  searchPos->PieceCount(BQ) == CurrentPos->PieceCount(BQ)) {
            match = true;
            const pieceT* b1 = CurrentPos->GetBoard();
            const pieceT* b2 = searchPos->GetBoard();
            if (searchType == GAME_EXACT_MATCH_Pawns) {
                for (squareT sq = A1;  sq <= H8;  sq++, b1++, b2++) {
                    if (*b1 != *b2  &&  (*b1 == WP  ||  *b1 == BP)) {
                        match = false; break;
                    }
                }
            } else if (searchType == GAME_EXACT_MATCH_Fyles) {
                uint wpf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
                uint bpf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
                uint fyle = 0;
                for (squareT sq = A1;  sq <= H8;  sq++, b1++) {
                    if (*b1 == WP) {
                        wpf[fyle]++;
                        if (wpf[fyle] > wpawnFyle[fyle]) { match = false; break; }
                    } else if (*b1 == BP) {
                        bpf[fyle]++;
                        if (bpf[fyle] > bpawnFyle[fyle]) { match = false; break; }
                    }
                    fyle = (fyle + 1) & 7;
                }
            } else if (searchType == GAME_EXACT_MATCH_Exact) {
                if (searchPos->HashValue() == CurrentPos->HashValue()) {
                    for (squareT sq = A1;  sq <= H8;  sq++, b1++, b2++) {
                        if (*b1 != *b2) { match = false; break; }
                    }
                } else {
                    match = false;
                }
            } else {
                // searchType == GAME_EXACT_MATCH_Material, so do nothing.
            }
        }
        if (match) { return true; }

        // Now try searching each variation in turn:
        for (uint i=0; i < CurrentMove->numVariations; i++) {
            MoveIntoVariation (i);
            match = VarExactMatch (searchPos, searchType);
            MoveExitVariation();
            if (match) { return true; }
        }
        // Continue down this variation:
        MoveForward();
        if (CurrentMove->marker == END_MARKER) {
            err = ERROR_EndOfMoveList;
        }
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::GetPartialMoveList():
//      Write the first few moves of a game.
//
errorT
Game::GetPartialMoveList (DString * outStr, uint plyCount)
{
    // First, copy the relevant data so we can leave the game state
    // unaltered:
    auto location = currentLocation();

    MoveToStart();
    char temp [80];
    for (uint i=0; i < plyCount; i++) {
        if (CurrentMove->marker == END_MARKER) {
            break;
        }
        if (i != 0) { outStr->Append (" "); }
        if (i == 0  ||  CurrentPos->GetToMove() == WHITE) {
            sprintf (temp, "%d%s", CurrentPos->GetFullMoveCount(),
                     (CurrentPos->GetToMove() == WHITE ? "." : "..."));
            outStr->Append (temp);
        }
        moveT * m = CurrentMove;
        if (m->san[0] == 0) {
            CurrentPos->MakeSANString(&(m->moveData),
                                      m->san, SAN_CHECKTEST);
        }
        // add one space for indenting to work out right
        outStr->Append (" ");
        outStr->Append (m->san);
        MoveForward();
    }

    // Now reconstruct the original game state:
    restoreLocation(location);
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Returns the SAN representation of the next move or an empty string ("") if
// not at a move.
const char* Game::GetNextSAN() {
	ASSERT(!CurrentMove->endMarker() || *CurrentMove->san == '\0');

	if (!CurrentMove->endMarker() && *CurrentMove->san == '\0') {
		CurrentPos->MakeSANString(
		    &CurrentMove->moveData, CurrentMove->san,
		    CurrentMove->next->endMarker() ? SAN_MATETEST : SAN_CHECKTEST);
	}
	return CurrentMove->san;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::GetSAN():
//      Print the SAN representation of the current move to a string.
//      Prints an empty string ("") if not at a move.
void Game::GetSAN(char* str) {
	ASSERT(str != NULL);
	strcpy(str, GetNextSAN());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::GetPrevSAN():
//      Print the SAN representation of the previous move to a string.
//      Prints an empty string ("") if not at a move.
void
Game::GetPrevSAN (char * str)
{
    ASSERT (str != NULL);
    moveT * m = CurrentMove->prev;
    if (m->marker == START_MARKER  ||  m->marker == END_MARKER) {
        str[0] = 0;
        return;
    }
    if (m->san[0] == 0) {
        MoveBackup();
        CurrentPos->MakeSANString (&(m->moveData), m->san, SAN_MATETEST);
        MoveForward();
    }
    strcpy (str, m->san);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::GetPrevMoveUCI():
//      Print the UCI representation of the current move to a string.
//      Prints an empty string ("") if not at a move.
void Game::GetPrevMoveUCI(char* str) const {
    ASSERT(str != NULL);
    const auto m = CurrentMove->prev;
    if (!m->startMarker())
        str = m->moveData.toLongNotation(str);

    *str = '\0';
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::GetNextMoveUCI():
//      Print the UCI representation of the next move to a string.
//      Prints an empty string ("") if not at a move.
void
Game::GetNextMoveUCI (char * str)
{
    ASSERT (str != NULL);
    if (!CurrentMove->endMarker())
        str = CurrentMove->moveData.toLongNotation(str);

    *str = '\0';
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// commentEmpty:
//    Called by WriteMoveList to check there is really
//    something to print given display options.
//    comment is supposed to be non null
bool
Game::CommentEmpty ( const char * comment)
{
    char * s = NULL;
    bool ret = false;

    if (comment == NULL)
      return true;

    if (comment[0] == '\0')
      return true;

    if (PgnStyle & PGN_STYLE_STRIP_MARKS) {
      s = strDuplicate (comment);
      strTrimMarkCodes (s);
      char * tmp = s;
      bool empty = true;
      while (tmp[0] != 0) {
        if (tmp[0] != ' ') {
          empty = false;
          break;
        }
        tmp++;
      }
      ret = empty;

      delete[] s;
    }

    return ret;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// writeComment:
//    Called by WriteMoveList to write a single comment.
void
Game::WriteComment (TextBuffer * tb, const char * preStr,
              const char * comment, const char * postStr)
{
    char* s_duplicate = nullptr;
    const char* s = comment;

    if (PgnStyle & PGN_STYLE_STRIP_MARKS) {
		s_duplicate = strDuplicate(comment);
		strTrimMarkCodes(s_duplicate);
		s = s_duplicate;
	}

    if (s[0] != '\0') {

        if (IsColorFormat()) {
            tb->PrintString ("<c_");
            tb->PrintInt (NumMovesPrinted);
            tb->PrintChar ('>');
        }

        if (IsColorFormat()) {
            // Translate "<", ">" in comments:
            tb->AddTranslation ('<', "<lt>");
            tb->AddTranslation ('>', "<gt>");
            // S.A any issues ?
            tb->NewlinesToSpaces (0);
            tb->PrintString (s);
            tb->ClearTranslation ('<');
            tb->ClearTranslation ('>');
        } else {
            tb->PrintString (preStr);
            tb->PrintString (s);
            tb->PrintString (postStr);
        }

        if (IsColorFormat()) { tb->PrintString ("</c>"); }
    }

    if (PgnStyle & PGN_STYLE_STRIP_MARKS) {
        delete[] s_duplicate;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::WriteMoveList():
//      Write the moves, variations and comments in PGN notation.
//      Recursive; calls itself to write variations.
//
errorT Game::WriteMoveList(TextBuffer* tb, moveT* oldCurrentMove,
                           bool printMoveNum, bool inComment) {
    char tempTrans[10];
    const char * preCommentStr = "{";
    const char * postCommentStr = "}";
    const char * startTable = "\n";
    const char * startColumn = "\t";
    const char * nextColumn = "\t";
    const char * endColumn = "\n";
    const char * endTable = "\n";
    bool printDiagrams = false;

    if (IsHtmlFormat()) {
        preCommentStr = "";
        postCommentStr = "";
        startTable = "<table width=\"50%\">\n";
        startColumn = "<tr align=left>\n  <td width=\"15%\"><b>";
        nextColumn = "</b></td>\n  <td width=\"45%\" align=left><b>";
        endColumn = "</b></td>\n</tr>\n";
        endTable = "</table>\n";
        printDiagrams = true;
    }
    if (IsLatexFormat()) {
        preCommentStr = "\\begin{nochess}{\\rm ";
        postCommentStr = "}\\end{nochess}";
        startTable = "\n\\begin{tabular}{p{1cm}p{2cm}p{2cm}}\n";
        startColumn = "";
        nextColumn = "&";
        endColumn = "\\\\\n";
        endTable = "\\end{tabular}\n\n";
        printDiagrams = true;
    }
    if (IsColorFormat()) {
        startTable = "<br>";
        endColumn = "<br>";
    }

    if (IsHtmlFormat()  &&  VarDepth == 0) { tb->PrintString ("<b>"); }
    if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
        tb->PrintString (startTable);
    }

    if (IsPlainFormat()  &&  inComment) {
        preCommentStr = "";
        postCommentStr = "";
    }
    moveT * m = CurrentMove;

    // Print null moves:
    if ((PgnStyle & PGN_STYLE_NO_NULL_MOVES) && !inComment &&
            IsPlainFormat() && m->isNull()) {
        inComment = true;
        tb->PrintString(preCommentStr);
        preCommentStr = "";
        postCommentStr = "";
    }

    // If this is a variation and it starts with a comment, print it:
    if ((VarDepth > 0 || CurrentMove->prev == FirstMove) && 
            ! CurrentMove->prev->comment.empty()) {
        if (PgnStyle & PGN_STYLE_COMMENTS) {
            WriteComment (tb, preCommentStr, CurrentMove->prev->comment.c_str(),
                          postCommentStr);
            tb->PrintSpace();
            if (!VarDepth) {
                tb->ClearTranslation ('\n');
                tb->NewLine();
                if (IsColorFormat() || IsLatexFormat()) {
                    tb->NewLine();
                }
            }
        }
    }

    while (CurrentMove->marker != END_MARKER) {
        moveT *m = CurrentMove;
        bool commentLine = false;

        if (m->san[0] == 0) {
            // If there is a next move we can skip the SAN_MATETEST
            CurrentPos->MakeSANString(
                &(m->moveData), m->san,
                (m->next->marker != END_MARKER) ? SAN_CHECKTEST : SAN_MATETEST);
        }

        bool printThisMove = true;
        if (m->isNull()) {
            // Null moves are not printed in LaTeX or HTML:
            if (IsLatexFormat()  ||  IsHtmlFormat()) {
                printThisMove = false;
                printMoveNum = true;
            }
            // If Plain PGN format, check whether to convert the
            // null move and remainder of the line to a comment:
            if ((PgnStyle & PGN_STYLE_NO_NULL_MOVES)  &&  IsPlainFormat()) {
                if (!inComment) {
                    // Enter inComment mode to convert rest of line
                    // to a comment:
                    inComment = true;
                    tb->PrintString(preCommentStr);
                    preCommentStr = "";
                    postCommentStr = "";
                }
                printThisMove = false;
                printMoveNum = true;
            }
        }
        int colWidth = 6;
        NumMovesPrinted++;

        if (printThisMove) {
        // Print the move number and following dots if necessary:
        if (IsColorFormat()) {
            tb->PrintString ("<m_");
            tb->PrintInt (NumMovesPrinted);
            tb->PrintChar ('>');
        }
        if (printMoveNum  ||  (CurrentPos->GetToMove() == WHITE)) {
            if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
                tb->PrintString (startColumn);
                char temp [10];
                sprintf (temp, "%4u.", CurrentPos->GetFullMoveCount());
                tb->PrintString (temp);
                if (CurrentPos->GetToMove() == BLACK) {
                    tb->PauseTranslations();
                    tb->PrintString (nextColumn);
                    tb->PrintString ("...");
                    if (IsPlainFormat()  ||  IsColorFormat()) {
                        tb->PrintString ("        ");
                    }
                    tb->ResumeTranslations();
                }
            } else {
            if (PgnStyle & PGN_STYLE_MOVENUM_SPACE) {
                tb->PrintInt(CurrentPos->GetFullMoveCount(), (CurrentPos->GetToMove() == WHITE ? "." : ". ..."));
                } else {
                    tb->PrintInt(CurrentPos->GetFullMoveCount(), (CurrentPos->GetToMove() == WHITE ? "." : "..."));
                }
                if (PgnStyle & PGN_STYLE_MOVENUM_SPACE) {
                    if (IsLatexFormat()) {
                        tb->PrintChar ('~');
                    } else {
                        tb->PrintChar (' ');
                    }
                }
            }
            printMoveNum = false;
        }

        // Now print the move: only regenerate the SAN string if necessary.

        if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
            tb->PauseTranslations();
            tb->PrintString (nextColumn);
            tb->ResumeTranslations();
        }
        if (IsColorFormat() && (PgnStyle & PGN_STYLE_UNICODE)) {
            char buf[100];
            char* q = buf;

            for (char const* p = m->san; *p; ++p) {
                ASSERT(q - buf < static_cast<std::ptrdiff_t>(sizeof(buf) - 4));

                switch (*p) {
                    case 'K':    q = std::copy_n("\xe2\x99\x94", 3, q); break;
                    case 'Q':    q = std::copy_n("\xe2\x99\x95", 3, q); break;
                    case 'R':    q = std::copy_n("\xe2\x99\x96", 3, q); break;
                    case 'B':    q = std::copy_n("\xe2\x99\x97", 3, q); break;
                    case 'N':    q = std::copy_n("\xe2\x99\x98", 3, q); break;
                    case 'P':    q = std::copy_n("\xe2\x99\x99", 3, q); break;
                    default:    *q++ = *p; break;
                }

            }
            *q = '\0';
            tb->PrintWord (buf);
        } else {
            // translate pieces
            strcpy(tempTrans, m->san);
            transPieces(tempTrans);
            //tb->PrintWord (m->san);
            tb->PrintWord (tempTrans);
        }
        colWidth -= (int) std::strlen(m->san);
        if (IsColorFormat()) {
            tb->PrintString ("</m>");
        }
        }

        bool endedColumn = false;

        // Print NAGs and comments if the style indicates:

        if (PgnStyle & PGN_STYLE_COMMENTS) {
            bool printDiagramHere = false;
            if (IsColorFormat()  &&  m->nagCount > 0) {
                tb->PrintString ("<nag>");
            }
            for (uint i = 0; i < (uint) m->nagCount; i++) {
                char temp[20];
                game_printNag (m->nags[i], temp, PgnStyle & PGN_STYLE_SYMBOLS,
                               PgnFormat);

                // Do not print a space before the Nag if it is the
                // first nag and starts with "!" or "?" -- those symbols
                // look better printed next to the move:

                if (i > 0  ||  (temp[0] != '!'  &&  temp[0] != '?')) {
                    tb->PrintSpace();
                    colWidth--;
                }
                if (printDiagrams  &&  m->nags[i] == NAG_Diagram) {
                    printDiagramHere = true;
                }
                tb->PrintWord (temp);
                colWidth -= (int) std::strlen(temp);

            }
            if (IsColorFormat()  &&  m->nagCount > 0) {
                tb->PrintString ("</nag>");
            }
            tb->PrintSpace();
            colWidth--;
            if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
                if (IsPlainFormat()  ||  IsColorFormat()) {
                    while (colWidth-- > 0) { tb->PrintSpace(); }
                }
            }

            if (printDiagramHere) {
                if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
                    if (! endedColumn) {
                        if (CurrentPos->GetToMove() == WHITE) {
                            tb->PauseTranslations ();
                            tb->PrintString (nextColumn);
                            tb->ResumeTranslations ();
                        }
                        tb->PrintString (endColumn);
                        tb->PrintString (endTable);
                        endedColumn = true;
                    }
                }
                if (IsHtmlFormat()  &&  VarDepth == 0) {
                    tb->PrintString ("</b>");
                }
                if (IsLatexFormat()) {
                    // The commented-out code below will print diagrams
                    // in variations smaller than game diagrams:
                    //if (VarDepth == 0) {
                    //    tb->PrintString("\n\\font\\Chess=chess20\n");
                    //} else {
                    //    tb->PrintString("\n\\font\\Chess=chess10\n");
                    //}
                    tb->PrintString ("\n\\begin{diagram}\n");
                }
                MoveForward ();
                DString * dstr = new DString;
                if (IsHtmlFormat()) {
                    CurrentPos->DumpHtmlBoard (dstr, HtmlStyle, NULL);
                } else {
                    CurrentPos->DumpLatexBoard (dstr);
                }
                MoveBackup ();
                tb->PrintString (dstr->Data());
                delete dstr;
                if (IsHtmlFormat()  &&  VarDepth == 0) {
                    tb->PrintString ("<b>");
                }
                if (IsLatexFormat()) {
                    tb->PrintString ("\n\\end{diagram}\n");
                }
                printMoveNum = true;
            }

            if (!m->comment.empty() && ! CommentEmpty(m->comment.c_str()) ) {
                if (!inComment && IsPlainFormat()  &&
                    (PgnStyle & PGN_STYLE_NO_NULL_MOVES)) {
                    // If this move has no variations, but the next move
                    // is a null move, enter inComment mode:
                    if (m->next->isNull()  &&
                          ((!(PgnStyle & PGN_STYLE_VARS))  ||
                            (CurrentMove->next->numVariations == 0))) {
                        inComment = true;
                        tb->PrintString(preCommentStr);
                        preCommentStr = "";
                        postCommentStr = "";
                    }
                }

/* Code commented to remove extra lines
                if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
                       if (! endedColumn) {
                           if (CurrentPos->GetToMove() == WHITE) {
                               tb->PauseTranslations ();
                               tb->PrintString (nextColumn);
                               tb->ResumeTranslations ();
                           }
                           tb->PrintString (endColumn);
                           tb->PrintString (endTable);
                           endedColumn = true;
                       }
                }
*/
                if (IsHtmlFormat()  &&  VarDepth == 0) {
                    tb->PrintString ("</b><dl><dd>");
                }
                if ((PgnStyle & PGN_STYLE_INDENT_COMMENTS) && VarDepth == 0) {
                    if (IsColorFormat()) {
                        tb->PrintString ("<br><ip1>");
                    } else {
                        tb->SetIndent (tb->GetIndent() + 4); tb->Indent();
                    }
                }

                WriteComment (tb, preCommentStr, m->comment.c_str(), postCommentStr);

                if ((PgnStyle & PGN_STYLE_INDENT_COMMENTS) && VarDepth == 0) {
                    if (IsColorFormat()) {
                        tb->PrintString ("</ip1><br>");
                        commentLine = true;
                    } else {
                        tb->SetIndent (tb->GetIndent() - 4); tb->Indent();
                    }
                } else {
                    tb->PrintSpace();
                }
                if (printDiagrams  &&  strIsPrefix ("#", m->comment.c_str())) {
                    if (IsLatexFormat()) {
                        tb->PrintString ("\n\\begin{diagram}\n");
                    }
                    MoveForward ();
                    DString * dstr = new DString;
                    if (IsHtmlFormat()) {
                        CurrentPos->DumpHtmlBoard (dstr, HtmlStyle, NULL);
                    } else {
                        CurrentPos->DumpLatexBoard (dstr);
                    }
                    MoveBackup ();
                    tb->PrintString (dstr->Data());
                    if (IsLatexFormat()) {
                        tb->PrintString ("\n\\end{diagram}\n");
                    }
                    delete dstr;
                }
                if (IsHtmlFormat() && VarDepth == 0) {
                    tb->PrintString ("</dl><b>");
                }
                printMoveNum = true;
            }
        } else {
            tb->PrintSpace();
        }

        // Print any variations if the style indicates:
        if ((PgnStyle & PGN_STYLE_VARS)  &&  (m->numVariations > 0)) {
            if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
                if (! endedColumn) {
                    if (CurrentPos->GetToMove() == WHITE) {
                        tb->PauseTranslations ();
                        tb->PrintString (nextColumn);
                        tb->ResumeTranslations ();
                    }
                    // Doesn't seem wanted!! S.A (see a few lines below)
                    // tb->PrintString (endColumn);
                    tb->PrintString (endTable);
                    endedColumn = true;
                }
            }
            if (IsColorFormat()  &&  VarDepth == 0) { tb->PrintString ("<var>"); }
            // Doesn't indent first var in column mode properly 
            // if including !(PgnStyle & PGN_STYLE_COLUMN) here.
            // But as-is, depth 3 vars don't indent in COLUMN mode (bug)
            if ((PgnStyle & PGN_STYLE_INDENT_VARS) && IsColorFormat()) {
                if ( !commentLine ) {
                    tb->PrintString ("<br>");
                }
            }
            for (uint i=0; i < m->numVariations; i++) {
                if (PgnStyle & PGN_STYLE_INDENT_VARS) {
                    if (IsColorFormat()) {
                        if (VarDepth < 19) {
                            char tmp_str[16];
                            sprintf(tmp_str, "<ip%u>", VarDepth + 1);
                            tb->PrintString(tmp_str);
                        }
                    } else {
                        tb->SetIndent (tb->GetIndent() + 4); tb->Indent();
                    }
                }
                if (IsHtmlFormat()) {
                    if (VarDepth == 0) { tb->PrintString ("</b><dl><dd>"); }
                }
                if (IsLatexFormat()  &&  VarDepth == 0) {
                    if (PgnStyle & PGN_STYLE_INDENT_VARS) {
                        tb->PrintLine ("\\begin{variation}");
                    } else {
                        tb->PrintString ("{\\rm ");
                    }
                }
                if (IsColorFormat()) { tb->PrintString ("<blue>"); }

                // Note tabs in column mode don't work after this VarDepth>1 for some reason
                // this VarDepth check is redundant i think 
                if (!IsLatexFormat()  ||  VarDepth != 0) {
                    tb->PrintChar ('(');
                }

                MoveIntoVariation (i);
                NumMovesPrinted++;
                tb->PrintSpace();

                // Recursively print the variation:
                WriteMoveList (tb, oldCurrentMove, true, inComment);

                MoveExitVariation();
                if (!IsLatexFormat()  ||  VarDepth != 0) {
                    tb->PrintChar (')');
                }
                if (IsColorFormat()) { tb->PrintString ("<blue>"); }
                if (IsHtmlFormat()) {
                    if (VarDepth == 0) { tb->PrintString ("</dl><b>"); }
                }
                if (IsLatexFormat()  &&  VarDepth == 0) {
                    if (PgnStyle & PGN_STYLE_INDENT_VARS) {
                        tb->PrintLine ("\\end{variation}");
                    } else {
                        tb->PrintString ("}");
                    }
                }
                if (PgnStyle & PGN_STYLE_INDENT_VARS) {
                    if (IsColorFormat()) {
                        if (VarDepth < 19) {
                            char tmp_str[16];
                            sprintf(tmp_str, "</ip%u><br>", VarDepth + 1);
                            tb->PrintString(tmp_str);
                        }
                    } else {
                        tb->SetIndent (tb->GetIndent() - 4); tb->Indent();
                    }
                } else { tb->PrintSpace(); }
                printMoveNum = true;
            }
            if (IsColorFormat()  &&  VarDepth == 0) {
                tb->PrintString ("</var>");
            }
        }
        if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
            if (endedColumn) { tb->PrintString(startTable); }
            if (!endedColumn  &&  CurrentPos->GetToMove() == BLACK) {
                tb->PrintString (endColumn);
                endedColumn = true;
            }
        }
        MoveForward();
    }
    if (inComment) { tb->PrintString ("}"); }
    if (IsHtmlFormat()  &&  VarDepth == 0) { tb->PrintString ("</b>"); }
    if ((PgnStyle & PGN_STYLE_COLUMN)  &&  VarDepth == 0) {
        tb->PrintString(endTable);
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::WritePGN():
//      Write a game in PGN to a textbuffer.
//
errorT Game::WritePGN(TextBuffer* tb) {
    char temp[256];
    char dateStr [20];
    const char * newline = "\n";
    tb->NewlinesToSpaces (false);
    if (IsHtmlFormat()) { newline = "<br>\n"; }
    if (IsLatexFormat()) {
        newline = "\\\\\n";
        tb->AddTranslation ('#', "\\#");
        tb->AddTranslation ('%', "\\%");
        tb->AddTranslation ('&', "\\&");
        tb->AddTranslation ('<', "$<$");
        tb->AddTranslation ('>', "$>$");
        tb->AddTranslation ('_', "\\_");
        // tb->AddTranslation ('[', "$[$");
        // tb->AddTranslation (']', "$]$");
    }
    if (IsColorFormat()) {
        newline = "<br>";
    }

    if (PgnStyle & PGN_STYLE_COLUMN) {
        PgnStyle |= PGN_STYLE_INDENT_COMMENTS;
        PgnStyle |= PGN_STYLE_INDENT_VARS;
    }

    // First: is there a pre-game comment? If so, print it:
//    if (FirstMove->comment != NULL && (PgnStyle & PGN_STYLE_COMMENTS)
//        &&  ! strIsAllWhitespace (FirstMove->comment)) {
//        tb->AddTranslation ('\n', newline);
//        char * s = FirstMove->comment;
//        if (PgnStyle & PGN_STYLE_STRIP_MARKS) {
//            s = strDuplicate (FirstMove->comment);
//            strTrimMarkCodes (s);
//        }
//        if (IsColorFormat()) {
//            sprintf (temp, "<c_%u>", NumMovesPrinted);
//            tb->PrintString (temp);
//            tb->AddTranslation ('<', "<lt>");
//            tb->AddTranslation ('>', "<gt>");
//            tb->PrintString (s);
//            tb->ClearTranslation ('<');
//            tb->ClearTranslation ('>');
//            tb->PrintLine ("</c>");
//        } else {
//            tb->PrintLine (s);
//        }
//        if (PgnStyle & PGN_STYLE_STRIP_MARKS) { delete[] s; }
//        tb->ClearTranslation ('\n');
//        tb->NewLine();
//    }

    date_DecodeToString (Date, dateStr);
    if (IsHtmlFormat()) { tb->PrintLine("<p><b>"); }
    if (IsLatexFormat()) { tb->PrintLine ("{\\bf"); }

//    if (IsColorFormat()) {
//        tb->AddTranslation ('<', "<lt>");
//        tb->AddTranslation ('>', "<gt>");
//    }

    if (PgnStyle & PGN_STYLE_SHORT_HEADER) {
        // Print tags in short, 3-line format:

        //if (IsHtmlFormat()) { tb->PrintString ("<font size=+1>"); }
        if (IsLatexFormat()) { tb->PrintString ("$\\circ$ "); }
        if (PgnFormat==PGN_FORMAT_Color) {tb->PrintString ("<tag>"); }
        tb->PrintString (GetWhiteStr());
        if (WhiteElo > 0) {
            sprintf (temp, "  (%u)", WhiteElo);
            tb->PrintString (temp);
        }
        switch (PgnFormat) {
        case PGN_FORMAT_HTML:
            tb->PrintString (" &nbsp;&nbsp; -- &nbsp;&nbsp; ");
            break;
        case PGN_FORMAT_LaTeX:
            tb->PrintString (newline);
            tb->PrintString ("$\\bullet$ ");
            break;
        default:
            tb->PrintString ("   --   ");
            break;
        }
        tb->PrintString (GetBlackStr());
        if (BlackElo > 0) {
            sprintf (temp, "  (%u)", BlackElo);
            tb->PrintString (temp);
        }
        //if (IsHtmlFormat()) { tb->PrintString ("</font>"); }
        tb->PrintString (newline);

        tb->PrintString (GetEventStr());
        if (!RoundStr.empty() && RoundStr != "?") {
            tb->PrintString (IsHtmlFormat() ? " &nbsp;(" : " (");
            tb->PrintString (GetRoundStr());
            tb->PrintString (")");
        }
        tb->PrintString (IsHtmlFormat() ? "&nbsp;&nbsp; " : "  ");
        if (IsLatexFormat()) { tb->PrintString (newline); }
        if (!SiteStr.empty() && SiteStr != "?") {
            tb->PrintString (GetSiteStr());
            tb->PrintString (newline);
        }

        // Remove ".??" or ".??.??" from end of dateStr, then print it:
        if (dateStr[4] == '.'  &&  dateStr[5] == '?') { dateStr[4] = 0; }
        if (dateStr[7] == '.'  &&  dateStr[8] == '?') { dateStr[7] = 0; }
        tb->PrintString (dateStr);

        // Print ECO code:
        tb->PrintString (IsHtmlFormat() ? " &nbsp; &nbsp; " : "  ");
        if (IsLatexFormat()) { tb->PrintString ("\\hfill "); }
        tb->PrintString (RESULT_LONGSTR[Result]);
        if (EcoCode != 0) {
            tb->PrintString (IsHtmlFormat() ? " &nbsp; &nbsp; " : "  ");
            if (IsLatexFormat()) { tb->PrintString ("\\hfill "); }
            ecoStringT ecoStr;
            eco_ToExtendedString (EcoCode, ecoStr);
            tb->PrintString (ecoStr);
        }
        auto annotator = FindExtraTag("Annotator");
        if (annotator != NULL) {
            sprintf(temp, " (%s)", annotator);
            tb->PrintString(temp);
        }

        tb->PrintString (newline);
        if (PgnFormat==PGN_FORMAT_Color) {tb->PrintString ("</tag>"); }

        // Print FEN if non-standard start:
        if (StartPos) {
            if (IsLatexFormat()) {
                tb->PrintString ("\n\\begin{diagram}\n");
                DString dstr;
                StartPos->DumpLatexBoard (&dstr);
                tb->PrintString (dstr.Data());
                tb->PrintString ("\n\\end{diagram}\n");
            } else if (IsHtmlFormat()) {
                DString dstr;
                StartPos->DumpHtmlBoard (&dstr, HtmlStyle, NULL);
                tb->PrintString (dstr.Data());
            } else {
                StartPos->PrintFEN(std::copy_n("Position: ", 10, temp),
                                   FEN_ALL_FIELDS);
                std::strcat(temp, newline);
                tb->PrintString (temp);
            }
        }
    } else {
        // Print tags in standard PGN format, one per line:
        // Note: we want no line-wrapping when printing PGN tags
        // so set it to a huge value for now:
        uint wrapColumn = tb->GetWrapColumn();
        tb->SetWrapColumn (99999);
        if (IsColorFormat()) { tb->PrintString ("<tag>"); }
        sprintf (temp, "[Event \"%s\"]%s", GetEventStr(), newline);
        tb->PrintString (temp);
        sprintf (temp, "[Site \"%s\"]%s", GetSiteStr(), newline);
        tb->PrintString (temp);
        sprintf (temp, "[Date \"%s\"]%s", dateStr, newline);
        tb->PrintString (temp);
        sprintf (temp, "[Round \"%s\"]%s", GetRoundStr(), newline);
        tb->PrintString (temp);
        sprintf (temp, "[White \"%s\"]%s", GetWhiteStr(), newline);
        tb->PrintString (temp);
        sprintf (temp, "[Black \"%s\"]%s", GetBlackStr(), newline);
        tb->PrintString (temp);
        sprintf (temp, "[Result \"%s\"]%s", RESULT_LONGSTR[Result], newline);
        tb->PrintString (temp);

        // Print all tags, not just the standard seven, if applicable:
        if (PgnStyle & PGN_STYLE_TAGS) {
            if (WhiteElo > 0) {
                sprintf (temp, "[White%s \"%u\"]%s",
                         ratingTypeNames [WhiteRatingType], WhiteElo, newline);
                tb->PrintString (temp);
            }
            if (BlackElo > 0) {
                sprintf (temp, "[Black%s \"%u\"]%s",
                         ratingTypeNames [BlackRatingType], BlackElo, newline);
                tb->PrintString (temp);
            }
            if (EcoCode != 0) {
                ecoStringT ecoStr;
                eco_ToExtendedString (EcoCode, ecoStr);
                sprintf (temp, "[ECO \"%s\"]%s", ecoStr, newline);
                tb->PrintString (temp);
            }
            if (EventDate != ZERO_DATE) {
                char edateStr [20];
                date_DecodeToString (EventDate, edateStr);
                sprintf (temp, "[EventDate \"%s\"]%s", edateStr, newline);
                tb->PrintString (temp);
            }

            if (PgnStyle & PGN_STYLE_SCIDFLAGS  &&  *ScidFlags != 0) {
                sprintf (temp, "[ScidFlags \"%s\"]%s", ScidFlags, newline);
                tb->PrintString (temp);
            }

            // Now print other tags
            for (auto& e : extraTags_) {
                sprintf(temp, "[%s \"%s\"]%s", e.first.c_str(),
                        e.second.c_str(), newline);
                tb->PrintString(temp);
            }
        }
        // Finally, write the FEN tag if necessary:
        if (StartPos) {
            StartPos->PrintFEN(std::copy_n("[FEN \"", 6, temp), FEN_ALL_FIELDS);
            auto it_end = std::copy_n("\"]", 2, temp + std::strlen(temp));
            std::strcpy(it_end, newline);
            tb->PrintString (temp);
        }
        if (IsColorFormat()) { tb->PrintString ("</tag>"); }
        // Now restore the linewrap column:
        tb->SetWrapColumn (wrapColumn);
    }

//    if (IsColorFormat()) {
//        tb->ClearTranslation ('<');
//        tb->ClearTranslation ('>');
//    }

    if (IsHtmlFormat()) { tb->PrintLine("</b></p>"); }
    if (IsLatexFormat()) {
        tb->PrintLine ("}\n\\begin{chess}{\\bf ");
    } else {
        tb->PrintString (newline);
    }

    MoveToStart();

    if (IsHtmlFormat()) { tb->PrintString ("<p>"); }
    NumMovesPrinted = 1;
    WriteMoveList(tb, CurrentMove, true, false);
    if (IsHtmlFormat()) { tb->PrintString ("<b>"); }
    if (IsLatexFormat()) { tb->PrintString ("\n}\\end{chess}\n{\\bf "); }
    if (IsColorFormat()) { tb->PrintString ("<tag>"); }
    tb->PrintWord (RESULT_LONGSTR [Result]);
    if (IsLatexFormat()) {
        tb->PrintString ("}\n\\begin{center} \\hrule \\end{center}");
    }
    if (IsHtmlFormat()) { tb->PrintString ("</b><hr></p>"); }
    if (IsColorFormat()) { tb->PrintString ("</tag>"); }
    tb->NewLine();

    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::WriteToPGN():
//      Print the entire game.
//
std::pair<const char*, unsigned>
Game::WriteToPGN(uint lineWidth, bool NewLineAtEnd, bool newLineToSpaces)
{
    static TextBuffer tbuf;

    auto location = currentLocation();
    tbuf.Empty();
    tbuf.SetWrapColumn(lineWidth ? lineWidth : tbuf.GetBufferSize());
    tbuf.NewlinesToSpaces(newLineToSpaces);
    WritePGN(&tbuf);
    if (NewLineAtEnd) tbuf.NewLine();
    restoreLocation(location);
    return std::make_pair(tbuf.GetBuffer(), tbuf.GetByteCount());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::LoadStandardTags():
//      Sets the standard tag values for this game, given an
//      index file entry and a namebase that stores the
//      player/site/event/round names.
//
void
Game::LoadStandardTags (const IndexEntry* ie, const NameBase* nb)
{
    ASSERT (ie != NULL  &&  nb != NULL);
    SetEventStr (ie->GetEventName (nb));
    SetSiteStr (ie->GetSiteName (nb));
    SetWhiteStr (ie->GetWhiteName (nb));
    SetBlackStr (ie->GetBlackName (nb));
    SetRoundStr (ie->GetRoundName (nb));
    SetDate (ie->GetDate());
    SetEventDate (ie->GetEventDate());
    SetWhiteElo (ie->GetWhiteElo());
    SetBlackElo (ie->GetBlackElo());
    WhiteEstimateElo = nb->GetElo (ie->GetWhite());
    BlackEstimateElo = nb->GetElo (ie->GetBlack());
    SetWhiteRatingType (ie->GetWhiteRatingType());
    SetBlackRatingType (ie->GetBlackRatingType());
    SetResult (ie->GetResult());
    SetEco (ie->GetEcoCode());
    ie->GetFlagStr (ScidFlags, NULL);
}

eloT
Game::GetAverageElo ()
{
    eloT white = WhiteElo;
    eloT black = BlackElo;
    if (white == 0) { white = WhiteEstimateElo; }
    if (black == 0) { black = BlackEstimateElo; }
    return (white + black) / 2;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// makeMoveByte(): inline routine used for encoding most moves.
//
static inline byte
makeMoveByte (byte pieceNum, byte value)
{
    ASSERT (pieceNum <= 15  &&  value <= 15);
    return (byte)((pieceNum & 15) << 4)  |  (byte)(value & 15);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encodeKing(): encoding of King moves.
//
static byte encodeKing(const simpleMoveT* sm) {
    // Valid King difference-from-old-square values are:
    // -9, -8, -7, -1, 1, 7, 8, 9, and -2 and 2 for castling.
    // To convert this to a val in the range [1-10], we add 9 and
    // then look up the val[] table.
    // Coded values 1-8 are one-square moves; 9 and 10 are Castling.

    ASSERT(sm->pieceNum == 0);  // Kings MUST be piece Number zero.
    int diff = (int) sm->to - (int) sm->from;
    static const byte val[] = {
    /* -9 -8 -7 -6 -5 -4 -3 -2 -1  0  1   2  3  4  5  6  7  8  9 */
        1, 2, 3, 0, 0, 0, 0, 9, 4, 0, 5, 10, 0, 0, 0, 0, 6, 7, 8
    };

    // If target square is the from square, it is the null move, which
    // is represented as a king move to its own square and is encoded
    // as the byte value zero.
    if (sm->to == sm->from) {
        return 0;
    }

    // Verify we have a valid King move:
    ASSERT(diff >= -9  &&  diff <= 9  &&  val[diff+9] != 0);
    return val[diff + 9];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encodeKnight(): encoding Knight moves.
//
static byte encodeKnight(const simpleMoveT* sm) {
    // Valid Knight difference-from-old-square values are:
    // -17, -15, -10, -6, 6, 10, 15, 17.
    // To convert this to a value in the range [1-8], we add 17 to
    // the difference and then look up the val[] table.

    int diff = (int) sm->to - (int) sm->from;
    static const byte val[] = {
    /* -17 -16 -15 -14 -13 -12 -11 -10 -9 -8 -7 -6 -5 -4 -3 -2 -1  0 */
        1,  0,  2,  0,  0,  0,  0,  3,  0, 0, 0, 4, 0, 0, 0, 0, 0, 0,

    /*  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 */
        0, 0, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 0, 7, 0, 8
    };

    // Verify we have a valid knight move:
    ASSERT (diff >= -17  &&  diff <= 17  &&  val[diff + 17] != 0);
    return val[diff + 17];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encodeRook(): encoding rook moves.
//
static byte encodeRook(const simpleMoveT* sm) {
    // Valid Rook moves are to same rank, OR to same fyle.
    // We encode the 8 squares on the same rank 0-8, and the 8
    // squares on the same fyle 9-15. This means that for any particular
    // rook move, two of the values in the range [0-15] will be
    // meaningless, as they will represent the from-square.

    ASSERT (sm->from <= H8  &&  sm->to <= H8);

    // Check if the two squares share the same rank:
    if (square_Rank(sm->from) == square_Rank(sm->to)) {
        return square_Fyle(sm->to);
    }
    return 8 + square_Rank(sm->to);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encodeBishop(): encoding Bishop moves.
//
static byte encodeBishop(const simpleMoveT* sm) {
    // We encode a Bishop move as the Fyle moved to, plus
    // a one-bit flag to indicate if the direction was
    // up-right/down-left or vice versa.

    ASSERT (sm->to <= H8  &&  sm->from <= H8);
    int rankdiff = (int)square_Rank(sm->to) - (int)square_Rank(sm->from);
    int fylediff = (int)square_Fyle(sm->to) - (int)square_Fyle(sm->from);

    // If (rankdiff * fylediff) is negative, it's up-left/down-right:
    if (rankdiff * fylediff < 0)
        return square_Fyle(sm->to) + 8;

    return square_Fyle(sm->to);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encodeQueen(): encoding Queen moves.
//
template <typename DestT>
void encodeQueen(DestT* buf, const simpleMoveT* sm) {
    // We cannot fit all Queen moves in one byte, so Rooklike moves
    // are in one byte (encoded the same way as Rook moves),
    // while diagonal moves are in two bytes.

    ASSERT (sm->to <= H8  &&  sm->from <= H8);
    byte val;

    if (square_Rank(sm->from) == square_Rank(sm->to)) {
        // Rook-horizontal move:

        val = square_Fyle(sm->to);
        buf->emplace_back(makeMoveByte (sm->pieceNum, val));

    } else if (square_Fyle(sm->from) == square_Fyle(sm->to)) {
        // Rook-vertical move:

        val = 8 + square_Rank(sm->to);
        buf->emplace_back(makeMoveByte (sm->pieceNum, val));

    } else {
        // Diagonal move:
        ASSERT(std::abs(sm->to / 8 - sm->from / 8) ==
               std::abs(sm->to % 8 - sm->from % 8));

        // First, we put a rook-horizontal move to the from square (which
        // is illegal of course) to indicate it is NOT a rooklike move:

        val = square_Fyle(sm->from);
        buf->emplace_back(makeMoveByte (sm->pieceNum, val));

        // Now we put the to-square in the next byte. We add a 64 to it
        // to make sure that it cannot clash with the Special tokens (which
        // are in the range 0 to 15, since they are special King moves).

        buf->emplace_back(sm->to + 64);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// encodePawn(): encoding Pawn moves.
//
static byte encodePawn(const simpleMoveT* sm) {
    // Pawn moves require a promotion encoding.
    // The pawn moves are:
    // 0 = capture-left,
    // 1 = forward,
    // 2 = capture-right (all no promotion);
    //    3/4/5 = 0/1/2 with Queen promo;
    //    6/7/8 = 0/1/2 with Rook promo;
    //  9/10/11 = 0/1/2 with Bishop promo;
    // 12/13/14 = 0/1/2 with Knight promo;
    // 15 = forward TWO squares.

    byte val;
    int diff = (int)(sm->to) - (int)(sm->from);

    if (diff < 0) { diff = -diff; }
    if (diff == 16) { // Move forward two squares
        val = 15;
        ASSERT (sm->promote == EMPTY);

    } else {
        if (diff == 7) { val = 0; }
        else if (diff == 8) { val = 1; }
        else {  // diff is 9:
            ASSERT (diff == 9);
            val = 2;
        }
        if (sm->promote != EMPTY) {
            // Handle promotions.
            // sm->promote must be Queen=2,Rook=3, Bishop=4 or Knight=5.
            // We add 3 for Queen, 6 for Rook, 9 for Bishop, 12 for Knight.

            ASSERT (sm->promote >= QUEEN  &&  sm->promote <= KNIGHT);
            val += 3 * ((sm->promote) - 1);
        }
    }
    return val;
}


// Special-move tokens:
// Since king-move values 1-10 are taken for actual King moves, only
// 11-15 (and zero) are available for non-move information.

#define ENCODE_NAG          11
#define ENCODE_COMMENT      12
#define ENCODE_START_MARKER 13
#define ENCODE_END_MARKER   14
#define ENCODE_END_GAME     15

#define ENCODE_FIRST        11
#define ENCODE_LAST         15

// The end-game and end-variation tokens could be the same single token,
// but having two different tokens allows for detecting corruption, since
// a game must end with the end-game token.


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// decodeMove():
//      Decode a move from a bytebuffer. Assumes the byte val is an
//      actual move, not the value of a "special" (non-move) token.
//      This function needs to be passed the bytebuffer because some
//      moves (only Queen diagonal moves) are encoded in two bytes, so
//      it may be necessary to read the next byte as well.
//
static errorT decodeMove(ByteBuffer* buf, simpleMoveT* sm, byte val,
                         const Position* pos) {
	const colorT toMove = pos->GetToMove();
	const squareT from = pos->GetList(toMove)[val >> 4];
	if (from > H8)
		return ERROR_Decode;
	sm->from = from;
	sm->to = from;
	sm->promote = EMPTY;
	sm->movingPiece = pos->GetBoard()[from];

	const auto [to, promo] =
	    (toMove == WHITE)
	        ? buf->decodeMove<WHITE>(piece_Type(sm->movingPiece), from, val)
	        : buf->decodeMove<BLACK>(piece_Type(sm->movingPiece), from, val);
	if (to < 0 || to > 63)
		return ERROR_Decode;

	if (to == from) {
		if (promo == INVALID_PIECE)
			return ERROR_Decode;

		if (promo == PAWN) // NULL MOVE
			return OK;

		if (!pos->validCastling(promo == KING, false))
			return ERROR_Decode;

		sm->to += (promo == KING) ? 2 : -2;
		return OK; // CASTLE
	}

	if (to == pos->GetKingSquare(WHITE) || to == pos->GetKingSquare(BLACK))
		return ERROR_Decode;

	if (promo != INVALID_PIECE)
		sm->promote = promo;

	sm->to = static_cast<squareT>(to);
	return OK;
}

template <typename DestT>
void encodeMove(const simpleMoveT& sm, DestT& dest) {
	byte val;
	switch (piece_Type(sm.movingPiece)) {
	case KING:
		val = encodeKing(&sm);
		break;
	case QUEEN:
		return encodeQueen(&dest, &sm);

	case ROOK:
		val = encodeRook(&sm);
		break;
	case BISHOP:
		val = encodeBishop(&sm);
		break;
	case KNIGHT:
		val = encodeKnight(&sm);
		break;
	default:
		ASSERT(PAWN == piece_Type(sm.movingPiece));
		val = encodePawn(&sm);
	}
	const auto encoded = makeMoveByte(sm.pieceNum, val);
	dest.emplace_back(encoded);
}

/// Encode the moves, the nags, the comment mark and the variations.
template <typename MoveT, typename DestT>
std::pair<unsigned, unsigned> encodeMovelist(const MoveT* m, DestT& dest) {
	ASSERT(m && m->startMarker());

	// Check if there is a pre-game comment
	if (!m->comment.empty())
		dest.emplace_back(ENCODE_COMMENT);

	unsigned n_vars = 0;
	unsigned n_nags = 0;
	while ((m = m->nextMoveInPGN())) {
		if (m->startMarker()) {
			++n_vars;
			dest.emplace_back(ENCODE_START_MARKER);
			if (!m->comment.empty())
				dest.emplace_back(ENCODE_COMMENT);

		} else if (m->endMarker()) {
			if (m->nextMoveInPGN())
				dest.emplace_back(ENCODE_END_MARKER);

		} else {
			encodeMove(m->moveData, dest);

			for (int i = 0, n = m->nagCount; i < n; ++i) {
				dest.emplace_back(ENCODE_NAG);
				dest.emplace_back(m->nags[i]);
				++n_nags;
			}
			if (!m->comment.empty())
				dest.emplace_back(ENCODE_COMMENT);
		}
	}
	dest.emplace_back(ENCODE_END_GAME);
	return {n_vars, n_nags};
}

/// Decodes the game moves
errorT Game::DecodeVariation(ByteBuffer& buf) {
	simpleMoveT sm;
	for (;;) {
		auto [err, val] = buf.nextMove(
		    this->VarDepth, [&](auto) { return true; },
		    [&] {
			    // Mark this comment as needing to be read
			    this->CurrentMove->prev->comment = '*';
		    },
		    [&](auto newVariation) {
			    if (newVariation)
				    return AddVariation() == OK;

			    return (MoveExitVariation() == OK && MoveForward() == OK);
		    },
		    [&](auto nag) {
			    return this->AddNag(nag) == OK;
		    });
		if (err)
			return (err == ERROR_EndOfMoveList) ? OK : err;

		auto errMove = decodeMove(&buf, &sm, val, currentPos());
		if (!errMove)
			errMove = AddMove(&sm);
		if (errMove)
			return errMove;
	}
}

/**
 * The Comments section is composed by null-terminated strings. The comments are
 * stored in the order in which they will appear in the PGN notation:
 * {C1} 1.d4 {C2} (1.b4 {C3} 1...e5 {C4} (1...Na6 {C5}) 2.e4 {C6})
 * ({C7} 1.g4 {C8}) 1...d5 {C9}
 */
template <typename MoveT, typename DestT>
auto encodeComments(const MoveT* m, DestT& dest) {
	ASSERT(m);

	unsigned n_comments = 0;
	do {
		if (!m->comment.empty()) {
			const auto len = m->comment.size() + 1; // Include the null char
			const auto data = m->comment.c_str();
			dest.insert(dest.end(), data, data + len);
			++n_comments;
		}
	} while ((m = m->nextMoveInPGN()));
	return n_comments;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// decodeComments():
//      Decodes the comments of the game. When decoding the moves, the
//      comment field of each move that has a comment is marked (made
//      not empty), so this function recurses the movelist and subvariations
//      and allocates each comment to its move.
//
template <typename SourceT, typename MoveT>
static errorT decodeComments(SourceT& buf, MoveT* m) {
	ASSERT(m);

	do {
		if (!m->comment.empty()) {
			ASSERT(m->comment == "*");
			auto str = buf.GetTerminatedString();
			if (!str)
				return ERROR_BufferRead;
			m->comment = str;
		}
	} while ((m = m->nextMoveInPGN()));
	return OK;
}

/// Calculate the game's main line information:
/// - home pawn delta information
/// - promotions flags
/// - number of half moves
/// - final material signature
/// - stored line code
template <typename MoveT>
std::pair<bool, bool> mainlineInfo(const Position* customStart,
                                   const MoveT* firstMove, IndexEntry& dest) {
	ushort nHalfMoves = 0;
	bool PromoFlag = false;
	bool UnderPromosFlag = false;
	unsigned hpCount = 0;
	byte hpVal[8] = {};
	Position pos = customStart ? *customStart : Position::getStdStart();

	auto hpOld = HPSIG_StdStart; // All 16 pawns are on their home squares.
	for (auto move = firstMove; !move->endMarker(); move = move->next) {
		++nHalfMoves;

		if (move->moveData.promote != EMPTY) {
			PromoFlag = true;
			if (piece_Type(move->moveData.promote) != QUEEN) {
				UnderPromosFlag = true;
			}
		}

		pos.DoSimpleMove(move->moveData);
		if (!customStart) {
			const auto hpNew = pos.GetHPSig();
			if (unsigned changed = hpOld - hpNew) {
				hpOld = hpNew;
				byte idxMovedPawn = 0; // __builtin_ctz(changed)
				while (changed >>= 1) {
					++idxMovedPawn;
				}
				assert(idxMovedPawn <= 0x0F);
				if ((hpCount & 1) == 0) // There are only 16 pawns, so we can
					idxMovedPawn <<= 4; // store two pawn moves in every byte
				hpVal[hpCount++ / 2] |= idxMovedPawn;
			}
		}
	}

	byte storedCode = 0;
	if (!customStart) {
		storedCode = StoredLine::classify([&](auto begin, auto end) {
			if (std::distance(begin, end) > nHalfMoves)
				return false;

			const moveT* gameMove = firstMove;
			for (; begin != end; ++begin) {
				if (gameMove->moveData.from != begin->getFrom() ||
				    gameMove->moveData.to != begin->getTo())
					return false;

				gameMove = gameMove->next;
			}
			return true;
		});
	}

	dest.SetHomePawnData(static_cast<byte>(hpCount), hpVal);
	dest.SetPromotionsFlag(PromoFlag);
	dest.SetUnderPromoFlag(UnderPromosFlag);
	dest.SetNumHalfMoves(nHalfMoves);
	dest.SetFinalMatSig(matsig_Make(pos.GetMaterial()));
	dest.SetStoredLineCode(storedCode);

	return {PromoFlag, UnderPromosFlag};
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::Encode(): Encode the game to a buffer for disk storage.
//      If passed a NON-null IndexEntry pointer, it will fill in the
//      following fields of that index entry, which are computed as
//      the game is encoded:
//       -  result, ecoCode, whiteElo, blackElo
//       -  promotion flag
//       -  nMoves: the number of halfmoves
//       -  finalMatSig: the material signature of the final position.
//       -  homePawnData: the home pawn change list.
//
errorT Game::Encode(std::vector<byte>& dest, IndexEntry& ie) const {
    ie.clearFlags();

    // Set the fields in the IndexEntry:
    ie.SetDate(Date);
    ie.SetEventDate(EventDate);
    ie.SetResult(Result);
    ie.SetEcoCode(EcoCode);
    ie.SetWhiteElo(WhiteElo);
    ie.SetBlackElo(BlackElo);
    ie.SetWhiteRatingType(WhiteRatingType);
    ie.SetBlackRatingType(BlackRatingType);
    ie.SetStartFlag(HasNonStandardStart());
    ie.SetFlag(IndexEntry::StrToFlagMask(ScidFlags), true);

    const auto [promo, underPromo] = mainlineInfo(StartPos.get(),
                                                  FirstMove->next, ie);

    // First, encode info not already stored in the index
    // This will be the non-STR (non-"seven tag roster") PGN tags.
    encodeTags(GetExtraTags(), dest);

    // Encode the promotion flags and the start position
    char FEN[256];
    encodeStartBoard(promo, underPromo,
                     HasNonStandardStart(FEN) ? FEN : nullptr, dest);

    // Now the movelist:
    auto [varCount, nagCount] = encodeMovelist(FirstMove, dest);

    // Now do the comments
    const auto commentCount = encodeComments(FirstMove, dest);

    ie.SetCommentCount(commentCount);
    ie.SetVariationCount(varCount);
    ie.SetNagCount(nagCount);

    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::DecodeNextMove():
//      Decodes one more mainline move of the game from the bytebuffer.
//      Used in searches for speed, since it is usually possible to
//      determine if a game matches the search criteria without decoding
//      all of it.
//      If the game flag KeepDecodedMoves is true, the move decodes is
//      added normally. If it is false, only the current position is
//      updated and the list of moves is not updated -- this is done
//      in searches for speed.
//      Returns OK if a move was found, or ERROR_EndOfMoveList if all the
//      moves have been decoded. Returns ERROR_Game if some corruption was
//      detected.
//
errorT Game::DecodeNextMove(ByteBuffer* buf, simpleMoveT& sm) {
	ASSERT(buf != NULL);

	auto [err, val] = buf->nextLineMove();
	if (err)
		return err;

	return decodeMove(buf, &sm, val, currentPos());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::DecodeStart():
//      Decodes the starting information from the game's on-disk
//      representation in the bytebuffer. After this is called,
//      DecodeNextMove() can be called to decode each successive
//      mainline move.
//
errorT Game::DecodeSkipTags(ByteBuffer* buf) {
    ASSERT(buf != NULL);

    Clear();
    errorT err = buf->decodeTags([](auto, auto) {});
    if (err != OK)
        return err;

    const auto [err_startpos, fen] = buf->decodeStartBoard();
    if (err_startpos)
        return err_startpos;

    if (fen)
        return SetStartFen(fen);

    return OK;
}

errorT Game::DecodeMovesOnly(ByteBuffer& buf) {
	if (errorT err = DecodeSkipTags(&buf))
		return err;

	return DecodeVariation(buf);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Game::Decode():
//      Decodes a game from its on-disk representation in a bytebuffer.
//      Decodes all the information: comments, variations, non-standard
//      tags, etc.
//
errorT Game::Decode(ByteBuffer& buf) {
    Clear();

    errorT err = buf.decodeTags([&](const auto& tag, const auto& value) {
        accessTagValue(tag.data(), tag.size()).assign(value);
    });
    if (err)
        return err;

    const auto [err_startpos, fen] = buf.decodeStartBoard();
    if (err_startpos)
        return err_startpos;

    if (fen)
        err = SetStartFen(fen);

    if (err == OK)
        err = DecodeVariation(buf);

    if (err == OK)
        err = decodeComments(buf, FirstMove);

    return err;
}

//////////////////////////////////////////////////////////////////////
//  EOF:    game.cpp
//////////////////////////////////////////////////////////////////////
