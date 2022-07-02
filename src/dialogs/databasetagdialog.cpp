#include "databasetagdialog.h"
#include "ui_databasetagdialog.h"

DatabaseTagDialog::DatabaseTagDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseTagDialog)
{
    ui->setupUi(this);
}

DatabaseTagDialog::~DatabaseTagDialog()
{
    delete ui;
}

void DatabaseTagDialog::setTagNames(QStringList tags)
{
    ui->cbTags->addItems(tags);
}

QString DatabaseTagDialog::currentTag() const
{
    return ui->cbTags->currentText();
}

QString DatabaseTagDialog::replacement() const
{
    return ui->replacement->text();
}

QString DatabaseTagDialog::current() const
{
    return ui->currentText->text();
}
