/***************************************************************************
                          chessbrowser.h  -  Tweaked QTextBrowser
                             -------------------
    begin                : Thu 31 Aug 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef __CHESSBROWSER_H__
#define __CHESSBROWSER_H__

#include <q3textbrowser.h>
#include <QMouseEvent>

class ChessBrowser : public Q3TextBrowser
{
  Q_OBJECT
  public:
    ChessBrowser(QWidget* p, const char* name = 0);
  protected:
    virtual void contentsMousePressEvent(QMouseEvent* e);
  signals:
    void linkPressed(const QString& name);
};

#endif

