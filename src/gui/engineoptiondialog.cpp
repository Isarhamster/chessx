/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "engineoptiondialog.h"
#include "ui_engineoptiondialog.h"
#include "enginex.h"
#include "settings.h"

#include <QtCore>
#include <QPushButton>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

EngineOptionDialog::EngineOptionDialog(QWidget *parent,
                                       EngineList& engineList,
                                       int index) :
    QDialog(parent),
    ui(new Ui::EngineOptionDialog)
{
    ui->setupUi(this);
    setObjectName("EngineOptionDialog");

    QString t = windowTitle();
    QString t1 = QString("%1 %2 (%3)").
                 arg(t,
                     engineList[index].name,
                     QString(engineList[index].protocol == EngineData::UCI ? "UCI" : "WinBoard"));

    setWindowTitle(t1);
    restoreLayout();
    if(engineList[index].protocol == EngineData::UCI)
    {
        ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled(false);
    }
    m_index = index;
    m_engine = EngineX::newEngine(engineList, index, true);
    assert(m_engine != 0);

    connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));

    m_engine->activate();
}

void EngineOptionDialog::restoreLayout()
{
    AppSettings->layout(this);
}

EngineOptionDialog::~EngineOptionDialog()
{
    assert(m_engine != 0);
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
    assert(m_engine != 0);
    ui->tableView->setDB(m_engine->m_options, m_engine->m_mapOptionValues);
}

OptionValueMap EngineOptionDialog::GetResults() const
{
    return (m_engine ? m_engine->m_mapOptionValues :
            QMap<QString, QVariant>());
}

void EngineOptionDialog::optionReset()
{
    assert(m_engine != 0);
    m_engine->m_mapOptionValues.clear();
    ui->tableView->resetModel();
}

void EngineOptionDialog::buttonClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton sb = ui->buttonBox->standardButton(button);
    switch(sb)
    {
    case QDialogButtonBox::Ok:
        accept();
        break;
    case QDialogButtonBox::Cancel:
        reject();
        break;
    case QDialogButtonBox::Reset:
        optionReset();
        break;
    default:
        break;
    }
}
