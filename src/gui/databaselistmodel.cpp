/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "databaselistmodel.h"
#include <QDateTime>
#include <QFileInfo>
#include <QFont>
#include <QPixmap>

#include "databaseinfo.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

static QString formatFileSize(qint64 size)
{
    static QStringList suffixes;
    if (suffixes.empty())
    {
        suffixes << "" << "k" << "M" << "G" << "T" << "P";
    }
    auto s = suffixes.cbegin();
    for (; s + 1 != suffixes.cend() && size >= 1024; ++s)
    {
        size /= 1024;
    }
    return QString("%1%2").arg(size).arg(*s);
}

DatabaseRegistry::~DatabaseRegistry()
{
    qDeleteAll(m_databases.begin(), m_databases.end());
    m_databases.clear();
}

DatabaseInfo* DatabaseRegistry::findDisplayName(QString path) const
{
    for (auto dbi: m_databases)
    {
        if (dbi->displayName() == path)
        {
            return dbi;
        }
    }
    return nullptr;
}

void DatabaseRegistry::append(DatabaseInfo* dbi)
{
    m_databases.append(dbi);
}

void DatabaseRegistry::remove(DatabaseInfo* dbi)
{
    m_databases.removeOne(dbi);
}

DatabaseListModel::DatabaseListModel(DatabaseRegistry* registry, QObject* parent)
    : QAbstractItemModel(parent)
    , m_registry(registry)
{
    m_columnNames << tr("Favorite") << tr("Name") << tr("Size") << tr("Open") << tr("Path") << tr("Format") << tr("Date") << tr("Read");
}

QModelIndex DatabaseListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return QModelIndex();
    }
    return createIndex(row, column, (void*)nullptr);
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
    if (!index.isValid() || index.row() >= m_databases.size())
    {
        return QVariant();
    }

    const auto& db = m_databases.at(index.row());
    if(role == Qt::DecorationRole)
    {
        switch(index.column())
        {
        case DBLV_FAVORITE:
        {
            int stars = db.m_stars;
            switch (stars)
            {
            case 0: return QPixmap(":/images/folder_grey.png");
            case 1: return QPixmap(":/images/folder_favorite1.png");
            case 2: return QPixmap(":/images/folder_favorite2.png");
            case 3: return QPixmap(":/images/folder_favorite3.png");
            case 4: return QPixmap(":/images/startup.png");
            case 5: return QPixmap(":/images/active.png");
            default: return QPixmap();
            }
        }
        case DBLV_OPEN:
        {
            bool bIsOpen = db.m_state == EDBL_OPEN;
            bool bIsCurrent = db.m_isCurrent;
            if(bIsOpen)
            {
                return QPixmap(bIsCurrent ? ":/images/folder_new.png" : ":/images/fileopen.png");
            }
            else
            {
                return QPixmap(":/images/folder_closed.png");
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
            QString s = db.m_name;
            if (s.endsWith(".pgn", Qt::CaseInsensitive)) s.remove(".pgn", Qt::CaseInsensitive);
            return s;
        }
        case DBLV_SIZE:
        {
            return formatFileSize(DatabaseInfo::GetDatabaseSize(db.m_path));
        }
        case DBLV_DATE:
        {
            QFileInfo f(db.m_path);
            return f.lastModified().date().toString(Qt::ISODate);
        }
        case DBLV_DATE_READ:
        {
            QFileInfo f(db.m_path);
            return f.lastRead().date().toString(Qt::ISODate);
        }
        case DBLV_OPEN:
            return QVariant();
        case DBLV_PATH:
            return db.m_path;
        case DBLV_UTF8:
            return db.classType();
        default:
            break;
        }
    }
    else if(role == Qt::FontRole)
    {
        if(db.m_isCurrent)
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
            return QString(db.isFavorite() ? tr("Favorite") : "");
        }
        case DBLV_PATH:
        {
            QString s = db.m_name;
            return s;
        }
        case DBLV_OPEN:
        {
            bool bIsOpen = db.m_state == EDBL_OPEN;
            return QString(bIsOpen ? tr("Open") : tr("Closed"));
        }
        case DBLV_UTF8:
        {
            return db.classType();
        }
        case DBLV_NAME:
        {
            QString s = db.m_name;
            s[0] = s[0].toUpper();
            return s;
        }
        case DBLV_DATE:
        {
            QFileInfo f(db.m_path);
            return f.lastModified();
        }
        case DBLV_DATE_READ:
        {
            QFileInfo f(db.m_path);
            return f.lastRead();
        }
        case DBLV_SIZE:
        {
            return formatFileSize(DatabaseInfo::GetDatabaseSize(db.m_path));
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
            int stars = db.m_stars;
            return QString::number(stars);
        }
        case DBLV_PATH:
        {
            QString s = db.m_name;
            return s;
        }
        case DBLV_OPEN:
        {
            bool bIsOpen = db.m_state == EDBL_OPEN;
            return QString(bIsOpen ? "Open" : "Closed");
        }
        case DBLV_UTF8:
        {
            return db.classType();
        }
        case DBLV_NAME:
        {
            QString s = db.m_name;
            s[0] = s[0].toUpper();
            return s;
        }
        case DBLV_DATE:
        {
            QFileInfo f(db.m_path);
            return f.lastModified();
        }
        case DBLV_DATE_READ:
        {
            QFileInfo f(db.m_path);
            return f.lastRead();
        }
        case DBLV_SIZE:
        {
            return DatabaseInfo::GetDatabaseSize(db.m_path);
        }
        default:
            break;
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
    return nullptr;
}

const DatabaseListEntry* DatabaseListModel::FindEntry(QString s) const
{
    QList<DatabaseListEntry>::ConstIterator it = m_databases.constBegin();
    for ( ; it != m_databases.constEnd(); ++it )
    {
        const DatabaseListEntry& i = *it;
        if (i.m_path == s) return &i;
    }
    return nullptr;
}

void DatabaseListModel::addEntry(DatabaseListEntry& d, const QString& s)
{
    beginInsertRows(QModelIndex(), m_databases.count(), m_databases.count());
    d.m_name = QFileInfo(s).fileName();
    if (d.m_name.isEmpty()) d.m_name = s; // Fix virtual database names
    m_databases.push_back(d);
    endInsertRows();
}

int DatabaseListModel::getLastIndex(const QString& s) const
{
    const DatabaseListEntry* e = FindEntry(s);
    return e ? e->m_lastGameIndex : 0;
}

void DatabaseListModel::limitStars(int limit)
{
    QMutableListIterator<DatabaseListEntry> i(m_databases);
    while (i.hasNext())
    {
        DatabaseListEntry& d = i.next();
        if (d.m_stars > limit)
        {
            d.m_stars = limit;
            QModelIndex m = createIndex(m_databases.indexOf(d), DBLV_FAVORITE, (void*) nullptr);
            emit QAbstractItemModel::dataChanged(m, m);
        }
    }
}

int DatabaseListModel::stars(const QString &s) const
{
    const DatabaseListEntry* e = FindEntry(s);
    return e ? e->m_stars : 0;
}

bool DatabaseListModel::exists(const QString &s) const
{
    const DatabaseListEntry* e = FindEntry(s);
    return !!e ? e->m_stars : 0;
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
            QModelIndex m = createIndex(m_databases.indexOf(e), DBLV_OPEN, (void*) nullptr);
            emit QAbstractItemModel::dataChanged(m, m);
            m = createIndex(m_databases.indexOf(e), DBLV_UTF8, (void*) nullptr);
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
        if(e.isFavorite() != bFavorite)
        {
            e.setIsFavorite(bFavorite);
            e.m_lastGameIndex = index;
            QModelIndex m = createIndex(m_databases.indexOf(e), DBLV_FAVORITE, (void*)  nullptr);
            emit QAbstractItemModel::dataChanged(m, m);
        }
        return;
    }

    d.setIsFavorite(bFavorite);
    d.m_lastGameIndex = index;
    addEntry(d, s);
    QModelIndex m = createIndex(rowCount()-1, DBLV_FAVORITE, (void*)  nullptr);
    emit OnSelectIndex(m);
}

void DatabaseListModel::setStars(const QString &s, int stars)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        if(e->m_stars != stars)
        {
            e->m_stars = stars;
            QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_FAVORITE, (void*) nullptr);
            emit QAbstractItemModel::dataChanged(m, m);
        }
    }
}

void DatabaseListModel::setFileClose(const QString& s, int lastIndex)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        if(e->m_state == EDBL_OPEN)
        {
            e->m_state = EDBL_CLOSE;
            e->m_lastGameIndex = lastIndex;
            QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_OPEN, (void*) nullptr);
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
            QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_UTF8, (void*) nullptr);
            emit QAbstractItemModel::dataChanged(m, m);
        }
    }
}

bool DatabaseListModel::fileUtf8(const QString& s)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        return e->m_utf8;
    }
    return false;
}

void DatabaseListModel::setFileCurrent(const QString& s)
{
    for(int i = 0; i < m_databases.count(); ++i)
    {
        if(m_databases[i].m_isCurrent)
        {
            m_databases[i].m_isCurrent = false;
            QModelIndex m = createIndex(i, DBLV_NAME, (void*) nullptr);
            QModelIndex n = createIndex(i, DBLV_UTF8, (void*) nullptr);
            emit QAbstractItemModel::dataChanged(m, n);
        }
    }

    if(DatabaseListEntry* e = FindEntry(s))
    {
        e->m_isCurrent = true;
        int index = m_databases.indexOf(*e);
        QModelIndex m = createIndex(index, DBLV_NAME, (void*) nullptr);
        QModelIndex n = createIndex(index, DBLV_UTF8, (void*) nullptr);
        emit QAbstractItemModel::dataChanged(m, n);
        emit OnSelectIndex(createIndex(index, DBLV_FAVORITE, (void*) nullptr));
    }
}

void DatabaseListModel::update(const QString& s)
{
    if(DatabaseListEntry* e = FindEntry(s))
    {
        QModelIndex m = createIndex(m_databases.indexOf(*e), DBLV_FIRST, (void*) nullptr);
        QModelIndex n = createIndex(m_databases.indexOf(*e), DBLV_LAST, (void*) nullptr);
        emit QAbstractItemModel::dataChanged(m, n);
    }
}

void DatabaseListModel::toStringList(QStringList& list)
{
    for(int i = 1; i < m_databases.count(); ++i)
    {
        if(m_databases[i].isFavorite())
        {
            list.append(m_databases[i].m_path);
        }
    }
}

void DatabaseListModel::toAttrStringList(QStringList& list) const
{
    for(int i = 1; i < m_databases.count(); ++i)
    {
        if(m_databases[i].isFavorite())
        {
            QString s;
            s = (m_databases[i].m_utf8) ? "utf8" : "ansi";
            s.append("+stars");
            s.append(QString::number(m_databases[i].m_stars));
            list.append(s);
        }
    }
}

void DatabaseListModel::toIndexList(QList<QVariant>& list) const
{
    for(int i = 1; i < m_databases.count(); ++i)
    {
        if(m_databases[i].isFavorite())
        {
            list.append(QVariant(m_databases[i].m_lastGameIndex));
        }
    }
}

//////////////////////////////////////////////////////
/// \brief DatabaseListEntry::setIsFavorite
/// \param isFavorite

void DatabaseListEntry::setIsFavorite(bool isFavorite)
{
    m_stars = isFavorite ? 1 : 0;
}

bool DatabaseListEntry::isFavorite() const
{
    return (m_stars > 0);
}
