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
    EngineOptionListDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent), m_model(0) {}

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

class EngineOptionList : public QTableView
{
    Q_OBJECT
public:
    explicit EngineOptionList(QWidget *parent = 0);

    void setDB(const QList<EngineOptionData>& options,
               OptionValueList& mapOptionValues);

    void resetModel();

protected:
    EngineOptionModel* m_model;

};

#endif // ENGINEOPTIONLIST_H
