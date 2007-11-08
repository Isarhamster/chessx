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

class QMenu;

/** @ingroup GUI
   The ChessBrowser class is a slightly modified QTextBrowser
   that handles internal pseudo-links. */
class ChessBrowser : public QTextBrowser
{
	Q_OBJECT
public:
	enum Action {NoAction, RemovePreviousMoves, RemoveNextMoves, RemoveVariation, AddNag};
	ChessBrowser(QWidget* p);
public slots:
	/** Store current configuration. */
	void saveConfig();
	/** Restore current configuration. */
	void slotReconfigure();
	/** Scroll to show given mode. */
	void showMove(int id);
	/** Toggle font */
	void slotToggleFont(bool toggled);
	/** Invoke action */
	void slotAction(QAction* action);
	/** Show menu */
	void slotContextMenu(const QPoint& pos);

signals:
	void actionRequested(int action, int move);

protected:
	virtual void selectAnchor(const QString& href);
	virtual void setSource(const QUrl& url);
	virtual void setupMenu();
virtual QAction* createAction(const QString& name, int action);
private:
	QAction* m_smallfont;
	QMenu* m_popup;
	int m_currentMove;
};

#endif

