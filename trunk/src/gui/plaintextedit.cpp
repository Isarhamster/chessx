/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#include "plaintextedit.h"


PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
}

void PlainTextEdit::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier) {
		event->ignore();
		return;
	}

	if (event->key() == Qt::Key_Return && event->modifiers() == Qt::ControlModifier)
		event->setModifiers(Qt::NoModifier);

	QPlainTextEdit::keyPressEvent(event);

}
