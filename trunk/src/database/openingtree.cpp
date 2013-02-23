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

#include <QtCore>

const unsigned MinAveYear = 1;
const unsigned MinAveRating = 5;

MoveData::MoveData()
{
	count = 0;
    for (int  r = Unknown; r <= BlackWin; ++r)
		result[r] = 0;
	year = rating = 0;
	dated = rated = 0;

}

void MoveData::addGame(Game& g, Color c, MoveType movetype)
{
	if (!count)
		move = (movetype == StandardMove) ? g.moveToSan(Game::MoveOnly, Game::PreviousMove)
			: qApp->translate("MoveData", "[end]");
    ++count;
	result[g.result()]++;
	unsigned elo = (c == White) ? g.tag("WhiteElo").toInt() : g.tag("BlackElo").toInt();
	if (elo >= 1000) {
		rating += elo;
        ++rated;
	}
	unsigned y = g.tag("Date").section(".", 0, 0).toInt();
	if (y > 1000) {
		year += y;
        ++dated;
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

OpeningTreeUpdater oupd;

void OpeningTreeUpdater::run()
{
    Game g;
    QMap<Move, MoveData> moves;
    int games = 0;
    for (int i = 0; i < m_filter->size(); ++i) {
        m_filter->database()->loadGameMoves(i, g);
        int id = g.findPosition(m_board);
        if (id != NO_MOVE)
        {
            if (m_updateFilter)
            {
                m_filter->set(i, id + 1); // not zero means success, but id could be 0.
            }
            m_filter->database()->loadGameHeaders(i, g);
            g.moveToId(id);
            if (g.atGameEnd())
            {
                moves[Move()].addGame(g, m_board.toMove(), MoveData::GameEnd);
            }
            else
            {
                g.forward();
                moves[g.move()].addGame(g, m_board.toMove());
            }
            ++games;
        }
        else
        {
            if (m_updateFilter)
            {
                m_filter->set(i, 0);
            }
        }
        if (i * 100 / m_filter->size() > (i - 1) * 100 / m_filter->size())
        {
            emit progress(i * 100 / m_filter->size());
        }
        if (m_break)
        {
            break;
        }
    }
    *m_games = games;
    m_moves->clear();
    if (!m_break)
    {
        for (QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
            m_moves->append(it.value());
        qSort(m_moves->begin(), m_moves->end());
        emit UpdateFinished(&m_board);
    }
    else
    {
        emit UpdateTerminated(&m_board);
    }
}

void OpeningTreeUpdater::cancel()
{
    m_break = true;
}

bool OpeningTreeUpdater::update(Filter& f, const Board& b, QList<MoveData>& m, int& g, bool updateFilter)
{
    m_break = false;
    m_filter = &f;
    m_board = b;
    m_moves = &m;
    m_games = &g;
    m_updateFilter = updateFilter;
    // todo: if running wait for stop
    start();
    return true;
}

bool OpeningTree::update(Filter& f, const Board& b, bool updateFilter)
{
    if (!oupd.isRunning())
    {
        if (&f==m_filter && b==m_board)
            return true;
        m_board = b;
        m_filter = &f;
        m_updateFilter = updateFilter;
        emit openingTreeUpdateStarted();
        m_bRequestPending = false;
        connect(&oupd, SIGNAL(UpdateFinished(Board*)), this, SLOT(updateFinished(Board*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(UpdateTerminated(Board*)), this, SLOT(updateTerminated(Board*)), Qt::UniqueConnection);
        connect(&oupd,SIGNAL(progress(int)), SIGNAL(progress(int)), Qt::UniqueConnection);
        return oupd.update(f,b, m_moves, m_games, m_updateFilter);
    }
    else
    {
        if (&f==m_filter && b==m_board)
            return true;
        m_board = b;
        m_filter = &f;
        m_updateFilter = updateFilter;
        m_bRequestPending = true;
        oupd.cancel();
        return false;
    }
}

void OpeningTree::cancel(bool bVisible)
{
    if (!bVisible && oupd.isRunning())
    {
        m_bRequestPending = false;
        oupd.cancel();
        oupd.wait(200);
    }
}

void OpeningTree::updateFinished(Board* b)
{
    sort();
    emit openingTreeUpdated();
    if (m_bRequestPending)
    {
        updateTerminated(b);
    }
}

void OpeningTree::updateTerminated(Board*)
{
    if (m_bRequestPending)
    {
        emit openingTreeUpdateStarted();
        m_bRequestPending = false;
        connect(&oupd, SIGNAL(UpdateFinished(Board*)), this, SLOT(updateFinished(Board*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(UpdateTerminated(Board*)), this, SLOT(updateTerminated(Board*)), Qt::UniqueConnection);
        connect(&oupd,SIGNAL(progress(int)),SIGNAL(progress(int)), Qt::UniqueConnection);
        oupd.update(*m_filter,m_board, m_moves, m_games, m_updateFilter);
    }
}

QString OpeningTree::debug()
{
	QString s;
    for (int i = 0; i < m_moves.count(); ++i)
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

OpeningTree::OpeningTree() : m_sortcolumn(1), m_order(Qt::DescendingOrder), m_filter(0)
{
    m_names << tr("Move") << tr("Count") << tr("Score") << tr("Rating") << tr("Year");
}

OpeningTree::OpeningTree(Filter & f, const Board & b, bool updateFilter) :
        m_sortcolumn(1), m_order(Qt::DescendingOrder), m_filter(0)
{
    m_names << tr("Move") << tr("Count") << tr("Score") << tr("Rating") << tr("Year");
    update(f, b, updateFilter);
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
        for (int i = 0; i < m_moves.count() / 2; ++i)
			qSwap(m_moves[i], m_moves[m_moves.count() - i -1]);
    beginResetModel();
    endResetModel();
}

void OpeningTree::sort()
{
	sort(m_sortcolumn, m_order);
}

QString OpeningTree::move(const QModelIndex& index) const
{
	return index.isValid() ? m_moves[index.row()].move : QString();
}

