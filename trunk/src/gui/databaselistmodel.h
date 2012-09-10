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
        m_isFavorite = false;
        m_isCurrent  = false;
        m_state      = EDBL_CLOSE;
    }

    QString m_name;
    QString m_path;
    bool    m_isFavorite;
    bool    m_isCurrent;
    DatabaseListEntryState m_state;
};

inline bool operator==(DatabaseListEntry const& lhs, DatabaseListEntry const& rhs)
{
    return (lhs.m_path == rhs.m_path);
}

inline bool operator != (DatabaseListEntry const& lhs, DatabaseListEntry const& rhs)
{
  return ! (lhs==rhs);
}
enum DblvColumns
{
    DBLV_FAVORITE,
    DBLV_NAME,
    DBLV_SIZE,
    DBLV_OPEN,
    DBLV_PATH
};

class DatabaseListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DatabaseListModel(QObject *parent = 0);

public slots:
    void addFileOpen(const QString& s);
    void addFavoriteFile(const QString& s, bool bFavorite);
    void setFileClose(const QString& s);
    void setFileCurrent(const QString& s);
    void update(const QString& s);

public:
    void toStringList(QStringList&);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                    int role = Qt::DisplayRole) const;
protected:
    QStringList m_columnNames;
    QList<DatabaseListEntry> m_databases;

protected:
    void addEntry(DatabaseListEntry& d, const QString& s);

};

#endif // DATABASELISTMODEL_H
