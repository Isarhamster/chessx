/***************************************************************************
                          qmainwindow.h  -  qt3/4 compatibility header
                             -------------------
    begin                : 3 July 2005
    copyright            : (C) 2005 William Hoggarth
                           <whoggarth@user.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __QT34_QMAINWINDOW_H__
#define __QT34_QMAINWINDOW_H__

#include <qglobal.h>
#if QT_VERSION < 0x040000
#include <qmainwindow.h>
#else
#include <q3mainwindow.h>
typedef Q3MainWindow QMainWindow;
using namespace Qt;
#endif // QT_VERSION < 0x040000

#endif // __QT32_QMAINWINDOW_H__
