/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "databaselistmodel.h"
#include <QDateTime>
#include <QFileInfo>
#include <QFont>
#include <QPixmap>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

DatabaseListModel::DatabaseListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_columnNames << tr("Favorite") << tr("Name") << tr("Size") << tr("Open") << tr("Path") << tr("Format") << tr("Date") << tr("Read");
}

QModelIndex DatabaseListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return QModelIndex();
    }
    return createIndex(row, column, (void*)0);
}

QModelIndex DatabaseListModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int DatabaseListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
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
    if(index.isValid())
    {
        if(role == Qt::DecorationRole)
        {
            switch(index.column())
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
                if(bIsOpen)
                {
                    return QPixmap(bIsCurrent ? ":/folder_new.png" : ":/fileopen.png");
                }
                else
                {
                    return QPixmap(":/folder_closed.png");
                }
            }
            default:
                return QVariant();
            }
        }
        else if(role == Qt::DisplayRole)
        {
            switch(index.column())
            {
            case DBLV_FAVORITE:
                return QVariant();
            case DBLV_NAME:
            {
                QString s = m_databases.at(index.row()).m_name;
                if (s.endsWith(".pgn")) s.remove(".pgn");
                return s;
            }
            case DBLV_SIZE:
            {
                QStringList sizes;
                sizes << "" << "k" << "M" << "G" << "T" << "P";
                QFileInfo f(m_databases.at(index.row()).m_path);
                int i = 0;
                qint64 size = f.size();
                while((size >= 1024) && (i < sizes.count()))
                {
                    size /= 1024;
                    i++;
                }
                return QString("%1%2").arg(size).arg(sizes[i]);
            }
            case DBLV_DATE:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                return f.lastModified().date();
            }
            case DBLV_DATE_READ:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                return f.lastRead().date();
            }
            case DBLV_OPEN:
                return QVariant();
            case DBLV_PATH:
                return m_databases.at(index.row()).m_path;
            case DBLV_UTF8:
                if (m_databases.at(index.row()).m_name.endsWith("bin"))
                {
                    return "Polyglot";
                }
                return m_databases.at(index.row()).m_utf8 ? "UTF8" : "ANSI";
            default:
                break;
            }
        }
        else if(role == Qt::FontRole)
        {
            if(m_databases.at(index.row()).m_isCurrent)
            {
                if((index.column() == DBLV_NAME) || (index.column() == DBLV_PATH))
                {
                    QFont boldFont;
                    boldFont.setBold(true);
                    return boldFont;
                }
            }
        }
        else if(role == Qt::ToolTipRole)
        {
            switch(index.column())
            {
            case DBLV_FAVORITE:
            {
                bool bIsFavorite = m_databases.at(index.row()).m_isFavorite;
                return QString(bIsFavorite ? tr("Favorite") : "");
            }
            case DBLV_PATH:
            {
                QString s = m_databases.at(index.row()).m_name;
                return s;
            }
            case DBLV_OPEN:
            {
                bool bIsOpen = m_databases.at(index.row()).m_state == EDBL_OPEN;
                return QString(bIsOpen ? tr("Open") : tr("Closed"));
            }
            case DBLV_UTF8:
            {
                if (m_databases.at(index.row()).m_name.endsWith("bin"))
                {
                    return "Polyglot";
                }
                return m_databases.at(index.row()).m_utf8 ? "UTF8" : "ANSI";
            }
            case DBLV_NAME:
            {
                QString s = m_databases.at(index.row()).m_name;
                s[0] = s[0].toUpper();
                return s;
            }
            case DBLV_DATE:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                return f.lastModified();
            }
            case DBLV_DATE_READ:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                return f.lastRead();
            }
            case DBLV_SIZE:
            {
                QStringList sizes;
                sizes << "" << "k" << "M" << "G" << "T" << "P";
                QFileInfo f(m_databases.at(index.row()).m_path);
                int i = 0;
                qint64 size = f.size();
                while((size >= 1024) && (i < sizes.count()))
                {
                    size /= 1024;
                    i++;
                }
                return QString("%1%2").arg(size).arg(sizes[i]);
            }
            default:
                break;
            }
        }
        else if(role == Qt::UserRole)
        {
            switch(index.column())
            {
            case DBLV_FAVORITE:
            {
                bool bIsFavorite = m_databases.at(index.row()).m_isFavorite;
                return QString(bIsFavorite ? "Favorite" : "");
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
            case DBLV_UTF8:
            {
                if (m_databases.at(index.row()).m_name.endsWith("bin"))
                {
                    return "Polyglot";
                }
                return m_databases.at(index.row()).m_utf8 ? "UTF8" : "ANSI";
            }
            case DBLV_NAME:
            {
                QString s = m_databases.at(index.row()).m_name;
                s[0] = s[0].toUpper();
                return s;
            }
            case DBLV_DATE:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                return f.lastModified();
            }
            case DBLV_DATE_READ:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                return f.lastRead();
            }
            case DBLV_SIZE:
            {
                QFileInfo f(m_databases.at(index.row()).m_path);
                qint64 size = f.size();
                return size;
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

Qt::ItemFlags DatabaseListModel::flags(const QModelIndex &index) const
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

DatabaseListEntry* DatabaseListModel::FindEntry(QString s)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;

    if(i.findNext(d))
    {
        return &(i.previous());
    }
    return 0;
}

void DatabaseListModel::addEntry(DatabaseListEntry& d, const QString& s)
{
    beginInsertRows(QModelIndex(), m_databases.count(), m_databases.count());
    d.m_name = QFileInfo(s).fileName();
    m_databases.push_back(d);
    endInsertRows();
}

int DatabaseListModel::getLastIndex(const QString& s) const
{
    QListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;

    if(i.findNext(d))
    {
        const DatabaseListEntry& e = i.previous();
        return e.m_lastGameIndex;
    }

    return 0;
}

void DatabaseListModel::addFileOpen(const QString& s, bool utf8)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;

    if(i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        e.m_utf8 = utf8;
        if(e.m_state != EDBL_OPEN)
        {
            e.m_state = EDBL_OPEN;
            QModelIndex m = createIndex(m_databases.indexOf(e), DBLV_OPEN, (void*) 0);
            emit QAbstractItemModel::dataChanged(m, m);
            m = createIndex(m_databases.indexOf(e), DBLV_UTF8, (void*) 0);
            emit QAbstractItemModel::dataChanged(m, m);
        }
        return;
    }

    d.m_utf8 = utf8;
    d.m_state = EDBL_OPEN;
    addEntry(d, s);
}

void DatabaseListModel::addFavoriteFile(const QString& s, bool bFavorite, int index)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    DatabaseListEntry d;
    d.m_path = s;
    if(i.findNext(d))
    {
        DatabaseListEntry& e = i.previous();
        if(e.m_isFavorite != bFavorite)
        {
            e.m_isFavorite = bFavorite;
            e.m_lastGameIndex = index;
            QModelIndex m = createIndex(m_databases.indexOf(e), DBLV_FAVORITE, (void*)  0);
            emit QAbstractItemModel::dataChanged(m, m);
            emit OnSelectIndex(m);
        }
        return;
    }

    d.m_isFavorite = bFavorite;
    d.m_lastGameIndex = index;
    addEntry(d, s);
    QModelIndex m = createIndex(rowCount()-1, DBLV_FAVORITE, (void*)  0);
    emit OnSelectIndex(m);
}

void DatabaseListModel::setFileClose(const QString& s, int lastIndex)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        if(e->m_state == EDBL_OPEN)
        {
            e->m_state = EDBL_CLOSE;
            e->m_lastGameIndex = lastIndex;
            QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_OPEN, (void*) 0);
            emit QAbstractItemModel::dataChanged(m, m);
        }
    }
}

void DatabaseListModel::setFileUtf8(const QString& s, bool utf8)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        if(e->m_utf8 != utf8)
        {
            e->m_utf8 = utf8;
            QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_UTF8, (void*) 0);
            emit QAbstractItemModel::dataChanged(m, m);
        }
    }
}

void DatabaseListModel::setFileCurrent(const QString& s)
{
    for(int i = 0; i < m_databases.count(); ++i)
    {
        if(m_databases[i].m_isCurrent)
        {
            m_databases[i].m_isCurrent = false;
            QModelIndex m = createIndex(i, DBLV_NAME, (void*) 0);
            QModelIndex n = createIndex(i, DBLV_UTF8, (void*) 0);
            emit QAbstractItemModel::dataChanged(m, n);
        }
    }

    if(DatabaseListEntry* e = FindEntry(s))
    {
        e->m_isCurrent = true;
        int index = m_databases.indexOf(*e);
        QModelIndex m = createIndex(index, DBLV_NAME, (void*) 0);
        QModelIndex n = createIndex(index, DBLV_UTF8, (void*) 0);
        emit QAbstractItemModel::dataChanged(m, n);
        emit OnSelectIndex(createIndex(index, DBLV_FAVORITE, (void*) 0));
    }
}

void DatabaseListModel::update(const QString& s)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_NAME, (void*) 0);
        QModelIndex n = createIndex(m_databases.indexOf(*e), DBLV_UTF8, (void*) 0);
        emit QAbstractItemModel::dataChanged(m, n);
    }
}

void DatabaseListModel::toStringList(QStringList& list)
{
    for(int i = 1; i < m_databases.count(); ++i)
    {
        if(m_databases[i].m_isFavorite)
        {
            list.append(m_databases[i].m_path);
        }
    }
}

void DatabaseListModel::toAttrStringList(QStringList& list) const
{
    for(int i = 1; i < m_databases.count(); ++i)
    {
        if(m_databases[i].m_isFavorite)
        {
            if(m_databases[i].m_utf8)
            {
                list.append("utf8");
            }
            else
            {
                list.append("ansi");
            }
        }
    }
}

void DatabaseListModel::toIndexList(QList<QVariant>& list) const
{
    for(int i = 1; i < m_databases.count(); ++i)
    {
        if(m_databases[i].m_isFavorite)
        {
            list.append(QVariant(m_databases[i].m_lastGameIndex));
        }
    }
}
