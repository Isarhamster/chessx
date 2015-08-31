/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QtCore>
#include <QTime>
#include "movelist.h"

class Board;

/** The analysis class contains engine analysis. */
class Analysis
{
    Q_DECLARE_TR_FUNCTIONS(Analysis)

public:
    Analysis();
    /** Reset values. */
    void clear();
    /** Check if analysis is valid. */
    bool isValid() const;
    /** order of the variarion in the variation list */
    int mpv() const;
    /** Set order of the variation. */
    void setNumpv(int n);
    /** Time elapsed in miliseconds. */
    int time() const;
    /** Set time in miliseconds. */
    void setTime(int msec);
    /** Evaluation in centipawns. */
    int score() const;
    /** Set evaluation in centipawns. */
    void setScore(int score);
    /** Depth in plies. */
    int depth() const;
    /** Set depth in plies. */
    void setDepth(int depth);
    /** Nodes spent on analysis. */
    quint64 nodes() const;
    /** Set nodes spent on analysis. */
    void setNodes(quint64 nodes);
    /** Main variation. */
    MoveList variation() const;
    /** Set main variation. */
    void setVariation(const MoveList& variation);
    /** Is mate. */
    bool isMate() const;
    /** @return moves to mate */
    int movesToMate() const;
    /** Set moves to mate. */
    void setMovesToMate(int mate);
    /** Moves to mate. */
    /** Convert analysis to formatted text. */
    QString toString(const Board& board) const;
    /** Assignment operator */
    Analysis& operator=(const Analysis& rhs);
    void setBestMove(bool bestMove);
    bool bestMove() const;

    bool isAlreadyMate() const;

    int elapsedTimeMS() const;
    void setElapsedTimeMS(int elapsedTimeMS);

    bool getEndOfGame() const;
    void setEndOfGame(bool value);

    bool getBookMove() const;
    void setBookMove(bool bookMove);

private:
    int m_numpv;
    int m_msec;
    int m_mateIn;
    int m_depth;
    int m_score;
    bool m_bestMove;
    bool m_endOfGame;
    bool m_bookMove;
    quint64 m_nodes;
    MoveList m_variation;
    int m_elapsedTimeMS;
};

#endif // ANALYSIS_H
