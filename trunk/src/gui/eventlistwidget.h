/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef EVENTLISTWIDGET_H
#define EVENTLISTWIDGET_H

#include <QWidget>
#include <QStringListModel>

#include "eventinfo.h"

class Database;
namespace Ui
{
    class TagDetailWidget;
}

class EventListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventListWidget(QWidget *parent = 0);
    ~EventListWidget();

signals:
    void filterRequest(QString ts);
    void raiseRequest();

public slots:
    void findEvent(const QString& s);
    void selectEvent(const QString& player);
    void setDatabase(Database* db);
    void showSelectedEvent();
    void filterSelectedEvent();
    void slotReconfigure();
    void selectionChangedSlot();

private:
    EventInfo m_event;
    QStringList m_list;
    Ui::TagDetailWidget *ui;
    QStringListModel* m_filterModel;
};


#endif // EVENTLISTWIDGET_H
