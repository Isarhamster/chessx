/***************************************************************************
                          OpeningTree
                             -------------------
    begin                : 9 Jun 2007
    copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "openingtree.h"
#include "common.h"
#include "database.h"

MoveData::MoveData()
{
	count = 0;
	for (int  r = Unknown; r <= BlackWin; r++)
		result[r] = 0;
	year = rating = 0;
	dated = rated = 0;

}

void MoveData::addGame(Game& g, Color c)
{
	if (move.isEmpty())
		move = g.atEnd() ? "[end]" : g.moveToSan();
	count++;
	result[g.result()]++;
	unsigned elo = (c == White) ? g.tag("WhiteElo").toInt() : g.tag("BlackElo").toInt();
	if (elo >= 1000) {
		rating += elo;
		rated++;
	}
	unsigned y = g.tag("Date").section(".", 0, 0).toInt();
	if (y > 1000) {
		year += y;
		dated++;
	}
}

double MoveData::percentage() const
{
	unsigned c = result[Unknown] + 2 * result[WhiteWin] + result[Draw];
	return c * 500 / count / 10.0;
}

int MoveData::averageRating() const
{
	return rated ? rating / rated : 0;
}

int MoveData::averageYear() const
{
	return dated ? year / dated : 0;
}

bool operator<(const MoveData& m1, const MoveData& m2)
{
	return m1.count > m2.count || (m1.count == m2.count && m1.move < m2.move);
}



void OpeningTree::update(Filter& f, const Board& b)
{
	Game g;
	QMap<Move, MoveData> moves;
	m_games = 0;
	for (int i = 0; i < f.size(); i++) {
		f.database()->loadGameMoves(i, g);
		int ply = g.findPosition(b);
		f.set(i, ply);
		if (ply)	{
			f.database()->loadGameHeaders(i, g);
			g.moveToPly(ply - 1);
			m_games++;
			moves[g.atEnd() ? Move() : g.move()].addGame(g, b.toMove());
		}
	}
	m_moves.clear();
	for (QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
		m_moves.append(it.value());
	qSort(m_moves.begin(), m_moves.end());
	reset();
}

QString OpeningTree::debug()
{
	QString s;
	for (int i = 0; i < m_moves.count(); i++)
		s.append(QString("%1. %2\t%3 games\t%4%\n")
				.arg(i+1).arg(m_moves[i].move).arg(m_moves[i].count).arg(m_moves[i].percentage()));
	return s;
}

int OpeningTree::rowCount(const QModelIndex& parent) const
{
	return parent.isValid() ? 0 : m_moves.count();
}

int OpeningTree::columnCount(const QModelIndex&) const
{
	return m_names.count();
}

OpeningTree::OpeningTree()
{
	m_names << tr("Move") << tr("Count") << tr("Score") << tr("Rating")
			<< tr("Year");
}

OpeningTree::OpeningTree(Filter & f, const Board & b)
{
	m_names << tr("Move") << tr("Count") << tr("Score") << tr("Rating")
			<< tr("Year");
	update(f, b);
}

QVariant OpeningTree::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return m_names[section];
	else return QVariant();
}

QVariant OpeningTree::data(const QModelIndex& index, int role) const
{
	if (role != Qt::DisplayRole || !index.isValid() || index.row() >= m_moves.count())
		return QVariant();
	switch (index.column()) {
		case 0: return QString("%1: %2").arg(index.row() + 1).arg(m_moves[index.row()].move);
		case 1: return QString("%1: %2%").arg(m_moves[index.row()].count)
				.arg(m_moves[index.row()].count * 1000 / m_games / 10.0);
		case 2: return QString("%1%").arg(m_moves[index.row()].percentage());
		case 3: {
						unsigned ave = m_moves[index.row()].averageRating();
						return ave ? ave : QVariant();
				  }
		case 4: {
						unsigned ave = m_moves[index.row()].averageYear();
						return ave ? ave : QVariant();
				  }

		default:
			return QVariant();
	}
}

