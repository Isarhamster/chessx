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
}

QString Analysis::toString(const Board& board) const
{
	Board testBoard = board;
	QString out;
	if (mateIn) {
		int movesToMate = int(score);
		QString color = movesToMate >= 0 ? "000080" : "800000";
		QString text = qApp->tr("Mate in");
		out = QString("<font color=\"#%1\"><b>%2 %3</b></font> ")
				.arg(color).arg(text).arg(abs(movesToMate));
	} else if (score >= 0.0)
		out = QString("<font color=\"#000080\"><b>+%1</b></font> ").arg(score, 0, 'f', 2);
	else out = QString("<font color=\"#800000\"><b>%1</b></font> ").arg(score, 0, 'f', 2);
	int moveNo = testBoard.moveNumber();
	bool white = testBoard.toMove() == White;
	for (int i = 0; i < variation.size(); ++i) {
		if (white)
			out += QString::number(moveNo++) + ". ";
		else  if (i == 0)
			out += QString::number(moveNo++) + "... ";
		const Move& m = variation[i];
		out += testBoard.moveToSan(m);
		out += " ";
		testBoard.doMove(m);
		white = !white;
	}
	out += qApp->tr(" (depth %1)").arg(depth);
	return out;
}

