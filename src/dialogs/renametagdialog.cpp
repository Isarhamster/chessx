/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "renametagdialog.h"
#include "settings.h"

#include "ui_renametagdialog.h"

#include <QPushButton>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

RenameTagDialog::RenameTagDialog(QWidget *parent, QString ts, QString tagName, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::RenameTagDialog),
    m_ts(ts),
    m_tagName(tagName)
{
    ui->setupUi(this);

    ui->editFrom->setText(m_ts);
    ui->editTo->setText(m_ts);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("Rename"));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    AppSettings->layout(this);
}

RenameTagDialog::~RenameTagDialog()
{
    delete ui;
}

void RenameTagDialog::slotRename()
{
    emit renameRequest(m_tagName, ui->editTo->text(), m_ts);
    AppSettings->setLayout(this);
    close();
}

void RenameTagDialog::accept()
{
    AppSettings->setLayout(this);
    QDialog::accept();
}

void RenameTagDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}

void RenameTagDialog::buttonClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton sb = ui->buttonBox->standardButton(button);
    switch(sb)
    {
    case QDialogButtonBox::Close:
        accept();
        break;
    case QDialogButtonBox::Apply:
        slotRename();
        break;
    default:
        break;
    }
}
