/***************************************************************************
                          qprocess.h  -  qt3/4 compatibility header
                             -------------------
    begin                : 19 September 2005
    copyright            : (C) 2005 William Hoggarth
                           <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __Q34_QPROCESS_H__
#define __Q34_QPROCESS_H__

#if QT_VERSION < 0x040000
#include <qprocess.h>
#else
#include <q3process.h>
#define QProcess Q3Process
using namespace Qt;
#endif // QT_VERSION < 0x040000

#endif // __Q34_QPROCESS_H__
