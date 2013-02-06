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
    void renameRequest(QString ts);
    void filterEventPlayerRequest(QString player, QString event);
    void raiseRequest();

public slots:
    void findEvent(const QString& s);
    void slotSelectEvent(const QString& player);
    void setDatabase(Database* db);
    void showSelectedEvent();
    void filterSelectedEvent();
    void renameSelectedEvent();
    void slotReconfigure();
    void selectionChangedSlot();
    void slotLinkClicked(const QUrl& url);

protected slots:
    void selectEvent(const QString& player);

private:
    EventInfo m_event;
    QStringList m_list;
    Ui::TagDetailWidget *ui;
    QStringListModel* m_filterModel;
};


#endif // EVENTLISTWIDGET_H
