/***************************************************************************
                          qpopupmenu.h  -  qt3/4 compatibility header
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

#ifndef __Q34_QPOPUPMENU_H__
#define __Q34_QPOPUPMENU_H__

#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <q3popupmenu.h>
typedef Q3PopupMenu QPopupMenu;
using namespace Qt;
#endif // QT_VERSION < 0x040000

#endif // __Q34_QPOPUPMENU_H__
