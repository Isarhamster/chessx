/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ENGINEOPTIONLIST_H
#define ENGINEOPTIONLIST_H

#include "engineoptionmodel.h"
#include "engineoptiondata.h"
#include "tableview.h"

#include <QList>
#include <QStyledItemDelegate>

class EngineOptionListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    EngineOptionListDelegate(QWidget *parent = nullptr) : QStyledItemDelegate(parent), m_model(nullptr) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void setModel(EngineOptionModel* model)
    {
        m_model = model;
    }
private slots:
    void commitAndCloseEditor();
protected:
    EngineOptionModel* m_model;
};

class EngineOptionList : public TableView
{
    Q_OBJECT
public:
    explicit EngineOptionList(QWidget *parent = nullptr);

    void setDB(const QList<EngineOptionData>& options,
               OptionValueMap& mapOptionValues);

    void resetModel();

protected:
    EngineOptionModel* m_model;

};

#endif // ENGINEOPTIONLIST_H
