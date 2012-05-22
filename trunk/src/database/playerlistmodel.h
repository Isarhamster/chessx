#ifndef PLAYERLISTMODEL_H
#define PLAYERLISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
class Database;
class PlayerInfo;

class PlayerListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PlayerListModel(QObject *parent = 0);
    void setDatabase(Database* db);
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
private:
    QStringList m_columnNames;
    Database* m_db;
    mutable PlayerInfo* m_pPlayerInfo;
};

#endif // PLAYERLISTMODEL_H
