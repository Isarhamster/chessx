/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QtCore>
#include "engineoptiondialog.h"
#include "ui_engineoptiondialog.h"
#include "engine.h"
#include "settings.h"

EngineOptionDialog::EngineOptionDialog(QWidget *parent, int index) :
    QDialog(parent),
    ui(new Ui::EngineOptionDialog)
{
    ui->setupUi(this);
    m_engine = Engine::newEngine(index,true);
    m_engine->activate();
    m_index = index;

    connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
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
    AppSettings->beginGroup("/Engines/");
    QString key(QString::number(m_index));
    AppSettings->setMap(key + "/OptionValues", m_engine->m_mapOptionValues);
    AppSettings->endGroup();

    QDialog::accept();
}

void EngineOptionDialog::engineActivated()
{
    ui->tableView->setDB(m_engine->m_options, m_engine->m_mapOptionValues);
}






