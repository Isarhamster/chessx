#include "onlinebase.h"
#include "ui_onlinebase.h"

OnlineBase::OnlineBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OnlineBase)
{
    ui->setupUi(this);
}

OnlineBase::~OnlineBase()
{
    delete ui;
}

QDate OnlineBase::getStartDate() const
{
    return ui->startDate->date();
}

void OnlineBase::setStartDate(const QDate &value)
{
    ui->startDate->setDate(value);
}

QString OnlineBase::getHandle() const
{
    return ui->userName->text();
}

void OnlineBase::setHandle(const QString &value)
{
    ui->userName->setText(value);
}
