/***************************************************************************
 *   (C) 2006 by Michal Rudolf mrudolf@kdewebdev.org                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "chessbrowser.h"
#include "settings.h"
#include "game.h"
#include "databaseinfo.h"

#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QTextBlock>

ChessBrowser::ChessBrowser(QWidget *p, bool showGameMenu) : QTextBrowser(p), m_gameMenu(NULL), m_databaseInfo(NULL)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	setupMenu(showGameMenu);
}

void ChessBrowser::setSource(const QUrl&)
{}

void ChessBrowser::showMove(int id)
{
	if (id)
		selectAnchor(QString("move:%1").arg(id));
	else {	// First move
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::Start);
		setTextCursor(cursor);
	}
}

void ChessBrowser::selectAnchor(const QString& href)
{
	for (QTextBlock block = document()->begin(); block != document()->end(); block = block.next()) {
		QTextBlock::iterator it;
		for (it = block.begin(); !it.atEnd(); ++it) {
			QTextFragment fragment = it.fragment();
			if (!fragment.isValid())
				continue;
			QTextCharFormat format = fragment.charFormat();
			if (format.isAnchor() && format.anchorHref() == href) {
				QTextCursor cursor(document());
				cursor.setPosition(fragment.position());
				cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
				setTextCursor(cursor);
				ensureCursorVisible();
				return;
			}
		}
	}
}

void ChessBrowser::saveConfig()
{
	AppSettings->setLayout(this);
	AppSettings->beginGroup(objectName());
	AppSettings->setValue("SmallFont", m_smallfont->isChecked());
}

void ChessBrowser::slotReconfigure()
{
	AppSettings->layout(this);
	AppSettings->beginGroup(objectName());
	if (AppSettings->value("SmallFont", false).toBool())
		m_smallfont->setChecked(true);
	AppSettings->endGroup();
}

QAction* ChessBrowser::createAction(const QString& name, int data)
{
	QAction* action = new QAction(name, this);
	action->setData(data);
	return action;
}

void ChessBrowser::setupMenu(bool setupGameMenu)
{
	if (setupGameMenu) {
		m_gameMenu = new QMenu(this);
		connect(m_gameMenu, SIGNAL(triggered(QAction*)), SLOT(slotAction(QAction*)));

		m_gameMenu->addAction(createAction(tr("Edit preceeding comment..."), EditPrecomment));
		m_gameMenu->addAction(createAction(tr("Edit comment..."), EditComment));

		QMenu* remove = m_gameMenu->addMenu(tr("Remove"));
		remove->addAction(createAction(tr("Previous moves"), RemovePreviousMoves));
		remove->addAction(createAction(tr("Next moves"), RemoveNextMoves));
		m_removeVariation = createAction(tr("Current variation"), RemoveVariation);
		remove->addAction(m_removeVariation);
	}
	
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));

	m_mainMenu = new QMenu(this);
	m_smallfont = createAction(tr("Small font"), NoAction);
	m_smallfont->setCheckable(true);
	m_smallfont->setChecked(false);
	connect(m_smallfont, SIGNAL(toggled(bool)), SLOT(slotToggleFont(bool)));
	m_mainMenu->addAction(m_smallfont);
}

void ChessBrowser::slotContextMenu(const QPoint& pos)
{
	if (m_gameMenu) {
		QString link = anchorAt(pos);
		if (!link.isEmpty()) {
			m_currentMove = link.section(':', 1).toInt();
			m_removeVariation->setDisabled(removeVariationDisabled());
			m_gameMenu->exec(mapToGlobal(pos));
			return;
		} 
	}
	
	m_mainMenu->exec(mapToGlobal(pos));
}

void ChessBrowser::slotDatabaseChanged(DatabaseInfo* dbInfo)
{
	m_databaseInfo = dbInfo;
}

void ChessBrowser::slotToggleFont(bool toggled)
{
	if (toggled)
		zoomOut();
	else
		zoomIn();
}

void ChessBrowser::slotAction(QAction* action)
{
	if (action->data().toInt() != NoAction)
		emit actionRequested(action->data().toInt(), m_currentMove);
}

bool ChessBrowser::removeVariationDisabled() const
{
	if (!m_databaseInfo)
		return true;
	return m_databaseInfo->currentGame().isMainline(m_currentMove);
}
