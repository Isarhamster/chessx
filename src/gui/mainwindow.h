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

#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qvaluelist.h>

#include "common.h"
#include "historylist.h"

class PlayerDatabase;
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
class QListBox;
class QVBoxLayout;
class QLabel;
class QPopupMenu;

class MainWindow: public QMainWindow
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
  void slotGameLoad(int id);
  void slotGameSave();
  void slotGameBrowse(int id);
  void slotGameView();
  void slotGameViewToggle();
  void slotFilterSwitch();
  void slotFilterUpdate();
  void slotFilterLoad(int index);
  void slotFileOpen();
  void slotFileOpenRecent(int);
  void slotFileClose();
  void slotMove(Square from, Square to);
  void slotMoveViewUpdate();
  void slotMoveViewLink(const QString& link);
  void slotStatusMessage(const QString& msg);
  void slotDatabaseChange(DatabaseInfo* current);
signals:
  /* Re-read configuration */
  void reconfigure();

private:
  enum {IdFirst, IdLast, IdNext, IdPrevious, IdNext5, IdPrevious5, IdRandom};
  /* Widget variables */
  PlayerDatabase* m_playerDatabase;
  PlayerDialog* m_playerDialog;
  GameList* m_gameList;
  SaveDialog* m_saveDialog;
  HelpWindow* m_helpWindow;
  BoardView* m_boardView;
  ChessBrowser* m_moveView;
  ChessBrowser* m_gameView;
  QVBoxLayout* m_layout;
  QLabel* m_statusFilter;
  HistoryList m_recentFiles;
  QPopupMenu* m_menuRecent;
  /* Local variables */
  Output* m_output;
  QPtrList<DatabaseInfo> m_databases;
};


#endif
