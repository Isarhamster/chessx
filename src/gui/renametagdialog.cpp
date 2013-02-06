#include "renametagdialog.h"
#include "ui_renametagdialog.h"

RenameTagDialog::RenameTagDialog(QWidget *parent, QString ts, QString tagName) :
    QDialog(parent),
    ui(new Ui::RenameTagDialog),
    m_ts(ts),
    m_tagName(tagName)
{
    ui->setupUi(this);

    ui->labelFrom->setText(m_ts);

    connect(ui->btClose, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->btRename, SIGNAL(clicked()), SLOT(slotRename()));
}

RenameTagDialog::~RenameTagDialog()
{
    delete ui;
}

void RenameTagDialog::slotRename()
{
    // todo
}
