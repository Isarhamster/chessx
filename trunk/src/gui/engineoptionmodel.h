#ifndef ENGINEOPTIONMODEL_H
#define ENGINEOPTIONMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include <QMap>

class EngineOptionData;

class EngineOptionModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EngineOptionModel(QObject *parent = 0);

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

    QMap<QString, QString>* m_pValueMap;
    const QList<EngineOptionData>* m_pOptionDataList;

protected:
    QStringList m_columnNames;
};

#endif // ENGINEOPTIONMODEL_H
