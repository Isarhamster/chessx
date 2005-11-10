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
#include <qdir.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

PreferencesDialog::PreferencesDialog(QWidget* parent) : PreferencesDialogBase(parent)
{
  connect(okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

  // Read Board settings
  AppSettings->beginGroup("/Board/");
  boardFrameCheck->setChecked(AppSettings->readBoolEntry("showFrame", true));
  QString name = AppSettings->readEntry("theme", "default");
  AppSettings->endGroup();

  QStringList themes = QDir("../data/themes").entryList("*.png");
  for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it)
  {
    (*it).truncate((*it).length() - 4);
    boardThemeCombo->insertItem(*it);
  }
  for (int i = 0; i<boardThemeCombo->count(); i++)
    if (name == boardThemeCombo->text(i))
    {
      boardThemeCombo->setCurrentItem(i);
      break;
    }

  // Read Players settings
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
    AppSettings->beginGroup("/Board/");
    AppSettings->writeEntry("theme", boardThemeCombo->currentText());
    AppSettings->writeEntry("showFrame", boardFrameCheck->isChecked());
    AppSettings->endGroup();
    AppSettings->beginGroup("/Players/");
    AppSettings->writeEntry("rating", playersRatingsCheck->isChecked());
    AppSettings->writeEntry("count", playersSpinbox->value());
    AppSettings->endGroup();
  }
  return result;
}

