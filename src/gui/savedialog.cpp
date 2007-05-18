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

int SaveDialog::exec(Game& game)
{
	ui.whiteCombo->setEditText(game.tag("White"));
	ui.blackEloEdit->setText(game.tag("WhiteElo"));
	ui.blackCombo->setEditText(game.tag("Black"));
	ui.blackEloEdit->setText(game.tag("BlackElo"));
	ui.eventCombo->setEditText(game.tag("Event"));
	ui.siteCombo->setEditText(game.tag("Site"));
	ui.roundEdit->setText(game.tag("Round"));
	ui.dateEdit->setText(game.tag("Date"));
	int result = QDialog::exec();
	if (result) {
		game.setTag("White", ui.whiteCombo->currentText());
		game.setTag("WhiteElo", ui.whiteEloEdit->text());
		game.setTag("Black", ui.blackCombo->currentText());
		game.setTag("BlackElo", ui.blackEloEdit->text());
		game.setTag("Event", ui.eventCombo->currentText());
		game.setTag("Site", ui.siteCombo->currentText());
		game.setTag("Round", ui.roundEdit->text());
		game.setTag("Date", ui.dateEdit->text());
	}
	return result;
}

