/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "openingtree.h"
#include "common.h"
#include "database.h"
#include <QApplication>

const unsigned MinAveYear = 1;
const unsigned MinAveRating = 5;

MoveData::MoveData()
{
	count = 0;
	for (int  r = Unknown; r <= BlackWin; r++)
		result[r] = 0;
	year = rating = 0;
	dated = rated = 0;

}

void MoveData::addGame(Game& g, Color c, MoveType movetype)
{
	if (!count)
		move = (movetype == StandardMove) ? g.moveToSan(Game::MoveOnly, Game::PreviousMove)
			: qApp->translate("MoveData", "[end]");
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
	return m1.count < m2.count || (m1.count == m2.count && m1.move < m2.move);
}

bool compareMove(const MoveData& m1, const MoveData& m2)
{
	return m1.move < m2.move;
}

bool compareScore(const MoveData& m1, const MoveData& m2)
{
	return m1.percentage() < m2.percentage() ||
			 (m1.percentage() == m2.percentage() && m1.move < m2.move);
}

bool compareRating(const MoveData& m1, const MoveData& m2)
{
	return m1.averageRating() < m2.averageRating() ||
			 (m1.averageRating() == m2.averageRating() && m1.move < m2.move);
}

bool compareYear(const MoveData& m1, const MoveData& m2)
{
	return m1.averageYear() < m2.averageYear() ||
			 (m1.averageYear() == m2.averageYear() && m1.move < m2.move);
}


bool OpeningTree::update(Filter& f, const Board& b)
{
	if (m_filter == &f && m_board == b)
		return false;
	int operationId = ++m_operationId;
	m_filter = &f;
	m_board = b;
	Game g;
	QMap<Move, MoveData> moves;
	m_games = 0;
	for (int i = 0; i < f.size(); i++) {
		if (operationId != m_operationId)
			return false;
		f.database()->loadGameMoves(i, g);
		int id = g.findPosition(b);
		if (id != NO_MOVE)	{
			f.set(i, id + 1); // not zero means success, but id could be 0.
			f.database()->loadGameHeaders(i, g);
			g.moveToId(id);
			if (g.atGameEnd())
				moves[Move()].addGame(g, b.toMove(), MoveData::GameEnd);
			else {
				g.forward();
				moves[g.move()].addGame(g, b.toMove());
			}
			m_games++;
		} else {
			f.set(i, 0);
		}
		if (i * 100 / f.size() > (i - 1) / f.size())
        {
            emit progress(i * 100 / f.size());
        }

	}
	m_moves.clear();
	for (QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
		m_moves.append(it.value());
	qSort(m_moves.begin(), m_moves.end());
	sort();
	return true;
}

QString OpeningTree::debug()
{
	QString s;
	for (int i = 0; i < m_moves.count(); i++)
		s.append(QString("%1. %2\t%3 games\t%4%\n")
			 .arg(i + 1).arg(m_moves[i].move).arg(m_moves[i].count).arg(m_moves[i].percentage()));
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

OpeningTree::OpeningTree() : m_sortcolumn(1), m_order(Qt::DescendingOrder), m_filter(0),
		m_operationId(0)
{
	m_names << tr("Move") << tr("Count") << tr("Score") << tr("Rating")
	<< tr("Year");
}

OpeningTree::OpeningTree(Filter & f, const Board & b) :
		m_sortcolumn(1), m_order(Qt::DescendingOrder), m_filter(0), m_operationId(0)
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
	case 1: return m_games == 0 ? "" : QString("%1: %2%").arg(m_moves[index.row()].count)
					 .arg(m_moves[index.row()].count * 1000 / m_games / 10.0);
	case 2: return QString("%1%").arg(m_moves[index.row()].percentage());
	case 3: return m_moves[index.row()].rated >= MinAveRating ?
					 m_moves[index.row()].averageRating() : QVariant();
	case 4: return m_moves[index.row()].dated >= MinAveYear ?
					 m_moves[index.row()].averageYear() : QVariant();

	default:
		return QVariant();
	}
}

void OpeningTree::sort(int column, Qt::SortOrder order)
{
	m_sortcolumn = column;
	m_order = order;
	switch (column) {
	case 0: qSort(m_moves.begin(), m_moves.end(), compareMove); break;
	case 1: qSort(m_moves.begin(), m_moves.end()); break;
	case 2: qSort(m_moves.begin(), m_moves.end(), compareScore); break;
	case 3: qSort(m_moves.begin(), m_moves.end(), compareRating); break;
	case 4: qSort(m_moves.begin(), m_moves.end(), compareYear); break;
	};
	if (order == Qt::DescendingOrder)
		for (int i = 0; i < m_moves.count() / 2; i++)
			qSwap(m_moves[i], m_moves[m_moves.count() - i -1]);
	reset();
}

void OpeningTree::sort()
{
	sort(m_sortcolumn, m_order);
}

QString OpeningTree::move(const QModelIndex& index) const
{
	return index.isValid() ? m_moves[index.row()].move : QString();
}

