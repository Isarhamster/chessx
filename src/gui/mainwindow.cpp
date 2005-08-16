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

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>

MainWindow::MainWindow():QMainWindow(0, "MainWindow", WDestructiveClose)
{
  /* Database initialization */
  m_playerDatabase = new PlayerDatabase;
  m_playerDatabase->open("../tests/playerdatabase/data/players");
  m_playerDialog = new PlayerDialog(m_playerDatabase);

  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  QPopupMenu *windows = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Windows"), windows);
  windows ->insertItem(tr("&Player Database..."), this, SLOT(slotPlayerDialog()), CTRL + SHIFT + Key_P);

  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()), Key_F1);
  statusBar()->message(tr("Ready"), 2000);
  resize(450, 600);

  /* Restoring layouts */
  connect(qApp, SIGNAL(aboutToQuit()), SLOT(writeConfig()));
  AppSettings->readLayout(m_playerDialog);
  AppSettings->readLayout(this);
}

MainWindow::~MainWindow()
{
  /* saving layouts */
  delete m_playerDialog;
  delete m_playerDatabase;
}

void MainWindow::writeConfig()
{
  AppSettings->writeLayout(m_playerDialog);
  AppSettings->writeLayout(this);
}



void MainWindow::slotAbout()
{
  QMessageBox::about(this, tr("Chess Database"),
      tr("Chess Database\n(C) 2005 Ejner Borgbjerg, Kamil Przybyla and Michal Rudolf") + QString("\nPosition: %1x%2").arg(x()).arg(y()));
}

void MainWindow::slotPlayerDialog()
{
  m_playerDialog->show();
}


