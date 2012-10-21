/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ENGINEOPTIONMODEL_H
#define ENGINEOPTIONMODEL_H

#include <QStandardItemModel>
#include <QStringList>

#include "engineoptiondata.h"

class EngineOptionModel : public QStandardItemModel
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
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                    int role = Qt::DisplayRole) const;

    OptionValueList* m_pValueMap;
    const QList<EngineOptionData>* m_pOptionDataList;
    void resetModel();
    QStringList getSelections(const QModelIndex& index);

protected:
    QStringList m_columnNames;
};

#endif // ENGINEOPTIONMODEL_H
