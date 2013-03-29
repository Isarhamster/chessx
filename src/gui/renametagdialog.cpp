#include "renametagdialog.h"
#include "ui_renametagdialog.h"

RenameTagDialog::RenameTagDialog(QWidget *parent, QString ts, QString tagName, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::RenameTagDialog),
    m_ts(ts),
    m_tagName(tagName)
{
    ui->setupUi(this);

    ui->editFrom->setText(m_ts);
    ui->editTo->setText(QString());

    connect(ui->btClose, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->btRename, SIGNAL(clicked()), SLOT(slotRename()));
}

RenameTagDialog::~RenameTagDialog()
{
    delete ui;
}

void RenameTagDialog::slotRename()
{
    emit renameRequest(m_tagName, ui->editTo->text(), m_ts);
    close();
}
