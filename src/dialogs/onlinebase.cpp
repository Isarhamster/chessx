#include "onlinebase.h"
#include "ui_onlinebase.h"

OnlineBase::OnlineBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OnlineBase)
{
    ui->setupUi(this);
    ui->tournament->hide();
    ui->lb_Tournament->hide();
}

OnlineBase::~OnlineBase()
{
    delete ui;
}

QDate OnlineBase::getStartDate() const
{
    return ui->startDate->date();
}

QDate OnlineBase::getEndDate() const
{
    return ui->endDate->date();
}

void OnlineBase::setDateFormat(const QString& format)
{
    ui->startDate->setDisplayFormat(format);
    ui->endDate->setDisplayFormat(format);
}

void OnlineBase::setStartDate(const QDate &value)
{
    ui->startDate->setDate(value);
}

void OnlineBase::setEndDate(const QDate &value)
{
    ui->endDate->setDate(value);
}

QString OnlineBase::getHandle() const
{
    return ui->userName->text();
}

void OnlineBase::setHandle(const QString &value)
{
    ui->userName->setText(value);
}

const QString OnlineBase::getTournament() const
{
    return ui->tournament->text();
}

void OnlineBase::setTournament(const QString &newTournament)
{
    ui->lb_Tournament->show();
    ui->tournament->show();
    ui->tournament->setText(newTournament);
}

