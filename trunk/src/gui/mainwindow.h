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

#include <q3mainwindow.h>
#include <qmessagebox.h>
#include <q3valuelist.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <QLabel>
#include <QCloseEvent>
#include <Q3PtrList>

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

class ChessBrowser;
class Q3ListBox;
class Q3VBoxLayout;
class QLabel;
class QMenu;
class QActionGroup;

class MainWindow: public Q3MainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

protected:
  bool yesNo(const QString& quetion, QMessageBox::Icon icon = QMessageBox::Information) const;
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
  void loadGame(int index);
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
  /** Browse games or moves */
  void slotGameBrowse(QAction* action);
  /** Browse current game */
  void slotGameBrowse(int wheel);
  void slotGameView();
  void slotGameViewToggle();
  void slotFilterSwitch();
  void slotFilterUpdate();
  void slotFilterLoad(int index);
  void slotFileOpen();
  void slotFileOpenRecent(int);
  void slotFileClose();
  void slotFileQuit();
  void slotMove(Square from, Square to);
  void slotMoveViewUpdate();
  void slotMoveViewLink(const QString& link);
  void slotStatusMessage(const QString& msg);
  void slotDatabaseChange(int current);
  void slotDatabaseChanged();
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
  BoardView* m_boardView;
  ChessBrowser* m_moveView;
  ChessBrowser* m_gameView;
  Q3VBoxLayout* m_layout;
  QLabel* m_statusFilter;
  HistoryList m_recentFiles;
  QMenu* m_menuRecent;
  QMenu* m_menuDatabases;
  /* Local variables */
  Output* m_output;
  Q3PtrList<DatabaseInfo> m_databases;
  QActionGroup* m_actions;
};


#endif
