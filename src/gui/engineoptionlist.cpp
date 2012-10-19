/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/
#include "engineoptionlist.h"

//////////////////////////////////////////////////////////////////////////////////////
// List Delegate
//////////////////////////////////////////////////////////////////////////////////////

QWidget *EngineOptionListDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{
    if(index.column() == 4)
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    return 0;
}

void EngineOptionListDelegate::commitAndCloseEditor()
{
    emit commitData((QWidget*)sender());
    emit closeEditor((QWidget*)sender());
}

void EngineOptionListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void EngineOptionListDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

//////////////////////////////////////////////////////////////////////////////////////
// List Widget
//////////////////////////////////////////////////////////////////////////////////////

EngineOptionList::EngineOptionList(QWidget *parent) :
    QTableView(parent)
{
    setObjectName("EngineOptionList");
    setWindowTitle(tr("Engine Options"));
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setSortingEnabled(false);
}

void EngineOptionList::setDB(const QList<EngineOptionData>& options,
           QMap<QString, QString>& mapOptionValues)
{
    m_model = new EngineOptionModel(this);
    m_model->m_pOptionDataList = &options;
    m_model->m_pValueMap = &mapOptionValues;

    setModel(m_model);
    setItemDelegate(new EngineOptionListDelegate());
}


