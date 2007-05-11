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

#include <QTextBrowser>

/** @ingroup GUI
   The ChessBrowser class is a slightly modified QTextBrowser
   that handles internal pseudo-links. */
class ChessBrowser : public QTextBrowser
{
	Q_OBJECT
public:
	ChessBrowser(QWidget* p);
	virtual void selectAnchor(const QString& href);
protected:
	virtual void setSource(const QUrl& url);
private:
};

#endif

