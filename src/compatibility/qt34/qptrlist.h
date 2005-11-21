/***************************************************************************
                          qptrlist.h  -  qt3/4 compatibility header
                             -------------------
    begin                : 21 November 2005
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

#ifndef __Q34_QPTRLIST_H__
#define __Q34_QPTRLIST_H__

#if QT_VERSION < 0x040000
#include <qptrlist.h>
#else
#include <q3ptrlist.h>
#define QPtrList Q3PtrList
using namespace Qt;
#endif // QT_VERSION < 0x040000

#endif // __Q34_QPTRLIST_H__
