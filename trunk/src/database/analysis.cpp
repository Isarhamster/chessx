/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QApplication>
#include "analysis.h"
#include "board.h"

Analysis::Analysis()
{
	clear();
}

void Analysis::clear()
{
	m_score = m_msec = m_depth = m_mateIn = 0;
	m_nodes = 0;
		  m_numpv = 1;
	m_variation.clear();
}


bool Analysis::isValid() const
{
	return !m_variation.isEmpty() && m_depth > 0 && m_msec > 0;
}

int Analysis::mpv() const
{
	 return m_numpv;
}

void Analysis::setNumpv(int n)
{
	 m_numpv = n;
}

int Analysis::time() const
{
	return m_msec;
}

void Analysis::setTime(int msec)
{
	m_msec = msec;
}

int Analysis::score() const
{
	return m_score;
}

void Analysis::setScore(int score)
{
	m_score = score;
}

int Analysis::depth() const
{
	return m_depth;
}

void Analysis::setDepth(int depth)
{
	m_depth = depth;
}


quint64 Analysis::nodes() const
{
	return m_nodes;
}

void Analysis::setNodes(quint64 nodes)
{
	m_nodes = nodes;
}

MoveList Analysis::variation() const
{
	return m_variation;
}

void Analysis::setVariation(const MoveList& variation)
{
	m_variation = variation;
}

bool Analysis::isMate() const
{
	return m_mateIn != 0;
}

int Analysis::movesToMate() const
{
	return m_mateIn;
}

void Analysis::setMovesToMate(int mate)
{
	m_mateIn = mate;
}

QString Analysis::toString(const Board& board) const
{
	Board testBoard = board;
	QString out;
	if (isMate()) {
		QString color = testBoard.toMove() == White ? "000080" : "800000";
		QString text = qApp->tr("Mate in");
		out = QString("<font color=\"#%1\"><b>%2 %3</b></font> ")
				.arg(color).arg(text).arg(movesToMate());
	}
	else if (score() > 0)
		out = QString("<font color=\"#000080\"><b>+%1</b></font> ").arg(score() / 100.0, 0, 'f', 2);
	else out = QString("<font color=\"#800000\"><b>%1</b></font> ").arg(score() / 100.0, 0, 'f', 2);

	int moveNo = testBoard.moveNumber();
	bool white = testBoard.toMove() == White;
	QString moveText;
	foreach (Move move, variation()) {
		if (white)
			moveText += QString::number(moveNo++) + ". ";
		else  if (moveText.isEmpty())
			moveText += QString::number(moveNo++) + "... ";
		moveText += testBoard.moveToSan(move);
		moveText += " ";
		testBoard.doMove(move);
		white = !white;
	}
	out += moveText;
	out += tr(" (depth %1)").arg(depth());
	out += " <a href=\""+QString::number(m_numpv)+"\">[+]</a>";
	return out;
}
