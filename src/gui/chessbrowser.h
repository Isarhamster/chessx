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

#include <QtGui>
#include <QTextBrowser>
#include "editaction.h"

class DatabaseInfo;
class QToolBar;

/** @ingroup GUI
	The ChessBrowser class is a slightly modified QTextBrowser
	that handles internal pseudo-links. */
class ChessBrowser : public QTextBrowser
{
	Q_OBJECT
public:
	/** Constructs new instance with parent @p parent. If @p showGameMenu is false, game menu is never shown. */
	ChessBrowser(QWidget* p, bool showGameMenu = false);
    QToolBar* toolBar;
public slots:
	/** Store current configuration. */
	void saveConfig();
	/** Restore current configuration. */
	void slotReconfigure();
	/** Scroll to show given mode. */
	void showMove(int id);

	/** Invoke action */
	void slotAction(QAction* action);
	/** Show menu */
	void slotContextMenu(const QPoint& pos);
    /** Show the time in the String for the player with @p color */
    void slotDisplayTime(const QString& text, Color color);

signals:
	void actionRequested(const EditAction& action);
    void queryActiveGame(const Game* game);

protected:
	virtual void selectAnchor(const QString& href);
	virtual void setSource(const QUrl& url);
	void setupMenu(bool setupGameMenu);
	QAction* createAction(const QString& name, EditAction::Type type);
	QAction* createNagAction(const Nag& nag);

    void setFontSize(int size);

private:
	QMap<QAction*, EditAction> m_actions;

	QAction* m_startComment;
	QAction* m_addComment;
	QAction* m_removeVariation;
	QAction* m_promoteVariation;
    QAction* m_VariationUp;
    QAction* m_VariationDown;
	QAction* m_removePrevious;
	QAction* m_removeNext;
    QAction* m_addNullMove;
	QAction* m_removeNags;
    QAction* m_enumerateVariations1;
    QAction* m_enumerateVariations2;
	QMenu* m_gameMenu;
	QMenu* m_mainMenu;
	int m_currentMove;
    int m_fontSize;

};

#endif

