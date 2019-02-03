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

#ifndef GAMELIST_H_INCLUDED
#define GAMELIST_H_INCLUDED

#include "tableview.h"
#include "game.h"

#include <QPointer>
#include <QSortFilterProxyModel>
#include "filteroperator.h"

class Filter;
class FilterModel;
class GameListSortModel;
class Search;

/** @ingroup GUI
The GameList class displays list of the games in current filter. It allows
user to click on list header and perform simple tag searches. */

class GameList : public TableView
{
    Q_OBJECT
public:
    GameList(Filter* filter, QWidget* parent = 0);
    ~GameList();

    void removeSelection();
    /** Set current database */
    QList<GameId> selectedGames();

    void startUpdate(); // TODO Remove this hack
    void endUpdate(); // TODO Remove this hack

public slots:
    /** Change current filter/database */
    void setFilter(Filter* filter);
    /** Update filter (called after changing filter outside) */
    void updateFilter(GameId index, int value);
    /** Perform simple search */
    void simpleSearch(int tag);
    void executeSearch(Search* search, FilterOperator searchOperator=FilterOperator::NullOperator);
    void endSearch();
    /** Select and show current game in the list */
    void selectGame(GameId index);
    /** Select and show current game in the list */
    void slotFilterListByPlayer(QString ns);
    /** Select and show current event in the list */
    void slotFilterListByEvent(QString ns);
    /** Select and show current ECO in the list */
    void slotFilterListByEco(QString ns);
    /** Select and show current event and player in the list */
    void slotFilterListByEventPlayer(QString player, QString event);
    /** Select and show current player and ECO in the list */
    void slotFilterListByEcoPlayer(QString tag, QString eco, QString player, QString result);
    /** Select the next visible game from the list */
    bool selectNextGame();
    /** Select a random but visible game from the list */
    void selectRandomGame();
    /** Select the previous visible game from the list */
    void selectPreviousGame();
    /** Show the context menu */
    virtual void ShowContextMenu(const QPoint& pos);
    void slotReconfigure();

public slots:
    /** Request a filter operation to invert the visibility of all items */
    void filterInvert();
    /** Request a filter operation to show all items*/
    void filterSetAll(int value=1);

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
    /** Remove deleted games from a filter */
    void slotHideDeletedGames();
    /** React to a change in selected item */
    void slotItemSelected(const QModelIndex&);
    bool triggerGameSelection(int sortRow);
signals:
    void gameSelected(GameId);
    void raiseRequest();
    void requestMergeGame(QList<GameId>);
    void requestMergeAllGames();
    void requestMergeFilter();
    void requestCopyGame(QList<GameId>);
    void requestFindDuplicates(QList<GameId>);
    void requestDeleteGame(QList<GameId>);
    void requestGameData(Game&);
    void signalFirstGameLoaded(bool);
    void signalLastGameLoaded(bool);
    void signalDropEvent(QDropEvent*);
    void searchProgress(int);
    void searchFinished();

protected: //Drag'n'Drop Support
    void startDrag(Qt::DropActions supportedActions);
    QModelIndex GetSourceIndex(const QModelIndex& index) const;
    QModelIndex NewSortIndex(int row) const;
    QModelIndex GetSortModelIndex(const QModelIndex& index) const;

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    FilterModel* m_model;
    QPointer<GameListSortModel> sortModel;
};

#endif
