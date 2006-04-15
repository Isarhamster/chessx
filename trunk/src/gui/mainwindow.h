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
class HelpWindow;
class BoardView;
class Game;

class QTextBrowser;


class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

protected:
  bool yesNo(const QString& quetion, QMessageBox::Icon icon = QMessageBox::Information) const;
  void closeEvent(QCloseEvent* e);

public slots:
  void slotAbout();
  void slotPlayerDialog();
  void slotEditBoard();
  void slotEditCopyFEN();
  void slotEditPasteFEN();
  void slotConfigure();
  void slotHelp();
  void slotConfigureFlip();
  void slotMove(Square from, Square to);
  void slotMoveToStart();
  void slotMoveForward();
  void slotMoveBackward();
  void slotMoveToEnd();
  void slotMoveViewUpdate();
  void slotMoveViewLink(const QString& link);
signals:
  /* Re-read configuration */
  void reconfigure();

private:
  PlayerDatabase* m_playerDatabase;
  PlayerDialog* m_playerDialog;
  HelpWindow* m_helpWindow;
  BoardView* m_boardView;
  QTextBrowser* m_moveView;
  Game* m_game;
};


#endif
