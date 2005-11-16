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
#include "helpwindow.h"
#include "mainwindow.h"
#include "playerdatabase.h"
#include "playerdialog.h"
#include "preferences.h"
#include "settings.h"

#include <qapplication.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>

MainWindow::MainWindow() : QMainWindow(0, "MainWindow", WDestructiveClose)
{
  /* Database initialization */
  m_playerDatabase = new PlayerDatabase;
  m_playerDatabase->open("../tests/playerdatabase/data/players");
  m_playerDialog = new PlayerDialog(m_playerDatabase, this);
  m_helpWindow = new HelpWindow();

  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  QPopupMenu *windows = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Windows"), windows);
  windows ->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  QPopupMenu *settings = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Settings"), settings);
  settings ->insertItem(tr("&Configure ChessX..."), this, SLOT(slotConfigure()));
  settings ->insertItem(tr("&Flip board"), this, SLOT(slotConfigureFlip()), CTRL + Key_B);
  settings ->insertItem(tr("&Setup board..."), this, SLOT(slotSetupBoard()));

  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem( tr( "ChessX &help..." ), this, SLOT( slotHelp()), Key_F4 );
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()), Key_F1);
  statusBar()->message(tr("Ready"), 2000);
  resize(450, 600);

  /* Board */
  m_boardView = new BoardView(this);
  Board board;
  board.setStandardPosition();
  m_boardView->setBoard(board);
  setCentralWidget(m_boardView);
  connect(this, SIGNAL(reconfigure()), m_boardView, SLOT(configure()));

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

void MainWindow::slotSetupBoard()
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



