#ifndef DATABASELISTMODEL_H
#define DATABASELISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QList>

enum DatabaseListEntryState
{
    EDBL_RECENT,   ///< Database was recently opened but is not open now
    EDBL_OPEN,     ///< Database is open
    EDBL_MODIFIED, ///< Database is open and at least one game needs saving
    EDBL_FAVORITE  ///< Database is a user added favorite (overrides RECENT)
};

class DatabaseListEntry
{
public:
    DatabaseListEntry()
    {
        m_isFavorite = false;
        m_isCurrent  = false;
        m_state      = EDBL_RECENT;
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

class DatabaseListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DatabaseListModel(QObject *parent = 0);

public slots:
    void addRecentFile(const QString& s);
    void addFileOpen(const QString& s);
    void setFileModified(const QString& s, bool modified);
    void addFavoriteFile(const QString& s);
    void setFileClose(const QString& s);
    void setFileCurrent(const QString& s);

public:
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
    QString stateString(DatabaseListEntryState e) const;
    void addEntry(DatabaseListEntry& d, const QString& s);

};

#endif // DATABASELISTMODEL_H
