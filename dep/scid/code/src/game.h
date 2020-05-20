//////////////////////////////////////////////////////////////////////
//
//  FILE:       game.h
//              Game class for Scid.
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2000-2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////


#ifndef SCID_GAME_H
#define SCID_GAME_H

#include "common.h"
#include "date.h"
#include "indexentry.h"
#include "matsig.h"
#include "movetree.h"
#include "namebase.h"
#include "position.h"
#include <forward_list>
#include <memory>
#include <string>
#include <vector>
class ByteBuffer;
class TextBuffer;

void transPieces(char *s);
char transPiecesChar(char c);

// Piece letters translation
extern int language; // default to english
//  0 = en, 1 = fr, 2 = es, 3 = de
extern const char * langPieces[];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Game: Constants

// Common NAG Annotation symbol values:
const byte
    NAG_GoodMove = 1,
    NAG_PoorMove = 2,
    NAG_ExcellentMove = 3,
    NAG_Blunder = 4,
    NAG_InterestingMove = 5,
    NAG_DubiousMove = 6,
    NAG_OnlyMove = 8, // new
    NAG_Equal = 10,
    NAG_Unclear = 13,
    NAG_WhiteSlight = 14,
    NAG_BlackSlight = 15,
    NAG_WhiteClear = 16,
    NAG_BlackClear = 17,
    NAG_WhiteDecisive = 18,
    NAG_BlackDecisive = 19,
    NAG_WhiteCrushing = 20,
    NAG_BlackCrushing = 21,
    NAG_ZugZwang = 22, // new
    NAG_BlackZugZwang = 23, // new
    NAG_MoreRoom = 26, // new
    NAG_DevelopmentAdvantage = 35,  // new
    NAG_WithInitiative = 36, //new
    NAG_WithAttack = 40, // new
    NAG_WithBlackAttack = 41, // new
    NAG_Compensation = 44,      // from Whites perspective
    NAG_SlightCentre = 48,      // from Whites perspective
    NAG_Centre = 50,            // new
    NAG_SlightKingSide = 54,    // from Whites perspective
    NAG_ModerateKingSide = 56,  // from Whites perspective
    NAG_KingSide = 58,          // from Whites perspective
    NAG_SlightQueenSide = 60,   // from Whites perspective
    NAG_ModerateQueenSide = 62, // from Whites perspective
    NAG_QueenSide = 64,         // from Whites perspective
    NAG_SlightCounterPlay = 130, // new
    NAG_CounterPlay = 132, // new
    NAG_DecisiveCounterPlay = 134, // new
    NAG_BlackSlightCounterPlay = 131, // new
    NAG_BlackCounterPlay = 133, // new
    NAG_BlackDecisiveCounterPlay = 135, // new
    NAG_TimeLimit = 136, // new
    NAG_WithIdea = 140, // new
    NAG_BetterIs = 142, // new
    NAG_VariousMoves = 144, // new
    NAG_Comment = 145, // new
    NAG_Novelty = 146,
    NAG_WeakPoint = 147, // new
    NAG_Ending = 148, // new
    NAG_File = 149, // new
    NAG_Diagonal = 150, // new
    NAG_BishopPair = 151, // new
    NAG_OppositeBishops = 153, // new
    NAG_SameBishops = 154, // new
    NAG_Etc = 190, // new
    NAG_DoublePawns = 191, // new
    NAG_SeparatedPawns = 192, // new
    NAG_UnitedPawns = 193, // new
    NAG_Diagram = 201,  // Scid-specific NAGs start at 201.
    NAG_See = 210,  // new
    NAG_Mate = 211, // new 
    NAG_PassedPawn = 212, // new
    NAG_MorePawns = 213, //new
    NAG_With = 214, // new
    NAG_Without = 215;

// MAX_NAGS: Maximum id of NAG codes
const byte MAX_NAGS_ARRAY = 215;

// patternT structure: a pattern filter for material searches.
//    It can specify, for example, a white Pawn on the f-fyle, or
//    a black Bishop on f2 and white King on e1.
struct patternT
{
    pieceT     pieceMatch;  // EMPTY, WK, BK, etc...
    rankT      rankMatch;   // RANK_1 .. RANK_8 or NO_RANK
    fyleT      fyleMatch;   // A_FYLE .. H_FYLE or NO_FYLE
    byte       flag;        // 0 means this pattern must NOT occur.
    patternT * next;
};

enum gameExactMatchT {
    GAME_EXACT_MATCH_Exact = 0,
    GAME_EXACT_MATCH_Pawns,
    GAME_EXACT_MATCH_Fyles,
    GAME_EXACT_MATCH_Material
};

enum gameFormatT {
    PGN_FORMAT_Plain = 0,   // Plain regular PGN output
    PGN_FORMAT_HTML = 1,    // HTML format
    PGN_FORMAT_LaTeX = 2,   // LaTeX (with chess12 package) format
    PGN_FORMAT_Color = 3    // PGN, with color tags <red> etc
};

#define PGN_STYLE_TAGS             1
#define PGN_STYLE_COMMENTS         2
#define PGN_STYLE_VARS             4
#define PGN_STYLE_INDENT_COMMENTS  8
#define PGN_STYLE_INDENT_VARS     16
#define PGN_STYLE_SYMBOLS         32   // e.g. "! +-" instead of "$2 $14"
#define PGN_STYLE_SHORT_HEADER    64
#define PGN_STYLE_MOVENUM_SPACE  128   // Space after move numbers.
#define PGN_STYLE_COLUMN         256   // Column style: one move per line.
#define PGN_STYLE_SCIDFLAGS      512
#define PGN_STYLE_STRIP_MARKS   1024   // Strip [%mark] and [%arrow] codes.
#define PGN_STYLE_NO_NULL_MOVES 2048   // Convert null moves to comments.
#define PGN_STYLE_UNICODE       4096   // Use U+2654..U+2659 for figurine


void  game_printNag (byte nag, char * str, bool asSymbol, gameFormatT format);
byte game_parseNag(std::pair<const char*, const char*> strview);

uint strGetRatingType (const char * name);

//////////////////////////////////////////////////////////////////////
//  Game:  Class Definition

class Game {
    // Header data: tag pairs
    std::vector<std::pair<std::string, std::string> > extraTags_;
    std::string WhiteStr;
    std::string BlackStr;
    std::string EventStr;
    std::string SiteStr;
    std::string RoundStr;
    dateT       Date;
    dateT       EventDate;
    ecoT        EcoCode;
    eloT        WhiteElo;
    eloT        BlackElo;
    byte        WhiteRatingType;
    byte        BlackRatingType;
    resultT     Result;
    char        ScidFlags[22];

    // Position and moves
    byte        moveChunkUsed_;
    std::forward_list<std::unique_ptr<moveT[]> > moveChunks_;
    std::unique_ptr<Position> StartPos;
    std::unique_ptr<Position> CurrentPos{new Position};
    moveT*      FirstMove;
    moveT*      CurrentMove;
    uint        VarDepth;     // Current variation depth.
    ushort      NumHalfMoves; // Total half moves in the main line.

    // TODO: The following variables should not be part of this class.
    eloT        WhiteEstimateElo;
    eloT        BlackEstimateElo;

    uint        NumMovesPrinted; // Used in recursive WriteMoveList method.
    uint        PgnStyle;        // see PGN_STYLE macros above.
    gameFormatT PgnFormat;       // see PGN_FORMAT macros above.
    uint        HtmlStyle;       // HTML diagram style, see DumpHtmlBoard method in position.cpp.

private:
    Game(const Game&);
    moveT* allocMove();
    moveT* NewMove(markerT marker);
    void ClearMoves();
    errorT DecodeVariation(ByteBuffer& buf);
    errorT WritePGN(TextBuffer* tb);

    /**
     * Contains the information of the current position in the game, so that
     * after an operation that alters the location, it can be restored.
     */
    struct GameSavedPos {
        Position pos;
        moveT* move;
        uint varDepth;
    };

public:
    Game() { Clear(); }
    void Clear();
    void strip(bool variations, bool comments, bool NAGs);

    bool HasNonStandardStart(char* outFEN = nullptr) const {
        if (!StartPos)
            return false;
        if (outFEN)
            StartPos->PrintFEN(outFEN, FEN_ALL_FIELDS);
        return true;
    }

    /// Setup the start position from a FEN string and remove all the moves.
    /// If the FEN is invalid the game is not changed.
    errorT SetStartFen(const char* fenStr);

    void SetScidFlags(const char* s, size_t len) {
        constexpr size_t size = sizeof(ScidFlags) / sizeof(*ScidFlags);
        std::fill_n(ScidFlags, size, 0);
        std::copy_n(s, std::min(size - 1, len), ScidFlags);
    }
    void SetScidFlags(const char* s) { SetScidFlags(s, std::strlen(s)); }

    ushort GetNumHalfMoves() { return NumHalfMoves; }

    //////////////////////////////////////////////////////////////
    // Functions to add or delete moves:
    //
    errorT AddMove(const simpleMoveT* sm);
    errorT AddVariation();
    errorT DeleteVariation();
    errorT FirstVariation();
    errorT MainVariation();
    void Truncate();
    void TruncateStart();

    //////////////////////////////////////////////////////////////
    // Functions that move the current location (only CurrentPos,
    // CurrentMove and VarDepth are modified by these functions):
    //
    errorT MoveForward();
    errorT MoveBackup();
    errorT MoveIntoVariation(uint varNumber);
    errorT MoveExitVariation();
    errorT MoveForwardInPGN();
    errorT MoveToLocationInPGN(unsigned stopLocation);
    void MoveToStart();
    /// Move to the end of the main line.
    void MoveToEnd();
    void MoveToPly(int hmNumber) { // Move to a specified
        MoveToStart();             // mainline ply in the game.
        for (int i = 0; i < hmNumber; ++i)
            MoveForward();
    }
    GameSavedPos currentLocation() const {
        return GameSavedPos{*CurrentPos, CurrentMove, VarDepth};
    }
    void restoreLocation(const GameSavedPos& savedPos) {
        *CurrentPos = savedPos.pos;
        CurrentMove = savedPos.move;
        VarDepth = savedPos.varDepth;
    }

    //////////////////////////////////////////////////////////////
    // Functions that get information about the current location.
    //
    const Position* currentPos() const { return CurrentPos.get(); }
    Position* GetCurrentPos() { // Deprecated, use the const version
        return CurrentPos.get();
    }
    /// @return an "UCI position" string that leads to the current position
    std::string currentPosUCI() const;
    simpleMoveT* GetCurrentMove() { // Deprecated
        return CurrentMove->endMarker() ? nullptr : &CurrentMove->moveData;
    }
    ushort GetCurrentPly() const {
        auto ply = CurrentPos->GetPlyCounter();
        return StartPos ? ply - StartPos->GetPlyCounter() : ply;
    }
    uint GetNumVariations() const { return CurrentMove->numVariations; }

    // Each variation has a "level" and a "number".
    // - "level" is the number of times that is necessary to call
    //   MoveExitVariation() to reach the main line.
    // - "number" is the ordered position in the list of variations for the
    // current root position (first variation is number 0).
    // The main line is 0,0.
    uint GetVarLevel() const { return VarDepth; }
    uint GetVarNumber() const {
        if (VarDepth != 0) {
            uint varNumber = 0;
            auto moves = CurrentMove->getParent();
            for (auto parent = moves.first; parent; varNumber++) {
                parent = parent->varChild;
                if (parent == moves.second)
                    return varNumber;
            }
        }
        return 0; // returns 0 if in main line
    }

    unsigned GetLocationInPGN() const;
    unsigned GetPgnOffset() const;

    bool AtVarStart() const { return CurrentMove->prev->startMarker(); }
    bool AtVarEnd() const { return CurrentMove->endMarker(); }
    bool AtStart() const { return (VarDepth == 0 && AtVarStart()); }
    bool AtEnd() const { return (VarDepth == 0 && AtVarEnd()); }

    //////////////////////////////////////////////////////////////
    // Functions that get/set information about the last/next move.
    // Notice: when location is at the start of the game or a variation,
    // infomation are stored into the START_MARKER.
    //
    errorT AddNag(byte nag);
    errorT RemoveNag(bool isMoveNag);
    void ClearNags() {
        CurrentMove->prev->nagCount = 0;
        CurrentMove->prev->nags[0] = 0;
    }
    byte* GetNags() const { return CurrentMove->prev->nags; }
    byte* GetNextNags() const { return CurrentMove->nags; }

    /**
     * Return the comment on the move previously played by CurrentPos->ToMove
     * If there are no previous moves, return an empty comment.
     */
    const char* GetPreviousMoveComment() const {
        const moveT* move = CurrentMove->getPrevMove();
        if (move)
            move = move->getPrevMove();

        return (move) ? move->comment.c_str() : "";
    }
    const char* GetMoveComment() const {
        return CurrentMove->prev->comment.c_str();
    }
    std::string& accessMoveComment() { return CurrentMove->prev->comment; }
    void SetMoveComment(const char* comment);

    const char* GetNextSAN();
    void GetSAN(char* str);
    void GetPrevSAN(char* str);
    void GetPrevMoveUCI(char* str) const;
    void GetNextMoveUCI(char* str);

    //////////////////////////////////////////////////////////////
    // Functions that get/set the tag pairs:
    //
    void AddPgnTag(const char* tag, const char* value);
    bool RemoveExtraTag(const char* tag);
    const char* FindExtraTag(const char* tag) const;
    std::string& accessTagValue(const char* tag, size_t tagLen);
    const decltype(extraTags_) & GetExtraTags() const { return extraTags_; }
    void ClearExtraTags() { extraTags_.clear(); }

    void LoadStandardTags(const IndexEntry* ie, const NameBase* nb);

    void     SetEventStr (const char * str) { EventStr = str; }
    void     SetSiteStr  (const char * str) { SiteStr  = str; }
    void     SetWhiteStr (const char * str) { WhiteStr = str; }
    void     SetBlackStr (const char * str) { BlackStr = str; }
    void     SetRoundStr (const char * str) { RoundStr = str; }
    void     SetDate (dateT date)    { Date = date; }
    void     SetEventDate (dateT date)  { EventDate = date; }
    void     SetResult (resultT res) { Result = res; }
    void     SetWhiteElo (eloT elo)  { WhiteElo = elo; }
    void     SetBlackElo (eloT elo)  { BlackElo = elo; }
    void     SetWhiteRatingType (byte b) { WhiteRatingType = b; }
    void     SetBlackRatingType (byte b) { BlackRatingType = b; }
    int setRating(colorT col, const char* ratingType, size_t ratingTypeLen,
                  std::pair<const char*, const char*> rating);
    void     SetEco (ecoT eco)       { EcoCode = eco; }
    const char* GetEventStr () const { return EventStr.c_str(); }
    const char* GetSiteStr ()  const { return SiteStr.c_str();  }
    const char* GetWhiteStr () const { return WhiteStr.c_str(); }
    const char* GetBlackStr () const { return BlackStr.c_str(); }
    const char* GetRoundStr () const { return RoundStr.c_str(); }
    dateT    GetDate ()        const { return Date; }
    dateT    GetEventDate ()   const { return EventDate; }
    resultT  GetResult ()      const { return Result; }
    eloT     GetWhiteElo ()    const { return WhiteElo; }
    eloT     GetBlackElo ()    const { return BlackElo; }
    eloT     GetWhiteEstimateElo() const { return WhiteEstimateElo; }
    eloT     GetBlackEstimateElo() const { return BlackEstimateElo; }
    byte     GetWhiteRatingType () const { return WhiteRatingType; }
    byte     GetBlackRatingType () const { return BlackRatingType; }
    ecoT     GetEco ()         const { return EcoCode; }
    eloT     GetAverageElo ();

    // PGN conversion
    bool      CommentEmpty ( const char * comment);
    void      WriteComment (TextBuffer * tb, const char * preStr,
                            const char * comment, const char * postStr);
    errorT    WriteMoveList(TextBuffer* tb, moveT* oldCurrentMove,
                            bool printMoveNum, bool inComment);
    std::pair<const char*, unsigned> WriteToPGN (uint lineWidth = 0,
                                                 bool NewLineAtEnd = false,
                                                 bool newLineToSpaces = true);

    void      ResetPgnStyle (void) { PgnStyle = 0; }
    void      ResetPgnStyle (uint flag) { PgnStyle = flag; }

    uint      GetPgnStyle () { return PgnStyle; }
    void      SetPgnStyle (uint mask, bool setting) {
        if (setting) { AddPgnStyle (mask); } else { RemovePgnStyle (mask); }
    }
    void      AddPgnStyle (uint mask) { PgnStyle |= mask; }
    void      RemovePgnStyle (uint mask) { PgnStyle &= ~mask; }

    void      SetPgnFormat (gameFormatT gf) { PgnFormat = gf; }
    bool      SetPgnFormatFromString (const char * str);
    static bool PgnFormatFromString (const char * str, gameFormatT * fmt);
    bool      IsPlainFormat () { return (PgnFormat == PGN_FORMAT_Plain); }
    bool      IsHtmlFormat  () { return (PgnFormat == PGN_FORMAT_HTML); }
    bool      IsLatexFormat () { return (PgnFormat == PGN_FORMAT_LaTeX); }
    bool      IsColorFormat () { return (PgnFormat == PGN_FORMAT_Color); }

    void      SetHtmlStyle (uint style) { HtmlStyle = style; }
    uint      GetHtmlStyle () { return HtmlStyle; }

    errorT    GetPartialMoveList (DString * str, uint plyCount);

    bool      MaterialMatch (bool PromotionsFlag, ByteBuffer& buf, byte * min, byte * max,
                             patternT * pattern, int minPly, int maxPly,
                             int matchLength,
                             bool oppBishops, bool sameBishops,
                             int minDiff, int maxDiff);
    bool      ExactMatch (Position * pos, ByteBuffer * buf, simpleMoveT * sm,
                          gameExactMatchT searchType);
    bool      VarExactMatch (Position * searchPos, gameExactMatchT searchType);

    errorT    Encode(std::vector<byte>& dest, IndexEntry& ie) const;
    errorT    DecodeSkipTags(ByteBuffer* buf);
    errorT    DecodeNextMove (ByteBuffer * buf, simpleMoveT& sm);
    errorT    Decode(ByteBuffer& buf);
    errorT    DecodeMovesOnly(ByteBuffer& buf);

    Game* clone();
};

namespace gamevisit {

template <typename TFunc> void tags_STR(const Game& game, TFunc visitor) {
	char dateBuf[16];
	visitor("Event", game.GetEventStr());
	visitor("Site", game.GetSiteStr());
	date_DecodeToString(game.GetDate(), dateBuf);
	visitor("Date", dateBuf);
	visitor("Round", game.GetRoundStr());
	visitor("White", game.GetWhiteStr());
	visitor("Black", game.GetBlackStr());
	visitor("Result", RESULT_LONGSTR[game.GetResult()]);
}

template <typename TFunc> void tags_extra(const Game& game, TFunc visitor) {
	char strBuf[256];
	if (auto elo = game.GetWhiteElo()) {
		std::string rType = "White";
		rType.append(ratingTypeNames[game.GetWhiteRatingType()]);
		visitor(rType.c_str(), std::to_string(elo).c_str());
	}
	if (auto elo = game.GetBlackElo()) {
		std::string rType = "Black";
		rType.append(ratingTypeNames[game.GetBlackRatingType()]);
		visitor(rType.c_str(), std::to_string(elo).c_str());
	}
	if (game.GetEco() != ECO_None) {
		eco_ToExtendedString(game.GetEco(), strBuf);
		visitor("ECO", strBuf);
	}
	if (game.GetEventDate() != ZERO_DATE) {
		date_DecodeToString(game.GetEventDate(), strBuf);
		visitor("EventDate", strBuf);
	}
	// TODO:
	// if (*ScidFlags)
	//	visitor("ScidFlags", ScidFlags);

	for (auto& e : game.GetExtraTags()) {
		visitor(e.first.c_str(), e.second.c_str());
	}
	if (game.HasNonStandardStart(strBuf)) {
		visitor("FEN", strBuf);
	}
}

} // namespace gamevisit

namespace gamepos {

struct GamePos {
	uint32_t RAVdepth;
	uint32_t RAVnum;
	std::string FEN; // "Forsyth-Edwards Notation" describing the position.
	std::vector<int> NAGs;   // "Numeric Annotation Glyph"
	std::string comment;     // text annotation of the position.
	std::string lastMoveSAN; // move that was played to reach the position.
};

/**
 * Iterate all the positions of a game and store the corresponding GamePos
 * objects into a container.
 *
 * The order of positions and of Recursive Annotation Variations (RAV) follows
 * the PGN standard: "The alternate move sequence given by an RAV is one that
 * may be legally played by first unplaying the move that appears immediately
 * prior to the RAV. Because the RAV is a recursive construct, it may be nested"
 * Each position have a RAVdepth and a RAVnum that allows to follow a
 * variation from any given position:
 * - skip all the next positions with a bigger RAVdepth
 * - the variation ends with:
 *   - a lower RAVdepth or
 *   - an equal RAVdepth but different RAVnum or
 *   - the end of @e dest
 * @param game: reference to the Game object where the positions are read.
 * @param dest: the container where the GamePos objects are appended.
 */
template <typename TCont>
inline void collectPositions(Game& game, TCont& dest) {
	do {
		if (game.AtVarStart() && !game.AtStart())
			continue;

		dest.emplace_back();
		auto& gamepos = dest.back();
		gamepos.RAVdepth = game.GetVarLevel();
		gamepos.RAVnum = game.GetVarNumber();
		char strBuf[256];
		game.currentPos()->PrintFEN(strBuf, FEN_ALL_FIELDS);
		gamepos.FEN = strBuf;
		for (byte* nag = game.GetNags(); *nag; nag++) {
			gamepos.NAGs.push_back(*nag);
		}
		gamepos.comment = game.GetMoveComment();
		game.GetPrevSAN(strBuf);
		gamepos.lastMoveSAN = strBuf;

	} while (game.MoveForwardInPGN() == OK);
}

/**
 * Returns all the positions of a game
 * @param game: reference to the Game object where the positions are read.
 * @returns a std::vector containing the GamePos objects corresponding to all
 * the positions of @e game.
 */
inline std::vector<GamePos> collectPositions(Game& game) {
	std::vector<GamePos> res;
	game.MoveToStart();
	collectPositions(game, res);
	return res;
}

} // namespace gamepos

#endif  // #ifndef SCID_GAME_H

//////////////////////////////////////////////////////////////////////
//  EOF:    game.h
//////////////////////////////////////////////////////////////////////
