/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "playerlistmodel.h"
#include "database.h"
#include "playerinfo.h"

PlayerListModel::PlayerListModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_db(0),
    m_pPlayerInfo(0)
{
    m_columnNames << tr("Name") << tr("Games") << tr("Date") << tr("Rating") << tr("Total") << tr("White")<< tr("Black");
}

QModelIndex PlayerListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex PlayerListModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int PlayerListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_db ? m_db->index()->tagValues(TagPlayerName)->rowCount(parent) : 0;
}

int PlayerListModel::columnCount(const QModelIndex &) const
{
    return m_columnNames.count();
}

bool PlayerListModel::hasChildren(const QModelIndex &parent) const
{
    return !parent.isValid();
}

QVariant PlayerListModel::data(const QModelIndex &index, int role) const
{
    if ((role != Qt::DisplayRole) || !m_db)
        return QVariant();

    QString player = m_db->index()->tagValues(TagPlayerName)->data(index,role).toString();
    if (index.column()==0) return player;
    if (player.isEmpty()) return QVariant();

    if (!m_pPlayerInfo)
    {
        m_pPlayerInfo = new PlayerInfo(m_db,player);
    }
    else if (m_pPlayerInfo->name() != player)
    {
        delete m_pPlayerInfo;
        m_pPlayerInfo = new PlayerInfo(m_db,player);
    }

    switch (index.column())
    {
    case 1: return m_pPlayerInfo->unformattedGameCount();
    case 2: return m_pPlayerInfo->unformattedRange();
    case 3: return m_pPlayerInfo->unformattedRating();
    case 4: return m_pPlayerInfo->unformattedScoreTotal();
    case 5: return m_pPlayerInfo->unformattedScoreWhite();
    case 6: return m_pPlayerInfo->unformattedScoreBlack();
    }

    return QVariant();
}

QVariant PlayerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("%1").arg(m_columnNames.at(section));
    else
        return QString("%1").arg(section);
}

void PlayerListModel::setDatabase(Database* db)
{
    m_db = db;
}
