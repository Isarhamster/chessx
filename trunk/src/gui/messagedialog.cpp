/***************************************************************************
   MessageDialog - wrapper for QMessageDialog
   (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>

   This file is a part of Chessx (http:/chess.sourceforge.net).
 	ChessX is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 3,
	as published by the Free Software Foundation.
***************************************************************************/

#include "messagedialog.h"
#include <QMessageBox>
#include <QPushButton>

void MessageDialog::error(QWidget* parent, const QString& text, const QString& title)
{
	QMessageBox::critical(parent, title, text);
}

void MessageDialog::warning(QWidget* parent, const QString& text, const QString& title)
{
	QMessageBox::warning(parent, title, text);
}

void MessageDialog::information(QWidget* parent, const QString& text, const QString& title)
{
	QMessageBox::information(parent, title, text);
}

bool MessageDialog::yesNo(QWidget* parent, const QString& text, const QString& title, const QString& yes,
									 const QString& no)
{
	QMessageBox mb(parent);
	mb.setWindowTitle(title);
	mb.setText(text);
	mb.setIcon(QMessageBox::Question);
	QPushButton* y = mb.addButton(yes, QMessageBox::ActionRole);
	mb.setDefaultButton(y);
	mb.addButton(no, QMessageBox::RejectRole);
	mb.exec();
	return mb.clickedButton() == y;
}

bool MessageDialog::okCancel(QWidget* parent, const QString& text, const QString& title, const QString& ok,
								  const QString& cancel)
{
	QMessageBox mb(parent);
	mb.setWindowTitle(title);
	mb.setText(text);
	mb.setIcon(QMessageBox::Question);
	QPushButton* o = mb.addButton(ok, QMessageBox::ActionRole);
	mb.setDefaultButton(o);
	QPushButton* c = mb.addButton(cancel, QMessageBox::RejectRole);
	mb.setEscapeButton(c);
	mb.exec();
	return mb.clickedButton() == o;
}

