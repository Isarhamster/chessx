/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "database.h"
#include "openingtree.h"
#include "settings.h"
#include "movedata.h"
#include "openingtreethread.h"

#include <QIcon>
#include <QImage>
#include <QPainter>
#include <QtCore>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

static bool compareMove(const MoveData& m1, const MoveData& m2)
{
    return m1.san < m2.san;
}

static bool compareScore(const MoveData& m1, const MoveData& m2)
{
    auto s1 = m1.results.scorePercentage();
    auto s2 = m2.results.scorePercentage();
    return s1 < s2 || (s1 == s2 && m1.san < m2.san);
}

static bool compareRating(const MoveData& m1, const MoveData& m2)
{
    auto r1 = m1.rating.average();
    auto r2 = m2.rating.average();
    return r1 < r2 || (r1 == r2 && m1.san < m2.san);
}

static bool compareYear(const MoveData& m1, const MoveData& m2)
{
    auto y1 = m1.year.average();
    auto y2 = m2.year.average();
    return y1 < y2 || (y1 == y2 && m1.san < m2.san);
}

const unsigned MinAveYear = 1;
const unsigned MinAveRating = 5;

bool OpeningTree::updateFilter(FilterX& f, const BoardX& b, bool updateFilter, bool sourceIsFilter, bool bEnd)
{
    if((&f == m_filter) && (updateFilter == m_updateFilter) && (b == m_board) && (m_bEnd == bEnd) && (m_sourceIsDatabase != sourceIsFilter))
    {
        return true;
    }
    m_bEnd = bEnd;
    m_board = b;
    m_filter = &f;
    m_updateFilter = updateFilter;
    m_sourceIsDatabase = !sourceIsFilter;

    if(!oupd.isRunning())
    {
        emit openingTreeUpdateStarted();
        m_bRequestPending = false;
        connect(&oupd, SIGNAL(UpdateFinished(BoardX*)), this, SLOT(updateFinished(BoardX*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(MoveUpdate(BoardX*,QList<MoveData>)), this, SLOT(moveUpdated(BoardX*,QList<MoveData>)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(UpdateTerminated(BoardX*)), this, SLOT(updateTerminated(BoardX*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(progress(int)), SIGNAL(progress(int)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(requestGameFilterUpdate(int,int)), SIGNAL(requestGameFilterUpdate(int,int)), Qt::UniqueConnection);
        return oupd.updateFilter(f, b, m_games, m_updateFilter, m_sourceIsDatabase, m_bEnd);
    }
    else
    {
        m_bRequestPending = true;
        oupd.cancel();
        return false;
    }
}

void OpeningTree::cancel()
{
    if(oupd.isRunning())
    {
        m_bRequestPending = false;
        oupd.cancel();
        oupd.wait(10000);
    }
}

void OpeningTree::updateFinished(BoardX* b)
{
    emit openingTreeUpdated();
    if(m_bRequestPending)
    {
        updateTerminated(b);
    }
}

void OpeningTree::moveUpdated(BoardX* b, QList<MoveData> moveList)
{
    if (*b == m_board)
    {
        beginResetModel();
        {
            m_moves = moveList;
            doSort(m_sortcolumn, m_order);
        }
        endResetModel();
    }
}

void OpeningTree::updateTerminated(BoardX*)
{
    if(m_bRequestPending)
    {
        emit openingTreeUpdateStarted();
        m_bRequestPending = false;
        connect(&oupd, SIGNAL(UpdateFinished(BoardX*)), this, SLOT(updateFinished(BoardX*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(MoveUpdate(BoardX*,QList<MoveData>)), this, SLOT(moveUpdated(BoardX*,QList<MoveData>)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(UpdateTerminated(BoardX*)), this, SLOT(updateTerminated(BoardX*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(progress(int)), SIGNAL(progress(int)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(requestGameFilterUpdate(int,int)), SIGNAL(requestGameFilterUpdate(int,int)), Qt::UniqueConnection);
        oupd.updateFilter(*m_filter, m_board, m_games, m_updateFilter, m_sourceIsDatabase, m_bEnd);
    }
}

int OpeningTree::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_moves.count();
}

int OpeningTree::columnCount(const QModelIndex&) const
{
    return m_names.count();
}

OpeningTree::OpeningTree(QObject* parent) :
    QAbstractTableModel(parent),
    m_sortcolumn(1),
    m_order(Qt::DescendingOrder),
    m_filter(nullptr),
    oupd(*new OpeningTreeThread)
{
    m_names << tr("Move") << tr("Count") << tr("Score") << tr("Rating") << tr("Year");
}

OpeningTree::~OpeningTree()
{
    delete &oupd;
}

QVariant OpeningTree::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return m_names[section];
    }
    else
    {
        return QVariant();
    }
}

QPixmap OpeningTree::paintPercentage(int percentageWhite, int percentageBlack) const
{
    QRect fullRect(QPoint(0,0),QSize(41, AppSettings->getValue("/General/ListFontSize").toInt()));
    QImage rowImg = QImage(fullRect.size(),QImage::Format_RGB16);
    rowImg.fill(Qt::lightGray);
    fullRect = fullRect.adjusted(0,0,-1,-1);

    QPainter p;
    p.begin(&rowImg);
    p.setPen(QPen(QColor(Qt::gray)));
    p.drawRect(fullRect);

    int percentWidthWhite = (percentageWhite * fullRect.width()) / 100;
    int percentWidthBlack = (percentageBlack * fullRect.width()) / 100;

    QRect blackRect(QPoint(fullRect.width()-percentWidthBlack, 0), QSize(percentWidthBlack, fullRect.height()));
    QRect whiteRect(QPoint(0, 0), QSize(percentWidthWhite, fullRect.height()));

    p.setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
    p.drawRect(whiteRect);

    p.setBrush(QBrush(QColor(Qt::darkGray), Qt::SolidPattern));
    p.drawRect(blackRect);
    p.end();

    return QPixmap().fromImage(rowImg);
}

bool OpeningTree::bEnd() const
{
    return m_bEnd;
}

QVariant OpeningTree::data(const QModelIndex& index, int role) const
{
    if(!index.isValid() || index.row() >= m_moves.count())
    {
        return QVariant();
    }

    const auto& data = m_moves[index.row()];
    switch (role)
    {
        case Qt::DisplayRole:
        {
            switch(index.column())
            {
            case 0:
                return QString("%1: %2").arg(index.row() + 1).arg(data.localsan);
            case 1:
            {
                if(m_games == 0)
                {
                    return "";
                }
                auto gamesCount = data.results.count();
                unsigned int percentage = gamesCount * 1000U / m_games / 10U;
                QString approx;
                if(percentage == 0)
                {
                    percentage = 1;
                    approx = "<";
                }
                return QString("%1: %2%3%")
                       .arg(gamesCount)
                       .arg(approx)
                       .arg(percentage);
            }
            case 2:
                if (data.results)
                    return QString("%1%").arg(data.results.scorePercentage(), 0, 'f', 1);
                break;
            case 3:
                return data.rating.count() >= MinAveRating ?
                       data.rating.average() : QVariant();
            case 4:
                return data.year.count() >= MinAveYear ?
                       data.year.average() : QVariant();

            default:
                return QVariant();
            }
            break;
        }
        case Qt::DecorationRole:
        {
            if ((index.column() == 2) && data.results)
            {
                return paintPercentage(static_cast<int>(data.results.whiteWinPercentage()),
                        static_cast<int>(data.results.blackWinPercentage()));
            }
            break;
        }
    }

    return QVariant();
}

void OpeningTree::doSort(int column, Qt::SortOrder order)
{
    m_sortcolumn = column;
    m_order = order;

    switch(column)
    {
    case 0:
        std::sort(m_moves.begin(), m_moves.end(), compareMove);
        break;
    case 1:
        std::sort(m_moves.begin(), m_moves.end());
        break;
    case 2:
        std::sort(m_moves.begin(), m_moves.end(), compareScore);
        break;
    case 3:
        std::sort(m_moves.begin(), m_moves.end(), compareRating);
        break;
    case 4:
        std::sort(m_moves.begin(), m_moves.end(), compareYear);
        break;
    };

    if(order == Qt::DescendingOrder)
    {
        for(int i = 0; i < m_moves.count() / 2; ++i)
        {
            qSwap(m_moves[i], m_moves[m_moves.count() - i - 1]);
        }
    }
}

void OpeningTree::sort(int column, Qt::SortOrder order)
{
    beginResetModel();
    doSort(column, order);
    endResetModel();
}

void OpeningTree::sort()
{
    sort(m_sortcolumn, m_order);
}

QString OpeningTree::move(const QModelIndex& index) const
{
    return index.isValid() ? m_moves[index.row()].san : QString();
}

BoardX OpeningTree::board() const
{
    return m_board;
}

