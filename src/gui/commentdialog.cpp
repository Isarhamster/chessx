/****************************************************************************
*   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
****************************************************************************/

#include "commentdialog.h"

CommentDialog::CommentDialog(QWidget* parent) :
	 QDialog(parent)
{
	ui.setupUi(this);
}

QString CommentDialog::text() const
{
	return ui.textEdit->toPlainText().trimmed();
}

void CommentDialog::setText(const QString &text)
{
	ui.textEdit->setPlainText(text);
}
