/***************************************************************************
                          common.cpp  -  common chess code
                             -------------------
    begin                : 27 August 2005
    copyright            : (C) 2005, 2006 William Hoggarth
														<whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "common.h"

QString resultString(Result result)
{
	switch (result) {
	case WhiteWin:
		return "1-0";
	case Draw:
		return "1/2-1/2";
	case BlackWin:
		return "0-1";
	case Unknown:
	default:
		return "*";
	}
}

QMap<Tag, Tag> BasicTagMap;
