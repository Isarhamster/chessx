#include "databaselistmodel.h"
#include <QFileInfo>
#include <QFont>
#include <QPixmap>

DatabaseListModel::DatabaseListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_columnNames << tr("Favorite") << tr("Name") << tr("Size") << tr("Open") << tr("Path");
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
        if (role == Qt::DecorationRole)
        {
            switch (index.column())
            {
            case DBLV_FAVORITE:
                {
                    bool bIsFavorite = m_databases.at(index.row()).m_isFavorite;
                    return QPixmap(bIsFavorite ? ":/folder_favorite.png" : ":/folder_grey.png");
                }
            case DBLV_OPEN:
                {
                    bool bIsOpen = m_databases.at(index.row()).m_state == EDBL_OPEN;
                    bool bIsCurrent = m_databases.at(index.row()).m_isCurrent;
                    if (bIsOpen)
                        return QPixmap(bIsCurrent ? ":/folder_new.png" : ":/fileopen.png");
                    else
                        return QVariant();
                }
                default:
                    return QVariant();
            }
        }
        else if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case DBLV_FAVORITE:
                return QVariant();
            case DBLV_NAME:
            {
                QString s = m_databases.at(index.row()).m_name;
                if (s.isEmpty()) return tr("ClipBoard");
                return s;
            }
            case DBLV_SIZE:
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
            case DBLV_OPEN:
                return QVariant();
            case DBLV_PATH:
                return m_databases.at(index.row()).m_path;
            default:
                break;
            }
        }
        else if (role == Qt::FontRole)
        {
            if (m_databases.at(index.row()).m_isCurrent)
            {
                if ((index.column() == DBLV_NAME) || (index.column() == DBLV_PATH))
                {
                    QFont boldFont;
                    boldFont.setBold(true);
                    return boldFont;
                }
            }
        }
        else if (role == Qt::ToolTipRole)
        {
            switch (index.column())
            {
            case DBLV_FAVORITE:
                {
                    bool bIsFavorite = m_databases.at(index.row()).m_isFavorite;
                    bool bIsClipBoard = m_databases.at(index.row()).m_name.isEmpty();
                    return QString(bIsFavorite ? "Favorite" : bIsClipBoard ? "ClipBoard" : "");
                }
            case DBLV_PATH:
                {
                    QString s = m_databases.at(index.row()).m_name;
                    return s;
                }
            case DBLV_OPEN:
                {
                    bool bIsOpen = m_databases.at(index.row()).m_state == EDBL_OPEN;
                    return QString(bIsOpen ? "Open" : "Closed");
                }
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

void DatabaseListModel::addEntry(DatabaseListEntry& d, const QString& s)
{
    beginInsertRows(QModelIndex(), m_databases.count(), m_databases.count());
    d.m_name = QFileInfo(s).fileName();
    m_databases.push_back(d);
    endInsertRows();
}

void DatabaseListModel::addFileOpen(const QString& s)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;

    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        if (e.m_state != EDBL_OPEN)
        {
            e.m_state = EDBL_OPEN;
            QModelIndex m = createIndex(m_databases.indexOf(e),DBLV_OPEN);
            emit QAbstractItemModel::dataChanged(m,m);
        }
        return;
    }

    d.m_state = EDBL_OPEN;
    addEntry(d,s);
}

void DatabaseListModel::addFavoriteFile(const QString& s, bool bFavorite)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();      
        if (e.m_isFavorite != bFavorite)
        {
            e.m_isFavorite = bFavorite;
            QModelIndex m = createIndex(m_databases.indexOf(e),DBLV_FAVORITE);
            emit QAbstractItemModel::dataChanged(m,m);
        }
        return;
    }

    d.m_isFavorite = bFavorite;
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
            e.m_state = EDBL_CLOSE;
            QModelIndex m = createIndex(m_databases.indexOf(e),DBLV_OPEN);
            emit QAbstractItemModel::dataChanged(m,m);
        }
    }
}

void DatabaseListModel::setFileCurrent(const QString& s)
{
    for (int i=0; i<m_databases.count();i++)
    {
        if (m_databases[i].m_isCurrent)
        {
            m_databases[i].m_isCurrent = false;
            QModelIndex m = createIndex(i,DBLV_NAME);
            QModelIndex n = createIndex(i,DBLV_PATH);
            emit QAbstractItemModel::dataChanged(m,n);
        }
    }

    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if (i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        e.m_isCurrent = true;
        QModelIndex m = createIndex(m_databases.indexOf(e),DBLV_NAME);
        QModelIndex n = createIndex(m_databases.indexOf(e),DBLV_PATH);
        emit QAbstractItemModel::dataChanged(m,n);
    }
}

void DatabaseListModel::toStringList(QStringList& list)
{
    for (int i=1; i<m_databases.count();i++)
    {
        if (m_databases[i].m_isFavorite)
        {
            list.append(m_databases[i].m_path);
        }
    }
}

