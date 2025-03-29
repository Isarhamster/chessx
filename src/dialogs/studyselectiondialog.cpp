#include "studyselectiondialog.h"
#include "ui_studyselectiondialog.h"
#include "lichesstransfer.h"
#include "settings.h"
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QDesktopServices>

StudySelectionDialog::StudySelectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudySelectionDialog)
{
    ui->setupUi(this);
}

StudySelectionDialog::~StudySelectionDialog()
{
    delete ui;
}

void StudySelectionDialog::fill()
{
    QString userName = AppSettings->getValue("Lichess/userName").toString();
    QString token = AppSettings->getValue("Lichess/passWord").toString();
    if (userName.isEmpty())
    {
        QTableWidget* w = ui->studies;
        w->insertRow(0);
        w->setItem(0,0,new QTableWidgetItem(tr("Configure lichess account!")));
        w->item(0,0)->setData(Qt::UserRole, "");
        ui->studies->resizeColumnsToContents();
        return;
    }
    QByteArray reply = LichessTransfer::queryStudies(userName, token);
    QList<QByteArray> l = reply.split('\n');
    foreach (QByteArray b, l)
    {
        if (!b.isEmpty())
        {
            QJsonDocument doc = QJsonDocument::fromJson(b);
            QJsonObject it = doc.object();
            QString id = it.value("id").toString();
            QString studyName = it.value("name").toString();

#if QT_VERSION < 0x060000
            qint64 cc = (qint64) it.value("createdAt").toInt();
            qint64 uu = (qint64) it.value("updatedAt").toInt();
#else
            qint64 cc = it.value("createdAt").toInteger();
            qint64 uu = it.value("updatedAt").toInteger();
#endif
            QDateTime ct = QDateTime::fromMSecsSinceEpoch(cc);
            QDateTime ut = QDateTime::fromMSecsSinceEpoch(uu);

            QTableWidget* w = ui->studies;
            int n = w->rowCount();
            w->insertRow(n);
            w->setItem(n,0,new QTableWidgetItem(studyName));
            w->setItem(n,1,new QTableWidgetItem(ct.date().toString()));
            w->setItem(n,2,new QTableWidgetItem(ut.date().toString()));
            w->item(n,0)->setData(Qt::UserRole, id);
            w->item(n,1)->setData(Qt::UserRole, id);
            w->item(n,2)->setData(Qt::UserRole, id);
        }
    }
}

QList<QPair<QString, QString> > StudySelectionDialog::getStudies() const
{
    return studies;
}

void StudySelectionDialog::closeEvent(QCloseEvent *e)
{
    if (e->isAccepted() && studies.isEmpty())
    {
        QTableWidget* w = ui->studies;
        int n = w->rowCount();
        for (int i=0; i<n; i++)
        {
            QTableWidgetItem* item = w->item(i,0);
            if (item->isSelected())
            {
                QString id = item->data(Qt::UserRole).toString();
                if (!id.isEmpty())
                {
                    QString name = item->text();
                    studies<< QPair<QString,QString>(id,name);
                }
            }
        }
    }
    QDialog::closeEvent(e);
}

void StudySelectionDialog::run()
{
    fill();
    exec();
}

void StudySelectionDialog::accept()
{
    if (studies.isEmpty())
    {
        QTableWidget* w = ui->studies;
        int n = w->rowCount();
        for (int i=0; i<n; i++)
        {
            QTableWidgetItem* item = w->item(i,0);
            if (item->isSelected())
            {
                QString id = item->data(Qt::UserRole).toString();
                if (!id.isEmpty())
                {
                    QString name = item->text();
                    studies<< QPair<QString,QString>(id,name);
                }
            }
        }
    }
    QDialog::accept();
}

void StudySelectionDialog::on_studies_itemDoubleClicked(QTableWidgetItem *item)
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
            if (!id.isEmpty())
            {
                QString name = item->text();
                studies<< QPair<QString,QString>(id,name);
                accept();
            }
        }
    }
}

