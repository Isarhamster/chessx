/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ECOLISTWIDGET_H
#define ECOLISTWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include "ecoinfo.h"

namespace Ui {
class TagDetailWidget;
}

class DatabaseInfo;

class ECOListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ECOListWidget(QWidget *parent = 0);
    ~ECOListWidget();

signals:
    void filterRequest(QString ts);
    void filterEcoPlayerRequest(QString tag, QString eco);
    void filterEcoPlayerRequest(QString tag, QString eco, QString player);
    void raiseRequest();

public slots:
    void findECO(const QString& s);
    void slotSelectECO(const QString& eco);
    void setDatabase(DatabaseInfo* dbInfo);
    void showSelectedECO();
    void filterSelectedECO();
    void slotReconfigure();
    void selectionChangedSlot();
    void slotLinkClicked(const QUrl& url);

protected slots:
    void selectECO(const QString& eco);
    void ecoSelected(const QString& eco);

private:
    EcoInfo m_eco;
    QStringList m_list;
    Ui::TagDetailWidget *ui;
    QStringListModel* m_filterModel;
};

#endif // ECOLISTWIDGET_H
