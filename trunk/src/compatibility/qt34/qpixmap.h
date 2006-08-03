/***************************************************************************
                          qpixmap.h  -  qt3/4 compatibility header
                             -------------------
    begin                : 24 November 2005
    copyright            : (C) 2005, 2006 William Hoggarth
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

#ifndef __Q34_QPIXMAP_H__
#define __Q34_QPIXMAP_H__

#include <qpixmap.h>

#if QT_VERSION >= 0x040000

#define bitBlt(dest,destx,desty,src,srcx,srcy,width,height,rop,ignoreMask)	bitBlt(dest,destx,desty,src,srcx,srcy,width,height)
#define copyBlt(dest,destx,desty,src,srcx,srcy,width,height)	(*(dest))=((src)->copy((srcx),(srcy),(width),(height)))

#endif // QT_VERSION >= 0x040000

#endif // __Q34_QPIXMAP_H__
