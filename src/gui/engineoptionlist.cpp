/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/
#include "engineoptionlist.h"

#include <QStringList>
#include <QComboBox>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////
// List Delegate
//////////////////////////////////////////////////////////////////////////////////////

QWidget *EngineOptionListDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const

{
    if(index.column() == 4)
    {
        QStringList varList = m_model->getSelections(index);
        if(!varList.size())
        {
            return QStyledItemDelegate::createEditor(parent, option, index);
        }
        else
        {
            QComboBox *comboBox = new QComboBox(parent);
            comboBox->setEditable(false);
            comboBox->setInsertPolicy(QComboBox::NoInsert);
            comboBox->addItems(varList);
            return comboBox;
        }
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
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if(comboBox && m_model->getSelections(index).size())
    {
        int selectedItem = comboBox->findText(comboBox->currentText());
        model->setData(index, comboBox->itemText(selectedItem), Qt::EditRole);
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void EngineOptionListDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if(comboBox && m_model->getSelections(index).size())
    {
        comboBox->clear();
        QStringList varList = m_model->getSelections(index);
        comboBox->addItems(varList);

        QString currentText = index.model()->data(index, Qt::DisplayRole).toString();
        int selectedItem = comboBox->findText(currentText);
        if(selectedItem == -1)
        {
            comboBox->setEditText(index.model()->data(index, Qt::DisplayRole).toString());
        }
        else
        {
            comboBox->setCurrentIndex(selectedItem);
        }
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// List Widget
//////////////////////////////////////////////////////////////////////////////////////

EngineOptionList::EngineOptionList(QWidget *parent) :
    TableView(parent),
    m_model(0)
{
    setObjectName("EngineOptionList");
    setWindowTitle(tr("Engine Options"));
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSortingEnabled(false);
}

void EngineOptionList::setDB(const QList<EngineOptionData>& options,
                             OptionValueList& mapOptionValues)
{
    m_model = new EngineOptionModel(this);
    m_model->m_pOptionDataList = &options;
    m_model->m_pValueMap = &mapOptionValues;

    setModel(m_model);
    EngineOptionListDelegate* delegate = new EngineOptionListDelegate(this);
    delegate->setModel(m_model);
    setItemDelegate(delegate);

    slotReconfigure();
}

void EngineOptionList::resetModel()
{
    m_model->resetModel();
}


