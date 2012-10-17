/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "engineoptionlist.h"

EngineOptionList::EngineOptionList(QWidget *parent) :
    TableView(parent)
{
    setObjectName("EngineOptionList");
    setWindowTitle(tr("Engine Options"));

    setSortingEnabled(false);
}

void EngineOptionList::setDB(const QList<EngineOptionData>& options,
           QMap<QString, QString>& mapOptionValues)
{
    m_model = new EngineOptionModel(this);
    m_model->m_pOptionDataList = &options;
    m_model->m_pValueMap = &mapOptionValues;

    setModel(m_model);
}

