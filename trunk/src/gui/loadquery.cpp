/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "loadquery.h"
#include "ui_loadquery.h"

#include <QPushButton>

LoadQuery::LoadQuery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadQuery)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Start Download!"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Skip Loading"));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
}

LoadQuery::~LoadQuery()
{
    delete ui;
}

void LoadQuery::buttonClicked(QAbstractButton* button)
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
    default:
        break;
    }
}
