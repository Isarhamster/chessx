#include "databaselistmodel.h"
#include <QFileInfo>

DatabaseListModel::DatabaseListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_columnNames << tr("Name") << tr("Size") << tr("State") << tr("Path");
}

QModelIndex DatabaseListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex DatabaseListModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int DatabaseListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_databases.count();
}

int DatabaseListModel::columnCount(const QModelIndex &) const
{
    return m_columnNames.count();
}

bool DatabaseListModel::hasChildren(const QModelIndex &parent) const
{
    return !parent.isValid();
}

QVariant DatabaseListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case 0:
                return m_databases.at(index.row()).m_name;
            case 1:
                return "";
            case 2:
                return stateString(m_databases.at(index.row()).m_state);
            case 3:
                return m_databases.at(index.row()).m_path;
            default:
                break;
            }
        }
    }

    return QVariant();
}

QVariant DatabaseListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("%1").arg(m_columnNames.at(section));
    else
        return QString("%1").arg(section);
}

void DatabaseListModel::addRecentFile(const QString& s)
{
    DatabaseListEntry e;
    foreach (e, m_databases)
    {
        if (e.m_path == s) return;
    }

    beginInsertRows(QModelIndex(), m_databases.count(), m_databases.count());
    DatabaseListEntry d;
    d.m_path = s;
    d.m_name = QFileInfo(s).fileName();
    d.m_state = EDBL_RECENT;
    m_databases.push_back(d);
    endInsertRows();
}

QString DatabaseListModel::stateString(DatabaseListEntryState e) const
{
    switch (e)
    {
    case EDBL_RECENT:   return tr("Recent");
    case EDBL_OPEN:     return tr("Open");
    case EDBL_MODIFIED: return tr("Modified");
    case EDBL_FAVORITE: return tr("Favorite");
    }
    return "";
}
