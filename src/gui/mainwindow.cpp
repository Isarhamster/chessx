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

#include "mainwindow.h"
#include "playerdialog.h"
#include "playerdatabase.h"
#include "settings.h"
#include "preferences.h"
#include "boardview.h"

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qlabel.h>

MainWindow::MainWindow() : QMainWindow(0, "MainWindow", WDestructiveClose)
{
  /* Database initialization */
  m_playerDatabase = new PlayerDatabase;
  m_playerDatabase->open("../tests/playerdatabase/data/players");
  m_playerDialog = new PlayerDialog(m_playerDatabase, this);

  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  QPopupMenu *windows = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Windows"), windows);
  windows ->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  QPopupMenu *settings = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Settings"), settings);
  settings ->insertItem(tr("&Configure ChessX..."), this, SLOT(slotConfigure()));

  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()), Key_F1);
  statusBar()->message(tr("Ready"), 2000);
  resize(450, 600);

  /* Board */
  m_boardView = new BoardView(this);
  Board board;
  board.fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  m_boardView->setBoard(board);
  qDebug("%d", m_boardView->setTheme("/home/ja/programming/chessdatabase/WCN.png"));
  setCentralWidget(m_boardView);

  /* Restoring layouts */
  AppSettings->readLayout(m_playerDialog, Settings::Show);
  AppSettings->readLayout(this);
}

MainWindow::~MainWindow()
{
  /* saving layouts */
  delete m_playerDialog;
  delete m_playerDatabase;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
  if (yesNo(tr("Do you want to quit?"))) {
    AppSettings->writeLayout(m_playerDialog);
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
  QMessageBox::about(this, tr("Chess Database"),
      tr("Chess Database\n(C) 2005 Ejner Borgbjerg, William Hoggarth, Kamil Przybyla and Michal Rudolf"));
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
