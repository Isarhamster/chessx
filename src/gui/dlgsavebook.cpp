/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/
#include "ui_dlgsavebook.h"

#include "dlgsavebook.h"
#include "settings.h"

#include <QFileInfo>

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
      QString in = fi.absoluteFilePath();
      m_OutputPath = fi.absoluteDir().absolutePath()+"/"+fi.baseName()+".bin";
      ui->inputFile->setText(fi.fileName());
  }
  else
  {
      ui->inputFile->setText(tr("Clipboard"));
      QString dir = AppSettings->value("/General/DefaultDataPath").toString();
      m_OutputPath = dir + "/Clipboard.bin";
  }

  ui->outputPath->setText(m_OutputPath);
}

void DlgSaveBook::restoreLayout()
{
  AppSettings->layout(this);
}

DlgSaveBook::~DlgSaveBook()
{
    delete ui;
}

void DlgSaveBook::getBookParameters(QString &out, int &maxPly, int &minGame, bool &uniform)
{
    out = ui->outputPath->text();
    maxPly = ui->maxPly->value();
    minGame = ui->minGame->value();
    uniform = ui->cbUniform->isChecked();
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
