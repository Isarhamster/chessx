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
#include "settings.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

CopyDialog::CopyDialog(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    ui.setupUi(this);
    connect(ui.databaseList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(accept()));
    AppSettings->layout(this);
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

void CopyDialog::setMode(SrcMode mode)
{
    switch(mode)
    {
    case SingleGame:
        if (ui.singleButton->isEnabled())
        {
            ui.singleButton->setChecked(true);
            break;
        }
        // fallthru
    case Selection:
        if (ui.selectedButton->isEnabled())
        {
            ui.selectedButton->setChecked(true);
            break;
        }
        // fallthru
    case AllGames:
        ui.allButton->setChecked(true);
        break;
    case Filter:
        ui.filterButton->setChecked(true);
        break;
    }
}

void CopyDialog::setCurrentGame(QString title, int selectedGames)
{
    if (title.isEmpty())
    {
        ui.singleButton->setEnabled(false);
    }
    else
    {
        ui.singleButton->setText(ui.singleButton->text()+" (" + title + ")");
    }

    if (!selectedGames)
    {
        ui.selectedButton->setEnabled(false);
    }
    else
    {
        ui.selectedButton->setText(ui.selectedButton->text()+" (" + QString::number(selectedGames) + ")");
    }
}

int CopyDialog::getMode() const
{
    if(ui.singleButton->isChecked())
    {
        return SingleGame;
    }
    else if(ui.selectedButton->isChecked())
    {
        return Selection;
    }
    else if(ui.filterButton->isChecked())
    {
        return Filter;
    }
    else
    {
        return AllGames;
    }
}

void CopyDialog::accept()
{
    AppSettings->setLayout(this);
    QDialog::accept();
}

void CopyDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}
