#ifndef PLAYERLIST_H
#define PLAYERLIST_H

class Database;
class QSortFilterProxyModel;

#include "tableview.h"
class QSortFilterProxyModel;
class PlayerListModel;

/** @ingroup GUI
The PlayerList class displays list of the players in the current filter. */

class PlayerList : public TableView
{
    Q_OBJECT
public:
    explicit PlayerList(QWidget *parent = 0);
    ~PlayerList();

signals:
    void selected(int);
    void filterRequest(QString ts);
    void raiseRequest();

public slots:
    /** Set database used for getting player information. */
    void setDatabase(Database* db);
    /** Re-read configuration */
    void configure();
    /** Select and show current player in the list */
    void selectPlayer(const QString &name);
private slots:
    void itemSelected(const QModelIndex& index);
private:
    QSortFilterProxyModel* m_filterModel;
    PlayerListModel* m_model;
};

#endif // PLAYERLIST_H
