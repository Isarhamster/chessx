/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DATABASELISTMODEL_H
#define DATABASELISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QList>

enum DatabaseListEntryState
{
    EDBL_OPEN,     ///< Database is open
    EDBL_CLOSE     ///< Database is closed
};

class DatabaseListEntry
{
public:
    DatabaseListEntry()
    {
        m_isFavorite    = false;
        m_isCurrent     = false;
        m_utf8          = false;
        m_state         = EDBL_CLOSE;
        m_lastGameIndex = 0;
    }

    QString m_name;
    QString m_path;
    bool    m_isFavorite;
    bool    m_isCurrent;
    bool    m_utf8;
    int     m_lastGameIndex;
    DatabaseListEntryState m_state;
};

inline bool operator==(DatabaseListEntry const& lhs, DatabaseListEntry const& rhs)
{
    return (lhs.m_path == rhs.m_path);
}

inline bool operator != (DatabaseListEntry const& lhs, DatabaseListEntry const& rhs)
{
    return !(lhs == rhs);
}
enum DblvColumns
{
    DBLV_FAVORITE,
    DBLV_NAME,
    DBLV_SIZE,
    DBLV_OPEN,
    DBLV_PATH,
    DBLV_UTF8,
    DBLV_DATE
};

class DatabaseListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DatabaseListModel(QObject *parent = 0);

    int getLastIndex(const QString& s) const;

signals:
    void OnSelectIndex(const QModelIndex&);
    void NoFileFavorite();

public slots:
    void addFileOpen(const QString& s, bool utf8);
    void addFavoriteFile(const QString& s, bool bFavorite, int index);
    void setFileUtf8(const QString&, bool);
    void setFileClose(const QString& s, int lastIndex);
    void setFileCurrent(const QString& s);
    void update(const QString& s);

public:
    void toStringList(QStringList&);
    void toAttrStringList(QStringList&) const;
    void toIndexList(QList<QVariant>& list) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

public:
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
protected:
    void checkFileFavorite();

    DatabaseListEntry* FindEntry(QString s);
    QStringList m_columnNames;
    QList<DatabaseListEntry> m_databases;

protected:
    void addEntry(DatabaseListEntry& d, const QString& s);

};

#endif // DATABASELISTMODEL_H
