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
class DatabaseInfo;

/** @ingroup GUI
   The ChessBrowser class is a slightly modified QTextBrowser
   that handles internal pseudo-links. */
class ChessBrowser : public QTextBrowser
{
	Q_OBJECT
public:
	enum Action {NoAction, RemovePreviousMoves, RemoveNextMoves, RemoveVariation, AddNag,
		EditPrecomment, EditComment};

	/** Constructs new instance with parent @p parent. If @p showGameMenu is false, game menu is never shown. */
	ChessBrowser(QWidget* p, bool showGameMenu = false);
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
	/** Database changed */
	void slotDatabaseChanged(DatabaseInfo* dbInfo);

signals:
	void actionRequested(int action, int move);

protected:
	virtual void selectAnchor(const QString& href);
	virtual void setSource(const QUrl& url);
	void setupMenu(bool setupGameMenu);
virtual QAction* createAction(const QString& name, int action);
private:
	/** Calculates if 'remove current variation' menu item should be disabled. 
		It is called just before showing context game menu. */
	bool removeVariationDisabled() const;

	QAction* m_smallfont;
	QAction* m_removeVariation;
	QMenu* m_gameMenu;
	QMenu* m_mainMenu;
	int m_currentMove;
	DatabaseInfo* m_databaseInfo;
};

#endif

