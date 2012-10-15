#include <QtCore>
#include "engineoptiondialog.h"
#include "ui_engineoptiondialog.h"
#include "engine.h"

EngineOptionDialog::EngineOptionDialog(QWidget *parent, int index) :
    QDialog(parent),
    ui(new Ui::EngineOptionDialog)
{
    ui->setupUi(this);
    m_engine = Engine::newEngine(index);
    m_engine->activate();

    connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
}

EngineOptionDialog::~EngineOptionDialog()
{
    disconnect();
    delete m_engine;
    delete ui;
}

void EngineOptionDialog::engineActivated()
{
    ui->tableView->setDB(m_engine->m_options, m_engine->m_mapOptionValues);
}






