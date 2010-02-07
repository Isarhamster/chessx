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
#include "movelist.h"

class Board;

/** The analysis class contains engine analysis. */
class Analysis
{
public:
	 Analysis();
	 /** Convert analysis to formatted text. */
	 QString toString(const Board& board) const;
	 float time;
	 quint64 nodes;
	 int depth;
	 float score;
	 MoveList variation;
	 bool mateIn;

};

#endif // ANALYSIS_H
