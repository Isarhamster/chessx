/***************************************************************************
*   CopyDialog - dialog to choose target database when copying games       *
*   copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>  *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "copydialog.h"


CopyDialog::CopyDialog(QWidget * parent) : QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.databaseList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(accept()));
}

CopyDialog::~CopyDialog()
{
}

void CopyDialog::setDatabases(const QStringList& databases)
{
	ui.databaseList->clear();
	ui.databaseList->addItems(databases);
	ui.databaseList->setCurrentRow(0);
}

int CopyDialog::getDatabase() const
{
	return ui.databaseList->currentRow();
}

int CopyDialog::getMode() const
{
	if (ui.singleButton->isChecked())
		return SingleGame;
	else if (ui.filterButton->isChecked())
		return Filter;
	else return AllGames;
}

