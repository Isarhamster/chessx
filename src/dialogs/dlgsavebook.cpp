/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/
#include "ui_dlgsavebook.h"

#include "dlgsavebook.h"
#include "settings.h"

#include <QFileInfo>
#include <QFileDialog>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

DlgSaveBook::DlgSaveBook(QString path, QWidget *parent) :
  QDialog(parent),
  m_InputPath(path),
  ui(new Ui::DlgSaveBook)
{
    ui->setupUi(this);
    restoreLayout();

    if (!path.isEmpty())
    {
        QFileInfo fi(m_InputPath);
        m_OutputPath = fi.absoluteDir().absolutePath() + QDir::separator() + fi.baseName()+".bin";
        ui->inputFile->setText(fi.fileName());
    }
    else
    {
        ui->inputFile->setText(tr("Clipboard"));
        QString dir = AppSettings->value("/General/DefaultDataPath").toString();
        m_OutputPath = dir + QDir::separator() + "Clipboard.bin";
    }

    ui->outputPath->setText(m_OutputPath);

    connect(ui->btBrowseTarget, SIGNAL(clicked(bool)), SLOT(slotSelectTargetPath()));
}

void DlgSaveBook::restoreLayout()
{
    AppSettings->layout(this);
}

DlgSaveBook::~DlgSaveBook()
{
    delete ui;
}

void DlgSaveBook::getBookParameters(QString &out, int &maxPly, int &minGame, bool &uniform, int& result, int& filterResults)
{
    out = ui->outputPath->text();
    maxPly = ui->maxPly->value();
    minGame = ui->minGame->value();
    uniform = ui->cbUniform->isChecked();
    switch (ui->cbSide->currentIndex())
    {
    case 0: result = 0; break;
    case 1: result = +1; break;
    case 2: result = -1; break;
    }
    switch (ui->cbFilterSide->currentIndex())
    {
    case 0: filterResults = 0; break;
    case 1: filterResults = +1; break;
    case 2: filterResults = -1; break;
    }
}

void DlgSaveBook::accept()
{
    AppSettings->setLayout(this);
    QDialog::accept();
}

void DlgSaveBook::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}

void DlgSaveBook::slotSelectTargetPath()
{
    QString file = QFileDialog::getSaveFileName(this, tr("New book"),
                   AppSettings->value("/General/DefaultDataPath").toString(),
                   tr("Polyglot Book (*.bin)"));
    if(file.isEmpty())
    {
        return;
    }
    if(!file.endsWith(".bin"))
    {
        file += ".bin";
    }
    ui->outputPath->setText(file);
}
