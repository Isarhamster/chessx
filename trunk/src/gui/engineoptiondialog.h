/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ENGINEOPTIONDIALOG_H
#define ENGINEOPTIONDIALOG_H

#include <QDialog>

#include "engineoptiondata.h"
#include "enginelist.h"

class Engine;

namespace Ui
{
class EngineOptionDialog;
}

class EngineOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EngineOptionDialog(QWidget *parent, EngineList& engineList, int index);
    ~EngineOptionDialog();
    OptionValueList GetResults() const;

public slots:
    void accept();
    void reject();

protected slots:
    void engineActivated();
    void optionReset();
    void restoreLayout();

private:
    Ui::EngineOptionDialog *ui;
    Engine* m_engine;
    int m_index;
};


#endif // ENGINEOPTIONDIALOG_H
