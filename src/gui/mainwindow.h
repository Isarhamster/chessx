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
class DatabaseInfo;
class Database;
class Output;
class TipOfDayDialog;

class QCloseEvent;
class ChessBrowser;
class QLabel;
class QMenu;
class QActionGroup;
class QTextBrowser;

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* e);
/** @return active database */
  Database* database();
  /** @return active game */
  Game* game();
  /** @return index of active game */
  int gameIndex() const;
  /** Open database */
  bool openDatabase(const QString& fname);
  /** Load given game */
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
  void slotGameLoadFirst()  {gameLoad(0);}
  /** Load next game */
  void slotGameLoadNext()  {gameLoad(gameIndex()+1);}
  /** Load previous game */
  void slotGameLoadPrevious() {gameLoad(gameIndex()-1);}
  /** Load last game */
  void slotGameLoadLast()   {gameLoad(9999999);}
  /** Load random game */
  void slotGameLoadRandom();
  void slotGameView();
  /** Handle link click in Game View panel */
  void slotGameViewLink(const QUrl& link);
  void slotGameViewToggle();
  //void slotFilterSwitch();
  void slotFilterUpdate();
  void slotFilterLoad(int index);
  void slotFileOpen();
  void slotFileOpenRecent(int);
  void slotFileClose();
  void slotFileQuit();
  void slotMove(Square from, Square to);
  void slotMoveViewUpdate();
  void slotStatusMessage(const QString& msg);
  void slotDatabaseChange(int current);
  void slotDatabaseChanged();
  /*Overriden to show the tip of the day dialog */
  void show();
signals:
  /* Re-read configuration */
  void reconfigure();

private:
  /* Setup actions */
  QAction* createAction(const QString& name, const char* slot, const QKeySequence& key = QKeySequence(),
                        const QString& tip = QString());
  /* Setup menus and actions */
  void setupActions();

  enum {GameFirst, GameLast, GameNext, GamePrevious, GameRandom, MoveFirst, MoveLast, MoveNext, MovePrevious,
     MoveNextN, MovePreviousN};
  /* Widget variables */
  PlayerDialog* m_playerDialog;
  GameList* m_gameList;
  SaveDialog* m_saveDialog;
  HelpWindow* m_helpWindow;
  TipOfDayDialog* m_tipDialog;
  BoardView* m_boardView;
  ChessBrowser* m_moveView;
  ChessBrowser* m_gameView;
  QLabel* m_statusFilter;
  HistoryList m_recentFiles;
  QMenu* m_menuRecent;
  QMenu* m_menuDatabases;
  QMenu* m_menuView;
  /* Local variables */
  Output* m_output;
  QList<DatabaseInfo*> m_databases;
  int m_currentDatabase;
  QActionGroup* m_actions;
  bool m_showTip;
};


#endif
