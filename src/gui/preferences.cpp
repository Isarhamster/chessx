/***************************************************************************
                          preferences  -  Preferences dialog
                             -------------------
    begin                : Thu 18 Aug 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "preferences.h"
#include "settings.h"

#include <qspinbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

PreferencesDialog::PreferencesDialog(QWidget* parent) : PreferencesDialogBase(parent)
{
  connect(okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
  
  /* read settings */
  AppSettings->beginGroup("/Players/");
  playersRatingsCheck->setChecked(AppSettings->readBoolEntry("rating", true));
  playersSpinbox->setValue(AppSettings->readNumEntry("count", 100));
  AppSettings->endGroup();
}

PreferencesDialog::~PreferencesDialog()
{
}

int PreferencesDialog::exec()
{
  int result = PreferencesDialogBase::exec();
  if (result == QDialog::Accepted)
  {
    AppSettings->beginGroup("/Players/");
    AppSettings->writeEntry("rating", playersRatingsCheck->isChecked());
    AppSettings->writeEntry("count", playersSpinbox->value());
    AppSettings->endGroup();
  }
  return result;
}
