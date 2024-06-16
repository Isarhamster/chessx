#include "tournamentselectiondialog.h"
#include "QtWidgets/qlistwidget.h"
#include "ui_tournamentselectiondialog.h"
#include "lichesstransfer.h"
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QDesktopServices>

TournamentSelectionDialog::TournamentSelectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TournamentSelectionDialog)
{
    ui->setupUi(this);
}

TournamentSelectionDialog::~TournamentSelectionDialog()
{
    delete ui;
}

void TournamentSelectionDialog::fill()
{
    QByteArray reply = LichessTransfer::queryTournaments(LichessTransfer::LichessTournamentTypeBroadcast);
    QList<QByteArray> l = reply.split('\n');
    foreach (QByteArray b, l)
    {
        if (!b.isEmpty())
        {
            QJsonDocument doc = QJsonDocument::fromJson(b);
            QJsonObject it = doc.object();
            QJsonObject tour = it.value("tour").toObject();

            QString description = tour.value("description").toString();
            QString name = tour.value("name").toString();
            QString id = tour.value("id").toString();
            QString url = tour.value("url").toString();

            QJsonArray rounds = it.value("rounds").toArray();
            QDateTime now = QDateTime::currentDateTime();
            qint64 startsAt = now.toMSecsSinceEpoch() + 1;
            foreach(QJsonValue round, rounds)
            {
                QString v = round.toObject().value("startsAt").toString();
                qint64 vv = v.toLongLong();
                startsAt = std::min(vv, startsAt);
            }
            QTableWidget* w = ui->tournements;
            if ((startsAt <= now.toMSecsSinceEpoch()) && !name.isEmpty() && !id.isEmpty())
            {
                int n = w->rowCount();
                w->insertRow(n);
                w->setItem(n,0,new QTableWidgetItem(name));
                w->setItem(n,1,new QTableWidgetItem(description));
                w->setItem(n,2,new QTableWidgetItem(url));
                w->item(n,0)->setData(Qt::UserRole, id);
                w->item(n,1)->setData(Qt::UserRole, id);
            }
        }
    }
}

QList<QPair<QString, QString> > TournamentSelectionDialog::getTournaments() const
{
    return tournaments;
}

void TournamentSelectionDialog::closeEvent(QCloseEvent *e)
{
    if (e->isAccepted() && tournaments.isEmpty())
    {
        QTableWidget* w = ui->tournements;
        int n = w->rowCount();
        for (int i=0; i<n; i++)
        {
            QTableWidgetItem* item = w->item(i,0);
            if (item->isSelected())
            {
                QString id = item->data(Qt::UserRole).toString();
                QString name = item->text();
                tournaments<< QPair<QString,QString>(id,name);
            }
        }
    }
    QDialog::closeEvent(e);
}

void TournamentSelectionDialog::run()
{
    fill();
    exec();
}

void TournamentSelectionDialog::accept()
{
    if (tournaments.isEmpty())
    {
        QTableWidget* w = ui->tournements;
        int n = w->rowCount();
        for (int i=0; i<n; i++)
        {
            QTableWidgetItem* item = w->item(i,0);
            if (item->isSelected())
            {
                QString id = item->data(Qt::UserRole).toString();
                QString name = item->text();
                tournaments<< QPair<QString,QString>(id,name);
            }
        }
    }
    QDialog::accept();
}

void TournamentSelectionDialog::on_tournements_itemDoubleClicked(QTableWidgetItem *item)
{
    if (item)
    {
        if (item->column()==2)
        {
            QDesktopServices::openUrl(item->text());
        }
        else
        {
            QString id = item->data(Qt::UserRole).toString();
            QString name = item->text();
            tournaments<< QPair<QString,QString>(id,name);
            accept();
        }
    }
}

