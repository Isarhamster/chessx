#ifndef PLAYERLISTWIDGET_H
#define PLAYERLISTWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include "playerinfo.h"

namespace Ui {
class PlayerListWidget;
}

class Database;

class PlayerListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerListWidget(QWidget *parent = 0);
    ~PlayerListWidget();

signals:
    void filterRequest(QString ts);
    void raiseRequest();

public slots:
    void findPlayers(const QString& s);
    void selectPlayer(const QString& player);
    void setDatabase(Database* db);
    void showSelectedPlayer();
    void slotReconfigure();

private:
    PlayerInfo m_player;
    QStringList m_list;
    Ui::PlayerListWidget *ui;
    QStringListModel* m_filterModel;
};

#endif // PLAYERLISTWIDGET_H
