/***************************************************************************
 *   (C) 2006 by Michal Rudolf mrudolf@kdewebdev.org                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "chessbrowser.h"
//Added by qt3to4:
#include <QMouseEvent>

ChessBrowser::ChessBrowser(QWidget *p, const char* name) : Q3TextBrowser(p, name)
{
  setLinkUnderline(false);
}

void ChessBrowser::contentsMousePressEvent(QMouseEvent* e)
{
  QString anchor = anchorAt(e->pos());
  if (!anchor.isEmpty() && e->button() == Qt::LeftButton)
    emit linkPressed(anchorAt(e->pos()));
  e->accept();
}

