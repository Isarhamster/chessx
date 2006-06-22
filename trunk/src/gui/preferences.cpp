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
#include "../database/settings.h"

#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qwidgetstack.h>

PreferencesDialog::PreferencesDialog(QWidget* parent) : PreferencesDialogBase(parent)
{
  connect(okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(boardLightButton, SIGNAL(clicked()), SLOT(slotBoardLightColor()));
  connect(boardDarkButton, SIGNAL(clicked()), SLOT(slotBoardDarkColor()));
  connect(boardTypeCombo, SIGNAL(activated(int)), SLOT(slotBoardMode(int)));
  restoreSettings();
}

PreferencesDialog::~PreferencesDialog()
{
}

int PreferencesDialog::exec()
{
  int result = PreferencesDialogBase::exec();
  if (result == QDialog::Accepted)
    saveSettings();
  return result;
}

void PreferencesDialog::slotBoardLightColor()
{
  QColor light = QColorDialog::getColor(boardLightButton->paletteBackgroundColor());
  if (light.isValid())
    boardLightButton->setPaletteBackgroundColor(light);
}

void PreferencesDialog::slotBoardDarkColor()
{
  QColor dark = QColorDialog::getColor(boardDarkButton->paletteBackgroundColor());
  if (dark.isValid())
    boardDarkButton->setPaletteBackgroundColor(dark);
}

void PreferencesDialog::slotBoardMode(int mode)
{
  if (mode == 2)
    widgetStack->raiseWidget(plainBoardWidget);
  else
    widgetStack->raiseWidget(themeBoardWidget);
}

void PreferencesDialog::restoreSettings()
{
  // Read Board settings
  AppSettings->beginGroup("/Board/");
  boardTypeCombo->setCurrentItem(AppSettings->readNumEntry("squareType", 0));
  boardFrameCheck->setChecked(AppSettings->readBoolEntry("showFrame", true));
  QString pieceTheme = AppSettings->readEntry("pieceTheme", "default");
  QString boardTheme = AppSettings->readEntry("boardTheme", "default");
  boardLightButton->setPaletteBackgroundColor(AppSettings->readEntry("lightColor", "#d0d0d0"));
  boardDarkButton->setPaletteBackgroundColor(AppSettings->readEntry("darkColor", "#a0a0a0"));
  AppSettings->endGroup();

  QStringList themes = QDir(AppSettings->dataPath() + "/themes").entryList("*.png");
  for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it)
  {
    (*it).truncate((*it).length() - 4);
    pieceThemeCombo->insertItem(*it);
    boardThemeCombo->insertItem(*it);
  }
  selectInCombo(pieceThemeCombo, pieceTheme);
  selectInCombo(boardThemeCombo, boardTheme);

  // Read Players settings
  AppSettings->beginGroup("/Players/");
  playersRatingsCheck->setChecked(AppSettings->readBoolEntry("rating", true));
  playersSpinbox->setValue(AppSettings->readNumEntry("count", 100));
  AppSettings->endGroup();
}

void PreferencesDialog::saveSettings()
{
  AppSettings->beginGroup("/Board/");
  AppSettings->writeEntry("squareType", boardTypeCombo->currentItem());
  AppSettings->writeEntry("showFrame", boardFrameCheck->isChecked());
  AppSettings->writeEntry("pieceTheme", pieceThemeCombo->currentText());
  AppSettings->writeEntry("boardTheme", boardThemeCombo->currentText());
  AppSettings->writeEntry("lightColor", boardLightButton->paletteBackgroundColor().name());
  AppSettings->writeEntry("darkColor", boardDarkButton->paletteBackgroundColor().name());
  AppSettings->endGroup();
  AppSettings->beginGroup("/Players/");
  AppSettings->writeEntry("rating", playersRatingsCheck->isChecked());
  AppSettings->writeEntry("count", playersSpinbox->value());
  AppSettings->endGroup();
}

bool PreferencesDialog::selectInCombo(QComboBox* combo, const QString& text)
{
  for (int i = 0; i<combo->count(); i++)
    if (combo->text(i) == text)
    {
      combo->setCurrentItem(i);
      return true;
    }
  return false;
}
