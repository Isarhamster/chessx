/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "aboutdlg.h"
#include "settings.h"
#include "version.h"

#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    QString refPath = AppSettings->dataPath();
    ui->labelDataPath->setText(refPath);
    refPath = AppSettings->getTempPath();
    ui->labelTempPath->setText(refPath);
    ui->labelCopyRightDate->setText(COPYRIGHT_DATE);
    QString version = QString(STR_REVISION).replace(',', '.');
    ui->labelVersion->setText(version);

    ui->labelTempPath->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    ui->labelDataPath->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    ui->labelHomepage->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelMailingList->setTextInteractionFlags(Qt::TextBrowserInteraction);
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
