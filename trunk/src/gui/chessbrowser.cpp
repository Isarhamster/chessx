/***************************************************************************
 *   (C) 2006 by Michal Rudolf mrudolf@kdewebdev.org                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "chessbrowser.h"
#include <QMouseEvent>
#include <QTextBlock>

ChessBrowser::ChessBrowser(QWidget *p) : QTextBrowser(p)
{}

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

