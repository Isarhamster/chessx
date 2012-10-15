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
#include "enginelist.h"

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
    /** Restore Default values */
    void slotReset();
	/** Execute dialog, save options on exit */
	int exec();
	void slotApply();
	/** select given engine # for display */
	void slotSelectEngine(QListWidgetItem* current, QListWidgetItem* previous);
	/** user asked for a new engine entry */
	void slotAddEngine();
	/** user asked to remove current engine entry */
	void slotDeleteEngine();
	/** user changed the name of an engine */
	void slotEngineUp();
	/** User asked to move current engine up in the list. */
	void slotEngineDown();
	/** User asked to move current engine down in the list. */
	void slotEngineNameChange(const QString&);
	/** user wants to use file dialog to select engine command */
	void slotSelectEngineCommand();
	/** user wants file dialog to select directory in which engine will run */
	void slotSelectEngineDirectory();
    /** user wants file dialog to select directory in which DataBases will be stored */
    void slotSelectDataBasePath();
    /** user wants option dialog to select parameters which will be sent at startup of engine */
    void slotShowOptionDialog();
protected:
	/** Overridden to save size */
    virtual void closeEvent(QCloseEvent* e);
    /** Overridden to save size */
    virtual void done(int);

private:
	Ui::PreferencesDialog ui;
	void restoreSettings();
	void saveSettings();
	/** Select given item in combo. If not found, select last one. */
	bool selectInCombo(QComboBox* combo, const QString& text);
	void restoreColorItem(ColorList* list, const QString& text, const QString& cfgname,
					const QColor& cfgcolor);
	void saveColorList(ColorList* list, const QStringList& cfgnames);

	EngineList engineList;
	/** Store any changes made to Engine fields in UI, into list */
	void updateEngineData(int index);
	/** Select engine file. */
	QString selectEngineFile(const QString& path = "");
    static int s_lastIndex;

signals:
	/** Signal emitted when changes are applied. */
	void reconfigure();
};


#endif

