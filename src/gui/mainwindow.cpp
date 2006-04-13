/***************************************************************************
                          mainwindow.cpp  -  main window
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

#include "boardsetup.h"
#include "boardview.h"
#include "game.h"
#include "helpwindow.h"
#include "mainwindow.h"
#include "playerdatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "settings.h"

#include <qapplication.h>
#include <qclipboard.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>

MainWindow::MainWindow() : QMainWindow(0, "MainWindow", WDestructiveClose)
{
  /* Database initialization */
  m_playerDatabase = new PlayerDatabase;
  m_playerDatabase->open("../tests/cppunit/data/small/players");
  m_playerDialog = new PlayerDialog(m_playerDatabase, this);
  m_helpWindow = new HelpWindow();

  /* File menu */
  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  /* Edit menu */
  QPopupMenu *edit = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Edit"), edit);
  edit->insertItem(tr("&Copy FEN"), this, SLOT(slotEditCopyFEN()), CTRL + SHIFT + Key_C);
  edit->insertItem(tr("&Paste FEN"), this, SLOT(slotEditPasteFEN()), CTRL + SHIFT + Key_V);
  edit->insertItem(tr("&Edit board..."), this, SLOT(slotEditBoard()));

  /* Game menu */
  QPopupMenu *gameMenu = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Game"), gameMenu);
  QPopupMenu* goMenu = new QPopupMenu(this);
  goMenu->insertItem(tr("&Start"), this, SLOT(slotMoveToStart()), Key_Home);
  goMenu->insertItem(tr("&End"), this, SLOT(slotMoveToEnd()), Key_End);
  goMenu->insertItem(tr("&Next move"), this, SLOT(slotMoveForward()), Key_Right);
  goMenu->insertItem(tr("&Previous move"), this, SLOT(slotMoveBackward()), Key_Left);
  gameMenu->insertItem(tr("&Go..."), goMenu);
  
  /* Windows menu */
  QPopupMenu *windows = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Windows"), windows);
  windows ->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  /* Settings menu */
  QPopupMenu *settings = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Settings"), settings);
  settings ->insertItem(tr("&Configure ChessX..."), this, SLOT(slotConfigure()));
  settings ->insertItem(tr("&Flip board"), this, SLOT(slotConfigureFlip()), CTRL + Key_B);

  /* Help menu */
  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem( tr( "ChessX &help..." ), this, SLOT( slotHelp()), Key_F4 );
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()), Key_F1);
  statusBar()->message(tr("Ready"), 2000);
  resize(450, 600);

  /* Game */
  m_game = new Game;

  /* Board */
  m_boardView = new BoardView(this);
  m_boardView->setBoard(m_game->board());
  setCentralWidget(m_boardView);
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));
  connect(m_boardView, SIGNAL(moveMade(Square, Square)), SLOT(slotMove(Square, Square)));

  /* Restoring layouts */
  AppSettings->readLayout(m_playerDialog, Settings::Show);
  AppSettings->readLayout(m_helpWindow, Settings::Show);
  AppSettings->readLayout(this);
  emit reconfigure();

}

MainWindow::~MainWindow()
{
  /* saving layouts */
  delete m_playerDialog;
  delete m_playerDatabase;
  delete m_helpWindow;
  delete m_game;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
  if (yesNo(tr("Do you want to quit?"))) {
    AppSettings->writeLayout(m_playerDialog);
    AppSettings->writeLayout(m_helpWindow);
    AppSettings->writeLayout(this);
    e->accept();
    qApp->quit();
    }
  else
    e->ignore();
}

bool MainWindow::yesNo(const QString& question, QMessageBox::Icon icon) const
{
  QMessageBox mb("ChessX", question, icon, QMessageBox::Yes, QMessageBox::No,
     QMessageBox::NoButton);
  return mb.exec() == QMessageBox::Yes;
}

void MainWindow::slotAbout()
{
  QMessageBox::about(this, tr("ChessX"),
      tr("Chess Database\n(C) 2005 William Hoggarth, Michal Rudolf and Ejner Borgbjerg"));
}

void MainWindow::slotPlayerDialog()
{
  m_playerDialog->show();
}

void MainWindow::slotConfigure()
{
  PreferencesDialog P;
  if (P.exec() == QDialog::Accepted)
    emit reconfigure();
}

void MainWindow::slotConfigureFlip()
{
  m_boardView->flip();
}

void MainWindow::slotEditCopyFEN()
{
  Board b = m_boardView->board();
  QApplication::clipboard()->setText(b.toFEN());
}

void MainWindow::slotEditPasteFEN()
{
  Board b = m_boardView->board();
  QString fen = QApplication::clipboard()->text();
  b.fromFEN(fen);
  m_boardView->setBoard(b);
}

void MainWindow::slotEditBoard()
{
  BoardSetupDialog B;
  B.setBoard(m_boardView->board());
  if (B.exec() == QDialog::Accepted)
    m_boardView->setBoard(B.board());
}


void MainWindow::slotHelp()
{
  if(!m_helpWindow->winId())
    m_helpWindow = new HelpWindow();
  m_helpWindow->show();
}

void MainWindow::slotMove(Square from, Square to)
{
  Move m(from, to);
  Board board = m_game->board();
  if (board.isLegal(m))
  {
    m_game->addMove(m);
    m_game->forward(1);
    m_boardView->setBoard(m_game->board());
  }
}

void MainWindow::slotMoveForward()
{
  if (m_game->forward())
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveBackward()
{
  if (m_game->backward())
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveToStart()
{
  if (m_game->backward(9999))
    m_boardView->setBoard(m_game->board());
}

void MainWindow::slotMoveToEnd()
{
  if (m_game->forward(9999))
    m_boardView->setBoard(m_game->board());
}
