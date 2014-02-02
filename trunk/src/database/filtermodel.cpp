/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "database.h"
#include "filtermodel.h"

#include <QtGui>

FilterModel::FilterModel(Filter* filter, QObject* parent)
    : QAbstractItemModel(parent), m_filter(filter), m_gameIndex(-1), m_gameIndex2(-1),m_lastGame(0)
{
    m_columnNames << tr("Nr")
                  << tr("White")
                  << tr("WhiteElo")
                  << tr("Black")
                  << tr("BlackElo")
                  << tr("Event")
                  << tr("Site")
                  << tr("Round")
                  << tr("Date")
                  << tr("Result")
                  << tr("ECO")
                  << tr("Moves");

    m_columnTags << "Nr"
                 << TagNameWhite
                 << TagNameWhiteElo
                 << TagNameBlack
                 << TagNameBlackElo
                 << TagNameEvent
                 << TagNameSite
                 << TagNameRound
                 << TagNameDate
                 << TagNameResult
                 << TagNameECO
                 << "Length";

    m_game  = new Game;
    m_game2 = new Game;
}

FilterModel::~FilterModel()
{
    delete m_game;
    delete m_game2;
    m_lastGame = 0;
}

int FilterModel::rowCount(const QModelIndex& index) const
{
    if(index.isValid())
    {
        return 0;
    }
    return m_filter->count();
}

int FilterModel::columnCount(const QModelIndex&) const
{
    return m_columnNames.count();
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && index.row() < m_filter->count())
    {
        int i = m_filter->indexToGame(index.row());
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
            {
                return i + 1;
            }
        }
        if(i != -1)
        {
            if ((i != m_gameIndex) && (i!=m_gameIndex2))
            {
                if (m_lastGame != m_game2)
                {
                    m_lastGame = m_game2;
                    m_gameIndex2 = i;
                }
                else
                {
                    m_lastGame = m_game;
                    m_gameIndex = i;
                }
                m_lastGame->clearTags();
            }
            else if (i == m_gameIndex)
            {
                m_lastGame = m_game;
            }
            else if (i == m_gameIndex2)
            {
                m_lastGame = m_game2;
            }
            if(role == Qt::DisplayRole)
            {
                m_filter->database()->loadGameHeader(i, *m_lastGame, m_columnTags.at(index.column()));
                QString tag = m_lastGame->tag(m_columnTags.at(index.column()));
                if(tag == "?")
                {
                    tag.clear();
                }
                return tag;
            }
            else if(role == Qt::FontRole)
            {
                if(m_filter->database()->deleted(i))
                {
                    QFont font;
                    font.setStrikeOut(true);
                    return font;
                }
            }
            else if(role == Qt::ForegroundRole)
            {
                if(!m_filter->database()->getValidFlag(i))
                {
                    QVariant v = QColor(Qt::red);
                    return v;
                }
                QVariant v = QColor(Qt::black);
                return v;
            }
        }
    }
    return QVariant();
}

QVariant FilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        return QString("%1").arg(m_columnNames.at(section));
    }
    else
    {
        return QString("%1").arg(section);
    }
}

Qt::ItemFlags FilterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if(index.isValid())
    {
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable;
    }
    else
    {
        return Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable;
    }
}

QModelIndex FilterModel::index(int row, int column, const QModelIndex& parent) const
{
    if(parent.isValid())
    {
        return QModelIndex();
    }
    return createIndex(row, column, (void*) 0);
}

void FilterModel::setFilter(Filter* filter)
{
    beginResetModel();
    m_filter = filter;
    m_gameIndex = -1;
    m_gameIndex2 = -1;
    endResetModel();
}

Filter* FilterModel::filter()
{
    return m_filter;
}

void FilterModel::sort(int column, Qt::SortOrder order)
{

}

