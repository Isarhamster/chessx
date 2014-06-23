//////////////////////////////////////////////////////////////////////
//
//  FILE:       engine.cpp
//              Engine class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2002-2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "attacks.h"
#include "guessengine.h"
#include "recog.h"

using namespace Guess;

// The Engine class implements the Scid built-in chess engine.
// See guessengine.h for details.


// Evaluation constants:

static const int Infinity    = 32000;
static const int KingValue   = 10000;
static const int QueenValue  =   900;
static const int RookValue   =   500;
static const int BishopValue =   300;
static const int KnightValue =   300;
static const int PawnValue   =   100;

// EndgameValue, MiddlegameValue:
//   If the combined material score of pieces on both sides (excluding
//   kings and pawns) is less than this value, we are in an endgame.
//   If it is greater than MiddlegameValue, we use middlegame scoring.
//   For anything in between, the score will be a weighted average of
//   the middlegame and endgame scores.
//
static const int EndgameValue    = 2400;
static const int MiddlegameValue = 4000;

// Bonuses and penalties:
//
static const int RookHalfOpenFile  =   8;
static const int RookOpenFile      =  20;
static const int RookPasserFile    =  25;  // Rook on passed pawn file.
static const int RookOnSeventh     =  25;  // Rook on its 7th rank.
static const int DoubledRooks      =  20;  // Two rooks on same file.
static const int RookEyesKing      =  12;  // Attacks squares near enemy king.
static const int KingTrapsRook     =  35;  // E.g. King on f1, Rook on h1
static const int DoubledPawn       =   8;
static const int IsolatedPawn      =  16;
static const int BackwardPawn      =  10;  // Pawn at base of pawn chain.
//static const int DispersedPawn     =  10;  // Not in pawn chain/duo. (Unused)
static const int BlockedHomePawn   =  15;  // Blocked pawn on d2/e2/d7/e7.
static const int BishopPair        =  25;  // Pair of bishops.
static const int BishopEyesKing    =  12;  // Bishop targets enemy king.
static const int BishopTrapped     = 120;  // E.g. Bxa7? ...b6!
static const int KnightOutpost     =  15;  // 4th/5th/6th rank outpost.
static const int KnightBadEndgame  =  30;  // Enemy pawns on both wings.
static const int BadPieceTrade     =  80;  // Bad trade, e.g. minor for pawns.
static const int CanCastle         =  10;  // Bonus for castling rights.
static const int Development       =   8;  // Moved minor pieces in opening.
static const int CentralPawnPair   =  15;  // For d4/d5 + e4/e5 pawns.
static const int CoverPawn         =  12;  // Pawn cover for king.
static const int PassedPawnRank[8] =
{
//  1   2   3   4   5   6    7  8th  rank
    0, 10, 15, 25, 50, 80, 120, 0
};

// Bishops (and rooks in endings) need to be mobile to be useful:
static const int BishopMobility[16] =
{
    //  0    1    2   3   4  5  6  7  8   9  10  11  12  13  14  15
    -20, -15, -10, -6, -3, 0, 3, 6, 9, 12, 15, 15, 15, 15, 15, 15
};
static const int RookEndgameMobility[16] =
{
    //  0    1    2    3   4  5  6  7  8  9 10 11 12 13 14 15
    -25, -20, -15, -10, -5, 2, 0, 2, 4, 6, 8, 8, 8, 8, 8, 8
};

// Piece distance to enemy king bonuses:    1   2   3   4   5   6   7
static const int KnightKingDist [8] = { 0, 10, 14, 10,  5,  2,  0,  0 };
static const int BishopKingDist [8] = { 0,  8,  6,  4,  2,  1,  0,  0 };
static const int RookKingDist   [8] = { 0,  8,  6,  4,  2,  1,  0,  0 };
static const int QueenKingDist  [8] = { 0, 15, 12,  9,  6,  3,  0,  0 };

// LazyEvalMargin
//   A score that is further than this margin outside the current
//   alpha-beta window after material evaluation is returned as-is.
//   A larger margin is used for endgames (especially pawn endings)
//   since positional bonuses can be much larger for them.
static const int LazyEvalMargin           = 250;
static const int LazyEvalEndingMargin     = 400;
static const int LazyEvalPawnEndingMargin = 800;

// NullMoveReduction:
//   The default reduced depth for a null move search.
static const int NullMoveReduction = 2;

// AspirationWindow:
//   The window around the score of the previous depth iteration
//   when searching at the root.
static const int AspirationWindow = 35;

// PawnSquare:
//   Gives bonuses to advanced pawns, especially in the centre.
static const int
PawnSquare [64] =
{
    0,   0,   0,   0,   0,   0,   0,   0, // A8 - H8
    4,   8,  12,  16,  16,  12,   8,   4,
    4,   8,  12,  16,  16,  12,   8,   4,
    3,   6,   9,  12,  12,   9,   6,   3,
    2,   4,   6,   8,   8,   6,   4,   2,
    1,   2,   3,   4,   4,   3,   2,   1,
    0,   0,   0,  -4,  -4,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0  // A1 - H1
};

// PawnStorm:
//    Bonus when side is castled queenside and opponent is
//    castled kingside. Gives a bonus for own sheltering pawns
//    and a penalty for pawns on the opposing wing to make them
//    disposable and encourage them to move forwards.
static const int
PawnStorm [64] =
{
    0,   0,   0,   0,   0,   0,   0,   0, // A8 - H8
    0,   0,   0,   0,   2,   2,   2,   2,
    0,   0,   0,   0,   4,   2,   2,   2,
    0,   0,   0,   4,   6,   0,   0,   0,
    4,   4,   4,   4,   4,  -4,  -4,  -4,
    8,   8,   8,   0,   0,  -8,  -8,  -8,
    12,  12,  12,   0,   0, -12, -12, -12,
    0,   0,   0,   0,   0,   0,   0,   0  // A1 - H1
};

// KnightSquare:
//    Rewards well-placed knights.
static const int
KnightSquare [64] =
{
    -24, -12,  -6,  -6,  -6,  -6, -12, -24,
    -8,   0,   0,   0,   0,   0,   0,  -8,
    -6,   5,  10,  10,  10,  10,   5,  -6,
    -6,   0,  10,  10,  10,  10,   0,  -6,
    -6,   0,   5,   8,   8,   5,   0,  -6,
    -6,   0,   5,   5,   5,   5,   0,  -6,
    -6,   0,   0,   0,   0,   0,   0,  -8,
    -10,  -8,  -5,  -6,  -6,  -6,  -6, -10
};

// BishopSquare:
//   Bonus array for bishops.
static const int
BishopSquare [64] =
{
    -10,  -5,   0,   0,   0,   0,  -5, -10,
    -5,   8,   0,   5,   5,   0,   8,  -5,
    0,   0,   5,   5,   5,   5,   0,   0,
    0,   5,  10,   5,   5,  10,   5,   0,
    0,   5,  10,   5,   5,  10,   5,   0,
    0,   0,   5,   5,   5,   5,   0,   0,
    -5,   8,   0,   5,   5,   0,   8,  -5,
    -10,  -5,  -2,  -2,  -2,  -2,  -5, -10
};

// RookFile:
//    Bonus array for Rooks, by file.
static const int /* a  b  c  d  e  f  g  h */
RookFile [8]    = { 0, 0, 4, 8, 8, 4, 0, 0 };

// QueenSquare:
//    Bonus array for Queens.
static const int
QueenSquare [64] =
{
    -5,   0,   0,   0,   0,   0,   0,  -5, // A8 - H8
    -5,   0,   3,   3,   3,   3,   0,  -5,
    0,   3,   6,   9,   9,   6,   3,   0,
    0,   3,   9,  12,  12,   9,   3,   0,
    -5,   3,   9,  12,  12,   9,   3,  -5,
    -5,   3,   6,   9,   9,   6,   3,  -5,
    -5,   0,   3,   3,   3,   3,   0,  -5,
    -10,  -5,   0,   0,   0,   0,  -5, -10  // A1 - H1
};

// KingSquare:
//    Bonus array for kings in the opening and middlegame.
static const int
KingSquare [64] =
{
    -50, -50, -50, -50, -50, -50, -50, -50,
    -50, -50, -50, -50, -50, -50, -50, -50,
    -50, -50, -50, -50, -50, -50, -50, -50,
    -50, -50, -50, -60, -60, -50, -50, -50,
    -40, -40, -40, -60, -60, -40, -40, -40,
    -15, -15, -15, -20, -20, -15, -15, -15,
    5,   5,   0,   0,   0,   0,   5,   5,
    20,  20,  15,   5,   5,   5,  20,  20
};

// EndgameKingSquare:
//    Rewards King centralisation in endgames.
//    TODO: Add separate king square tables for endgames where all
//          pawns are on one side of the board.
static const int
KingEndgameSquare [64] =
{
    -10,  -5,   0,   5,   5,   0,  -5, -10,
    -5,   0,   5,  10,  10,   5,   0,  -5,
    0,   5,  10,  15,  15,  10,   5,   0,
    5,  10,  15,  20,  20,  15,  10,   5,
    5,  10,  15,  20,  20,  15,  10,   5,
    0,   5,  10,  15,  15,  10,   5,   0,
    -5,   0,   5,  10,  10,   5,   0,  -5,
    -10,  -5,   0,   5,   5,   0,  -5, -10
};

static const int
pieceValues [8] =
{
    0,  // Invalid
    KingValue,
    QueenValue,
    RookValue,
    BishopValue,
    KnightValue,
    PawnValue,
    0   // Empty
};

inline int
Engine::PieceValue(pieceT piece)
{
    return pieceValues[piece_Type(piece)];
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// isOutpost
//   Returns true if the square is on the 4th/5th/6th rank (3rd/4th/5th
//   for Black) and cannot be attacked by an enemy pawn.
static bool
isOutpost(const pieceT * board, squareT sq, colorT color)
{
    pieceT enemyPawn = piece_Make(color_Flip(color), PAWN);
    rankT rank = square_Rank(sq);
    fyleT fyle = square_Fyle(sq);

    // Build the list of squares to check for enemy pawns:
    SquareList squares;
    if(color == WHITE)
    {
        if(rank < RANK_4  ||  rank > RANK_6)
        {
            return false;
        }
        if(fyle > A_FYLE)
        {
            squares.Add(square_Make(fyle - 1, RANK_7));
            if(rank == RANK_5)
            {
                squares.Add(square_Make(fyle - 1, RANK_6));
            }
        }
        if(fyle < H_FYLE)
        {
            squares.Add(square_Make(fyle + 1, RANK_7));
            if(rank == RANK_5)
            {
                squares.Add(square_Make(fyle + 1, RANK_6));
            }
        }
    }
    else
    {
        if(rank < RANK_3  ||  rank > RANK_5)
        {
            return false;
        }
        if(fyle > A_FYLE)
        {
            squares.Add(square_Make(fyle - 1, RANK_2));
            if(rank == RANK_4)
            {
                squares.Add(square_Make(fyle - 1, RANK_3));
            }
        }
        if(fyle < H_FYLE)
        {
            squares.Add(square_Make(fyle + 1, RANK_2));
            if(rank == RANK_4)
            {
                squares.Add(square_Make(fyle + 1, RANK_3));
            }
        }
    }

    // Now check each square for an enemy pawn:
    for(unsigned int i = 0; i < squares.Size(); i++)
    {
        if(board[squares.Get(i)] == enemyPawn)
        {
            return false;
        }
    }
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::Score
//   Returns a score in centipawns for the current engine position,
//   from the perspective of the side to move.
int
Engine::Score(void)
{
    // Look for a recognized ending with an exact score:
    int recog = Recognizer::Recognize(&Pos);
    if(recogFlag(recog) == SCORE_EXACT)
    {
        return recogScore(recog);
    }

    return Score(-Infinity, Infinity);
}

static unsigned int nScoreCalls = 0;
static unsigned int nScoreFull  = 0;

inline int
Engine::ScoreWhiteMaterial(void)
{
    unsigned char * pieceCount = Pos.GetMaterial();
    return  pieceCount[WQ] * QueenValue   +  pieceCount[WR] * RookValue
            +  pieceCount[WB] * BishopValue  +  pieceCount[WN] * KnightValue
            +  pieceCount[WP] * PawnValue;
}

inline int
Engine::ScoreBlackMaterial(void)
{
    unsigned char * pieceCount = Pos.GetMaterial();
    return  pieceCount[BQ] * QueenValue   +  pieceCount[BR] * RookValue
            +  pieceCount[BB] * BishopValue  +  pieceCount[BN] * KnightValue
            +  pieceCount[BP] * PawnValue;
}

int
Engine::ScoreMaterial(void)
{
    int score = ScoreWhiteMaterial() - ScoreBlackMaterial();
    return (Pos.GetToMove() == WHITE) ? score : -score;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::Score
//   Returns a score in centipawns for the current engine position,
//   from the perspective of the side to move.
//   Alpha and beta cutoff scores are specified for performance. If
//   simple material counting produces a score much lower than alpha
//   or much greater than beta, the score is returned without
//   slower square-based evaluation.
int
Engine::Score(int alpha, int beta)
{
    colorT toMove = Pos.GetToMove();
    const unsigned char * pieceCount = Pos.GetMaterial();
    const pieceT * board = Pos.GetBoard();
    int materialScore[2] = {0, 0};
    int allscore[2] = {0, 0};   // Scoring in all positions
    int endscore[2] = {0, 0};   // Scoring in endgames
    int midscore[2] = {0, 0};   // Scoring in middlegames
    int nNonPawns[2] = {0, 0};  // Non-pawns on each side, including kings

    nScoreCalls++;

    nNonPawns[WHITE] = Pos.NumNonPawns(WHITE);
    nNonPawns[BLACK] = Pos.NumNonPawns(BLACK);

    // First compute material scores:
    allscore[WHITE] = materialScore[WHITE] = ScoreWhiteMaterial();
    allscore[BLACK] = materialScore[BLACK] = ScoreBlackMaterial();

    int pieceMaterial = (materialScore[WHITE] - pieceCount[WP] * PawnValue)
                        + (materialScore[BLACK] - pieceCount[BP] * PawnValue);
    bool inEndgame = false;
    bool inMiddlegame = false;
    if(pieceMaterial <= EndgameValue)
    {
        inEndgame = true;
    }
    if(pieceMaterial >= MiddlegameValue)
    {
        inMiddlegame = true;
    }
    bool inPawnEndgame = Pos.InPawnEnding();

    // Look for a bad trade: minor piece for pawns; Q for R+Pawns; etc.
    // But only do this if both sides have pawns.
    if(pieceCount[WP] > 0  &&  pieceCount[BP] > 0)
    {
        unsigned int wminors = pieceCount[WB] + pieceCount[WN];
        unsigned int bminors = pieceCount[BB] + pieceCount[BN];
        unsigned int wmajors = pieceCount[WR] + (2 * pieceCount[WQ]);
        unsigned int bmajors = pieceCount[BR] + (2 * pieceCount[BQ]);
        if(wmajors == bmajors)
        {
            if(wminors < bminors)
            {
                allscore[WHITE] -= BadPieceTrade;
            }
            if(wminors > bminors)
            {
                allscore[BLACK] -= BadPieceTrade;
            }
        }
        else if(wminors == bminors)
        {
            if(wmajors < bmajors)
            {
                allscore[WHITE] -= BadPieceTrade;
            }
            if(wmajors > bmajors)
            {
                allscore[BLACK] -= BadPieceTrade;
            }
        }
    }

    // Add the Bishop-pair bonus now, because it is fast and easy:
    if(pieceCount[WB] >= 2)
    {
        allscore[WHITE] += BishopPair;
    }
    if(pieceCount[BB] >= 2)
    {
        allscore[BLACK] += BishopPair;
    }

    // If there are no pawns, a material advantage of only one minor
    // piece is worth very little so reduce the material score.
    if(pieceCount[WP] + pieceCount[BP] == 0)
    {
        int materialDiff = materialScore[WHITE] - materialScore[BLACK];
        if(materialDiff < 0)
        {
            materialDiff = -materialDiff;
        }
        if(materialDiff == BishopValue  ||  materialDiff == KnightValue)
        {
            allscore[WHITE] /= 4;
            allscore[BLACK] /= 4;
        }
    }

    // Look for a trapped bishop on a7/h7/a2/h2:
    if(Pos.RankCount(WB, RANK_7) > 0)
    {
        if(board[A7] == WB  &&  board[B6] == BP)
        {
            allscore[WHITE] -= BishopTrapped;
        }
        if(board[H7] == WB  &&  board[G6] == BP)
        {
            allscore[WHITE] -= BishopTrapped;
        }
    }
    if(Pos.RankCount(BB, RANK_2) > 0)
    {
        if(board[A2] == BB  &&  board[B3] == WP)
        {
            allscore[BLACK] -= BishopTrapped;
        }
        if(board[H2] == BB  &&  board[G6] == WP)
        {
            allscore[BLACK] -= BishopTrapped;
        }
    }

    // Check for a score much worse than alpha or better than beta
    // which can be returned immediately on the assumption that
    // a full evaluation could not get inside the alpha-beta range.
    // If we are in a pawn ending, a much larger margin is used since
    // huge bonuses can be added for passed pawns in such endgames.
    int lazyMargin = LazyEvalMargin;
    if(inEndgame)
    {
        lazyMargin = LazyEvalEndingMargin;
    }
    if(inPawnEndgame)
    {
        lazyMargin = LazyEvalPawnEndingMargin;
    }

    int fastScore = allscore[WHITE] - allscore[BLACK];
    if(toMove == BLACK)
    {
        fastScore = -fastScore;
    }
    if(fastScore - lazyMargin > beta)
    {
        return fastScore;
    }
    if(fastScore + lazyMargin < alpha)
    {
        return fastScore;
    }

    // Get the pawn structure score next, because it is usually fast:
    pawnTableEntryT pawnEntry;
    ScorePawnStructure(&pawnEntry);

    // Penalise d-file and e-file pawns blocked on their home squares:
    if(board[D2] == WP  &&  board[D3] != EMPTY)
    {
        allscore[WHITE] -= BlockedHomePawn;
    }
    if(board[E2] == WP  &&  board[E3] != EMPTY)
    {
        allscore[WHITE] -= BlockedHomePawn;
    }
    if(board[D7] == BP  &&  board[D6] != EMPTY)
    {
        allscore[BLACK] -= BlockedHomePawn;
    }
    if(board[E7] == BP  &&  board[E6] != EMPTY)
    {
        allscore[BLACK] -= BlockedHomePawn;
    }

    // Incentive for side ahead in material to trade nonpawn pieces and
    // for side behind in material to avoid trades:
    if(materialScore[WHITE] > materialScore[BLACK])
    {
        int bonus = (5 - nNonPawns[BLACK]) * 5;
        allscore[WHITE] += bonus;
    }
    else if(materialScore[BLACK] > materialScore[WHITE])
    {
        int bonus = (5 - nNonPawns[WHITE]) * 5;
        allscore[BLACK] += bonus;
    }

    // Check again for a score outside the alpha-beta range, using a
    // smaller fixed margin of error since the pawn structure score
    // has been added:
    fastScore = (allscore[WHITE] - allscore[BLACK]) + pawnEntry.score;
    if(toMove == BLACK)
    {
        fastScore = -fastScore;
    }
    if(fastScore > beta + 200)
    {
        return fastScore;
    }
    if(fastScore < alpha - 200)
    {
        return fastScore;
    }

    nScoreFull++;

    // Now refine the score with piece-square bonuses:

    squareT wk = Pos.GetKingSquare(WHITE);
    squareT bk = Pos.GetKingSquare(BLACK);
    fyleT wkFyle = square_Fyle(wk);
    fyleT bkFyle = square_Fyle(bk);

    // Check if each side should be storming the enemy king:
    if(!inEndgame)
    {
        if(wkFyle <= C_FYLE  &&  bkFyle >= F_FYLE)
        {
            midscore[WHITE] += pawnEntry.wLongbShortScore;
        }
        else if(wkFyle >= F_FYLE  &&  bkFyle <= C_FYLE)
        {
            midscore[WHITE] += pawnEntry.wShortbLongScore;
        }
    }

    // Iterate over the piece for each color:

    for(colorT c = WHITE; c <= BLACK; c++)
    {
        colorT enemy = color_Flip(c);
        // squareT ownKing = Pos.GetKingSquare(c);
        squareT enemyKing = Pos.GetKingSquare(enemy);
        unsigned int npieces = Pos.GetCount(c);
        squareT * sqlist = Pos.GetList(c);
        int mscore = 0;  // Middlegame score adjustments
        int escore = 0;  // Endgame score adjustments
        int ascore = 0;  // All-position adjustments (middle and endgame)

        for(unsigned int i = 0; i < npieces; i++)
        {
            squareT sq = sqlist[i];
            pieceT p = board[sq];
            pieceT ptype = piece_Type(p);
            ASSERT(p != EMPTY  &&  piece_Color(p) == c);
            squareT bonusSq = (c == WHITE) ? square_FlipRank(sq) : sq;
            unsigned int rank = RANK_1 + RANK_8 - square_Rank(bonusSq);

            // Piece-specific bonuses. The use of if-else instead of
            // a switch statement was observed to be faster since
            // the most common piece types are handled first.

            if(ptype == PAWN)
            {
                // Most pawn-specific bonuses are in ScorePawnStructure().

                // Kings should be close to pawns in endgames:
                // if (!inMiddlegame) {
                //     escore += 3 * square_Distance (sq, enemyKing)
                //             - 2 * square_Distance (sq, ownKing);
                // }
            }
            else if(ptype == ROOK)
            {
                ascore += RookFile[square_Fyle(sq)];
                if(rank == RANK_7)
                {
                    ascore += RookOnSeventh;
                    // Even bigger bonus if rook traps king on 8th rank:
                    bool kingOn8th = (p == WR) ? (bk >= A8) : (wk <= H1);
                    if(kingOn8th)
                    {
                        ascore += RookOnSeventh;
                    }
                }
                if(!inEndgame)
                {
                    mscore += RookKingDist[square_Distance(sq, enemyKing)];
                }
                if(!inMiddlegame)
                {
                    unsigned int mobility = Pos.Mobility(ROOK, c, sq);
                    escore += RookEndgameMobility [mobility];
                }
            }
            else if(ptype == KING)
            {
                if(Pos.GetCount(c) == 1)
                {
                    // Forcing a lone king to a corner:
                    ascore += 5 * KingEndgameSquare[bonusSq] - 150;
                }
                else
                {
                    mscore += KingSquare[bonusSq];
                    escore += KingEndgameSquare[bonusSq];
                }
            }
            else if(ptype == BISHOP)
            {
                ascore += BishopSquare[bonusSq];
                ascore += BishopMobility [Pos.Mobility(BISHOP, c, sq)];
                // Middlegame bonus for diagonal close to enemy king:
                if(! inEndgame)
                {
                    mscore += BishopKingDist[square_Distance(sq, enemyKing)];
                    // Reward a bishop attacking the enemy king vicinity:
                    int leftdiff = (int)square_LeftDiag(sq)
                                   - (int)square_LeftDiag(enemyKing);
                    int rightdiff = (int)square_RightDiag(sq)
                                    - (int)square_RightDiag(enemyKing);
                    if((leftdiff >= -2  &&  leftdiff <= 2)
                            || (rightdiff >= -2  &&  rightdiff <= 2))
                    {
                        mscore += BishopEyesKing;
                    }
                }
            }
            else if(ptype == KNIGHT)
            {
                ascore += KnightSquare[bonusSq];
                if(!inEndgame)
                {
                    mscore += KnightKingDist[square_Distance(sq, enemyKing)];
                    // Bonus for a useful outpost:
                    if(rank >= RANK_4  &&  !square_IsEdgeSquare(sq)
                            &&  isOutpost(board, sq, c))
                    {
                        mscore += KnightOutpost;
                    }
                }
                if(!inMiddlegame)
                {
                    // Penalty for knight in an endgame with enemy
                    // pawns on both wings.
                    pieceT enemyPawn = piece_Make(enemy, PAWN);
                    unsigned int qsidePawns = Pos.FyleCount(enemyPawn, A_FYLE)
                                              + Pos.FyleCount(enemyPawn, B_FYLE)
                                              + Pos.FyleCount(enemyPawn, C_FYLE);
                    unsigned int ksidePawns = Pos.FyleCount(enemyPawn, F_FYLE)
                                              + Pos.FyleCount(enemyPawn, G_FYLE)
                                              + Pos.FyleCount(enemyPawn, H_FYLE);
                    if(ksidePawns > 0  &&  qsidePawns > 0)
                    {
                        escore -= KnightBadEndgame;
                    }
                }
            }
            else /* (ptype == QUEEN) */
            {
                ASSERT(ptype == QUEEN);
                ascore += QueenSquare[bonusSq];
                ascore += QueenKingDist[square_Distance(sq, enemyKing)];
            }
        }
        allscore[c] += ascore;
        midscore[c] += mscore;
        endscore[c] += escore;
    }

    // Now reward rooks on open files or behind passed pawns:
    unsigned char passedPawnFyles =
        pawnEntry.fyleHasPassers[WHITE] | pawnEntry.fyleHasPassers[BLACK];
    for(colorT color = WHITE; color <= BLACK; color++)
    {
        pieceT rook = piece_Make(color, ROOK);
        if(pieceCount[rook] == 0)
        {
            continue;
        }
        colorT enemy = color_Flip(color);
        pieceT ownPawn = piece_Make(color, PAWN);
        pieceT enemyPawn = piece_Make(enemy, PAWN);
        fyleT enemyKingFyle = square_Fyle(Pos.GetKingSquare(enemy));
        int bonus = 0;

        for(fyleT fyle = A_FYLE; fyle <= H_FYLE; fyle++)
        {
            unsigned int nRooks = Pos.FyleCount(rook, fyle);
            if(nRooks == 0)
            {
                continue;
            }
            if(nRooks > 1)
            {
                bonus += DoubledRooks;
            }
            unsigned int passedPawnsOnFyle = passedPawnFyles & (1 << fyle);
            if(passedPawnsOnFyle != 0)
            {
                // Rook is on same file as a passed pawn.
                // TODO: make bonus bigger when rook is *behind* the pawn.
                bonus += RookPasserFile;
            }
            else if(Pos.FyleCount(ownPawn, fyle) == 0)
            {
                // Rook on open or half-open file:
                if(Pos.FyleCount(enemyPawn, fyle) == 0)
                {
                    bonus += RookOpenFile;
                }
                else
                {
                    bonus += RookHalfOpenFile;
                }
                // If this open/half-open file leads to a square adjacent
                // to the enemy king, give a further bonus:
                int fdiff = (int)fyle - (int)enemyKingFyle;
                if(fdiff >= -1  &&  fdiff < 1)
                {
                    bonus += RookEyesKing;
                }
            }
        }
        allscore[color] += bonus;
    }

    // King safety:
    if(! inEndgame)
    {
        if(pieceCount[BQ] > 0)
        {
            if(Pos.GetCastling(WHITE, KSIDE))
            {
                midscore[WHITE] += CanCastle;
            }
            if(Pos.GetCastling(WHITE, QSIDE))
            {
                midscore[WHITE] += CanCastle;
            }
        }
        if(pieceCount[WQ] > 0)
        {
            if(Pos.GetCastling(BLACK, KSIDE))
            {
                midscore[BLACK] += CanCastle;
            }
            if(Pos.GetCastling(BLACK, QSIDE))
            {
                midscore[BLACK] += CanCastle;
            }
        }
        // Bonus for pawn cover in front of a castled king. Actually we
        // also include bishops because they are important for defence.
        if(square_Rank(wk) == RANK_1  &&  wk != D1  &&  wk != E1)
        {
            unsigned int nCoverPawns = 0;
            pieceT p = board[square_Move(wk, UP_LEFT)];
            if(p == WP  ||  p == WB)
            {
                nCoverPawns++;
            }
            p = board[square_Move(wk, UP)];
            if(p == WP  ||  p == WB)
            {
                nCoverPawns++;
            }
            p = board[square_Move(wk, UP_RIGHT)];
            if(p == WP  ||  p == WB)
            {
                nCoverPawns++;
            }
            midscore[WHITE] += CoverPawn * nCoverPawns;
            if((wk == F1  ||  wk == G1)
                    && (board[G1] == WR || board[H1] == WR || board[H2] == WR))
            {
                midscore[WHITE] -= KingTrapsRook;
            }
            if((wk == C1  ||  wk == B1)
                    && (board[B1] == WR || board[A1] == WR || board[A2] == WR))
            {
                midscore[WHITE] -= KingTrapsRook;
            }
        }
        if(square_Rank(bk) == RANK_8  &&  bk != D8  &&  bk != E8)
        {
            unsigned int nCoverPawns = 0;
            pieceT p = board[square_Move(bk, DOWN_LEFT)];
            if(p == BP  ||  p == BB)
            {
                nCoverPawns++;
            }
            p = board[square_Move(bk, DOWN)];
            if(p == BP  ||  p == BB)
            {
                nCoverPawns++;
            }
            p = board[square_Move(bk, DOWN_RIGHT)];
            if(p == BP  ||  p == BB)
            {
                nCoverPawns++;
            }
            midscore[BLACK] += CoverPawn * nCoverPawns;
            if((bk == F8  ||  bk == G8)
                    && (board[G8] == BR || board[H8] == BR || board[H7] == BR))
            {
                midscore[BLACK] -= KingTrapsRook;
            }
            if((bk == C8  ||  bk == B8)
                    && (board[B8] == BR || board[A8] == BR || board[A7] == BR))
            {
                midscore[BLACK] -= KingTrapsRook;
            }
        }

        // Pawn centre:
        if((board[D4] == WP  ||  board[D5] == WP)
                && (board[E4] == WP  ||  board[E5] == WP))
        {
            midscore[WHITE] += CentralPawnPair;
        }
        if((board[D4] == BP  ||  board[D5] == BP)
                && (board[E4] == BP  ||  board[E5] == BP))
        {
            midscore[BLACK] += CentralPawnPair;
        }

        // Minor pieces developed:
        if(board[B1] != WN)
        {
            midscore[WHITE] += Development;
        }
        if(board[C1] != WB)
        {
            midscore[WHITE] += Development;
        }
        if(board[F1] != WB)
        {
            midscore[WHITE] += Development;
        }
        if(board[G1] != WN)
        {
            midscore[WHITE] += Development;
        }
        if(board[B8] != BN)
        {
            midscore[BLACK] += Development;
        }
        if(board[C8] != BB)
        {
            midscore[BLACK] += Development;
        }
        if(board[F8] != BB)
        {
            midscore[BLACK] += Development;
        }
        if(board[G8] != BN)
        {
            midscore[BLACK] += Development;
        }
    }

    // Work out the middlegame and endgame scores including pawn structure
    // evaluation, with a larger pawn structure weight in endgames:
    int baseScore = allscore[WHITE] - allscore[BLACK];
    int mgScore = baseScore + midscore[WHITE] - midscore[BLACK];
    int egScore = baseScore + endscore[WHITE] - endscore[BLACK];
    mgScore += pawnEntry.score;
    egScore += (pawnEntry.score * 5) / 4;

    // Scale down the endgame score for bishops of opposite colors, if both
    // sides have the same non-pawn material:
    if(pieceCount[WB] == 1  &&  pieceCount[BB] == 1)
    {
        if(Pos.SquareColorCount(WB, WHITE) != Pos.SquareColorCount(BB, WHITE))
        {
            if(pieceCount[WQ] == pieceCount[BQ]
                    && pieceCount[WR] == pieceCount[BR]
                    && pieceCount[WN] == pieceCount[BN])
            {
                egScore = egScore * 5 / 8;
            }
        }
    }

    // Negate scores for Black to move:
    if(toMove == BLACK)
    {
        mgScore = -mgScore;
        egScore = -egScore;
    }

    // Determine the final score from the middlegame and endgame scores:
    int finalScore = 0;
    if(inMiddlegame)
    {
        finalScore = mgScore;   // Use the middlegame score only.
    }
    else if(inEndgame)
    {
        finalScore = egScore;   // Use the endgame score only.
    }
    else
    {
        // The final score is a weighted mean of the two scores:
        int midpart = (pieceMaterial - EndgameValue) * mgScore;
        int endpart = (MiddlegameValue - pieceMaterial) * egScore;
        finalScore = (endpart + midpart) / (MiddlegameValue - EndgameValue);
    }
    return finalScore;
}

static unsigned int nPawnHashProbes = 0;
static unsigned int nPawnHashHits = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::ScorePawnStructure
//   Fill in the provided pawnTableEntryT structure with pawn structure
//   scoring information, using the pawn hash table wherever possible.
void
Engine::ScorePawnStructure(pawnTableEntryT * pawnEntry)
{
    nPawnHashProbes++;
    unsigned int pawnhash = Pos.PawnHashValue();
    // We only use 32-bit hash values, so without further safety checks
    // the rate of false hits in the pawn hash table could be high.
    // To reduce the chance of false hits, we compute an extra signature.
    unsigned int sig = (Pos.SquareColorCount(WP, WHITE) << 12)
                       | (Pos.SquareColorCount(BP, BLACK) << 8)
                       | (Pos.PieceCount(WP) << 4) | Pos.PieceCount(BP);
    pawnEntry->pawnhash = pawnhash;
    pawnEntry->sig = sig;
    pawnEntry->fyleHasPassers[WHITE] = 0;
    pawnEntry->fyleHasPassers[BLACK] = 0;

    bool inPawnEndgame = (Pos.NumNonPawns(WHITE) == 1
                          &&  Pos.NumNonPawns(BLACK) == 1);
    pawnTableEntryT * hashEntry = NULL;

    // Check for a pawn hash table hit, but not in pawn endings:
    if(!inPawnEndgame)
    {
        unsigned int hashSlot = pawnhash % PawnTableSize;
        hashEntry = &(PawnTable[hashSlot]);
        if(pawnhash == hashEntry->pawnhash  &&  sig == hashEntry->sig)
        {
            nPawnHashHits++;
            *pawnEntry = *hashEntry;
            return;
        }
    }

    // The pawnFiles array contains the number of pawns of each color on
    // each file. Indexes 1-8 are used while 0 and 9 are empty dummy files
    // added so that even the a and h files have two adjacent files, making
    // isolated/passed pawn calculation easier.
    unsigned int pawnFiles[2][10] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    // firstRank stores the rank of the leading pawn on each file.
    unsigned int firstRank[2][10] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    // lastRank stores the rank of the rearmost pawn on each file.
    unsigned int lastRank[2][10]  = { {7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7}
    };

    int pawnScore[2] = {0, 0};
    int longVsShortScore[2] = {0, 0};  // Pawn storm bonuses, O-O-O vs O-O
    int shortVsLongScore[2] = {0, 0};  // Pawn storm bonuses, O-O vs O-O-O
    rankT bestRacingPawn[2] = {RANK_1, RANK_1};

    for(fyleT f = A_FYLE; f <= H_FYLE; f++)
    {
        pawnFiles[WHITE][f + 1] = Pos.FyleCount(WP, f);
        pawnFiles[BLACK][f + 1] = Pos.FyleCount(BP, f);
    }

    for(colorT c = WHITE; c <= BLACK; c++)
    {
        pieceT pawn = piece_Make(c, PAWN);
        unsigned int npawns = Pos.PieceCount(pawn);
        SquareList sqlist;
        Pos.GetSquares(pawn, &sqlist);
        for(unsigned int i = 0; i < npawns; i++)
        {
            squareT sq = sqlist.Get(i);
            squareT wsq = (c == WHITE) ? sq : square_FlipRank(sq);
            squareT bonusSq = square_FlipRank(wsq);
            pawnScore[c] += PawnSquare[bonusSq];
            longVsShortScore[c] += PawnStorm[bonusSq];
            shortVsLongScore[c] += PawnStorm[square_FlipFyle(bonusSq)];
            unsigned int fyle = square_Fyle(wsq) + 1;
            unsigned int rank = square_Rank(wsq);
            if(rank > firstRank[c][fyle])
            {
                firstRank[c][fyle] = rank;
            }
            if(rank < lastRank[c][fyle])
            {
                lastRank[c][fyle] = rank;
            }
        }
    }

    unsigned char fyleHasPassers[2] = {0, 0};

    for(colorT color = WHITE; color <= BLACK; color++)
    {
        if(Pos.PieceCount(piece_Make(color, PAWN)) == 0)
        {
            continue;
        }
        colorT enemy = color_Flip(color);

        for(unsigned int fyle = 1; fyle <= 8; fyle++)
        {
            unsigned int pawnCount = pawnFiles[color][fyle];
            if(pawnCount == 0)
            {
                continue;
            }
            unsigned int pawnRank = firstRank[color][fyle];

            // Doubled pawn penalty:
            if(pawnCount > 1)
            {
                pawnScore[color] -= DoubledPawn * pawnCount;
            }

            // Isolated pawn penalty:
            bool isolated = false;
            if(pawnFiles[color][fyle - 1] == 0
                    &&  pawnFiles[color][fyle + 1] == 0)
            {
                isolated = true;
                pawnScore[color] -= IsolatedPawn * pawnCount;
                // Extra penalty for isolated on half-open file:
                if(pawnFiles[enemy][fyle] == 0)
                {
                    pawnScore[color] -= IsolatedPawn * pawnCount / 2;
                }
            }
            else if(lastRank[color][fyle - 1] > lastRank[color][fyle]
                    &&  lastRank[color][fyle + 1] > lastRank[color][fyle])
            {
                // Not isolated, but backward:
                pawnScore[color] -= BackwardPawn;
                // Extra penalty for backward on half-open file:
                if(pawnFiles[enemy][fyle] == 0)
                {
                    pawnScore[color] -= BackwardPawn;
                }
            }

            // Passed pawn bonus:
            if(pawnRank >= 7 - lastRank[enemy][fyle]
                    &&  pawnRank >= 7 - lastRank[enemy][fyle - 1]
                    &&  pawnRank >= 7 - lastRank[enemy][fyle + 1])
            {
                int bonus = PassedPawnRank[pawnRank];
                // Smaller bonus for rook-file or isolated passed pawns:
                if(fyle == 1  ||  fyle == 8  ||  isolated)
                {
                    bonus = bonus * 3 / 4;
                }
                // Bigger bonus for a passed pawn protected by another pawn:
                if(!isolated)
                {
                    if(pawnRank == firstRank[color][fyle - 1] + 1
                            ||  pawnRank == firstRank[color][fyle + 1] + 1)
                    {
                        bonus = (bonus * 3) / 2;
                    }
                }
                pawnScore[color] += bonus;
                // Update the passed-pawn-files bitmap:
                fyleHasPassers[color] |= (1 << (fyle - 1));

                // Give a big bonus for a connected passed pawn on
                // the 6th or 7th rank.
                if(pawnRank >= RANK_6  &&  pawnFiles[color][fyle - 1] > 0
                        &&  firstRank[color][fyle - 1] >= RANK_6)
                {
                    // pawnScore[color] += some_bonus...;
                }

                // Check for passed pawn races in pawn endgames:
                if(inPawnEndgame)
                {
                    // Check if the enemy king is outside the square:
                    squareT kingSq = Pos.GetKingSquare(color_Flip(color));
                    squareT pawnSq = square_Make(fyle - 1, pawnRank);
                    squareT promoSq = square_Make(fyle - 1, RANK_8);
                    if(color == BLACK)
                    {
                        pawnSq = square_FlipRank(pawnSq);
                        promoSq = square_FlipRank(promoSq);
                    }
                    unsigned int kingDist = square_Distance(kingSq, promoSq);
                    unsigned int pawnDist = square_Distance(pawnSq, promoSq);
                    if(color != Pos.GetToMove())
                    {
                        pawnDist++;
                    }
                    if(pawnDist < kingDist)
                    {
                        bestRacingPawn[color] = pawnRank;
                    }
                }
            }
        }
    }

    int score = pawnScore[WHITE] - pawnScore[BLACK];
    pawnEntry->score = score;
    pawnEntry->fyleHasPassers[WHITE] = fyleHasPassers[WHITE];
    pawnEntry->fyleHasPassers[BLACK] = fyleHasPassers[BLACK];
    pawnEntry->wLongbShortScore = longVsShortScore[WHITE] - shortVsLongScore[BLACK];
    pawnEntry->wShortbLongScore = shortVsLongScore[WHITE] - longVsShortScore[BLACK];

    // If not a pawn endgame, store the score in the pawn hash table:
    if(!inPawnEndgame)
    {
        *hashEntry = *pawnEntry;
        return;
    }

    // This is a pawn endgame, so we cannot store the score in the
    // pawn hash table since we include king positions as a factor.
    // If one side has a pawn that clearly queens before the best
    // enemy pawn in a race (where kings cannot catch the pawns),
    // give a huge bonus since it almost certainly wins:

    if(bestRacingPawn[WHITE] > bestRacingPawn[BLACK] + 1)
    {
        pawnEntry->score += RookValue;
    }
    else if(bestRacingPawn[BLACK] > bestRacingPawn[WHITE] + 1)
    {
        pawnEntry->score -= RookValue;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::IsMatingScore
//   Returns true if the score indicates the side to move will checkmate.
inline bool
Engine::IsMatingScore(int score)
{
    return (score > (Infinity - (int)ENGINE_MAX_PLY));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::IsGettingMatedScore
//   Returns true if the score indicates the side to move will be checkmated.
inline bool
Engine::IsGettingMatedScore(int score)
{
    return (score < (-Infinity + (int)ENGINE_MAX_PLY));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::PlayMove
//   Play the specified move, not in a search.
void
Engine::PlayMove(simpleMoveT * sm)
{
    PushRepeat(&RootPos);
    RootPos.DoSimpleMove(sm);
    Pos.DoSimpleMove(sm);
    simpleMoveT * newMove = new simpleMoveT;
    *newMove = *sm;
    GameMoves[NumGameMoves] = newMove;
    NumGameMoves++;
    // Change the transposition table sequence number:
    TranTableSequence++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::RetractMove
//    Take back a move played in the game.
void
Engine::RetractMove(void)
{
    if(NumGameMoves == 0)
    {
        return;
    }
    PopRepeat();
    NumGameMoves--;
    RootPos.UndoSimpleMove(GameMoves[NumGameMoves]);
    Pos.UndoSimpleMove(GameMoves[NumGameMoves]);
    delete GameMoves[NumGameMoves];
    TranTableSequence--;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::DoMove
//   Make the specified move in a search.
inline void
Engine::DoMove(simpleMoveT * sm)
{
    PushRepeat(&Pos);
    Pos.DoSimpleMove(sm);
    Ply++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::UndoMove
//    Take back the specified move in a search.
inline void
Engine::UndoMove(simpleMoveT * sm)
{
    PopRepeat();
    Pos.UndoSimpleMove(sm);
    Ply--;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::PushRepeat
//    Remember the current position on the repetition stack.
inline void
Engine::PushRepeat(Position * pos)
{
    repeatT * rep = &(RepStack[RepStackSize]);
    rep->hash = pos->HashValue();
    rep->pawnhash = pos->PawnHashValue();
    rep->npieces = pos->TotalMaterial();
    rep->stm = pos->GetToMove();
    RepStackSize++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::PopRepeat
//   Pops the last entry off the repetition stack.
inline void
Engine::PopRepeat(void)
{
    ASSERT(RepStackSize > 0);
    RepStackSize--;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::NoMatingMaterial
//   Returns true if the position is a certain draw through neither
//   side having mating material.
bool
Engine::NoMatingMaterial(void)
{
    unsigned int npieces = Pos.TotalMaterial();

    // Check for K vs K, K+N vs K, and K+B vs K:
    if(npieces <= 2)
    {
        return true;
    }
    if(npieces == 3)
    {
        unsigned char * material = Pos.GetMaterial();
        if(material[WB] == 1  ||  material[WN] == 1)
        {
            return true;
        }
        if(material[BB] == 1  ||  material[BN] == 1)
        {
            return true;
        }
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::FiftyMoveDraw
//   Returns  true if a draw has been reached through fifty full
//   moves since the last capture or pawn move.
bool
Engine::FiftyMoveDraw(void)
{
    if(RepStackSize < 100)
    {
        return false;
    }

    unsigned int pawnhash = Pos.PawnHashValue();
    unsigned int npieces = Pos.TotalMaterial();

    // Go back through the stack of hash values:
    unsigned int plycount = 0;
    for(unsigned int i = RepStackSize; i > 0; i--)
    {
        repeatT * rep = &(RepStack[i - 1]);
        // Stop at an irreversible move:
        if(npieces != rep->npieces)
        {
            break;
        }
        if(pawnhash != rep->pawnhash)
        {
            break;
        }
        plycount++;
    }
    if(plycount >= 100)
    {
        return true;
    }
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::RepeatedPosition
//   Returns the number if times the current position has been reached,
//   with the same side to move, castling and en passant settings.
//   The current occurrence is included in the returned count.
unsigned int
Engine::RepeatedPosition(void)
{
    unsigned int hash = Pos.HashValue();
    unsigned int pawnhash = Pos.PawnHashValue();
    unsigned int npieces = Pos.TotalMaterial();
    colorT stm = Pos.GetToMove();

    // Go back through the stack of hash values detecting repetition:
    unsigned int ntimes = 1;
    for(unsigned int i = RepStackSize; i > 0; i--)
    {
        repeatT * rep = &(RepStack[i - 1]);
        // Stop at an irreversible move:
        if(npieces != rep->npieces)
        {
            break;
        }
        if(pawnhash != rep->pawnhash)
        {
            break;
        }
        // Look for repetition:
        if(hash == rep->hash  &&  stm == rep->stm)
        {
            ntimes++;
        }
    }
    return ntimes;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::SetHashTableKilobytes
//   Set the transposition table size in kilobytes.
void
Engine::SetHashTableKilobytes(unsigned int size)
{
    // Compute the number of entries, which must be even:
    unsigned int bytes = size * 1024;
    TranTableSize = bytes / sizeof(transTableEntryT);
    if((TranTableSize % 2) == 1)
    {
        TranTableSize--;
    }
    if(TranTable != NULL)
    {
        delete[] TranTable;
    }
    TranTable = new transTableEntryT [TranTableSize];
    ClearHashTable();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::SetPawnTableKilobytes
//   Set the pawn structure hash table size in kilobytes.
void
Engine::SetPawnTableKilobytes(unsigned int size)
{
    // Compute the number of entries:
    unsigned int bytes = size * 1024;
    PawnTableSize = bytes / sizeof(pawnTableEntryT);
    if(PawnTable != NULL)
    {
        delete[] PawnTable;
    }
    PawnTable = new pawnTableEntryT [PawnTableSize];
    ClearPawnTable();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::ClearHashTable
//   Clear the transposition table.
void
Engine::ClearHashTable(void)
{
    for(unsigned int i = 0; i < TranTableSize; i++)
    {
        TranTable[i].flags = SCORE_NONE;
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::ClearPawnTable
//   Clear the pawn structure hash table.
void
Engine::ClearPawnTable(void)
{
    for(unsigned int i = 0; i < PawnTableSize; i++)
    {
        PawnTable[i].pawnhash = 0;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// tte_Get/Set functions
//   Helpers for packing/extracting transposition table entry fields.

inline void tte_SetFlags(transTableEntryT * tte, scoreFlagT sflag,
                         colorT stm, unsigned char castling, bool isOnlyMove)
{
    tte->flags = (castling << 4) | (stm << 3) | (isOnlyMove ? 4 : 0) | sflag;
}

inline scoreFlagT tte_ScoreFlag(transTableEntryT * tte)
{
    return (tte->flags & 7);
}

inline colorT tte_SideToMove(transTableEntryT * tte)
{
    return ((tte->flags >> 3) & 1);
}

inline unsigned char tte_Castling(transTableEntryT * tte)
{
    return (tte->flags >> 4);
}

inline bool tte_IsOnlyMove(transTableEntryT * tte)
{
    return (((tte->flags >> 2) & 1) == 1);
}

inline void tte_SetBestMove(transTableEntryT * tte, simpleMoveT * bestMove)
{
    ASSERT(bestMove->from <= H8  &&  bestMove->to <= H8);
    unsigned short bm = bestMove->from;
    bm <<= 6;
    bm |= bestMove->to;
    bm <<= 4;
    bm |= bestMove->promote;
    tte->bestMove = bm;
}

inline void tte_GetBestMove(transTableEntryT * tte, simpleMoveT * bestMove)
{
    unsigned short bm = tte->bestMove;
    bestMove->promote = bm & 15;
    bm >>= 4;
    bestMove->to = bm & 63;
    bm >>= 6;
    bestMove->from = bm & 63;
}

static unsigned int ProbeCounts[4] = {0};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::StoreHash
//   Store the score for the current position in the transposition table.
void
Engine::StoreHash(int depth, scoreFlagT ttFlag, int score,
                  simpleMoveT * bestMove, bool isOnlyMove)
{
    if(TranTableSize == 0)
    {
        return;
    }
    ASSERT(ttFlag <= SCORE_UPPER);

    unsigned int hash = Pos.HashValue();
    unsigned int pawnhash = Pos.PawnHashValue();
    colorT stm = Pos.GetToMove();
    if(stm == BLACK)
    {
        hash = ~hash;
    }

    // Find the least useful (lowest depth) of two entries to replace
    // but replace the previous entry for this position if it exists
    // and use an empty hash table entry if possible:

    unsigned int ttSlot = (hash % TranTableSize) & 0xFFFFFFFEU;
    ASSERT(ttSlot < TranTableSize - 1);
    transTableEntryT * ttEntry1 = &(TranTable[ttSlot]);
    transTableEntryT * ttEntry2 = &(TranTable[ttSlot + 1]);
    bool replacingSameEntry = false;

    transTableEntryT * ttEntry;
    if(ttEntry1->hash == hash  &&  ttEntry1->pawnhash == pawnhash)
    {
        ttEntry = ttEntry1;    // Replace this existing entry.
        replacingSameEntry = true;
    }
    else if(ttEntry2->hash == hash  &&  ttEntry2->pawnhash == pawnhash)
    {
        ttEntry = ttEntry2;    // Replace this existing entry.
        replacingSameEntry = true;
    }
    else if(tte_ScoreFlag(ttEntry1) == SCORE_NONE)
    {
        ttEntry = ttEntry1;    // Use this empty entry.
    }
    else if(tte_ScoreFlag(ttEntry2) == SCORE_NONE)
    {
        ttEntry = ttEntry2;    // Use this empty entry.
    }
    else
    {
        // Replace the entry with the shallower depth, unless the deeper
        // entry has an old sequence number:
        transTableEntryT * ttDeeper = ttEntry1;
        transTableEntryT * ttShallower = ttEntry2;
        if(ttEntry1->depth < ttEntry2->depth)
        {
            ttDeeper = ttEntry2;
            ttShallower = ttEntry1;
        }
        if(ttShallower->sequence != TranTableSequence)
        {
            ttEntry = ttShallower;    // Replace this old entry
        }
        else if(ttDeeper->sequence != TranTableSequence)
        {
            ttEntry = ttDeeper;       // Replace this old entry
        }
        else
        {
            ttEntry = ttShallower;    // Replace this shallow entry
        }
    }

    if(replacingSameEntry)
    {
        if(depth < ttEntry->depth)
        {
            // Do not overwrite an existing better entry for the same
            // position; but if there was no move, add one:
            if(ttEntry->bestMove == 0  &&  bestMove != NULL)
            {
                tte_SetBestMove(ttEntry, bestMove);
            }
            return;
        }
        if(depth == ttEntry->depth)
        {
            // Do not replace an exact score entry of the same depth for
            // the same position with an inexact entry:
            if(tte_ScoreFlag(ttEntry) == SCORE_EXACT  &&  ttFlag != SCORE_EXACT)
            {
                return;
            }
        }
    }

    // Convert mating scores to include the current Ply count:
    if(IsMatingScore(score))
    {
        score += Ply;
    }
    if(IsGettingMatedScore(score))
    {
        score -= Ply;
    }

    // Fill in the hash entry fields:
    ttEntry->hash = hash;
    ttEntry->pawnhash = pawnhash;
    ttEntry->depth = depth;
    ttEntry->score = score;
    tte_SetFlags(ttEntry, ttFlag, stm, Pos.GetCastlingFlags(), isOnlyMove);
    ttEntry->sequence = TranTableSequence;
    ttEntry->bestMove = 0;
    if(bestMove != NULL)
    {
        ASSERT(bestMove->movingPiece != EMPTY);
        ASSERT(piece_Color(bestMove->movingPiece) == stm);
        ASSERT(bestMove->from <= H8);
        tte_SetBestMove(ttEntry, bestMove);
    }
    ttEntry->enpassant = Pos.GetEPTarget();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::ProbeHash
//    Probe the transposition table for the current position.
//
scoreFlagT
Engine::ProbeHash(int depth, int * score, simpleMoveT * bestMove, bool * isOnlyMove)
{
    // Clear the best move:
    if(bestMove != NULL)
    {
        bestMove->from = bestMove->to = NULL_SQUARE;
    }

    if(TranTableSize == 0)
    {
        return SCORE_NONE;
    }

    unsigned int hash = Pos.HashValue();
    colorT stm = Pos.GetToMove();
    if(stm == BLACK)
    {
        hash = ~hash;
    }

    // Examine the corresponding pair of table entries:
    unsigned int ttSlot = (hash % TranTableSize) & 0xFFFFFFFEU;
    ASSERT(ttSlot + 1 < TranTableSize);
    transTableEntryT * ttEntry = &(TranTable[ttSlot]);
    if(ttEntry->hash != hash)
    {
        ttEntry++;
    }
    if(ttEntry->hash != hash)
    {
        return SCORE_NONE;
    }
    if(tte_ScoreFlag(ttEntry) == SCORE_NONE)
    {
        return SCORE_NONE;
    }
    unsigned int pawnhash = Pos.PawnHashValue();
    if(ttEntry->pawnhash != pawnhash)
    {
        return SCORE_NONE;
    }
    if(tte_SideToMove(ttEntry) != stm)
    {
        return SCORE_NONE;
    }
    if(tte_Castling(ttEntry) != Pos.GetCastlingFlags())
    {
        return SCORE_NONE;
    }
    if(ttEntry->enpassant != Pos.GetEPTarget())
    {
        return SCORE_NONE;
    }

    // If a hash move is stored, we return it even if the depth is not
    // sufficient, because it will be useful for move ordering anyway.
    if(bestMove != NULL  &&  ttEntry->bestMove != 0)
    {
        tte_GetBestMove(ttEntry, bestMove);
        pieceT * board = Pos.GetBoard();
        bestMove->movingPiece = board[bestMove->from];
    }
    if(isOnlyMove != NULL)
    {
        *isOnlyMove = tte_IsOnlyMove(ttEntry);
    }
    // Only return an exact or bounded score if the stored depth is at
    // least as large as the requested depth:
    if(ttEntry->depth < depth)
    {
        return SCORE_NONE;
    }
    if(score != NULL)
    {
        *score = ttEntry->score;
        // Convert mating scores to exclude the current Ply count:
        if(IsMatingScore(*score))
        {
            *score -= Ply;
        }
        if(IsGettingMatedScore(*score))
        {
            *score += Ply;
        }
    }
    return tte_ScoreFlag(ttEntry);
}

static unsigned int nFailHigh = 0;
static unsigned int nFailHighFirstMove = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::SetPosition
//   Set the current position. If the new position parameter
//   is NULL, the standard starting position is used.
void
Engine::SetPosition(Position * newpos)
{
    // Delete old game moves:
    for(unsigned int i = 0; i < NumGameMoves; i++)
    {
        delete GameMoves[i];
    }
    NumGameMoves = 0;

    // Set the position:
    if(newpos == NULL)
    {
        RootPos.StdStart();
        Pos.StdStart();
    }
    else
    {
        RootPos.CopyFrom(newpos);
        Pos.CopyFrom(newpos);
    }

    // Clear the repetition stack:
    RepStackSize = 0;

    // Clear the PV:
    PV[0].length = 0;

    // Change the tranposition table sequence number so existing
    // entries can be detected as old ones:
    TranTableSequence++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::Think
//   Initiate a search from the current position. If the supplied
//   move list is NULL, generate and examine all legal moves at the
//   root position. However, if the move list is not NULL, it
//   contains a subset of the legal moves to be analyzed.
//
//   Returns the score (in centipawns, for the side to move) and
//   reorders the move list (if supplied) so the best move is at
//   the start of the list.
int
Engine::Think(MoveList * mlist)
{
    Elapsed.restart();
    NodeCount = 0;
    QNodeCount = 0;
    Ply = 0;
    IsOutOfTime = false;
    EasyMove = false;
    HardMove = false;
    InNullMove = 0;
    SetPVLength();

    ClearKillerMoves();
    ClearHistoryValues();

    // If no legal move list was specified, generate and search all moves:
    if(mlist == NULL || !mlist->size())
    {
        Pos.GenerateMoves();
        mlist = Pos.GetLegalMoves();
    }

    // No legal moves? Return 0 for stalemate, -Infinity for checkmate.
    if(mlist->size() == 0)
    {
        return (Pos.IsKingInCheck() ? -Infinity : 0);
    }

    // Sort the root move list by quiescent evaluation to get a
    // reasonably good initial move order:
    for(unsigned int i = 0; (int) i < mlist->size(); i++)
    {
        simpleMoveT * sm = mlist->Get(i);
        DoMove(sm);
        sm->score = -Quiesce(-Infinity, Infinity);
        UndoMove(sm);
    }
    mlist->Sort();

    // Check for an easy move, one that scores more than two pawns
    // better than any alternative:
    if(mlist->size() > 1)
    {
        int margin = mlist->Get(0)->score - mlist->Get(1)->score;
        if(margin > (2 * PawnValue))
        {
            // Output ("Easy move: margin = %d\n", margin);
            EasyMove = true;
        }
    }

    int bestScore = -Infinity;

    // Do iterative deepening starting at depth 1, until out of
    // time or the maximum depth is reached:
    for(unsigned int depth = 1; depth <= MaxDepth; depth++)
    {
        HardMove = false;

        // If we have searched at least a few ply, and there is less
        // than 30% of the recommended search time remaining, then
        // continuing the search is unlikely to be useful since it
        // will probably spend all remaining time on the first move:
        if(depth > 4)
        {
            double used = (double)Elapsed.elapsed() / (double)SearchTime;
            if(used > 0.7)
            {
                break;
            }
        }

        // Set up the alpha-beta range. For all but the first depth,
        // use a small aspiration window around the previous score
        // since we do not expect the score to change much:
        int alpha = -Infinity - 1;
        int beta = Infinity + 1;
        if(depth > 1)
        {
            alpha = bestScore - AspirationWindow;
            beta = bestScore + AspirationWindow;
        }

        int score = SearchRoot(depth, alpha, beta, mlist);
        if(OutOfTime())
        {
            break;
        }
        if(score >= beta)
        {
            // Aspiration window fail-high:
            PrintPV(depth, score, "++");
            alpha = score - 1;
            beta = Infinity + 1;
            score = SearchRoot(depth, alpha, beta, mlist);
        }
        else if(score <= alpha)
        {
            // Aspiration window fail-low:
            PrintPV(depth, score, "--");
            EasyMove = false;
            HardMove = true;
            alpha = -Infinity - 1;
            beta = score + 1;
            score = SearchRoot(depth, alpha, beta, mlist);
        }
        if(OutOfTime())
        {
            break;
        }
        // If the 2nd search failed, try again with an infinite window.
        // This is rare, but can happen with hashing/null-move effects.
        if(score < alpha  ||  score > beta)
        {
            alpha = -Infinity;
            beta = Infinity;
            EasyMove = false;
            HardMove = true;
            score = SearchRoot(depth, alpha, beta, mlist);
        }

        if(OutOfTime())
        {
            break;
        }
        bestScore = score;
        PrintPV(depth, bestScore, ">>>");

        // Stop if checkmate has been found, but not too soon:
        if(depth >= 5  &&  IsMatingScore(bestScore))
        {
            break;
        }

        // Make sure the first move in the list remains there by
        // giving it a huge node count for its move ordering score:
        mlist->Get(0)->score = 1 << 30;

        // Sort the move list based on node counts from this iteration:
        mlist->Sort();
    }

    // Statistics for debugging:
//    Output ("Hash probes: Exact:%u Upper:%u Lower:%u None:%u\n",
//            ProbeCounts[SCORE_EXACT], ProbeCounts[SCORE_UPPER],
//            ProbeCounts[SCORE_LOWER], ProbeCounts[SCORE_NONE]);
//    Output ("Fail-High first move: %u / %u = %f\n", nFailHighFirstMove, nFailHigh,
//            (double)nFailHighFirstMove * 100.0 / (double)nFailHigh);
//    Output ("Pawn hash hits: %u / %u = %f\n", nPawnHashHits, nPawnHashProbes,
//            (double)nPawnHashHits * 100.0 / (double)nPawnHashProbes);
//    Output ("Full scores: %u / %u = %f\n", nScoreFull, nScoreCalls,
//            (double)nScoreFull * 100.0 / (double) nScoreCalls);

    return bestScore;
}

int
Engine::SearchRoot(int depth, int alpha, int beta, MoveList * mlist)
{
    ASSERT(depth >= 1);

    // If no legal move list was specified, generate and search all moves:
    if(mlist == NULL)
    {
        Pos.GenerateMoves();
        mlist = Pos.GetLegalMoves();
    }

    // No legal moves to search? Just return an equal score for
    // stalemate or -Infinity for checkmate.
    if(mlist->size() == 0)
    {
        return (Pos.IsKingInCheck() ? -Infinity : 0);
    }

    bool isOnlyMove = (mlist->size() == 1);
    int bestScore = -Infinity - 1;

    for(unsigned int movenum = 0; (int) movenum < mlist->size(); movenum++)
    {
        simpleMoveT * sm = mlist->Get(movenum);
        unsigned int oldNodeCount = NodeCount;
        // Make this move and search it:
        DoMove(sm);
        InCheck[Ply] = Pos.IsKingInCheck(sm);
#define PVS_SEARCH
#ifdef PVS_SEARCH
        int score = alpha;
        if(movenum == 0)
        {
            score = -Search(depth - 1, -beta, -alpha, true);
        }
        else
        {
            // Do a minimal window search first, to try and quickly
            // identify the common case of a move not being good
            // enough to improve alpha:
            score = -Search(depth - 1, -alpha - 1, -alpha, true);
            if(score > alpha  &&  score < beta)
            {
                // This move is good enough to search with the proper
                // window; use the score it returned as the lower bound:
                score = -Search(depth - 1, -beta, -score, true);
            }
        }
#else
        int score = -Search(depth - 1, -beta, -alpha, true);
#endif
        UndoMove(sm);
        if(OutOfTime())
        {
            break;
        }

        // Set the move ordering score of this move to be the number of
        // nodes spent on it, so interesting moves of this iteration are
        // searched first at the next iteration depth:
        sm->score = NodeCount - oldNodeCount;

        // If this is the first move searched at this depth or
        // a new best move, update the best score and promote
        // the move to be first in the list:
        if(movenum == 0  ||  score > bestScore)
        {
            bestScore = score;
            alpha = score;
            UpdatePV(sm);
            PrintPV(depth, bestScore);
            StoreHash(depth, SCORE_EXACT, score, sm, isOnlyMove);
            mlist->MoveToFront(movenum);
            if(movenum > 0)
            {
                EasyMove = false;
            }
        }
    }
    return bestScore;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::Search
//   Internal Search routine, used at every depth except
//   the root position.
int
Engine::Search(int depth, int alpha, int beta, bool tryNullMove)
{
    SetPVLength();

    // If there is no remaining depth, return a qiuescent evaluation:
    if(depth <= 0)
    {
        return Quiesce(alpha, beta);
    }

    // Check that the absolute depth limit is not exceeded:
    if(Ply >= ENGINE_MAX_PLY - 1)
    {
        return alpha;
    }

    // Check for a drawn position (no mating material, repetition, etc):
    if(NoMatingMaterial())
    {
        return 0;
    }
    if(FiftyMoveDraw())
    {
        return 0;
    }
    unsigned int repeats = RepeatedPosition();
    if(repeats >= 3  || (repeats == 2  &&  Ply > 2))
    {
        return 0;
    }

    colorT toMove = Pos.GetToMove();
    NodeCount++;

    // Stop now if we ran out of time:
    if(OutOfTime())
    {
        return alpha;
    }

    // Check for a recognized endgame score:
    if(Pos.TotalMaterial() <= Recognizer::MaxPieces())
    {
        int recog = Recognizer::Recognize(&Pos);
        int rscore = recogScore(recog);
        scoreFlagT rflag = recogFlag(recog);

        if(rflag == SCORE_EXACT)
        {
            return rscore;
        }
        else if(rflag == SCORE_LOWER)
        {
            if(rscore >= beta)
            {
                return rscore;
            }
            if(rscore < alpha)
            {
                alpha = rscore;
            }
        }
        else if(rflag == SCORE_UPPER)
        {
            if(rscore <= alpha)
            {
                return rscore;
            }
            if(rscore > beta)
            {
                beta = rscore;
            }
        }
    }

    // Probe the hash table:
    int hashscore = alpha;
    simpleMoveT hashmove;
    bool isOnlyMove;
    scoreFlagT hashflag = ProbeHash(depth, &hashscore, &hashmove, &isOnlyMove);
    ProbeCounts[hashflag]++;

    switch(hashflag)
    {
    case SCORE_NONE:
        break;
    case SCORE_LOWER:
        if(hashscore >= beta)
        {
            return hashscore;
        }
        if(hashscore > alpha)
        {
            alpha = hashscore;
        }
        break;
    case SCORE_UPPER:
        if(hashscore <= alpha)
        {
            return hashscore;
        }
        if(hashscore < beta)
        {
            beta = hashscore;
        }
        break;
    case SCORE_EXACT:
        if(hashscore > alpha  &&  hashscore < beta)
        {
            UpdatePV(&hashmove);
        }
        return hashscore;
    }

    int baseExtensions = 0;
    bool inCheck = InCheck[Ply];

    // Null move pruning:
    // If the side to move has at least a few pieces (to reduce the risk
    // of zugzwang) and is not in check, and a null move was not made to
    // reach this point in the search, try making a null move now. The
    // idea is to pass on our move and see (with a shallow search) if
    // if the enemy has any move that can score better than the beta
    // cutoff. If they have no such move, it means our position is good
    // enough to cut off the search without even considering our own
    // possible moves.

    if(inCheck  ||  depth < 2  ||  Pos.NumNonPawns(toMove) < 3)
    {
        tryNullMove = false;
    }

    if(tryNullMove)
    {
        Pos.SetToMove(color_Flip(toMove));
        squareT oldEPTarget = Pos.GetEPTarget();
        Pos.SetEPTarget(NULL_SQUARE);
        // We keep track of whether we are in a null move search or
        // not, to avoid updating the PV.
        InNullMove++;
        // Do an R=2 or R=3 nullmove search, depending on remaining depth:
        int nulldepth = depth - NullMoveReduction;
        if(depth > 6)
        {
            nulldepth--;   // An R=3 null move search.
        }
        int nullscore = -Search(nulldepth - 1, -beta, -beta + 1, false);
        InNullMove--;
        Pos.SetEPTarget(oldEPTarget);
        Pos.SetToMove(toMove);

        // If the null-move score is better than beta, cut the search:
        if(nullscore >= beta)
        {
            return beta;
        }

        // If the null-move score indicates that making a null move
        // would lead to us getting mated, extend the search another
        // ply to try and avoid the mate threats:
        if(IsGettingMatedScore(nullscore))
        {
            baseExtensions++;
        }
    }

    // In-check extension: search one ply deeper if we are in check.
    if(inCheck)
    {
        baseExtensions++;
    }

    // Now we want to generate all legal moves and order them. But if
    // we got a move from the hash table, it is worth trying that move
    // first, and only generating and scoring the rest of the moves if
    // the hash move does not cause a beta cutoff.
    // Note that we already know whether the side to move is in check,
    // so we pass this information to GenerateMoves to speed it up.

    MoveList mlist;
    bool gotHashMove;
    if(Pos.IsLegalMove(&hashmove))
    {
        gotHashMove = true;
        // For now, we only add the hash move to the move list.
        mlist.append(hashmove);
        mlist.Get(0)->score = ENGINE_HASH_SCORE;
    }
    else
    {
        // No hash table move, so generate and score all the moves now.
        gotHashMove = false;
        Pos.GenerateMoves(&mlist, EMPTY, GEN_ALL_MOVES, InCheck[Ply]);
        ScoreMoves(&mlist);
        isOnlyMove = (mlist.size() == 1);
    }

    // If there is only one legal move, extend the search:
    if(isOnlyMove)
    {
        baseExtensions++;
    }

    // Remember the original alpha score:
    int oldAlpha = alpha;
    int bestMoveIndex = -1;

    // Search each move:
    for(unsigned int movenum = 0; (int) movenum < mlist.size(); movenum++)
    {
        // Find the highest-scoring remaining move:
        mlist.FindBest(movenum);
        simpleMoveT * sm = mlist.Get(movenum);

        // Move-specific extensions:
        int extensions = baseExtensions;

        // If moving a pawn to the 7th or 8th rank, extend the search:
        if(piece_Type(sm->movingPiece) == PAWN)
        {
            rankT rank = square_Rank(sm->to);
            if(rank <= RANK_2  ||  rank >= RANK_7)
            {
                extensions++;
            }
        }

        // Reduce extensions if the search is deep:
        if(extensions > 0  && (int)Ply >= depth + depth)
        {
            extensions /= 2;
        }

        // Limit extensions to one ply (only if deep enough?):
        if(extensions > 1  /*&&  (int)Ply >= depth*/)
        {
            extensions = 1;
        }

        // Make this move and remember if it gives check:
        DoMove(sm);
        InCheck[Ply] = Pos.IsKingInCheck(sm);

        // Simple futility pruning. Note that pruning with depth of two
        // remaining is risky, but seems to work well enough in practise.
        // We only prune when:
        //   (1) there are no extensions,
        //   (2) we are at ply 3 or deeper,
        //   (3) the move made does not give check,
        //   (4) the score does not indicate mate,
        //   (5) the move is not the only legal move, and
        //   (6) we are not in a pawn ending.

        if(Pruning  &&  extensions == 0  &&  Ply > 2  &&  depth <= 2
                &&  !InCheck[Ply]  &&  !IsMatingScore(alpha)  &&  !isOnlyMove
                &&  Pos.NumNonPawns(WHITE) > 1  &&  Pos.NumNonPawns(BLACK) > 1)
        {
            int mscore = -ScoreMaterial();
            bool futile = false;
            if(depth == 1)
            {
                // Futility pruning, when 2 pawns below alpha:
                futile = ((mscore + (PawnValue * 2)) < alpha);
            }
            else if(depth == 2)
            {
                // Extended futility pruning, when a rook below alpha:
                futile = ((mscore + RookValue) < alpha);
            }
            // Skip this move if it is futile:
            if(futile)
            {
                UndoMove(sm);
                continue;
            }
        }

#define PVS_SEARCH
#ifdef PVS_SEARCH
        // We do a normal search for the first move, but for all other
        // moves we try a minimal window search first to save time:
        int score = alpha;
        if(movenum == 0)
        {
            score = -Search(depth + extensions - 1, -beta, -alpha, true);
        }
        else
        {
            score = -Search(depth + extensions - 1, -alpha - 1, -alpha, true);
            if(score > alpha  &&  score < beta)
            {
                // This move is good enough to search with the proper
                // window; use the score it returned as the lower bound:
                score = -Search(depth + extensions - 1, -beta, -score, true);
            }
        }
#else
        // No PVS, just do a regular search at every move:
        int score = -Search(depth + extensions - 1, -beta, -alpha, true);
#endif
        UndoMove(sm);

        // If this move scored at least as good as beta, we have
        // "failed high" so there is no need to continue searching
        // for an even better move:
        if(score >= beta)
        {
            IncHistoryValue(sm, depth * depth);
            AddKillerMove(sm);
            StoreHash(depth, SCORE_LOWER, score, sm, isOnlyMove);
            // Fail-high-first-move stats:
            nFailHigh++;
            if(movenum == 0)
            {
                nFailHighFirstMove++;
            }
            return beta;
        }

        // If this move is better than the alpha score, it is a new
        // best move at this point in the search tree. Update the PV
        // (and boost the history value of the move a little? - no):
        if(score > alpha)
        {
            alpha = score;
            bestMoveIndex = movenum;
            UpdatePV(sm);
            // IncHistoryValue (sm, depth);
        }

        // All done with that move. If it was the first move in the list and
        // it was the move from the hashtable, then the remaining moves have
        // not been generated and scored for move ordering. We do that now,
        // ensuring that the hash table move we just examined is moved to
        // the start of the list so it does not get searched again.
        if(movenum == 0  &&  gotHashMove  &&  !isOnlyMove)
        {
            mlist.clear();
            Pos.GenerateMoves(&mlist, EMPTY, GEN_ALL_MOVES, InCheck[Ply]);
            ScoreMoves(&mlist);
            int hashIndex = mlist.Find(&hashmove);
            if(hashIndex >= 0)
            {
                mlist.SwapWithFirst(hashIndex);
            }
            else
            {
                // The hash table move was legal, but not found in the
                // move list -- Bizzare!
                Output("# Yikes! Hash table move not in move list! Bug?\n");
            }
        }
    }

    if(mlist.size() == 0)
    {
        // No legal moves? Must be checkmate or stalemate:
        return (InCheck[Ply] ? (-Infinity + Ply) : 0);
    }

    // If alpha did not get improved, we "failed low"; every move
    // scored worse than our lower bound.
    // Store alpha in the transposition table as an upper bound on
    // the true score of this position, with no best move.
    if(alpha == oldAlpha)
    {
        ASSERT(bestMoveIndex < 0);
        StoreHash(depth, SCORE_UPPER, alpha, NULL, isOnlyMove);
    }
    else
    {
        // Update the transposition table with the best move:
        ASSERT(bestMoveIndex >= 0);
        simpleMoveT * bestMove = mlist.Get(bestMoveIndex);
        IncHistoryValue(bestMove, depth * depth);
        // Should we also add this as a killer move? Possibly not,
        // since it was not good enough to cause a beta cutoff.
        // It seems to make little difference.
        AddKillerMove(bestMove);
        StoreHash(depth, SCORE_EXACT, alpha, bestMove, isOnlyMove);
    }
    return alpha;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::Quiesce
//   Search only captures until a stable position is reached
//   that can be evaluated.
int
Engine::Quiesce(int alpha, int beta)
{
    NodeCount++;
    QNodeCount++;

    // Check that the absolute depth limit is not exceeded:
    if(Ply >= ENGINE_MAX_PLY - 1)
    {
        return alpha;
    }
    SetPVLength();

    // Stop now if we are out of time:
    if(OutOfTime())
    {
        return alpha;
    }

    // Check for a recognized endgame score:
    if(Pos.TotalMaterial() <= Recognizer::MaxPieces())
    {
        int recog = Recognizer::Recognize(&Pos);
        int rscore = recogScore(recog);
        scoreFlagT rflag = recogFlag(recog);

        if(rflag == SCORE_EXACT)
        {
            return rscore;
        }
        else if(rflag == SCORE_LOWER)
        {
            if(rscore >= beta)
            {
                return rscore;
            }
            if(rscore < alpha)
            {
                alpha = rscore;
            }
        }
        else if(rflag == SCORE_UPPER)
        {
            if(rscore <= alpha)
            {
                return rscore;
            }
            if(rscore > beta)
            {
                beta = rscore;
            }
        }
    }

    // Find the static evaluation of this position, to either cause
    // a beta cutoff or improve the alpha score:
    int staticScore = Score(alpha, beta);
    if(staticScore >= beta)
    {
        return beta;
    }
    if(staticScore > alpha)
    {
        alpha = staticScore;
    }

    // Check for a static score so far below alpha that no capture
    // is going to be good enough anyway:
    int margin = PawnValue;
    if(staticScore + QueenValue + margin < alpha)
    {
        return alpha;
    }

    // Generate and score the list of captures:
    MoveList mlist;
    Pos.GenerateMoves(&mlist, GEN_CAPTURES);
    for(unsigned int m = 0; (int) m < mlist.size(); m++)
    {
        simpleMoveT * sm = mlist.Get(m);
        sm->score = SEE(sm->from, sm->to);
    }

    // Iterate through each quiescent move to find a beta cutoff or
    // improve the alpha score:

    for(unsigned int i = 0; (int) i < mlist.size(); i++)
    {
        // Find the highest-scoring remaining move, make it and search:
        mlist.FindBest(i);
        simpleMoveT * sm = mlist.Get(i);
        pieceT promote = piece_Type(sm->promote);

        // Skip underpromotions:
        if(promote != EMPTY  &&  promote != QUEEN)
        {
            continue;
        }

        // Stop if the capture gain is negative or is so small that it
        // will (very likely) not improve alpha:
        if(sm->score < 0)
        {
            break;
        }
        if((sm->score + staticScore + margin) < alpha)
        {
            break;
        }

        // Make the move and evaluate it:
        DoMove(sm);
        int score = -Quiesce(-beta, -alpha);
        UndoMove(sm);

        // Check for a score so good it causes a beta cutoff:
        if(score >= beta)
        {
            return score;
        }

        // See if we have a new best move:
        if(score > alpha)
        {
            alpha = score;
            UpdatePV(sm);
        }
    }
    return alpha;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::SEE
//   Static Exchange Evaluator.
//   Evaluates the approximate material result of moving the piece
//   from the from square (which must not be empty) to the target
//   square (which may be empty or may hold an enemy piece).
int
Engine::SEE(squareT from, squareT target)
{
    const pieceT * board = Pos.GetBoard();
    SquareList attackers[2];
    pieceT mover = piece_Type(board[from]);
    ASSERT(mover != EMPTY);
    colorT stm = piece_Color_NotEmpty(board[from]);

#define SEE_ADD(c,sq) attackers[(c)].Add(sq)

    // Currently the SEE method is only called for legal moves, so if
    // the moving piece is a king then it clearly cannot be captured.
    // If potentially illegal king moves are to be passed to this
    // method, the following optimisation should be removed.
    if(mover == KING)
    {
        return PieceValue(board[target]);
    }

    // Find the estimated result assuming one recapture:
    int fastResult = PieceValue(board[target]) - PieceValue(mover);

    // We can do quick estimation for a big gain, but have to be
    // careful since move ordering is very sensitive to positive SEE
    // scores. Only return a fast estimate for PxQ, NxQ, BxQ and PxR:
    if(fastResult > KnightValue  &&  mover != ROOK)
    {
        return fastResult;
    }

    // Add attacking pawns to the attackers list:
    squareT pawnSq = square_Move(target, DOWN_LEFT);
    if(board[pawnSq] == WP  &&  pawnSq != from)
    {
        SEE_ADD(WHITE, pawnSq);
    }
    pawnSq = square_Move(target, DOWN_RIGHT);
    if(board[pawnSq] == WP  &&  pawnSq != from)
    {
        SEE_ADD(WHITE, pawnSq);
    }
    pawnSq = square_Move(target, UP_LEFT);
    if(board[pawnSq] == BP  &&  pawnSq != from)
    {
        SEE_ADD(BLACK, pawnSq);
    }
    pawnSq = square_Move(target, UP_RIGHT);
    if(board[pawnSq] == BP  &&  pawnSq != from)
    {
        SEE_ADD(BLACK, pawnSq);
    }

    // Quick estimation for a nonpawn capturing a lesser-valued piece (or
    // moving to an empty square) which is defended by an enemy pawn.
    if(fastResult < -PawnValue  &&  attackers[color_Flip(stm)].Size() > 0)
    {
        return fastResult;
    }

    // Add attacking knights. Only bother searching for them if there
    // are any knights on the appropriate square color.
    colorT knightSquareColor = color_Flip(square_Color(target));
    unsigned int nEligibleKnights = Pos.SquareColorCount(WN, knightSquareColor)
                                    + Pos.SquareColorCount(BN, knightSquareColor);
    if(nEligibleKnights > 0)
    {
        const squareT * nextKnightSq = knightAttacks[target];
        while(true)
        {
            squareT dest = *nextKnightSq;
            if(dest == NULL_SQUARE)
            {
                break;
            }
            nextKnightSq++;
            pieceT p = board[dest];
            if(piece_Type(p) != KNIGHT)
            {
                continue;
            }
            if(dest == from)
            {
                continue;
            }
            // Quick estimate when this recapture ensures a negative result:
            colorT knightColor = piece_Color_NotEmpty(p);
            if(fastResult < -KnightValue  &&  knightColor != stm)
            {
                return fastResult + KnightValue / 2;
            }
            SEE_ADD(knightColor, dest);
        }
    }

    // Add the first sliding attackers in each direction. Others
    // may appear later as appropriate, when the piece in front
    // of them takes part in the capture sequence.

    // First make an array containing all the directions that contain
    // potential sliding attackers, to avoid searching useless directions.
    rankT rank = square_Rank(target);
    fyleT fyle = square_Fyle(target);
    leftDiagT ul = square_LeftDiag(target);
    rightDiagT ur = square_RightDiag(target);
    unsigned int rankCount = Pos.RankCount(WQ, rank) + Pos.RankCount(BQ, rank)
                             + Pos.RankCount(WR, rank) + Pos.RankCount(BR, rank);
    unsigned int fyleCount = Pos.FyleCount(WQ, fyle) + Pos.FyleCount(BQ, fyle)
                             + Pos.FyleCount(WR, fyle) + Pos.FyleCount(BR, fyle);
    unsigned int upLeftCount = Pos.LeftDiagCount(WQ, ul) + Pos.LeftDiagCount(BQ, ul)
                               + Pos.LeftDiagCount(WB, ul) + Pos.LeftDiagCount(BB, ul);
    unsigned int upRightCount = Pos.RightDiagCount(WQ, ur) + Pos.RightDiagCount(BQ, ur)
                                + Pos.RightDiagCount(WB, ur) + Pos.RightDiagCount(BB, ur);

    // If the moving piece is a slider, it is worth removing it from the
    // rank/file/diagonal counts because we will avoid searching two
    // directions if it is the only slider on its rank/file/diagonal.
    if(piece_IsSlider(mover))
    {
        if(square_Rank(from) == square_Rank(target))
        {
            rankCount--;
        }
        else if(square_Fyle(from) == square_Fyle(target))
        {
            fyleCount--;
        }
        else if(square_LeftDiag(from) == square_LeftDiag(target))
        {
            upLeftCount--;
        }
        else
        {
            ASSERT(square_RightDiag(from) == square_RightDiag(target));
            upRightCount--;
        }
    }

    // Build the list of directions with potential sliding capturers:
    unsigned int nDirs = 0;
    directionT sliderDir[8];
    if(rankCount > 0)
    {
        sliderDir[nDirs++] = LEFT;
        sliderDir[nDirs++] = RIGHT;
    }
    if(fyleCount > 0)
    {
        sliderDir[nDirs++] = UP;
        sliderDir[nDirs++] = DOWN;
    }
    if(upLeftCount > 0)
    {
        sliderDir[nDirs++] = UP_LEFT;
        sliderDir[nDirs++] = DOWN_RIGHT;
    }
    if(upRightCount > 0)
    {
        sliderDir[nDirs++] = UP_RIGHT;
        sliderDir[nDirs++] = DOWN_LEFT;
    }

    // Iterate over each direction, looking for an attacking slider:

    for(unsigned int dirIndex = 0; dirIndex < nDirs; dirIndex++)
    {
        directionT dir = sliderDir[dirIndex];
        squareT dest = target;
        squareT last = square_Last(target, dir);
        int delta = direction_Delta(dir);
        unsigned int distance = 0;

        while(dest != last)
        {
            dest += delta;
            distance++;
            pieceT p = board[dest];
            if(p == EMPTY)
            {
                continue;
            }
            if(dest == from)
            {
                continue;
            }
            pieceT ptype = piece_Type(p);
            if(ptype == PAWN)
            {
                // Look through this pawn if it was also a capturer.
                if(distance != 1)
                {
                    break;
                }
                if(p == WP)
                {
                    if(dir == DOWN_LEFT  ||  dir == DOWN_RIGHT)
                    {
                        continue;
                    }
                }
                else
                {
                    if(dir == UP_LEFT  ||  dir == UP_RIGHT)
                    {
                        continue;
                    }
                }
                break;
            }
            if(! piece_IsSlider(ptype))
            {
                break;
            }
            if(ptype == ROOK  &&  direction_IsDiagonal(dir))
            {
                break;
            }
            if(ptype == BISHOP  &&  !direction_IsDiagonal(dir))
            {
                break;
            }
            colorT c = piece_Color_NotEmpty(p);

            // Quick estimate when this recapture ensures a negative result:
            if(fastResult < -BishopValue  &&  ptype == BISHOP)
            {
                if(c != stm)
                {
                    return fastResult + BishopValue / 2;
                }
            }
            else if(fastResult < -RookValue  &&  ptype == ROOK)
            {
                if(c != stm)
                {
                    return fastResult + RookValue / 2;
                }
            }

            // OK, we have a sliding attacker. Add it:
            SEE_ADD(c, dest);
            break;
        }
    }

    // Add one capturing king if the other king cannot capture:
    squareT wk = Pos.GetKingSquare(WHITE);
    squareT bk = Pos.GetKingSquare(BLACK);
    if(wk != from  &&  bk != from)
    {
        bool wkAttacks = square_Adjacent(target, wk);
        bool bkAttacks = square_Adjacent(target, bk);
        if(wkAttacks && !bkAttacks)
        {
            SEE_ADD(WHITE, wk);
        }
        else if(bkAttacks && !wkAttacks)
        {
            SEE_ADD(BLACK, bk);
        }
    }

    // Now go through the attack lists (which may get hidden sliders added
    // as sliding pieces make captures) finding the best capture sequence.

    bool targetIsPromoSquare = (target <= H1  ||  target >= A8);
    int swaplist[32];
    unsigned int nswaps = 1;
    swaplist[0] = PieceValue(board[target]);
    int attackedVal = PieceValue(mover);

    // Adjust the swap value for a promotion:
    if(targetIsPromoSquare  &&  attackedVal == PawnValue)
    {
        swaplist[0] += QueenValue - PawnValue;
        attackedVal = QueenValue;
    }

    // Add as many captures to the sequence as possible, using
    // lowest-valued pieces first:
    while(true)
    {
        // Switch to the other side:
        stm = color_Flip(stm);
        SquareList * attackList = &(attackers[stm]);
        unsigned int attackCount = attackList->Size();

        // Has this side run out of pieces to capture with?
        if(attackCount == 0)
        {
            break;
        }

        // Find the best (lowest-valued) piece to capture with:
        unsigned int bestIndex = 0;
        squareT attackSquare = attackList->Get(0);
        int attackValue = PieceValue(board[attackSquare]);
        for(unsigned int i = 1; i < attackCount; i++)
        {
            if(attackValue == PawnValue)
            {
                break;
            }
            squareT newSquare = attackList->Get(i);
            int newValue = PieceValue(board[newSquare]);
            if(newValue < attackValue)
            {
                attackSquare = newSquare;
                attackValue = newValue;
                bestIndex = i;
            }
        }
        pieceT attackPiece = piece_Type(board[attackSquare]);

        // Update the swap list:
        swaplist[nswaps] = -swaplist[nswaps - 1] + attackedVal;
        nswaps++;
        attackedVal = attackValue;

        // Fudge the value for a promotion, turning the pawn into a queen:
        if(targetIsPromoSquare  &&  attackValue == PawnValue)
        {
            swaplist[nswaps - 1] += QueenValue - PawnValue;
            attackedVal = QueenValue;
        }

        // Remove the chosen attacker from the list:
        attackList->Remove(bestIndex);

        // If the attacker is a slider, look for another slider behind it:
        if(piece_IsSlider(attackPiece))
        {
            directionT dir = sqDir[target][attackSquare];
            ASSERT(dir != NULL_DIR);
            squareT dest = attackSquare;
            squareT last = square_Last(dest, dir);
            int delta = direction_Delta(dir);

            while(dest != last)
            {
                dest += delta;
                pieceT p = board[dest];
                if(p == EMPTY)
                {
                    continue;
                }
                pieceT pt = piece_Type(p);
                if(! piece_IsSlider(pt))
                {
                    break;
                }
                if(pt == ROOK  &&  direction_IsDiagonal(dir))
                {
                    break;
                }
                if(pt == BISHOP  &&  !direction_IsDiagonal(dir))
                {
                    break;
                }
                // OK, we have another sliding attacker. Add it:
                SEE_ADD(piece_Color_NotEmpty(p), dest);
                break;
            }
        }
    }

    // Finally, go backwards through the swap list and determine when one
    // side would stop because further exchanges would be useless:
    nswaps--;
    while(nswaps > 0)
    {
        unsigned int prev = nswaps - 1;
        if(swaplist[nswaps] > -swaplist[prev])
        {
            swaplist[prev] = -swaplist[nswaps];
        }
        nswaps--;
    }
    return swaplist[0];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::ScoreMoves
//   Gives each move in the specified move list a score for move
//   ordering. Captures are scored using static exchange evaluation
//   while non-capture scores are based on killer move and history
//   heuristic information. Promotions are treated as captures.
//   The ordering has four basic categories:
//      (1) Non-losing captures (ordered by SEE value, score >= EMH * 2);
//      (2) Non-capture killer moves (EMH <= score < 2 * EMH);
//      (3) Other non-captures (by history heuristic, 0 <= score < EMH);
//      (4) Losing captures (ordered by SEE value, score < 0).
//   where EMH = ENGINE_MAX_HISTORY is the history value threshold.
void
Engine::ScoreMoves(MoveList * mlist)
{
    for(unsigned int i = 0; (int) i < mlist->size(); i++)
    {
        simpleMoveT * sm = mlist->Get(i);
        if(sm->capturedPiece != EMPTY  ||  sm->promote != EMPTY)
        {
            int see = SEE(sm->from, sm->to);
            if(see >= 0)
            {
                sm->score = ENGINE_MAX_HISTORY * 2 + see;
            }
            else
            {
                sm->score = see;
            }
        }
        else
        {
            // Non-capture; just use the history/killer value for this move.
            sm->score = GetHistoryValue(sm);
            if(IsKillerMove(sm))
            {
                sm->score += ENGINE_MAX_HISTORY;
            }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::Output
//    Prints a formatted string (as passed to printf) to standard output
//    and the the log file if one is being used.
void
Engine::Output(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    if(LogFile != NULL)
    {
        vfprintf(LogFile, format, ap);
    }
    va_end(ap);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::PrintPV
//   Print the current depth, score and principal variation.
void
Engine::PrintPV(unsigned int depth, int score, const char * note)
{
    if(! PostInfo)
    {
        return;
    }
    unsigned int ms = Elapsed.elapsed();
    if(XBoardMode  &&  ms < 50  &&  Ply < 6)
    {
        return;
    }

    if(XBoardMode)
    {
        Output(" %2u %6d %5u %9u  ",  depth, score, ms / 10, NodeCount);
    }
    else
    {
        Output(" %2u %-3s %+6d %5u %9u  ", depth, note, score, ms, NodeCount);
    }

    principalVarT * pv = &(PV[0]);
    unsigned int i;

    if(Pos.GetToMove() == BLACK)
    {
        Output("%u...", Pos.GetFullMoveCount());
    }

    // Make and print each PV move:
    for(i = 0; i < pv->length; i++)
    {
        simpleMoveT * sm = &(pv->move[i]);

        // Check for legality, to protect against hash table
        // false hits and bugs in PV updating:
        if(! Pos.IsLegalMove(sm))
        {
            Output(" <illegal>");
            break;
        }

        if(i > 0)
        {
            Output(" ");
        }
        if(Pos.GetToMove() == WHITE)
        {
            Output("%u.", Pos.GetFullMoveCount());
        }
        char s[10];
        Pos.MakeSANString(sm, s, SAN_MATETEST);
        Output("%s", s);
        Pos.DoSimpleMove(sm);
    }
    Output("\n");

    // Undo each PV move that was made:
    for(; i > 0; i--)
    {
        Pos.UndoSimpleMove(&(pv->move[i - 1]));
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::OutOfTime
//   Returns true if the search time limit has been reached.
//   "Out Of Time" is also the name of a great R.E.M. album. :-)
bool
Engine::OutOfTime()
{
    if(IsOutOfTime)
    {
        return true;
    }

    // Only check the time approximately every 1000 nodes for speed:
    if((NodeCount & 1023) != 0)
    {
        return false;
    }

    int ms = Elapsed.elapsed();

    if(EasyMove)
    {
        IsOutOfTime = (ms > MinSearchTime);
    }
    else if(HardMove)
    {
        IsOutOfTime = (ms > MaxSearchTime);
    }
    else
    {
        IsOutOfTime = (ms > SearchTime);
    }

    if(!IsOutOfTime  &&  CallbackFunction != NULL)
    {
        IsOutOfTime = CallbackFunction(this, CallbackData);
    }

    return IsOutOfTime;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Engine::PerfTest
//   Returns the number of leaf node moves when generating, making and
//   unmaking every move to the specified depth from the current position.
unsigned int
Engine::PerfTest(unsigned int depth)
{
    if(depth <= 0)
    {
        return 1;
    }
    MoveList mlist;
    Pos.GenerateMoves(&mlist);
    unsigned int nmoves = 0;
    for(unsigned int i = 0; (int) i < mlist.size(); i++)
    {
        simpleMoveT * sm = mlist.Get(i);
        Pos.DoSimpleMove(sm);
        nmoves += PerfTest(depth - 1);
        Pos.UndoSimpleMove(sm);
    }
    return nmoves;
}

void Engine::UpdatePV(simpleMoveT * sm)
{
    if(Ply >= ENGINE_MAX_PLY - 1)
    {
        return;
    }
    if(InNullMove > 0)
    {
        return;
    }

    PV[Ply].move[Ply] = *sm;
    for(unsigned int j = Ply + 1; j < PV[Ply + 1].length; j++)
    {
        PV[Ply].move[j] = PV[Ply + 1].move[j];
    }
    PV[Ply].length = PV[Ply + 1].length;
}

