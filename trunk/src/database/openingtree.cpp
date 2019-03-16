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

const unsigned MinAveYear = 1;
const unsigned MinAveRating = 5;

bool OpeningTree::updateFilter(Filter& f, const Board& b, bool updateFilter, bool sourceIsFilter, bool bEnd)
{
    if(&f == m_filter && updateFilter == m_updateFilter && b == m_board && m_bEnd == bEnd)
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
        connect(&oupd, SIGNAL(UpdateFinished(Board*)), this, SLOT(updateFinished(Board*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(MoveUpdate(Board*,QList<MoveData>)), this, SLOT(moveUpdated(Board*,QList<MoveData>)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(UpdateTerminated(Board*)), this, SLOT(updateTerminated(Board*)), Qt::UniqueConnection);
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

void OpeningTree::updateFinished(Board* b)
{
    emit openingTreeUpdated();
    if(m_bRequestPending)
    {
        updateTerminated(b);
    }
}

void OpeningTree::moveUpdated(Board* b, QList<MoveData> moveList)
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

void OpeningTree::updateTerminated(Board*)
{
    if(m_bRequestPending)
    {
        emit openingTreeUpdateStarted();
        m_bRequestPending = false;
        connect(&oupd, SIGNAL(UpdateFinished(Board*)), this, SLOT(updateFinished(Board*)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(MoveUpdate(Board*,QList<MoveData>)), this, SLOT(moveUpdated(Board*,QList<MoveData>)), Qt::UniqueConnection);
        connect(&oupd, SIGNAL(UpdateTerminated(Board*)), this, SLOT(updateTerminated(Board*)), Qt::UniqueConnection);
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
    m_filter(0),
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

QPixmap OpeningTree::paintPercentage(int percentage) const
{
    QRect fullRect(QPoint(0,0),QSize(41, AppSettings->getValue("/General/ListFontSize").toInt()));
    QImage rowImg = QImage(fullRect.size(),QImage::Format_RGB16);
    rowImg.fill(Qt::white);
    fullRect = fullRect.adjusted(0,0,-1,-1);
    QPainter p;
    p.begin(&rowImg);
    p.setPen(QPen(QColor(Qt::black)));
    p.drawRect(fullRect);
    int percentWidth = (percentage * fullRect.width()) / 100;
    int remainder = fullRect.width() - percentWidth;
    QRect blackRect(QPoint(percentWidth, 0), QSize(remainder, fullRect.height()));
    p.setBrush(QBrush(QColor(Qt::gray), Qt::SolidPattern));
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

    switch (role)
    {
        case Qt::DisplayRole:
        {
            switch(index.column())
            {
            case 0:
                return QString("%1: %2").arg(index.row() + 1).arg(m_moves[index.row()].localsan);
            case 1:
            {
                if(m_games == 0)
                {
                    return "";
                }
                unsigned int percentage = m_moves[index.row()].count * 1000u / m_games / 10u;
                QString approx;
                if(percentage == 0)
                {
                    percentage = 1;
                    approx = "<";
                }
                return QString("%1: %2%3%")
                       .arg(m_moves[index.row()].count)
                       .arg(approx)
                       .arg(percentage);
            }
            case 2:
                if (m_moves[index.row()].hasPercent())
                    return QString("%1%").arg(m_moves[index.row()].percentage());
                break;
            case 3:
                return m_moves[index.row()].rated >= MinAveRating ?
                       m_moves[index.row()].averageRating() : QVariant();
            case 4:
                return m_moves[index.row()].dated >= MinAveYear ?
                       m_moves[index.row()].averageYear() : QVariant();

            default:
                return QVariant();
            }
            break;
        }
        case Qt::DecorationRole:
        {
            if ((index.column() == 2) && m_moves[index.row()].hasPercent())
            {
                return paintPercentage(static_cast<int>(m_moves[index.row()].percentage()));
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

Board OpeningTree::board() const
{
    return m_board;
}

