/***************************************************************************
                          main.cpp  -  main application
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

#include <qapplication.h>
#include "mainwindow.h"
#include "move.h"

int main( int argc, char ** argv ) 
{
  Move m(5, 20);
  QApplication a(argc, argv);
  MainWindow* mw = new MainWindow;
  mw->setCaption("Chess Database");
  mw->show();
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}
