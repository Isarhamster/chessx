/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ENGINEOPTIONLIST_H
#define ENGINEOPTIONLIST_H

#include "engineoptionmodel.h"
#include "engineoptiondata.h"
#include "tableview.h"

#include <QMap>
#include <QList>

class EngineOptionList : public TableView
{
    Q_OBJECT
public:
    explicit EngineOptionList(QWidget *parent = 0);

    void setDB(const QList<EngineOptionData>& options,
          QMap<QString, QString>& mapOptionValues);

protected:
    EngineOptionModel* m_model;

};

#endif // ENGINEOPTIONLIST_H
