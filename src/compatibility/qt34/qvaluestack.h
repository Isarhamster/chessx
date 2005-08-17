/***************************************************************************
                          qvaluestack.h  -  qt3/4 compatibility header
                             -------------------
    begin                : 17 August 2005
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

#ifndef __Q34_QVALUESTACK_H__
#define __Q34_QVALUESTACK_H__

#if QT_VERSION < 0x040000
#include <qvaluestack.h>
#else
#include <q3valuestack.h>
#define QValueStack Q3ValueStack
using namespace Qt;
#endif // QT_VERSION < 0x040000

#endif // __Q34_QVALUESTACK_H__
