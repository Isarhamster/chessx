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

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDir>
#include <QPushButton>
#include <QSpinBox>

PreferencesDialog::PreferencesDialog(QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
  connect(ui.boardLightButton, SIGNAL(clicked()), SLOT(slotBoardColor()));
  connect(ui.boardDarkButton, SIGNAL(clicked()), SLOT(slotBoardColor()));
  connect(ui.boardHighlightButton, SIGNAL(clicked()), SLOT(slotBoardColor()));
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
  {
    saveSettings();
    emit reconfigure();
  }
  return result;
}

void PreferencesDialog::slotApply()
{
  saveSettings();
  emit reconfigure();
}

void PreferencesDialog::slotBoardColor()
{
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  if (button)
  {
    QColor color = QColorDialog::getColor(buttonColor(button));
    setButtonColor(button, color);
  }
}

void PreferencesDialog::slotBoardMode(int mode)
{
  bool theme = mode != 2;
  ui.lightLabel->setVisible(!theme);
  ui.darkLabel->setVisible(!theme);
  ui.darkLabel->setVisible(!theme);
  ui.boardLightButton->setVisible(!theme);
  ui.boardDarkButton->setVisible(!theme);
  ui.boardThemeLabel->setVisible(theme);
  ui.boardThemeCombo->setVisible(theme);
}

void PreferencesDialog::restoreSettings()
{
  // Read Board settings
  AppSettings->beginGroup("/Board/");
  ui.boardTypeCombo->setCurrentIndex(AppSettings->value("squareType", 0).toInt());
  ui.boardFrameCheck->setChecked(AppSettings->value("showFrame", true).toBool());
  ui.guessMoveCheck->setChecked(AppSettings->value("guessMove", true).toBool());
  QString pieceTheme = AppSettings->value("pieceTheme", "default").toString();
  QString boardTheme = AppSettings->value("boardTheme", "default").toString();
  QVariant color = AppSettings->value("lightColor", "#d0d0d0");
  setButtonColor(ui.boardLightButton, color.value<QColor>());
  color = AppSettings->value("darkColor", "#a0a0a0");
  setButtonColor(ui.boardDarkButton, color.value<QColor>());
  color = AppSettings->value("highlightColor", "#ffff00");
  setButtonColor(ui.boardHighlightButton, color.value<QColor>());
  AppSettings->endGroup();

  QStringList themes = QDir(AppSettings->dataPath() + "/themes").entryList(QStringList("*.png"));
  for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it)
  {
    (*it).truncate((*it).length() - 4);
    ui.pieceThemeCombo->addItem(*it);
    ui.boardThemeCombo->addItem(*it);
  }
  selectInCombo(ui.pieceThemeCombo, pieceTheme);
  selectInCombo(ui.boardThemeCombo, boardTheme);

  // Read Players settings
  AppSettings->beginGroup("/Players/");
  ui.playersRatingsCheck->setChecked(AppSettings->value("rating", true).toBool());
  ui.playersSpinbox->setValue(AppSettings->value("count", 100).toInt());
  AppSettings->endGroup();

  slotBoardMode(ui.boardTypeCombo->currentIndex());
}

void PreferencesDialog::saveSettings()
{
  AppSettings->beginGroup("/Board/");
  AppSettings->setValue("squareType", ui.boardTypeCombo->currentIndex());
  AppSettings->setValue("showFrame", ui.boardFrameCheck->isChecked());
  AppSettings->setValue("guessMove", ui.guessMoveCheck->isChecked());
  AppSettings->setValue("pieceTheme", ui.pieceThemeCombo->currentText());
  AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
  AppSettings->setValue("lightColor", buttonColor(ui.boardLightButton));
  AppSettings->setValue("highlightColor", buttonColor(ui.boardHighlightButton));
  AppSettings->setValue("darkColor", buttonColor(ui.boardDarkButton));
  AppSettings->endGroup();
  AppSettings->beginGroup("/Players/");
  AppSettings->setValue("rating", ui.playersRatingsCheck->isChecked());
  AppSettings->setValue("count", ui.playersSpinbox->value());
  AppSettings->endGroup();
}

bool PreferencesDialog::selectInCombo(QComboBox* combo, const QString& text)
{
  for (int i = 0; i<combo->count(); i++)
    if (combo->itemText(i) == text)
    {
      combo->setCurrentIndex(i);
      return true;
    }
  return false;
}

void PreferencesDialog::setButtonColor(QPushButton* button, const QColor& color)
{
  if (color.isValid())
  {
     QPalette palette;
     palette.setColor(button->backgroundRole(), color);
     button->setPalette(palette);
  }
}

QColor PreferencesDialog::buttonColor(const QPushButton* button) const
{
  return button->palette().color(button->backgroundRole());
}

