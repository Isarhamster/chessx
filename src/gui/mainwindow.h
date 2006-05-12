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
#include "common.h"

class PlayerDatabase;
class PlayerDialog;
class SaveDialog;
class HelpWindow;
class BoardView;
class Game;
class Database;

class QTextBrowser;
class QListBox;
class QGridLayout;
class QLabel;

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
  Database* database()  {return m_database;}
  /** @return index of active game */
  int activeGameIndex() const {return m_gameIndex;}
  /** Load given game */
  void loadGame(int index);

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
  void slotFilterSwitch();
  void slotFilterUpdate();
  void slotFilterLoad(int index);
  void slotFileOpen();
  void slotFileClose();
  void slotMove(Square from, Square to);
  void slotMoveViewUpdate();
  void slotMoveViewLink(const QString& link);
  void slotStatusMessage(const QString& msg);
  void slotStatusFilter();
signals:
  /* Re-read configuration */
  void reconfigure();

private:
  enum {IdFirst, IdLast, IdNext, IdPrevious, IdNext5, IdPrevious5, IdRandom};
  PlayerDatabase* m_playerDatabase;
  PlayerDialog* m_playerDialog;
  SaveDialog* m_saveDialog;
  HelpWindow* m_helpWindow;
  BoardView* m_boardView;
  QTextBrowser* m_moveView;
  QListBox* m_filterView;
  QGridLayout* m_layout;
  QLabel* m_statusFilter;
  Game* m_game;
  Database* m_database;
  int m_gameIndex;
};


#endif
