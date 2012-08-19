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
#include "database.h"
#include "partialdate.h"
#include "messagedialog.h"

#include <QLineEdit>
#include <QCompleter>

SaveDialog::SaveDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	QButtonGroup* group = new QButtonGroup(this);
	group->addButton(ui.result1Button);
	group->addButton(ui.result5Button);
	group->addButton(ui.result0Button);
	group->addButton(ui.resultNoneButton);
}

SaveDialog::~SaveDialog()
{}


QString formatTagValue(const QString& s)
{
	return (s.trimmed().isEmpty()) ? "?" : s;
}

QString formatTagDate(const QString& s)
{
    return (s.trimmed().isEmpty()) ? PDInvalidDate.asString() : s;
}

int SaveDialog::exec(Database* database, Game& game)
{
	ui.whiteEdit->setText(game.tag("White"));
	ui.whiteEloEdit->setText(game.tag("WhiteElo"));
	ui.blackEdit->setText(game.tag("Black"));
	ui.blackEloEdit->setText(game.tag("BlackElo"));
	ui.eventEdit->setText(game.tag("Event"));
	ui.siteEdit->setText(game.tag("Site"));
	ui.roundEdit->setText(game.tag("Round"));
	ui.dateEdit->setText(game.tag("Date"));
	ui.eventDateEdit->setText(game.tag("EventDate"));
	QList<QAbstractButton*> buttons = ui.result1Button->group()->buttons();
	for (int i = 0; i < buttons.count(); i++)
		if (buttons[i]->text() == game.tag("Result"))
			buttons[i]->setChecked(true);
	// Completion
	setLineEdit(ui.whiteEdit, database, TagPlayerName);
	setLineEdit(ui.blackEdit, database, TagPlayerName);
	setLineEdit(ui.siteEdit, database, TagSite);
	setLineEdit(ui.eventEdit, database, TagEvent);
	int result = QDialog::exec();
    if (result == Accepted) {
		game.setTag("White", formatTagValue(ui.whiteEdit->text()));
		game.setTag("Black", formatTagValue(ui.blackEdit->text()));
		game.setTag("Event", formatTagValue(ui.eventEdit->text()));
		game.setTag("Site", formatTagValue(ui.siteEdit->text()));
		game.setTag("Round", formatTagValue(ui.roundEdit->text()));
		game.setTag("Date", formatTagDate(ui.dateEdit->text()));
		game.setTag("EventDate", formatTagDate(ui.eventDateEdit->text()));
		game.setTag("Result", ui.result1Button->group()->checkedButton()->text());
		// Optional tag
		if (ui.whiteEloEdit->text().toInt() || game.tag("WhiteElo").toInt())
			game.setTag("WhiteElo", ui.whiteEloEdit->text());
		if (ui.whiteEloEdit->text().toInt() || game.tag("BlackElo").toInt())
			game.setTag("BlackElo", ui.blackEloEdit->text());
	}
	return result;
}

void SaveDialog::setLineEdit(QLineEdit* edit, Database* database, Tag tag)
{
	QCompleter* completer = new QCompleter(database->index()->tagValues(tag), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	edit->setCompleter(completer);
}

void SaveDialog::accept()
{
    if ((PartialDate().fromString(ui.dateEdit->text()).isValid()) &&
        (PartialDate().fromString(ui.eventDateEdit->text()).isValid()))
    {
        //  dates are formatted properly
        QDialog::accept();
    }
    else
    {
        MessageDialog::error(tr("Dates are not properly formatted!","Invalid Data"));
    }
}
