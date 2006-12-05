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
#include <q3groupbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <q3widgetstack.h>

PreferencesDialog::PreferencesDialog(QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.boardLightButton, SIGNAL(clicked()), SLOT(slotBoardLightColor()));
  connect(ui.boardDarkButton, SIGNAL(clicked()), SLOT(slotBoardDarkColor()));
  connect(ui.boardTypeCombo, SIGNAL(activated(int)), SLOT(slotBoardMode(int)));
  restoreSettings();
}

PreferencesDialog::~PreferencesDialog()
{
}

int PreferencesDialog::exec()
{
  int result = QDialog::exec();
  if (result == QDialog::Accepted)
    saveSettings();
  return result;
}

void PreferencesDialog::slotBoardLightColor()
{
  QColor light = QColorDialog::getColor(ui.boardLightButton->paletteBackgroundColor());
  if (light.isValid())
    ui.boardLightButton->setPaletteBackgroundColor(light);
}

void PreferencesDialog::slotBoardDarkColor()
{
  QColor dark = QColorDialog::getColor(ui.boardDarkButton->paletteBackgroundColor());
  if (dark.isValid())
    ui.boardDarkButton->setPaletteBackgroundColor(dark);
}

void PreferencesDialog::slotBoardMode(int mode)
{
  if (mode == 2)
    ui.widgetStack->raiseWidget(ui.plainBoardWidget);
  else
    ui.widgetStack->raiseWidget(ui.themeBoardWidget);
}

void PreferencesDialog::restoreSettings()
{
  // Read Board settings
  AppSettings->beginGroup("/Board/");
  ui.boardTypeCombo->setCurrentItem(AppSettings->readNumEntry("squareType", 0));
  ui.boardFrameCheck->setChecked(AppSettings->readBoolEntry("showFrame", true));
  QString pieceTheme = AppSettings->readEntry("pieceTheme", "default");
  QString boardTheme = AppSettings->readEntry("boardTheme", "default");
  ui.boardLightButton->setPaletteBackgroundColor(AppSettings->readEntry("lightColor", "#d0d0d0"));
  ui.boardDarkButton->setPaletteBackgroundColor(AppSettings->readEntry("darkColor", "#a0a0a0"));
  AppSettings->endGroup();

  QStringList themes = QDir(AppSettings->dataPath() + "/themes").entryList("*.png");
  for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it)
  {
    (*it).truncate((*it).length() - 4);
    ui.pieceThemeCombo->insertItem(*it);
    ui.boardThemeCombo->insertItem(*it);
  }
  selectInCombo(ui.pieceThemeCombo, pieceTheme);
  selectInCombo(ui.boardThemeCombo, boardTheme);

  // Read Players settings
  AppSettings->beginGroup("/Players/");
  ui.playersRatingsCheck->setChecked(AppSettings->readBoolEntry("rating", true));
  ui.playersSpinbox->setValue(AppSettings->readNumEntry("count", 100));
  AppSettings->endGroup();
}

void PreferencesDialog::saveSettings()
{
  AppSettings->beginGroup("/Board/");
  AppSettings->writeEntry("squareType", ui.boardTypeCombo->currentItem());
  AppSettings->writeEntry("showFrame", ui.boardFrameCheck->isChecked());
  AppSettings->writeEntry("pieceTheme", ui.pieceThemeCombo->currentText());
  AppSettings->writeEntry("boardTheme", ui.boardThemeCombo->currentText());
  AppSettings->writeEntry("lightColor", ui.boardLightButton->paletteBackgroundColor().name());
  AppSettings->writeEntry("darkColor", ui.boardDarkButton->paletteBackgroundColor().name());
  AppSettings->endGroup();
  AppSettings->beginGroup("/Players/");
  AppSettings->writeEntry("rating", ui.playersRatingsCheck->isChecked());
  AppSettings->writeEntry("count", ui.playersSpinbox->value());
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
