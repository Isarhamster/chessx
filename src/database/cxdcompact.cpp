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

#include "cxdcompact.h"
#include "common.h"


template<class T, class C>
void CxdCompact::compactList(QList<T>& ql, const C& cl)
{
  int currentPos=0;
  for(int i=0; i<cl.size(); ++i)
  {
    if(cl[i]) ql.swap(i,currentPos++);
  }
    typename QList<T>::iterator eraseStart(ql.begin()),eraseEnd(ql.end());
    eraseStart+=currentPos;
    ql.erase(eraseStart,eraseEnd);
}

template extern void CxdCompact::compactList<QString,QList<bool> >
              (QList<QString>&, const QList<bool>&); 

template extern void CxdCompact::compactList<QString,QVector<bool> >
              (QList<QString>&, const QVector<bool>&); 

template extern void CxdCompact::compactList<IndexItem*,QList<bool> >
              (QList<IndexItem*>&, const QList<bool>&); 

template extern void CxdCompact::compactList<bool,QList<bool> >
              (QList<bool>&, const QList<bool>&); 

template extern void CxdCompact::compactList<qint32,QList<bool> >
              (QList<qint32>&, const QList<bool>&); 
