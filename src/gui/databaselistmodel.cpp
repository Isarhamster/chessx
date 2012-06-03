#include "databaselistmodel.h"
#include <QFileInfo>
#include <QFont>

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
            {
                QStringList sizes;
                sizes << "" << "k" << "M" << "G" << "T" << "P";
                QFileInfo f(m_databases.at(index.row()).m_path);
                int i=0;
                qint64 size = f.size();
                while ((size>=1024) && (i<sizes.count()))
                {
                       size /= 1024;
                       i++;
                }
                return QString("%1%2").arg(size).arg(sizes[i]);
            }
            case 2:
                return stateString(m_databases.at(index.row()).m_state);
            case 3:
                return m_databases.at(index.row()).m_path;
            default:
                break;
            }
        }
        else if (role == Qt::FontRole)
        {
            if (m_databases.at(index.row()).m_isCurrent)
            {
                if (index.column() == 0) //change font only for cell(0,0)
                {
                    QFont boldFont;
                    boldFont.setBold(true);
                    return boldFont;
                }
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

void DatabaseListModel::addEntry(DatabaseListEntry& d, const QString& s)
{
    beginInsertRows(QModelIndex(), m_databases.count(), m_databases.count());
    d.m_name = QFileInfo(s).fileName();
    m_databases.push_back(d);
    endInsertRows();
}

void DatabaseListModel::addRecentFile(const QString& s)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        if ((e.m_state != EDBL_OPEN) && (e.m_state != EDBL_MODIFIED))
        {
            e.m_state = e.m_isFavorite ? EDBL_FAVORITE : EDBL_RECENT;
            QModelIndex m = createIndex(m_databases.indexOf(e),2);
            emit QAbstractItemModel::dataChanged(m,m);
        }
        return;
    }

    addEntry(d,s);
}

void DatabaseListModel::addFileOpen(const QString& s)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;

    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        if (e.m_state != EDBL_MODIFIED)
        {
            e.m_state = EDBL_OPEN;
            QModelIndex m = createIndex(m_databases.indexOf(e),2);
            emit QAbstractItemModel::dataChanged(m,m);
        }
        return;
    }

    d.m_state = EDBL_OPEN;
    addEntry(d,s);
}

void DatabaseListModel::setFileModified(const QString& s, bool modified)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        e.m_state = modified ? EDBL_MODIFIED : EDBL_OPEN;
        QModelIndex m = createIndex(m_databases.indexOf(e),2);
        emit QAbstractItemModel::dataChanged(m,m);
        return;
    }
}

void DatabaseListModel::addFavoriteFile(const QString& s)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        if (e.m_state == EDBL_RECENT)
        {
            e.m_isFavorite = true;
            e.m_state = EDBL_FAVORITE;
            QModelIndex m = createIndex(m_databases.indexOf(e),2);
            emit QAbstractItemModel::dataChanged(m,m);
        }
        return;
    }

    d.m_state = EDBL_FAVORITE;
    addEntry(d,s);
}

void DatabaseListModel::setFileClose(const QString& s)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        if (e.m_state == EDBL_OPEN)
        {
            e.m_state = e.m_isFavorite ? EDBL_FAVORITE : EDBL_RECENT;
            QModelIndex m = createIndex(m_databases.indexOf(e),2);
            emit QAbstractItemModel::dataChanged(m,m);
        }
        return;
    }
}

void DatabaseListModel::setFileCurrent(const QString& s)
{
    for (int i=0; i<m_databases.count();i++)
    {
        if (m_databases[i].m_isCurrent)
        {
            m_databases[i].m_isCurrent = false;
            QModelIndex m = createIndex(i,0);
            emit QAbstractItemModel::dataChanged(m,m);
        }
    }

    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        e.m_isCurrent = true;
        QModelIndex m = createIndex(m_databases.indexOf(e),0);
        emit QAbstractItemModel::dataChanged(m,m);
    }
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
