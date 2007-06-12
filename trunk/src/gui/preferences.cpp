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

	connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
	connect(ui.engineList, SIGNAL(currentRowChanged(int)), SLOT(slotSelectEngine(int)));
	connect(ui.engineName, SIGNAL(textChanged(const QString&)), SLOT(slotEngineNameChange(const QString&)));
	connect(ui.newEngineButton, SIGNAL(clicked(bool)), SLOT(slotNewEngine()));
	connect(ui.deleteEngineButton, SIGNAL(clicked(bool)), SLOT(slotDeleteEngine()));
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
	currentEngineRow = -1;
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
	if (!com.isEmpty())
		ui.engineCommand->setText(com);
}

void PreferencesDialog::slotDeleteEngine()
{
	int row = ui.engineList->currentRow();
	if (row >= 0) {
		QListWidgetItem *del = ui.engineList->takeItem(row);
		delete del;
		engineData.removeAt(row);
		currentEngineRow = ui.engineList->currentRow();
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
	if (currentEngineRow >= 0) {
		ui.engineList->currentItem()->setText(name);
		engineData[currentEngineRow].name = name;
	}
}

void PreferencesDialog::updateCurrentEngineData()
{
	if (currentEngineRow >= 0) {
		engineData[currentEngineRow].name = ui.engineName->text();
		engineData[currentEngineRow].command = ui.engineCommand->text();
		engineData[currentEngineRow].options = ui.engineOptions->text();
		engineData[currentEngineRow].directory = ui.engineDirectory->text();
		if (ui.engineProtocolWinBoard->isChecked())
			engineData[currentEngineRow].protocol = WinBoard;
		else	engineData[currentEngineRow].protocol = UCI;
	}
}

void PreferencesDialog::slotSelectEngine(int newRow)
{
	if (currentEngineRow < 0)
		ui.engineEditWidget->setEnabled(true);

	// Store any edits done to current data
	updateCurrentEngineData();

	if (newRow >= 0) {
		// Fill edit fields with data for selected engine
		currentEngineRow = newRow;
		ui.engineName->setText(engineData[currentEngineRow].name);
		ui.engineCommand->setText(engineData[currentEngineRow].command);
		ui.engineOptions->setText(engineData[currentEngineRow].options);
		ui.engineDirectory->setText(engineData[currentEngineRow].directory);
		if (engineData[currentEngineRow].protocol == WinBoard)
			ui.engineProtocolWinBoard->setChecked(true);
		else	ui.engineProtocolUCI->setChecked(true);
	} else {
		currentEngineRow = -1;
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
	QString pieceTheme = AppSettings->value("pieceTheme", "default").toString();
	QString pieceEffect = AppSettings->value("pieceEffect", "Plain").toString();
	QString boardTheme = AppSettings->value("boardTheme", "default").toString();
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

	ui.pieceEffect->addItem(tr("Plain"));
	ui.pieceEffect->addItem(tr("Outline"));
	ui.pieceEffect->addItem(tr("Shadow"));

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
	updateCurrentEngineData();  // Make sure current edits are saved
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
