/***************************************************************************
                          Savedialog  -  dialog for saving game
                             -------------------
    begin                : Mon 17 Apr 2006
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

#include "savedialog.h"
#include "game.h"

#include <QComboBox>
#include <QLineEdit>


SaveDialog::SaveDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
}

SaveDialog::~SaveDialog()
{}


QString formatTagValue(const QString& s)
{
	return (s.trimmed().isEmpty()) ? "?" : s;
}

QString formatTagDate(const QString& s)
{
	return (s.trimmed().isEmpty()) ? "????.??.??" : s;
}

int SaveDialog::exec(Game& game)
{
	ui.whiteCombo->setEditText(game.tag("White"));
	ui.whiteEloEdit->setText(game.tag("WhiteElo"));
	ui.blackCombo->setEditText(game.tag("Black"));
	ui.blackEloEdit->setText(game.tag("BlackElo"));
	ui.eventCombo->setEditText(game.tag("Event"));
	ui.siteCombo->setEditText(game.tag("Site"));
	ui.roundEdit->setText(game.tag("Round"));
	ui.dateEdit->setText(game.tag("Date"));
	int result = QDialog::exec();
	if (result) {
		game.setTag("White", formatTagValue(ui.whiteCombo->currentText()));
		game.setTag("Black", formatTagValue(ui.blackCombo->currentText()));
		game.setTag("Event", formatTagValue(ui.eventCombo->currentText()));
		game.setTag("Site", formatTagValue(ui.siteCombo->currentText()));
		game.setTag("Round", formatTagValue(ui.roundEdit->text()));
		game.setTag("Date", formatTagDate(ui.dateEdit->text()));
		// Optional tag
		if (ui.whiteEloEdit->text().toInt() || game.tag("WhiteElo").toInt())
			game.setTag("WhiteElo", ui.whiteEloEdit->text());
		if (ui.whiteEloEdit->text().toInt() || game.tag("BlackElo").toInt())
			game.setTag("BlackElo", ui.blackEloEdit->text());
	}
	return result;
}

