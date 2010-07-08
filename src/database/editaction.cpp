/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "editaction.h"

EditAction::EditAction(Type type)
	: m_type(type), m_move(NO_MOVE)
{

}

EditAction::EditAction(Type type, const QVariant& data)
		: m_type(type), m_data(data), m_move(NO_MOVE)
{

}

EditAction::Type EditAction::type() const
{
	return m_type;
}

MoveId EditAction::move() const
{
	return m_move;
}

void EditAction::setMove(MoveId move)
{
	m_move = move;
}

QVariant EditAction::data() const
{
	return m_data;
}


