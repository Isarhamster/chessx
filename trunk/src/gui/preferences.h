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

/** @ingroup GUI
The PreferencesDialog class is a dialog for configuring ChessX.
After changes are done, reconfigure() signal is emitted. */
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
	void slotBoardColor();
	void slotBoardMode(int mode);
	void slotApply();

private:
	Ui::PreferencesDialog ui;
	void restoreSettings();
	void saveSettings();
	bool selectInCombo(QComboBox* combo, const QString& text);
	void setButtonColor(QPushButton* button, const QColor& color);
	QColor buttonColor(const QPushButton* button) const;

signals:
	/** Signal emitted when changes are applied. */
	void reconfigure();
};




#endif

