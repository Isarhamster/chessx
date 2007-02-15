/***************************************************************************
                          mainwindow.h  -  main window
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QList>
#include <QMainWindow>
#include <QUrl>

#include "common.h"
#include "historylist.h"

class PlayerDialog;
class GameList;
class SaveDialog;
class HelpWindow;
class BoardView;
class Game;
class Database;
class DatabaseInfo;
class Output;
class TipOfDayDialog;

class QCloseEvent;
class ChessBrowser;
class QLabel;
class QMenu;
class QActionGroup;
class QTextBrowser;

/** 
@defgroup GUI
*/

/*
@ingroup GUI
The MainWindow class represents main ChessX window. */

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

protected:
  /** Overridden to ask for confirmation */
  void closeEvent(QCloseEvent* e);
  /** @return active database */
  Database* database();
  /** @return active database structure */
  DatabaseInfo* databaseInfo();
  /** @return database name of database @p index (or active, if no parameter). */
  QString databaseName(int index = -1) const;
  /** @return active database structure */
  const DatabaseInfo* databaseInfo() const;
  /** @return active game */
  Game* game();
  /** @return index of active game */
  int gameIndex() const;
  /** Open database */
  bool openDatabase(const QString& fname);
  /** Load game @p index. Does nothing if @p index is not correct. */
  void gameLoad(int index);
  /** Make given number of moves in current game */
  void gameMoveBy(int change);
  /** Update recent files menu */
  void updateMenuRecent();
  /** Update recent files menu */
  void updateMenuDatabases();
  /** Player dialog (created when used first) */
  PlayerDialog* playerDialog();
  /** Help window (created when used first) */
  HelpWindow* helpWindow();
public slots:
  void slotAbout();
  void slotPlayerDialog();
  void slotEditBoard();
  void slotEditCopyFEN();
  void slotEditPasteFEN();
  void slotConfigure();
  void slotConfigureChessEngines();
  void slotHelp();
  void slotConfigureFlip();
  void slotGameSave();
  /** Go to first move of the game */
  void slotGameMoveFirst()  {gameMoveBy(-999);}
  /** Go to next move of the game */
  void slotGameMoveNext()   {gameMoveBy(1);}
  /** Go to previous move of the game */
  void slotGameMovePrevious() {gameMoveBy(-1);}
  /** Go to first move of the game */
  void slotGameMoveLast()  {gameMoveBy(999);}
  /** Go N moves forward in the game. For now fixed at @p 10 */
  void slotGameMoveNextN()  {gameMoveBy(10);}
  /** Go N moves backward in the game. For now fixed at @p 10 */
  void slotGameMovePreviousN()  {gameMoveBy(-10);}
  /** Browse current game by mouse wheel */
  void slotGameMoveWheel(int wheel);
  /** Load first game */
  void slotGameLoadFirst();
  /** Load next game */
  void slotGameLoadNext();
  /** Load previous game */
  void slotGameLoadPrevious();
  /** Load last game */
  void slotGameLoadLast();
  /** Load random game */
  void slotGameLoadRandom();
  /** Choose a game and load it */
  void slotGameLoadChosen();
  /** Update GameView after move/game change */
  void slotGameViewUpdate();
  /** Handle link click in Game View panel */
  void slotGameViewLink(const QUrl& link);
  /** Show/hide GameView source - useful for debugging. */
  void slotGameViewToggle(bool source);
  /** Filter was changed - update status bar information */
  void slotFilterChanged();
  /** Load given game (triggered from Game List) */
  void slotFilterLoad(int index);
  /** Open File dialog, choose a database and open it */
  void slotFileOpen();
  /** Open given database from Recent files list. */
  void slotFileOpenRecent();
  /** Close current database. Switches to next active (or clipboard if there is none). */
  void slotFileClose();
  /** Quit ChessX. Ask for confirmation before */
  void slotFileQuit();
  /** Find current position */
 // void slotSearchBoard();
  /** Reset current filter */
  void slotSearchReset();
  /** Reverse current filter */
  void slotSearchReverse();
  /** Made given move on the board */
  void slotMove(Square from, Square to);
  /** Update GameInfo after move was made. */
  void slotMoveViewUpdate();
  /** Show temporary message. */
  void slotStatusMessage(const QString& msg);
  /** Change database */
  void slotDatabaseChange();
  /** Database was changed - change informations */
  void slotDatabaseChanged();
  /** Show window, then show tip dialog if enabled. */
  virtual void show();
signals:
  /** Re-read configuration */
  void reconfigure();

private:
  /** Create single menu action. */
  QAction* createAction(const QString& name, const char* slot, const QKeySequence& key = QKeySequence(),
                        const QString& tip = QString());
  /** Create all the menus and actions */
  void setupActions();

  enum {GameFirst, GameLast, GameNext, GamePrevious, GameRandom, MoveFirst, MoveLast, MoveNext, MovePrevious,
     MoveNextN, MovePreviousN};
  /* Dialogs  */
  PlayerDialog* m_playerDialog;
  GameList* m_gameList;
  SaveDialog* m_saveDialog;
  HelpWindow* m_helpWindow;
  TipOfDayDialog* m_tipDialog;
  /* Main gui parts */
  BoardView* m_boardView;
  ChessBrowser* m_moveView;
  ChessBrowser* m_gameView;
  /* Status */
  QLabel* m_statusFilter;
  /* Menus */
  QMenu* m_menuDatabases;
  QMenu* m_menuView;

  /* Local variables */
  HistoryList m_recentFiles;
  Output* m_output;
  QList<DatabaseInfo*> m_databases;
  QList<QAction*> m_recentFileActions;
  QList<QAction*> m_databaseActions;
  int m_currentDatabase;
  QActionGroup* m_actions;
  bool m_showTip;
  bool m_showPgnSource; // for debugging
};


#endif
