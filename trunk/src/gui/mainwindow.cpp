/***************************************************************************
                          MainWindow.cpp  -  description
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 $AUTHOR <$EMAIL@>
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

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>

MainWindow::MainWindow():QMainWindow(0, "MainWindow", WDestructiveClose)
{
  QPopupMenu *file = new QPopupMenu(this);
  menuBar()->insertItem(tr("&File"), file);
  file->insertItem(tr("&Quit"), qApp, SLOT(closeAllWindows()), CTRL + Key_Q);

  menuBar()->insertSeparator();
  QPopupMenu *help = new QPopupMenu(this);
  menuBar()->insertItem(tr("&Help"), help);
  help->insertItem(tr("&About..."), this, SLOT(slotAbout()), Key_F1);
  statusBar()->message(tr("Ready"), 2000);
  resize(450, 600);
}

MainWindow::~MainWindow()
{
}


void MainWindow::slotAbout()
{
  QMessageBox::about(this, tr("Chess Database"),
      tr("Chess Database\n(C) 2005 Ejner Borgbjerg, Kamil Przybyla and Michal Rudolf"));
}

