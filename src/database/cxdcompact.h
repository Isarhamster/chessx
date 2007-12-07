/***************************************************************************
 * 	cxdcompact.h - Helper functions for compactification of cxd databases
 *
 * 	(C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 *
 * 	This file is a part of Chessx (http://chessx.sourceforge.net).
 * 	ChessX is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License version 3,
 * 	as published by the Free Software Foundation.
 ****************************************************************************/

#ifndef __CXDCOMPACT_H__
#define __CXDCOMPACT_H__

#include "indexitem.h"
#include <QList>
#include <QString>


namespace CxdCompact
{
  template<class T, class C>
  void compactList(QList<T>& ql, const C& cl);

  extern template void compactList<QString,QList<bool> >(QList<QString>&, const QList<bool>&); 
  extern template void compactList<QString,QVector<bool> >(QList<QString>&, const QVector<bool>&); 
  extern template void compactList<IndexItem*,QList<bool> >(QList<IndexItem*>&, const QList<bool>&); 
  extern template void compactList<bool,QList<bool> >(QList<bool>&, const QList<bool>&); 
  extern template void compactList<qint32,QList<bool> >(QList<qint32>&, const QList<bool>&); 


}

#endif
