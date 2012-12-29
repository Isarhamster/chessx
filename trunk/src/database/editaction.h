/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef EDITACTION_H
#define EDITACTION_H

#include "game.h"
#include <QtCore>

/** The EditAction is a ChessBrowser action. */
class EditAction
{
public:
    enum Type {None, RemovePreviousMoves, RemoveNextMoves, RemoveVariation, PromoteVariation, VariationUp, VariationDown,
            EditPrecomment, EditComment, AddNag, ClearNags, AddNullMove};
	/** Standard constructor. */
	EditAction(Type type = None);
	/** Standard constructor with additional data. */
	EditAction(Type type, const QVariant& data);
	/** @return action type. */
	Type type() const;
	/** @return move associated with the action. */
	MoveId move() const;
	/** Set associated move. */
	void setMove(MoveId move);
	/** @return additonal data. */
	QVariant data() const;
private:
	Type m_type;
	QVariant m_data;
	MoveId m_move;
};

#endif // EDITACTION_H
