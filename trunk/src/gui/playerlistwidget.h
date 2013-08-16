/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef PLAYERLISTWIDGET_H
#define PLAYERLISTWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include "playerinfo.h"

namespace Ui {
class TagDetailWidget;
}

class DatabaseInfo;

class PlayerListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerListWidget(QWidget *parent = 0);
    ~PlayerListWidget();

signals:
    void filterRequest(QString ts);
    void renameRequest(QString ts);
    void filterEcoPlayerRequest(QString tag, QString eco, QString player);
    void raiseRequest();

public slots:
    void findPlayers(const QString& s);
    void slotSelectPlayer(const QString& player);
    void setDatabase(DatabaseInfo* dbInfo);
    void filterSelectedPlayer();
    void renameSelectedPlayer();
    void slotReconfigure();
    void selectionChangedSlot();
    void slotLinkClicked(const QUrl& url);

protected slots:
    void selectPlayer(const QString& player);
    void playerSelected(const QString& player);

private:
    PlayerInfo m_player;
    QStringList m_list;
    Ui::TagDetailWidget *ui;
    QStringListModel* m_filterModel;
};

#endif // PLAYERLISTWIDGET_H
