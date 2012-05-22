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

struct DatabaseListEntry
{
    QString m_name;
    QString m_path;
    DatabaseListEntryState m_state;
};

class DatabaseListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DatabaseListModel(QObject *parent = 0);
    void addRecentFile(const QString& s);

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

};

#endif // DATABASELISTMODEL_H
