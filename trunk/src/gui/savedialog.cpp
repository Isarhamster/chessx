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

#include <qlineedit.h>
#include <qcombobox.h>


SaveDialog::SaveDialog(QWidget* parent) : SaveDialogBase(parent)
{
}

SaveDialog::~SaveDialog()
{
}

int SaveDialog::exec(Game* game)
{
  whiteCombo->setCurrentText(game->tag("White"));
  blackEloEdit->setText(game->tag("WhiteElo"));
  blackCombo->setCurrentText(game->tag("Black"));
  blackEloEdit->setText(game->tag("BlackElo"));
  eventCombo->setCurrentText(game->tag("Event"));
  siteCombo->setCurrentText(game->tag("Site"));
  roundEdit->setText(game->tag("Round"));
  dateEdit->setText(game->tag("Date"));
  int result = QDialog::exec();
  if (result)
  {
    game->setTag("White", whiteCombo->currentText());
    game->setTag("WhiteElo", whiteEloEdit->text());
    game->setTag("Black", blackCombo->currentText());
    game->setTag("BlackElo", blackEloEdit->text());
    game->setTag("Event", eventCombo->currentText());
    game->setTag("Site", siteCombo->currentText());
    game->setTag("Round", roundEdit->text());
    game->setTag("Date", dateEdit->text());
  }
  return result;
}

