/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QtCore>
#include "engineoptiondialog.h"
#include "ui_engineoptiondialog.h"
#include "engine.h"
#include "settings.h"

EngineOptionDialog::EngineOptionDialog(QWidget *parent,
                                       EngineList& engineList,
                                       int index) :
    QDialog(parent),
    ui(new Ui::EngineOptionDialog)
{
    ui->setupUi(this);

    QString t = windowTitle();
    QString t1 = QString("%1 %2 (%3)").
                 arg(t).
                 arg(engineList[index].name).
                 arg(QString(engineList[index].protocol == EngineData::UCI ? "UCI" : "WinBoard"));

    setWindowTitle(t1);
    restoreLayout();
    if(engineList[index].protocol == EngineData::UCI)
    {
        ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    else
    {
        ui->resetButton->setEnabled(false);
    }
    m_index = index;
    m_engine = Engine::newEngine(engineList, index, true);
    m_engine->activate();

    connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(optionReset()));
}

void EngineOptionDialog::restoreLayout()
{
    AppSettings->layout(this);
}

EngineOptionDialog::~EngineOptionDialog()
{
    m_engine->deactivate();
    disconnect();
    delete m_engine;
    delete ui;
}

void EngineOptionDialog::accept()
{
    ui->tableView->saveConfig();
    AppSettings->setLayout(this);
    QDialog::accept();
}

void EngineOptionDialog::reject()
{
    ui->tableView->saveConfig();
    AppSettings->setLayout(this);
    QDialog::reject();
}

void EngineOptionDialog::engineActivated()
{
    ui->tableView->setDB(m_engine->m_options, m_engine->m_mapOptionValues);
}

OptionValueList EngineOptionDialog::GetResults() const
{
    return (m_engine ? m_engine->m_mapOptionValues :
            QMap<QString, QVariant>());
}

void EngineOptionDialog::optionReset()
{
    m_engine->m_mapOptionValues.clear();
    ui->tableView->resetModel();
}


