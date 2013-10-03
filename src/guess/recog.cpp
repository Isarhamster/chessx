//////////////////////////////////////////////////////////////////////
//
//  FILE:       recog.cpp
//              Endgame knowledge recognition methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.5
//
//  Notice:     Copyright (c) 2002-2003 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#include "guessengine.h"
#include "recog.h"

namespace Guess
{

// The Recognizer class provides score bound information for chess
// endgames.


static const int DRAW = recogValue(SCORE_EXACT, 0);
static const int UNKNOWN = recogValue(SCORE_NONE, 0);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::Recognize
//   Checks the current position for a recognized endgame.
//   Returns a recognition value, which contains a regular score
//   for the position and a score flag indicating if the score
//   is exact, an upper bound, a lower bound, or useless.
int
Recognizer::Recognize(Position * pos)
{
    unsigned int npieces = pos->TotalMaterial();
    unsigned char * material = pos->GetMaterial();

    if(npieces > 6)
    {
        return UNKNOWN;
    }

    // TODO: Speed up this code which selects the appropriate recognizer.
    // TODO:  We should maintain a material signature (or hash code) in
    // TODO:  the position and then we could use a small lookup table here
    // TODO:  find the appropriate recognizer quickly.

    if(npieces == 3)
    {
        if(material[WB] == 1  ||  material[BB] == 1)
        {
            return DRAW;
        }
        if(material[WN] == 1  ||  material[BN] == 1)
        {
            return DRAW;
        }
        if(material[WP] == 1  ||  material[BP] == 1)
        {
            return KPK(pos);
        }
        return UNKNOWN;
    }
    if(npieces == 5)
    {
        // KRP-KR:
        if(material[WR] == 1  &&  material[WP] == 1  &&  material[BR] == 1)
        {
            return KRPKR(pos);
        }
        if(material[BR] == 1  &&  material[BP] == 1  &&  material[WR] == 1)
        {
            return KRPKR(pos);
        }
        // Bishop and 2 pawns on same rook file with bishop of wrong color:
        if(material[WB] == 1  &&  material[WP] == 2)
        {
            return KBPK(pos);
        }
        if(material[BB] == 1  &&  material[BP] == 2)
        {
            return KBPK(pos);
        }
        return UNKNOWN;
    }
    if(npieces == 6)
    {
        // Bishop and 3 pawns on same rook file with bishop of wrong color:
        if(material[WB] == 1  &&  material[WP] == 3)
        {
            return KBPK(pos);
        }
        if(material[BB] == 1  &&  material[BP] == 3)
        {
            return KBPK(pos);
        }
        return UNKNOWN;
    }
    if(npieces <= 2)
    {
        return DRAW;    // Lone kings.
    }

    // If we get here, there are four pieces (including kings and pawns)
    ASSERT(npieces == 4);

    // Positions with pawns:
    if(material[WP] == 1)
    {
        if(material[BQ] == 1)
        {
            return KQKP(pos);
        }
        if(material[BR] == 1)
        {
            return KRKP(pos);
        }
        if(material[BB] == 1  ||  material[BN] == 1)
        {
            return KMKP(pos);
        }
        if(material[WB] == 1)
        {
            return KBPK(pos);
        }
        return UNKNOWN;
    }
    if(material[BP] == 1)
    {
        if(material[WQ] == 1)
        {
            return KQKP(pos);
        }
        if(material[WR] == 1)
        {
            return KRKP(pos);
        }
        if(material[WB] == 1  ||  material[WN] == 1)
        {
            return KMKP(pos);
        }
        if(material[BB] == 1)
        {
            return KBPK(pos);
        }
        return UNKNOWN;
    }

    if(material[WB] == 2  ||  material[BB] == 2)
    {
        return KBBK(pos);
    }
    if(material[WB] == 1  &&  material[WN] == 1)
    {
        return KBNK(pos);
    }
    if(material[BB] == 1  &&  material[BN] == 1)
    {
        return KBNK(pos);
    }
    if(material[WR] == 1  &&  material[BB] == 1)
    {
        return KRKB(pos);
    }
    if(material[BR] == 1  &&  material[WB] == 1)
    {
        return KRKB(pos);
    }
    if(material[WR] == 1  &&  material[BN] == 1)
    {
        return KRKN(pos);
    }
    if(material[BR] == 1  &&  material[WN] == 1)
    {
        return KRKN(pos);
    }

    // KBKB, KBKN, KNKN and KNNK are all draws, but may have a
    // trivial mate-in-one if the non-side-to-move king is in a corner:
    if(material[WN] == 2  ||  material[BN] == 2
            || (material[WB] == 1  &&  material[BB] == 1)
            || (material[WB] == 1  &&  material[BN] == 1)
            || (material[WN] == 1  &&  material[BB] == 1)
            || (material[WN] == 1  &&  material[BN] == 1))
    {
        squareT wk = pos->GetKingSquare(WHITE);
        squareT bk = pos->GetKingSquare(BLACK);
        colorT stm = pos->GetToMove();
        if(stm == WHITE  &&  square_IsCornerSquare(bk))
        {
            return UNKNOWN;
        }
        if(stm == BLACK  &&  square_IsCornerSquare(wk))
        {
            return UNKNOWN;
        }
        return DRAW;
    }

    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KPK
//   Returns a recognition score for King and one Pawn vs King.
int
Recognizer::KPK(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wp;
    colorT stm = pos->GetToMove();

    // Find piece squares with White having the pawn:
    if(material[WP] == 1)
    {
        wk = pos->GetKingSquare(WHITE);
        bk = pos->GetKingSquare(BLACK);
        squareT * sqlist = pos->GetList(WHITE);
        wp = sqlist[1];
    }
    else
    {
        ASSERT(material[BP] == 1);
        wk = square_FlipRank(pos->GetKingSquare(BLACK));
        bk = square_FlipRank(pos->GetKingSquare(WHITE));
        squareT * sqlist = pos->GetList(BLACK);
        wp = square_FlipRank(sqlist[1]);
        stm = color_Flip(stm);
    }

    // Find pawn, king and enemy king rank and file values:
    rankT wpRank = square_Rank(wp);
    fyleT wpFyle = square_Fyle(wp);
    rankT wkRank = square_Rank(wk);
    rankT wkFyle = square_Fyle(wk);
    rankT bkRank = square_Rank(bk);
    rankT bkFyle = square_Fyle(bk);

    // Find value to use if it is a win:
    int winScore = 500 + 5 * wpRank;
    int winValue = recogValue(SCORE_LOWER, winScore);
    int lossValue = recogValue(SCORE_UPPER, -winScore);

    // If the enemy king is behind or equal to the pawn rank: win or unknown
    if(bkRank <= wpRank)
    {
        if((stm == WHITE  &&  wpRank > bkRank)
                || (stm == BLACK  &&  wpRank > bkRank + 1))
        {
            // Runaway pawn wins:
            return (stm == WHITE) ? winValue : lossValue;
        }
        // Cannot easily determine the result of this position:
        return UNKNOWN;
    }

    // Black king is clearly the closest king to the pawn: draw
    unsigned int wdist = square_Distance(wk, wp);
    unsigned int bdist = square_Distance(bk, wp);
    if(stm == WHITE)
    {
        bdist++;
    }
    // TODO: check if the "+ 2" below can safely be "+ 1".
    if(bdist + 2 < wdist)
    {
        return DRAW;
    }

    // Black king in front of a rook pawn: safe draw.
    if((wpFyle == A_FYLE  ||  wpFyle == H_FYLE)  &&  wpFyle == bkFyle)
    {
        return DRAW;
    }
    // King the two squares in front of any pawn before the 6th rank: draw
    if(wpRank < RANK_6  &&  wpFyle == bkFyle
            && (wpRank + 1 == bkRank  ||  wpRank + 2 == bkRank))
    {
        return DRAW;
    }
    // Pawn on 6th rank, enemy king blocking it on 7th: draw
    if(wpRank == RANK_6  &&  wpFyle == bkFyle  &&  wpRank + 1 == bkRank)
    {
        return DRAW;
    }

    // White king two ranks in front of the pawn, on the same file or an
    // adjacent file: win
    if(wpRank + 2 == wkRank)
    {
        int fileDiff = (int)wpFyle - (int)wkFyle;
        if(fileDiff >= -1  &&  fileDiff < 1)
        {
            return (stm == WHITE) ? winValue : lossValue;
        }
    }

    // Pawn-King-space-EnemyKing formation, draw if pawn is before 5th rank
    // and side with the pawn is to move; otherwise a win.
    if(wpFyle == wkFyle  &&  wpFyle == bkFyle
            &&  wpRank + 1 == wkRank  &&  wpRank + 3 == bkRank)
    {
        if(wpRank < RANK_5  &&  stm == WHITE)
        {
            return DRAW;
        }
        return (stm == WHITE) ? winValue : lossValue;
    }

    // No key KPK position was found:
    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KBBK
//   Returns a recognition score for King and two Bishops vs King.
int
Recognizer::KBBK(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wb1, wb2;
    colorT stm = pos->GetToMove();

    // Set up piece squares so that White has the bishops:
    if(material[WB] == 2)
    {
        wk = pos->GetKingSquare(WHITE);
        bk = pos->GetKingSquare(BLACK);
        squareT * sqlist = pos->GetList(WHITE);
        wb1 = sqlist[1];
        wb2 = sqlist[2];
    }
    else
    {
        ASSERT(material[BB] == 2);
        wk = square_FlipRank(pos->GetKingSquare(BLACK));
        bk = square_FlipRank(pos->GetKingSquare(WHITE));
        squareT * sqlist = pos->GetList(BLACK);
        wb1 = square_FlipRank(sqlist[1]);
        wb2 = square_FlipRank(sqlist[2]);
        stm = color_Flip(stm);
    }

    // If the bishops are of the same square color, it is a draw:
    if(square_Color(wb1) == square_Color(wb2))
    {
        return DRAW;
    }

    // If the lone king is to move and is next to a bishop, or on the
    // edge of the board with the enemy king nearby, it may be a draw
    // by capturing a bishop or by stalemate:
    if(stm == BLACK)
    {
        if(square_Adjacent(bk, wb1))
        {
            return UNKNOWN;
        }
        if(square_Adjacent(bk, wb2))
        {
            return UNKNOWN;
        }
        if(square_IsEdgeSquare(bk)  &&  square_Distance(wk, bk) == 2)
        {
            return UNKNOWN;
        }
    }

    // Compute the lone king distances used for scoring:
    int bkCorner = square_Distance(bk, square_NearestCorner(bk));
    int kingDist = square_Distance(bk, wk);

    // Return a suitable winning score:
    int score = 900 + (25 * bkCorner) - (10 * kingDist);
    int winValue = recogValue(SCORE_LOWER, score);
    int lossValue = recogValue(SCORE_UPPER, -score);
    return (stm == WHITE) ? winValue : lossValue;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KBNK
//   Returns a recognition score for King, Bishop and Knight vs King.
int
Recognizer::KBNK(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    pieceT * board = pos->GetBoard();
    squareT wk, bk, wb, wn;
    colorT stm = pos->GetToMove();

    // Set up piece squares so that White has the bishop and knight:
    if(material[WB] == 1)
    {
        wk = pos->GetKingSquare(WHITE);
        bk = pos->GetKingSquare(BLACK);
        squareT * sqlist = pos->GetList(WHITE);
        wb = sqlist[1];
        wn = sqlist[2];
        if(board[wb] != WB)
        {
            squareT temp = wb;
            wb = wn;
            wn = temp;
        }
    }
    else
    {
        ASSERT(material[BB] == 1   &&  material[BN] == 1);
        wk = square_FlipRank(pos->GetKingSquare(BLACK));
        bk = square_FlipRank(pos->GetKingSquare(WHITE));
        squareT * sqlist = pos->GetList(BLACK);
        wb = sqlist[1];
        wn = sqlist[2];
        if(board[wb] != BB)
        {
            squareT temp = wb;
            wb = wn;
            wn = temp;
        }
        wb = square_FlipRank(wb);
        wn = square_FlipRank(wn);
        stm = color_Flip(stm);
    }

    // If the lone king attacks a piece, the result is unclear:
    if(square_Adjacent(wb, bk)  ||  square_Adjacent(wn, bk))
    {
        return UNKNOWN;
    }

    // If the lone king is to move and possible stalemate, unclear result:
    if(stm == BLACK  &&  square_IsEdgeSquare(bk)
            &&  square_Distance(wk, bk) == 2)
    {
        return UNKNOWN;
    }

    // Find lone king distance from the appropriate corner
    squareT corner1 = A1;
    squareT corner2 = H8;
    if(square_Color(wb) == WHITE)
    {
        corner1 = H1;
        corner2 = A8;
    }
    int cornerDist = square_Distance(bk, corner1);
    int cornerDist2 = square_Distance(bk, corner2);
    if(cornerDist2 < cornerDist)
    {
        cornerDist = cornerDist2;
    }
    int kingDist = square_Distance(wk, bk);

    // Return an appropriate winning or losing score:
    int winScore = 700 - 25 * cornerDist - 10 * kingDist;
    int winValue = recogValue(SCORE_LOWER, winScore);
    int lossValue = recogValue(SCORE_UPPER, -winScore);
    return (stm == WHITE) ? winValue : lossValue;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KBPK
//   Returns a recognition score for King, Bishop and Pawn(s) vs King.
//   Recognizes draws with wrong-color bishop for rook pawn(s).
int
Recognizer::KBPK(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk;
    fyleT wrongFile = A_FYLE;  // Wrong-color bishop rook-pawn file.
    // Set up piece squares so that White has the bishop and pawn(s),
    // and make sure all pawns are on the wrong rook-pawn file:
    if(material[WB] == 1)
    {
        wk = pos->GetKingSquare(WHITE);
        bk = pos->GetKingSquare(BLACK);
        if(pos->SquareColorCount(WB, WHITE) == 1)
        {
            wrongFile = H_FYLE;
        }
        if(pos->FyleCount(WP, wrongFile) != material[WP])
        {
            return UNKNOWN;
        }
    }
    else
    {
        ASSERT(material[BB] == 1);
        wk = square_FlipRank(pos->GetKingSquare(BLACK));
        bk = square_FlipRank(pos->GetKingSquare(WHITE));
        if(pos->SquareColorCount(BB, BLACK) == 1)
        {
            wrongFile = H_FYLE;
        }
        if(pos->FyleCount(BP, wrongFile) != material[BP])
        {
            return UNKNOWN;
        }
    }
    // OK, we have it set up so white has the bishop and pawns, and we
    // know all pawns are on the wrong rook-pawn file. So recognise a
    // draw if the black king controls the queening square:
    squareT promoSq = square_Make(wrongFile, RANK_8);
    if(bk == promoSq  ||  square_Adjacent(bk, promoSq))
    {
        return DRAW;
    }
    // The black king does not control the queening square, so no draw yet:
    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KRKB
//   Returns a recognition score for King and Rook vs King and Bishop.
//   Contains simple rules covering many drawn positions, which were
//   checked using tablebases.
int
Recognizer::KRKB(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wr, bb;
    colorT stm = pos->GetToMove();

    // Find piece squares with White having the rook:
    wk = pos->GetKingSquare(WHITE);
    bk = pos->GetKingSquare(BLACK);
    squareT * sqlist = pos->GetList(WHITE);
    wr = sqlist[1];
    sqlist = pos->GetList(BLACK);
    bb = sqlist[1];
    if(material[WB] == 1)
    {
        squareT t = wk;
        wk = square_FlipRank(bk);
        bk = square_FlipRank(t);
        t = wr;
        wr = square_FlipRank(bb);
        bb = square_FlipRank(t);
        stm = color_Flip(stm);
    }

    // Flip board so the black king is in the 10-square a1-d1-d4 triangle:
    if(square_Rank(bk) >= RANK_5)
    {
        wk = square_FlipRank(wk);
        wr = square_FlipRank(wr);
        bk = square_FlipRank(bk);
        bb = square_FlipRank(bb);
    }
    if(square_Fyle(bk) >= E_FYLE)
    {
        wk = square_FlipFyle(wk);
        wr = square_FlipFyle(wr);
        bk = square_FlipFyle(bk);
        bb = square_FlipFyle(bb);
    }
    if(bk == A2 || bk == A3 || bk == A4 || bk == B3 || bk == B4 || bk == C4)
    {
        wk = square_FlipDiag(wk);
        wr = square_FlipDiag(wr);
        bk = square_FlipDiag(bk);
        bb = square_FlipDiag(bb);
    }

    int kingsDist = square_Distance(wk, bk);

    if(stm == WHITE)
    {
        // Black king on c2/c3/d3/d4, black bishop adjacent,
        // white king more than 2 squares away: draw.
        if(kingsDist > 2  &&  square_Adjacent(bk, bb)
                && (bk == C2 || bk == C3 || bk == D3 || bk == D4))
        {
            return DRAW;
        }
    }
    else
    {
        // Black king anywhere but a1 or b1, black bishop adjacent,
        // white king more than 2 squares away: draw.
        if(kingsDist > 2  &&  square_Adjacent(bk, bb)
                &&  bk != A1  &&  bk != B1)
        {
            return DRAW;
        }
        // Black king not on edge or b2, not sharing a rank or file with
        // the white rook; white king more than 2 squares away: draw.
        if(kingsDist > 2  &&  !square_IsEdgeSquare(bk)  &&  bk != B2
                &&  square_Rank(bk) != square_Rank(wr)
                &&  square_Fyle(bk) != square_Fyle(wr))
        {
            return DRAW;
        }
    }
    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KRKN
//   Returns a recognition score for King and Rook vs King and Knight.
//   Contains simple rules covering many drawn positions, which were
//   checked using tablebases.
int
Recognizer::KRKN(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wr, bn;
    colorT stm = pos->GetToMove();

    // Find piece squares with White having the rook:
    wk = pos->GetKingSquare(WHITE);
    bk = pos->GetKingSquare(BLACK);
    squareT * sqlist = pos->GetList(WHITE);
    wr = sqlist[1];
    sqlist = pos->GetList(BLACK);
    bn = sqlist[1];
    if(material[WN] == 1)
    {
        squareT t = wk;
        wk = square_FlipRank(bk);
        bk = square_FlipRank(t);
        t = wr;
        wr = square_FlipRank(bn);
        bn = square_FlipRank(t);
        stm = color_Flip(stm);
    }

    // Flip board so the black king is in the 10-square a1-d1-d4 triangle:
    if(square_Rank(bk) >= RANK_5)
    {
        wk = square_FlipRank(wk);
        wr = square_FlipRank(wr);
        bk = square_FlipRank(bk);
        bn = square_FlipRank(bn);
    }
    if(square_Fyle(bk) >= E_FYLE)
    {
        wk = square_FlipFyle(wk);
        wr = square_FlipFyle(wr);
        bk = square_FlipFyle(bk);
        bn = square_FlipFyle(bn);
    }
    if(bk == A2 || bk == A3 || bk == A4 || bk == B3 || bk == B4 || bk == C4)
    {
        wk = square_FlipDiag(wk);
        wr = square_FlipDiag(wr);
        bk = square_FlipDiag(bk);
        bn = square_FlipDiag(bn);
    }

    int kingsDist = square_Distance(wk, bk);

    if(stm == WHITE)
    {
        // Black king not on edge or b2, adjacent to black knight,
        // not sharing a rank or file with the white rook; white king
        // more than 2 squares away: draw.
        if(kingsDist > 2  &&  !square_IsEdgeSquare(bk)  &&  bk != B2
                &&  square_Adjacent(bk, bn)
                &&  square_Rank(bk) != square_Rank(wr)
                &&  square_Fyle(bk) != square_Fyle(wr))
        {
            return DRAW;
        }
    }
    else
    {
        // Black king anywhere but a1 or b1, adjacent to the knight,
        // white king more than 2 squares away: draw.
        if(bk != A1  &&  bk != B1  &&  kingsDist > 2
                &&  square_Adjacent(bk, bn))
        {
            return DRAW;
        }
        // Black king not on edge or b2, adjacent to black knight,
        // not sharing a rank/file with the white rook: draw.
        if(!square_IsEdgeSquare(bk)  &&  bk != B2
                &&  square_Adjacent(bk, bn)
                &&  square_Rank(bk) != square_Rank(wr)
                &&  square_Fyle(bk) != square_Fyle(wr))
        {
            return DRAW;
        }
        // Black king anywhere but a1/b1/c1/b2, adjacent to the black
        // knight, not sharing a rank/file with the rook; white king
        // not adjacent to the knight: draw.
        if(bk != A1  &&  bk != B1  &&  bk != C1  &&  bk != B2
                && !square_Adjacent(wk, bn)  &&  square_Adjacent(bk, bn)
                &&  square_Rank(bk) != square_Rank(wr)
                &&  square_Fyle(bk) != square_Fyle(wr))
        {
            return DRAW;
        }
    }
    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KMKP
//   Returns a recognition score for King and Minor Piece vs King and Pawn.
int
Recognizer::KMKP(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wm, bp;
    colorT stm = pos->GetToMove();

    // Find piece squares with White having the minor piece:
    wk = pos->GetKingSquare(WHITE);
    bk = pos->GetKingSquare(BLACK);
    squareT * sqlist = pos->GetList(WHITE);
    wm = sqlist[1];
    sqlist = pos->GetList(BLACK);
    bp = sqlist[1];
    if(material[WP] == 1)
    {
        squareT t = wk;
        wk = square_FlipRank(bk);
        bk = square_FlipRank(t);
        t = wm;
        wm = square_FlipRank(bp);
        bp = square_FlipRank(t);
        stm = color_Flip(stm);
    }

    // Black (the side with the pawn) can only lose to trivial mates
    // in a corner trapped by the pawn:
    if(stm == WHITE  &&  square_IsCornerSquare(bk)
            &&  square_Adjacent(bk, bp)  &&  square_Distance(bk, wk) == 2)
    {
        return UNKNOWN;
    }

    // Black has at least a draw:
    return recogValue((stm == BLACK) ? SCORE_LOWER : SCORE_UPPER, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KQKP
//   Returns a recognition score for King and Queen vs King and Pawn.
//   Contains rules for many draws with Queen vs a2 or c2 pawn,
//   checked using tablebases.
int
Recognizer::KQKP(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wq, bp;
    colorT stm = pos->GetToMove();

    // Find piece squares with White having the queen:
    wk = pos->GetKingSquare(WHITE);
    bk = pos->GetKingSquare(BLACK);
    squareT * sqlist = pos->GetList(WHITE);
    wq = sqlist[1];
    sqlist = pos->GetList(BLACK);
    bp = sqlist[1];
    if(material[WQ] != 1)
    {
        squareT t = wk;
        wk = square_FlipRank(bk);
        bk = square_FlipRank(t);
        t = wq;
        wq = square_FlipRank(bp);
        bp = square_FlipRank(t);
        stm = color_Flip(stm);
    }

    // There are only recognizable draws with a pawn on its 7th rank,
    // defended by its king:
    if(square_Rank(bp) != RANK_2  ||  ! square_Adjacent(bk, bp))
    {
        return UNKNOWN;
    }

    // Make sure the pawn is on the queenside:
    if(square_Fyle(bp) >= E_FYLE)
    {
        wk = square_FlipFyle(wk);
        wq = square_FlipFyle(wq);
        bk = square_FlipFyle(bk);
        bp = square_FlipFyle(bp);
    }

    // Lists of squares for each (black king, black pawn) pair listing
    // the white king squares for which white to move only draws
    // regardless of where the white queen is:

    static const squareT drawSquares_None [] = { NULL_SQUARE };
    static const squareT drawSquares_BPa2_BKb1 [] =
    {
        G1, H1, F2, G2, H2, G3, H3, F4, G4, H4, E5, F5, G5, H5,
        A6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa2_BKb2 [] =
    {
        F1, G1, H1, G2, H2, F3, G3, H3, F4, G4, H4, F5, G5, H5,
        A6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKc1 [] =
    {
        H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKd1 [] =
    {
        F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKd2 [] =
    {
        F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };

    const squareT * drawSquares = drawSquares_None;
    if(bp == A2)
    {
        if(bk == B1)
        {
            drawSquares = drawSquares_BPa2_BKb1;
        }
        if(bk == B2)
        {
            drawSquares = drawSquares_BPa2_BKb2;
        }
    }
    else if(bp == C2)
    {
        if(bk == C1)
        {
            drawSquares = drawSquares_BPc2_BKc1;
        }
        if(bk == D1)
        {
            drawSquares = drawSquares_BPc2_BKd1;
        }
        if(bk == D2)
        {
            drawSquares = drawSquares_BPc2_BKd2;
        }
    }

    // Scan the list of drawn squares for the white king square:
    while(*drawSquares != NULL_SQUARE)
    {
        if(wk == *drawSquares)
        {
            return DRAW;
        }
        drawSquares++;
    }

    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KRKP
//   Returns a recognition score for King and Rook vs King and Pawn.
//   Contains rules for many draws, checked using tablebases.
int
Recognizer::KRKP(Position * pos)
{
    unsigned char * material = pos->GetMaterial();
    squareT wk, bk, wr, bp;
    colorT stm = pos->GetToMove();

    // Find piece squares with White having the rook:
    wk = pos->GetKingSquare(WHITE);
    bk = pos->GetKingSquare(BLACK);
    squareT * sqlist = pos->GetList(WHITE);
    wr = sqlist[1];
    sqlist = pos->GetList(BLACK);
    bp = sqlist[1];
    if(material[WR] != 1)
    {
        squareT t = wk;
        wk = square_FlipRank(bk);
        bk = square_FlipRank(t);
        t = wr;
        wr = square_FlipRank(bp);
        bp = square_FlipRank(t);
        stm = color_Flip(stm);
    }

    // There are only recognized draws when the pawn is defended by its king:
    if(! square_Adjacent(bk, bp))
    {
        return UNKNOWN;
    }

    // Make sure the pawn is on the queenside:
    if(square_Fyle(bp) >= E_FYLE)
    {
        wk = square_FlipFyle(wk);
        wr = square_FlipFyle(wr);
        bk = square_FlipFyle(bk);
        bp = square_FlipFyle(bp);
    }

    // Lists of squares for each (black king, black pawn) pair listing
    // the white king squares for which white to move only draws
    // regardless of where the white rook is:

    static const squareT drawSquares_None [] = { NULL_SQUARE };
    static const squareT drawSquares_BPa2_BKb2 [] =
    {
        D1, D2, D3, C4, NULL_SQUARE
    };
    static const squareT drawSquares_BPa2_BKb3 [] =
    {
        E1, F1, G1, H1, D4, E4, F4, G4, H4, A5, B5, C5, D5, E5, F5, G5, H5, A6, B6, C6,
        D6, E6, F6, G6, H6, A7, B7, NULL_SQUARE
    };
    static const squareT drawSquares_BPa3_BKb2 [] =
    {
        E1, F1, G1, H1, E2, F2, G2, H2, D3, E3, F3, G3, H3, D4, E4, F4, G4, H4, D5, E5,
        F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8,
        B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa3_BKb3 [] =
    {
        F1, G1, H1, F2, G2, H2, F3, G3, H3, D4, E4, F4, G4, H4, D5, E5, F5, G5, H5, A6,
        B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8,
        F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa3_BKb4 [] =
    {
        G1, H1, G2, H2, G3, H3, G4, H4, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6,
        H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa4_BKa3 [] =
    {
        H1, H2, H3, H4, H5, H6, H7, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa4_BKa5 [] =
    {
        A7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa4_BKb3 [] =
    {
        G1, H1, G2, H2, G3, H3, G4, H4, E5, F5, G5, H5, E6, F6, G6, H6, A7, B7, C7, D7,
        E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa4_BKb4 [] =
    {
        H1, H2, H3, H4, H5, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8,
        C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa4_BKb5 [] =
    {
        A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa5_BKb4 [] =
    {
        E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPa5_BKb5 [] =
    {
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb2_BKa2 [] =
    {
        E1, F1, G1, H1, E2, F2, G2, H2, E3, F3, G3, H3, D4, E4, F4, G4, H4, B5, C5, D5,
        E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb2_BKa3 [] =
    {
        F2, G2, H2, F3, G3, H3, F4, G4, H4, A5, E5, G5, H5, A6, B6, C6, D6, E6, F6, H6,
        A7, B7, C7, D7, E7, F7, G7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb2_BKb3 [] =
    {
        F2, G2, H2, F3, G3, H3, F4, G4, H4, A5, B5, C5, D5, E5, G5, H5, A6, B6, C6, D6,
        E6, F6, H6, A7, B7, C7, D7, E7, F7, G7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb2_BKc1 [] =
    {
        F1, G1, H1, F2, G2, H2, E3, F3, G3, H3, A4, D4, E4, F4, G4, H4, A5, C5, D5, E5,
        F5, G5, H5, A6, C6, D6, E6, F6, G6, H6, A7, C7, D7, E7, F7, G7, H7, A8, C8, D8,
        E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb2_BKc2 [] =
    {
        E1, E2, E3, C4, D4, NULL_SQUARE
    };
    static const squareT drawSquares_BPb2_BKc3 [] =
    {
        E3, A5, B5, C5, D5, E5, A6, B6, C6, D6, A7, B7, C7, A8, B8, C8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKa2 [] =
    {
        F1, G1, H1, F2, G2, H2, F3, G3, H3, F4, G4, H4, E5, F5, G5, H5, B6, C6, D6, E6,
        F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKa3 [] =
    {
        G1, H1, G2, H2, G3, H3, G4, H4, G5, H5, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7,
        H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKa4 [] =
    {
        H1, H2, H3, H4, H5, H6, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKb2 [] =
    {
        G1, H1, G2, H2, G3, H3, G4, H4, F5, G5, H5, F6, G6, H6, A7, B7, C7, D7, E7, F7,
        G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKb4 [] =
    {
        G1, H1, G2, H2, G3, H3, G4, H4, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7,
        B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKc2 [] =
    {
        F1, G1, H1, G2, H2, E3, F3, G3, H3, E4, F4, G4, H4, A5, B5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8,
        E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKc3 [] =
    {
        G1, H1, G2, H2, G3, H3, E4, F4, G4, H4, C5, D5, E5, F5, G5, H5, A6, B6, C6, D6,
        E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8,
        NULL_SQUARE
    };
    static const squareT drawSquares_BPb3_BKc4 [] =
    {
        H1, H2, H3, H4, E5, F5, G5, H5, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7,
        G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKa3 [] =
    {
        H1, H2, H3, H4, H5, H6, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKa4 [] =
    {
        G7, H7, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKa5 [] =
    {
        H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKb3 [] =
    {
        H6, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKc3 [] =
    {
        H1, H2, H3, H4, E5, F5, G5, H5, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7,
        H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKc4 [] =
    {
        D6, E6, F6, G6, H6, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb4_BKc5 [] =
    {
        E7, F7, G7, H7, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb5_BKc4 [] =
    {
        E7, F7, G7, H7, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPb5_BKc5 [] =
    {
        F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKb2 [] =
    {
        A4, B4, B5, B6, B7, B8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKb3 [] =
    {
        A5, B5, C5, D5, A6, B6, C6, D6, B7, C7, D7, E7, B8, C8, D8, E8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKd1 [] =
    {
        G1, H1, G2, H2, F3, G3, H3, A4, B4, E4, F4, G4, H4, A5, B5, D5, E5, F5, G5, H5,
        A6, B6, D6, E6, F6, G6, H6, A7, B7, D7, E7, F7, G7, H7, A8, B8, D8, E8, F8, G8,
        H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc2_BKd3 [] =
    {
        F3, B5, C5, D5, E5, F5, A6, B6, C6, D6, E6, A7, B7, C7, D7, A8, B8, C8, D8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKb2 [] =
    {
        G1, H1, G2, H2, G3, H3, G4, H4, A5, C5, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6,
        F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKb3 [] =
    {
        H1, H2, H3, H4, A5, B5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7,
        F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKb4 [] =
    {
        A6, B6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKc2 [] =
    {
        H1, H2, H3, H4, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7,
        G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKc4 [] =
    {
        H1, H2, H3, H4, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7,
        G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKd2 [] =
    {
        G1, H1, H2, F3, G3, H3, F4, G4, H4, A5, B5, C5, E5, F5, G5, H5, A6, B6, C6, D6,
        E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8,
        NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKd3 [] =
    {
        H1, H2, H3, F4, G4, H4, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7,
        B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc3_BKd4 [] =
    {
        F5, G5, H5, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8,
        E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc4_BKb3 [] =
    {
        A6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc4_BKb4 [] =
    {
        A6, A7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc4_BKc3 [] =
    {
        F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc4_BKd3 [] =
    {
        F5, G5, H5, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8,
        F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc4_BKd4 [] =
    {
        E6, F6, G6, H6, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPc4_BKd5 [] =
    {
        F7, G7, H7, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd2_BKc1 [] =
    {
        G1, H1, G2, H2, A3, G3, H3, A4, B4, E4, F4, G4, H4, A5, B5, E5, F5, G5, H5, A6,
        B6, E6, F6, G6, H6, A7, B7, E7, F7, G7, H7, A8, B8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd2_BKc3 [] =
    {
        A3, A5, B5, C5, D5, E5, B6, C6, D6, E6, C7, D7, E7, F7, C8, D8, E8, F8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd2_BKd1 [] =
    {
        H1, H2, H3, A4, G4, H4, A5, B5, C5, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6,
        G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd2_BKd3 [] =
    {
        H2, H3, H4, A5, B5, C5, D5, E5, F5, G5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7,
        C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd2_BKe1 [] =
    {
        A1, H1, A2, H2, A3, G3, H3, A4, B4, C4, F4, G4, H4, A5, B5, C5, E5, F5, G5, H5,
        A6, B6, C6, E6, F6, G6, H6, A7, B7, C7, E7, F7, G7, H7, A8, B8, C8, E8, F8, G8,
        H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd2_BKe3 [] =
    {
        G3, C5, D5, E5, F5, G5, C6, D6, E6, F6, B7, C7, D7, E7, B8, C8, D8, E8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKc2 [] =
    {
        H1, H2, A3, H3, A4, H4, A5, B5, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6,
        H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKc3 [] =
    {
        A4, A5, B5, C5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKc4 [] =
    {
        A5, A6, B6, C6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8,
        NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKd2 [] =
    {
        H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8,
        D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKd4 [] =
    {
        H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8,
        D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKe2 [] =
    {
        H1, H2, G3, H3, G4, H4, A5, B5, C5, D5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6,
        H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKe3 [] =
    {
        G4, H4, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7,
        G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd3_BKe4 [] =
    {
        G5, H5, E6, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8,
        G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKc3 [] =
    {
        A5, A6, B6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKc4 [] =
    {
        A6, B6, A7, B7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKc5 [] =
    {
        A7, A8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKd3 [] =
    {
        A7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKd5 [] =
    {
        A8, B8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKe3 [] =
    {
        G5, H5, F6, G6, H6, A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8,
        H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKe4 [] =
    {
        F6, G6, H6, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8, NULL_SQUARE
    };
    static const squareT drawSquares_BPd4_BKe5 [] =
    {
        G7, H7, G8, H8, NULL_SQUARE
    };

    const squareT * drawSquares = drawSquares_None;
    if(bp == A2)
    {
        if(bk == B2)
        {
            drawSquares = drawSquares_BPa2_BKb2;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPa2_BKb3;
        }
    }
    else if(bp == A3)
    {
        if(bk == B2)
        {
            drawSquares = drawSquares_BPa3_BKb2;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPa3_BKb3;
        }
        else if(bk == B4)
        {
            drawSquares = drawSquares_BPa3_BKb4;
        }
    }
    else if(bp == A4)
    {
        if(bk == A3)
        {
            drawSquares = drawSquares_BPa4_BKa3;
        }
        else if(bk == A5)
        {
            drawSquares = drawSquares_BPa4_BKa5;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPa4_BKb3;
        }
        else if(bk == B4)
        {
            drawSquares = drawSquares_BPa4_BKb4;
        }
        else if(bk == B5)
        {
            drawSquares = drawSquares_BPa4_BKb5;
        }
    }
    else if(bp == A5)
    {
        if(bk == B4)
        {
            drawSquares = drawSquares_BPa5_BKb4;
        }
        else if(bk == B5)
        {
            drawSquares = drawSquares_BPa5_BKb5;
        }
    }
    else if(bp == B2)
    {
        if(bk == A2)
        {
            drawSquares = drawSquares_BPb2_BKa2;
        }
        else if(bk == A3)
        {
            drawSquares = drawSquares_BPb2_BKa3;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPb2_BKb3;
        }
        else if(bk == C1)
        {
            drawSquares = drawSquares_BPb2_BKc1;
        }
        else if(bk == C2)
        {
            drawSquares = drawSquares_BPb2_BKc2;
        }
        else if(bk == C3)
        {
            drawSquares = drawSquares_BPb2_BKc3;
        }
    }
    else if(bp == B3)
    {
        if(bk == A2)
        {
            drawSquares = drawSquares_BPb3_BKa2;
        }
        else if(bk == A3)
        {
            drawSquares = drawSquares_BPb3_BKa3;
        }
        else if(bk == A4)
        {
            drawSquares = drawSquares_BPb3_BKa4;
        }
        else if(bk == B2)
        {
            drawSquares = drawSquares_BPb3_BKb2;
        }
        else if(bk == B4)
        {
            drawSquares = drawSquares_BPb3_BKb4;
        }
        else if(bk == C2)
        {
            drawSquares = drawSquares_BPb3_BKc2;
        }
        else if(bk == C3)
        {
            drawSquares = drawSquares_BPb3_BKc3;
        }
        else if(bk == C4)
        {
            drawSquares = drawSquares_BPb3_BKc4;
        }
    }
    else if(bp == B4)
    {
        if(bk == A3)
        {
            drawSquares = drawSquares_BPb4_BKa3;
        }
        else if(bk == A4)
        {
            drawSquares = drawSquares_BPb4_BKa4;
        }
        else if(bk == A5)
        {
            drawSquares = drawSquares_BPb4_BKa5;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPb4_BKb3;
        }
        else if(bk == C3)
        {
            drawSquares = drawSquares_BPb4_BKc3;
        }
        else if(bk == C4)
        {
            drawSquares = drawSquares_BPb4_BKc4;
        }
        else if(bk == C5)
        {
            drawSquares = drawSquares_BPb4_BKc5;
        }
    }
    else if(bp == B5)
    {
        if(bk == C4)
        {
            drawSquares = drawSquares_BPb5_BKc4;
        }
        else if(bk == C5)
        {
            drawSquares = drawSquares_BPb5_BKc5;
        }
    }
    else if(bp == C2)
    {
        if(bk == B2)
        {
            drawSquares = drawSquares_BPc2_BKb2;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPc2_BKb3;
        }
        else if(bk == D1)
        {
            drawSquares = drawSquares_BPc2_BKd1;
        }
        else if(bk == D3)
        {
            drawSquares = drawSquares_BPc2_BKd3;
        }
    }
    else if(bp == C3)
    {
        if(bk == B2)
        {
            drawSquares = drawSquares_BPc3_BKb2;
        }
        else if(bk == B3)
        {
            drawSquares = drawSquares_BPc3_BKb3;
        }
        else if(bk == B4)
        {
            drawSquares = drawSquares_BPc3_BKb4;
        }
        else if(bk == C2)
        {
            drawSquares = drawSquares_BPc3_BKc2;
        }
        else if(bk == C4)
        {
            drawSquares = drawSquares_BPc3_BKc4;
        }
        else if(bk == D2)
        {
            drawSquares = drawSquares_BPc3_BKd2;
        }
        else if(bk == D3)
        {
            drawSquares = drawSquares_BPc3_BKd3;
        }
        else if(bk == D4)
        {
            drawSquares = drawSquares_BPc3_BKd4;
        }
    }
    else if(bp == C4)
    {
        if(bk == B3)
        {
            drawSquares = drawSquares_BPc4_BKb3;
        }
        else if(bk == B4)
        {
            drawSquares = drawSquares_BPc4_BKb4;
        }
        else if(bk == C3)
        {
            drawSquares = drawSquares_BPc4_BKc3;
        }
        else if(bk == D3)
        {
            drawSquares = drawSquares_BPc4_BKd3;
        }
        else if(bk == D4)
        {
            drawSquares = drawSquares_BPc4_BKd4;
        }
        else if(bk == D5)
        {
            drawSquares = drawSquares_BPc4_BKd5;
        }
    }
    else if(bp == D2)
    {
        if(bk == C1)
        {
            drawSquares = drawSquares_BPd2_BKc1;
        }
        else if(bk == C3)
        {
            drawSquares = drawSquares_BPd2_BKc3;
        }
        else if(bk == D1)
        {
            drawSquares = drawSquares_BPd2_BKd1;
        }
        else if(bk == D3)
        {
            drawSquares = drawSquares_BPd2_BKd3;
        }
        else if(bk == E1)
        {
            drawSquares = drawSquares_BPd2_BKe1;
        }
        else if(bk == E3)
        {
            drawSquares = drawSquares_BPd2_BKe3;
        }
    }
    else if(bp == D3)
    {
        if(bk == C2)
        {
            drawSquares = drawSquares_BPd3_BKc2;
        }
        else if(bk == C3)
        {
            drawSquares = drawSquares_BPd3_BKc3;
        }
        else if(bk == C4)
        {
            drawSquares = drawSquares_BPd3_BKc4;
        }
        else if(bk == D2)
        {
            drawSquares = drawSquares_BPd3_BKd2;
        }
        else if(bk == D4)
        {
            drawSquares = drawSquares_BPd3_BKd4;
        }
        else if(bk == E2)
        {
            drawSquares = drawSquares_BPd3_BKe2;
        }
        else if(bk == E3)
        {
            drawSquares = drawSquares_BPd3_BKe3;
        }
        else if(bk == E4)
        {
            drawSquares = drawSquares_BPd3_BKe4;
        }
    }
    else if(bp == D4)
    {
        if(bk == C3)
        {
            drawSquares = drawSquares_BPd4_BKc3;
        }
        else if(bk == C4)
        {
            drawSquares = drawSquares_BPd4_BKc4;
        }
        else if(bk == C5)
        {
            drawSquares = drawSquares_BPd4_BKc5;
        }
        else if(bk == D3)
        {
            drawSquares = drawSquares_BPd4_BKd3;
        }
        else if(bk == D5)
        {
            drawSquares = drawSquares_BPd4_BKd5;
        }
        else if(bk == E3)
        {
            drawSquares = drawSquares_BPd4_BKe3;
        }
        else if(bk == E4)
        {
            drawSquares = drawSquares_BPd4_BKe4;
        }
        else if(bk == E5)
        {
            drawSquares = drawSquares_BPd4_BKe5;
        }
    }

    // Scan the list of drawn squares for the white king square:
    while(*drawSquares != NULL_SQUARE)
    {
        if(wk == *drawSquares)
        {
            return DRAW;
        }
        drawSquares++;
    }

    return UNKNOWN;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Recognizer::KRPKR
//   Returns a recognition score for King, Rook and Pawn vs King and Rook.
//   Contains rules for some draws, checked using tablebases.
int
Recognizer::KRPKR(Position * pos)
{

    // XXX  INCOMPLETE  XXX
    return UNKNOWN;

    unsigned char * material = pos->GetMaterial();
    pieceT * board = pos->GetBoard();
    squareT wk, bk, wr, wp, br;
    colorT stm = pos->GetToMove();

    // Set up piece squares so that White has the pawn:
    if(material[WP] == 1)
    {
        wk = pos->GetKingSquare(WHITE);
        bk = pos->GetKingSquare(BLACK);
        squareT * sqlist = pos->GetList(WHITE);
        wr = sqlist[1];
        wp = sqlist[2];
        if(board[wr] != WR)
        {
            squareT temp = wr;
            wr = wp;
            wp = temp;
        }
        sqlist = pos->GetList(BLACK);
        br = sqlist[1];
    }
    else
    {
        wk = square_FlipRank(pos->GetKingSquare(BLACK));
        bk = square_FlipRank(pos->GetKingSquare(WHITE));
        squareT * sqlist = pos->GetList(BLACK);
        wr = sqlist[1];
        wp = sqlist[2];
        if(board[wr] != BR)
        {
            squareT temp = wr;
            wr = wp;
            wp = temp;
        }
        wr = square_FlipRank(wr);
        wp = square_FlipRank(wp);
        sqlist = pos->GetList(WHITE);
        br = square_FlipRank(sqlist[1]);
        stm = color_Flip(stm);
    }

    // Make sure the pawn is on the queenside:
    if(square_Fyle(wp) >= E_FYLE)
    {
        wk = square_FlipFyle(wk);
        wr = square_FlipFyle(wr);
        wp = square_FlipFyle(wp);
        bk = square_FlipFyle(bk);
        br = square_FlipFyle(br);
    }

    // Get ranks and files of piece squares:
    int wkRank = square_Rank(wk);
    int wkFyle = square_Fyle(wk);
    int wrRank = square_Rank(wr);
    int wrFyle = square_Fyle(wr);
    int wpRank = square_Rank(wp);
    int wpFyle = square_Fyle(wp);
    int bkRank = square_Rank(bk);
    int bkFyle = square_Fyle(bk);
    int brRank = square_Rank(br);
    int brFyle = square_Fyle(br);

    // No draws recognized if the black king is behind the pawn:
    if(bkRank < wpRank)
    {
        return UNKNOWN;
    }

    // We cannot recognize draws if the rooks share a rank or file:
    if(wrRank == brRank  ||  wrFyle == brFyle)
    {
        return UNKNOWN;
    }

    // Designate side-to-move king,rook as sk and sr, enemy as ek and er
    squareT sk, sr, ek, er;
    int skRank, srRank, ekRank, erRank;
    int skFyle, srFyle, ekFyle, erFyle;
    if(stm == WHITE)
    {
        sk = wk;
        sr = wr;
        ek = bk;
        er = br;
        skRank = wkRank;
        skFyle = wkFyle;
        srRank = wrRank;
        srFyle = wrFyle;
        ekRank = bkRank;
        ekFyle = bkFyle;
        erRank = brRank;
        erFyle = brFyle;
    }
    else
    {
        sk = bk;
        sr = br;
        ek = wk;
        er = wr;
        skRank = bkRank;
        skFyle = bkFyle;
        srRank = brRank;
        srFyle = brFyle;
        ekRank = wkRank;
        ekFyle = wkFyle;
        erRank = wrRank;
        erFyle = wrFyle;
    }
    unsigned int kingDist = square_Distance(wk, bk);

    // No recognition if the king and rook of the side NOT to move share
    // a rank or file, since there may be a pin or skewer.
    if(ekRank == erRank  ||  ekFyle == erFyle)
    {
        return UNKNOWN;
    }

    // No draws if the side NOT to move king and rook nearly share a
    // rank or file (that is, are on adjacent ranks or files) and the
    // king is on an edge, or the enemy king is only 2 squares away.
    // This can lead to loss of a rook, e.g. WK=d1 WR=a2 and ...Rh1+
    // or BK=d6 BR=h7 WK=d4 and Ra6+ wins the rook.
    if(square_IsEdgeSquare(ek)  ||  kingDist == 2)
    {
        int rankDiff = (int)ekRank - (int)erRank;
        if(rankDiff >= -1  &&  rankDiff <= 1)
        {
            return UNKNOWN;
        }
        int fyleDiff = (int)ekFyle - (int)erFyle;
        if(fyleDiff >= -1  &&  fyleDiff <= 1)
        {
            return UNKNOWN;
        }
    }

    // No recognition if either king attacks the enemy rook:
    if(square_Adjacent(wk, br))
    {
        return UNKNOWN;
    }
    if(square_Adjacent(bk, wr))
    {
        return UNKNOWN;
    }

    // No recognition if the white pawn attacks the black rook:
    if(square_Move(wp, UP_LEFT) == br)
    {
        return UNKNOWN;
    }
    if(square_Move(wp, UP_RIGHT) == br)
    {
        return UNKNOWN;
    }

    // Philidor draw:
    //   white pawn and king are on 5th rank or lower, on any file;
    //   black king is on the pawn file and on the 7th/8th rank;
    //   black rook is anywhere on the 6th rank.
    //   Drawn for white or black to move.

    if(wpRank <= RANK_5  &&  wkRank <= RANK_5  &&  bkFyle == wpFyle
            &&  bkRank >= RANK_7  &&  brRank == RANK_6)
    {
        // Only exception: WK=a1, WP=b2, WR=b1; White may be checkmated.
        if(wk == A1  &&  wp == B2  &&  wr == B1)
        {
            return UNKNOWN;
        }
        return DRAW;
    }

    // Sixth-rank pawn draws:
    //   white pawn on 6th rank; black king on pawn file on 7th/8th rank;
    //   black rook anywhere on 1st rank; white king on 6th rank or less;
    //   white rook and black king are not on same rank.
    if(wpRank == RANK_6  &&  bkFyle == wpFyle  &&  bkRank >= RANK_7
            &&  brRank == RANK_1  &&  wkRank <= RANK_6  &&  wrRank != bkRank)
    {
        // Black to move: draw
        if(stm == BLACK)
        {
            return DRAW;
        }
        // White to move: draw if the kings are > 2 squares apart, and
        // the white king and black rook do not share a rank or file.
        if(wkRank != brRank  &&  wkFyle != brFyle
                &&  square_Distance(wk, bk) > 2)
        {
            return DRAW;
        }
    }

    // TODO: Common a-pawn draws.
    if(wpFyle == A_FYLE)
    {
        // ....
    }

    return UNKNOWN;
}

} // End namespace Guess

//////////////////////////////////////////////////////////////////////
//  EOF: recog.cpp
//////////////////////////////////////////////////////////////////////
