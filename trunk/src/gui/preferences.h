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

#ifndef __PREFERENCESDIALOG_H__
#define __PREFERENCESDIALOG_H__

#include "ui_preferences.h"

class QComboBox;

class PreferencesDialog : public QDialog
{
  Q_OBJECT
public:
  /** Create ChessX Preferences dialog */
  PreferencesDialog(QWidget* parent = 0);
  /** Destroy the dialog */
  virtual ~PreferencesDialog();

public slots:
  /** Execute dialog, save options on exit */
  int exec();
  void slotBoardLightColor();
  void slotBoardDarkColor();
  void slotBoardMode(int mode);

private:
  Ui::PreferencesDialog ui;
  void restoreSettings();
  void saveSettings();
  bool selectInCombo(QComboBox* combo, const QString& text);
  void setButtonColor(QPushButton* button, const QColor& color);
  QColor buttonColor(const QPushButton* button) const;

signals:
  void reconfigure();
};




#endif

