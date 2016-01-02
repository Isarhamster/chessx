/***************************************************************************
                          gamelist.h  -  Game List window
                             -------------------
    begin                : Sun 23 Jul 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GAMELIST_H__
#define __GAMELIST_H__

#include "tableview.h"
#include "game.h"

#include <QSortFilterProxyModel>

class Filter;
class FilterModel;

/** @ingroup GUI
The GameList class displays list of the games in current filter. It allows
user to click on list header and perform simple tag searches. */

class GameListSortModel : public QSortFilterProxyModel
{
public:
    explicit GameListSortModel(QObject *parent = 0) : QSortFilterProxyModel(parent) {}
};

class GameList : public TableView
{
    Q_OBJECT
public:
    GameList(Filter* filter, QWidget* parent = 0);
    ~GameList();

    void removeSelection();
    /** Set current database */
    QList<int> selectedGames();
public slots:
    /** Change current filter/database */
    void setFilter(Filter* filter);
    /** Update filter (called after changing filter outside) */
    void updateFilter();
    /** Perform simple search */
    void simpleSearch(int tag);
    /** Select and show current game in the list */
    void selectGame(int index);
    /** Select and show current game in the list */
    void slotFilterListByPlayer(QString ns);
    /** Select and show current event in the list */
    void slotFilterListByEvent(QString ns);
    /** Select and show current ECO in the list */
    void slotFilterListByEco(QString ns);
    /** Select and show current event and player in the list */
    void slotFilterListByEventPlayer(QString player, QString event);
    /** Select and show current player and ECO in the list */
    void slotFilterListByEcoPlayer(QString tag, QString eco, QString player);
    /** Select the next game from the list */
    bool selectNextGame();
    /** Select the previous game from the list */
    void selectPreviousGame();
    /** Show the context menu */
    virtual void ShowContextMenu(const QPoint& pos);
    void slotReconfigure();
private slots:
    /** Re-emit the request to the receivers to perform some action */
    void itemSelected(const QModelIndex& index);
    /** Request a context menu for the list members */
    void slotContextMenu(const QPoint& pos);
    /** Request to merge all games from the database into the current game */
    void slotMergeAllGames();
    /** Request to merge the complete filter into the current game */
    void slotMergeFilter();
    /** Request to merge a game into the current game */
    void slotMergeGame();
    /** Request a copy dialog for the games selected */
    void slotCopyGame();
    /** Request finding duplicates for the games selected */
    void slotFindDuplicate();
    /** Delete or undelete a game */
    void slotDeleteGame();
    /** Remove a game from a filter */
    void slotHideGame();
    /** React to a change in selected item */
    void slotItemSelected(const QModelIndex&);

signals:
    void selected(int);
    void raiseRequest();
    void requestMergeGame(QList<int>);
    void requestMergeAllGames();
    void requestMergeFilter();
    void requestCopyGame(QList<int>);
    void requestFindDuplicates(QList<int>);
    void requestDeleteGame(QList<int>);
    void requestGameData(Game&);
    void signalFirstGameLoaded(bool);
    void signalLastGameLoaded(bool);

protected: //Drag'n'Drop Support
    void startDrag(Qt::DropActions supportedActions);
    QModelIndex GetSourceIndex(const QModelIndex& index) const;
    QModelIndex NewSortIndex(int row) const;
    QModelIndex GetSortModelIndex(const QModelIndex& index) const;

private:
    FilterModel* m_model;
    GameListSortModel* sortModel;
};

#endif
