/***************************************************************************
                          stdlib.h  -  cross platform compatibility header
                             -------------------
    begin                : 3 August 2006
    copyright            : (C) 2006 William Hoggarth
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

#ifndef __PLATFORM_STDLIB_H__
#define __PLATFORM_STDLIB_H__

#include <qglobal.h>
#include <stdlib.h>

#ifdef Q_OS_WIN
#define random()	rand()
#define srandom(seed)	srand(seed)
#endif // Q_OS_MSDOS

#endif // __PLATFORM_STDLIB_H__
