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

#include "colorlist.h"
#include "preferences.h"
#include "settings.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QPainter>
#include <QPaintEvent>
#include <QDir>
#include <QPushButton>
#include <QSpinBox>
#include <QFileDialog>

PreferencesDialog::PreferencesDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

#ifndef Q_WS_WIN
	ui.engineProtocolWinBoard->setText(tr("XBoard"));
#endif

	connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
	connect(ui.engineList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  SLOT(slotSelectEngine(QListWidgetItem*, QListWidgetItem*)));
	connect(ui.engineName, SIGNAL(textChanged(const QString&)), SLOT(slotEngineNameChange(const QString&)));
	connect(ui.newEngineButton, SIGNAL(clicked(bool)), SLOT(slotNewEngine()));
	connect(ui.deleteEngineButton, SIGNAL(clicked(bool)), SLOT(slotDeleteEngine()));
	connect(ui.engineUpButton, SIGNAL(clicked(bool)), SLOT(slotEngineUp()));
	connect(ui.engineDownButton, SIGNAL(clicked(bool)), SLOT(slotEngineDown()));
	connect(ui.directoryButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineDirectory()));
	connect(ui.commandButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineCommand()));

	restoreSettings();
	AppSettings->beginGroup("/Board/");
	restoreColorItem(ui.boardColorsList, tr("Light squares"), "lightColor", "#a0a0a0");
	restoreColorItem(ui.boardColorsList, tr("Dark squares"), "darkColor", "#d0d0d0");
	restoreColorItem(ui.boardColorsList, tr("Highlighted squares"), "highlightColor", "#ffff00");
	restoreColorItem(ui.boardColorsList, tr("Frame"), "frameColor", "#000000");
	AppSettings->endGroup();

	// Start off with no Engine selected
	ui.engineEditWidget->setEnabled(false);
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::slotSelectEngineDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(
			this,
			tr("Select Directory in which Engine will run"),
			ui.engineDirectory->text(),
			QFileDialog::ShowDirsOnly);
	if (!dir.isEmpty())
		ui.engineDirectory->setText(dir);
}

void PreferencesDialog::slotSelectEngineCommand()
{
	QString com = QFileDialog::getOpenFileName(
			this,
			tr("Select engine command file to run"),
			ui.engineCommand->text());
	if (!com.isEmpty()) {
		ui.engineCommand->setText(com);
		if (engineData[ui.engineList->currentIndex().row()].name == tr("New Engine")) {
			QString name = com.section('/', -1, -1);
			if (!name.isEmpty())
				name[0] = name[0].toUpper();
			if (name.endsWith(".exe"))
				name.truncate(name.length() - 4);
			ui.engineName->setText(name);
		}
	}
}

void PreferencesDialog::slotDeleteEngine()
{
	int row = ui.engineList->currentRow();
	if (row >= 0) {
		QListWidgetItem *del = ui.engineList->takeItem(row);
		delete del;
		engineData.removeAt(row);
	}
}

void PreferencesDialog::slotNewEngine()
{
	int newnum = engineData.count();
	ui.engineList->insertItem(newnum, tr("New Engine"));
	engineData.append(EngineData(tr("New Engine")));
	ui.engineList->setCurrentRow(newnum);
}

void PreferencesDialog::slotEngineNameChange(const QString& name)
{
	ui.engineList->currentItem()->setText(name);
	engineData[ui.engineList->currentIndex().row()].name = name;
}

void PreferencesDialog::slotEngineUp()
{
	int index = ui.engineList->currentIndex().row();
	if (index > 0) {
		engineData.swap(index, index - 1);
		QListWidgetItem* item = ui.engineList->takeItem(index-1);
		ui.engineList->insertItem(index, item);
	}
}

void PreferencesDialog::slotEngineDown()
{
	int index = ui.engineList->currentIndex().row();
	if (index < ui.engineList->count() - 1) {
		engineData.swap(index, index + 1);
		QListWidgetItem* item = ui.engineList->takeItem(index+1);
		ui.engineList->insertItem(index, item);
	}
}

void PreferencesDialog::updateEngineData(int index)
{
	if (index < 0 || index >= engineData.count())
		return;
	engineData[index].name = ui.engineName->text();
	engineData[index].command = ui.engineCommand->text();
	engineData[index].options = ui.engineOptions->text();
	engineData[index].directory = ui.engineDirectory->text();
	engineData[index].protocol = ui.engineProtocolWinBoard->isChecked() ?
			WinBoard : UCI;
}

void PreferencesDialog::slotSelectEngine(QListWidgetItem* current, QListWidgetItem* previous)
{
	// Store any edits done to current data
	if (previous)
		updateEngineData(ui.engineList->row(previous));

	if (current) {
		ui.engineEditWidget->setEnabled(true);
		// Fill edit fields with data for selected engine
		int index = ui.engineList->row(current);
		ui.engineName->setText(engineData[index].name);
		ui.engineCommand->setText(engineData[index].command);
		ui.engineOptions->setText(engineData[index].options);
		ui.engineDirectory->setText(engineData[index].directory);
		if (engineData[index].protocol == WinBoard)
			ui.engineProtocolWinBoard->setChecked(true);
		else	ui.engineProtocolUCI->setChecked(true);
	} else {
		ui.engineName->clear();
		ui.engineCommand->clear();
		ui.engineOptions->clear();
		ui.engineDirectory->clear();
		ui.engineProtocolUCI->setChecked(true);
		ui.engineEditWidget->setEnabled(false);
	}
}

int PreferencesDialog::exec()
{
	int result = QDialog::exec();
	if (result == QDialog::Accepted) {
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

void PreferencesDialog::restoreSettings()
{
	// Read Board settings
	AppSettings->beginGroup("/General/");
	ui.quitCheck->setChecked(AppSettings->value("confirmQuit", true).toBool());
	ui.tablebaseCheck->setChecked(AppSettings->value("onlineTablebases", true).toBool());
	AppSettings->endGroup();
	AppSettings->beginGroup("/Tips/");
	ui.tipCheck->setChecked(AppSettings->value("showTips", true).toBool());
	AppSettings->endGroup();
	AppSettings->beginGroup("/Board/");
	ui.boardFrameCheck->setChecked(AppSettings->value("showFrame", true).toBool());
	ui.guessMoveCheck->setChecked(AppSettings->value("guessMove", true).toBool());
	QString pieceTheme = AppSettings->value("pieceTheme", "merida").toString();
	QString pieceEffect = AppSettings->value("pieceEffect", "shadow").toString();
	QString boardTheme = AppSettings->value("boardTheme", "slate").toString();
	AppSettings->endGroup();

	QString themeDir(AppSettings->dataPath() + "/themes");
	QStringList themes = QDir(themeDir).entryList(QStringList("*.png"));
	for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it) {
		(*it).truncate((*it).length() - 4);
		ui.pieceThemeCombo->addItem(*it);
	}

	themes = QDir(themeDir+"/boards").entryList(QStringList("*.png"));
	QStringListIterator it(themes);
	while (it.hasNext()) {
		QString trim(it.next());
		ui.boardThemeCombo->addItem(trim.left(trim.length() - 4));
	}
	ui.boardThemeCombo->addItem(tr("[plain colors]"));

	ui.pieceEffect->addItem(tr("plain"));
	ui.pieceEffect->addItem(tr("outline"));
	ui.pieceEffect->addItem(tr("shadow"));

	selectInCombo(ui.pieceThemeCombo, pieceTheme);
	selectInCombo(ui.boardThemeCombo, boardTheme);
	selectInCombo(ui.pieceEffect, pieceEffect);

	// Read Players settings
	AppSettings->beginGroup("/Players/");
	ui.playersRatingsCheck->setChecked(AppSettings->value("rating", true).toBool());
	AppSettings->endGroup();

	// Read Engine settings
	QStringList lnames;
	AppSettings->beginGroup("/Engines/");
	QStringList engines = AppSettings->childGroups();
	for (int i=0; i < engines.size(); ++i) {
		QString key = engines[i];
		QString name = AppSettings->value(key + "/Name").toString();
		lnames.append(name);
		engineData.append(EngineData(name));
		engineData[i].command = AppSettings->value(key + "/Command").toString();
		engineData[i].options = AppSettings->value(key + "/Options").toString();
		engineData[i].directory = AppSettings->value(key + "/Directory").toString();
		QString protocolName = AppSettings->value(key + "/Protocol").toString();
		if (protocolName == "WinBoard")
			engineData[i].protocol = WinBoard;
		else	engineData[i].protocol = UCI;
	}
	AppSettings->endGroup();

	ui.engineList->insertItems(0, lnames);
}

void PreferencesDialog::saveSettings()
{
	AppSettings->beginGroup("/General/");
	AppSettings->setValue("confirmQuit", ui.quitCheck->isChecked());
	AppSettings->setValue("onlineTablebases", ui.tablebaseCheck->isChecked());
	AppSettings->endGroup();
	AppSettings->beginGroup("/Tips/");
	AppSettings->setValue("showTips", ui.tipCheck->isChecked());
	AppSettings->endGroup();
	AppSettings->beginGroup("/Board/");
	AppSettings->setValue("showFrame", ui.boardFrameCheck->isChecked());
	AppSettings->setValue("guessMove", ui.guessMoveCheck->isChecked());
	AppSettings->setValue("pieceTheme", ui.pieceThemeCombo->currentText());
	AppSettings->setValue("pieceEffect", ui.pieceEffect->currentText());
	if (ui.boardThemeCombo->currentIndex() != ui.boardThemeCombo->count() - 1)
		AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
	else	AppSettings->setValue("boardTheme", QString());
	QStringList colorNames;
	colorNames << "lightColor" << "darkColor" << "highlightColor" << "frameColor";
	saveColorList(ui.boardColorsList, colorNames);
	AppSettings->endGroup();
	AppSettings->beginGroup("/Players/");
	AppSettings->setValue("rating", ui.playersRatingsCheck->isChecked());
	AppSettings->endGroup();

	// Save engine settings
	updateEngineData(ui.engineList->currentIndex().row());  // Make sure current edits are saved
	AppSettings->beginGroup("/Engines/");
	AppSettings->remove("");
	for (int i=0; i < engineData.size(); ++i) {
		QString key = QString::number(i);
		AppSettings->setValue(key + "/Name", engineData[i].name);
		AppSettings->setValue(key + "/Command", engineData[i].command);
		AppSettings->setValue(key + "/Options", engineData[i].options);
		AppSettings->setValue(key + "/Directory", engineData[i].directory);
		if (engineData[i].protocol == WinBoard)
			AppSettings->setValue(key + "/Protocol", "WinBoard");
		else	AppSettings->setValue(key + "/Protocol", "UCI");
	}
	AppSettings->endGroup();

}

bool PreferencesDialog::selectInCombo(QComboBox* combo, const QString& text)
{
	for (int i = 0; i < combo->count(); i++)
		if (combo->itemText(i) == text) {
			combo->setCurrentIndex(i);
			return true;
		}
	combo->setCurrentIndex(combo->count() - 1);
	return false;
}
void PreferencesDialog::restoreColorItem(ColorList* list, const QString& text, const QString& cfgname, const QColor& cfgcolor)
{
	QColor color = AppSettings->value(cfgname, cfgcolor).value<QColor>();
	list->addItem(text, color);
}

void PreferencesDialog::saveColorList(ColorList* list, const QStringList& cfgnames)
{
	for (int i = 0; i < list->count(); i++)
		AppSettings->setValue(cfgnames[i], list->color(i));
}

