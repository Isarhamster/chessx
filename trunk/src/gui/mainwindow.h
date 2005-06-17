/***************************************************************************
                          $FILE$  -  description
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


#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <qmainwindow.h>

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

protected:

private slots:
  void slotAbout();

private:
};


#endif
