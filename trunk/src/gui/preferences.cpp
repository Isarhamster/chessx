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

PreferencesDialog::PreferencesDialog(QWidget* parent) :
		QDialog(parent), m_engineCurrent(-1)

{
	ui.setupUi(this);

#ifndef Q_WS_WIN
	ui.engineProtocolWinBoard->setText(tr("XBoard"));
#endif

	connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
	connect(ui.engineList, SIGNAL(clicked(QModelIndex)), SLOT(slotSelectEngine()));
	connect(ui.engineName, SIGNAL(textChanged(const QString&)), SLOT(slotEngineNameChange(const QString&)));
	connect(ui.newEngineButton, SIGNAL(clicked(bool)), SLOT(slotNewEngine()));
	connect(ui.deleteEngineButton, SIGNAL(clicked(bool)), SLOT(slotDeleteEngine()));
	connect(ui.engineUpButton, SIGNAL(clicked(bool)), SLOT(slotEngineUp()));
	connect(ui.engineDownButton, SIGNAL(clicked(bool)), SLOT(slotEngineDown()));
	connect(ui.directoryButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineDirectory()));
	connect(ui.commandButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineCommand()));

	// Start off with no Engine selected
	m_engineListModel = new QStringListModel(QStringList());
	ui.engineList->setModel(m_engineListModel);
	ui.engineEditWidget->setEnabled(false);

	restoreSettings();
	AppSettings->beginGroup("/Board/");
	restoreColorItem(ui.boardColorsList, tr("Light squares"), "lightColor", "#a0a0a0");
	restoreColorItem(ui.boardColorsList, tr("Dark squares"), "darkColor", "#d0d0d0");
	restoreColorItem(ui.boardColorsList, tr("Highlighted squares"), "highlightColor", "#ffff00");
	restoreColorItem(ui.boardColorsList, tr("Frame"), "frameColor", "#000000");
	AppSettings->endGroup();

}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::closeEvent(QCloseEvent*)
{
	AppSettings->setLayout(this);
}

void PreferencesDialog::slotSelectEngineDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory in which engine will run"),
					ui.engineDirectory->text(),
					QFileDialog::ShowDirsOnly);
	if (!dir.isEmpty())
		ui.engineDirectory->setText(dir);
}

void PreferencesDialog::slotSelectEngineCommand()
{
	QString com = QFileDialog::getOpenFileName(this, tr("Select engine command file to run"),
					ui.engineCommand->text());
	if (!com.isEmpty()) {
		ui.engineCommand->setText(com);
		if (engineList[ui.engineList->currentIndex().row()].name == tr("New Engine")) {
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
	int row = ui.engineList->currentIndex().row();
	if (row != -1) {
		m_engineCurrent = -1;
		engineList.removeAt(row);
		updateEngineList(qBound(0, row, engineList.count() - 1));
	}
}

void PreferencesDialog::slotNewEngine()
{
	engineList.append(EngineData(tr("New Engine")));
	updateEngineList(engineList.count() - 1);
}

void PreferencesDialog::slotEngineNameChange(const QString& name)
{
	/*
	if (ui.engineList->count()) {
		ui.engineList->currentItem()->setText(name);
		engineList[ui.engineList->currentIndex().row()].name = name;
	}
	*/
}

void PreferencesDialog::slotEngineUp()
{
	int index = ui.engineList->currentIndex().row();
	if (index > 0) {
		m_engineCurrent = -1;
		engineList.swap(index, index - 1);
		updateEngineList(index - 1);
	}
}

void PreferencesDialog::slotEngineDown()
{
	int index = ui.engineList->currentIndex().row();
	qDebug() << "Down " << index;
	if (index >= 0 && index < engineList.count() - 1) {
		m_engineCurrent = -1;
		engineList.swap(index, index + 1);
		updateEngineList(index + 1);
	}
}

void PreferencesDialog::updateEngineData(int index)
{
	if (index < 0 || index >= engineList.count())
		return;
	engineList[index].name = ui.engineName->text();
	engineList[index].command = ui.engineCommand->text();
	engineList[index].options = ui.engineOptions->text();
	engineList[index].directory = ui.engineDirectory->text();
	engineList[index].protocol = ui.engineProtocolWinBoard->isChecked() ?
					  EngineData::WinBoard : EngineData::UCI;
	qDebug() << "Updated" << index;
}

void PreferencesDialog::updateEngineList(int row)
{
	m_engineListModel->setStringList(engineList.names());
	if (row != -1) {
		QModelIndex index = m_engineListModel->index(row, 0);
		if (index.isValid()) {
			qDebug() << "Selected" << row;
			ui.engineList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
			slotSelectEngine();
		}
	}
}

void PreferencesDialog::slotSelectEngine()
{
	// Store any edits done to current data
	if (m_engineCurrent > -1)
		updateEngineData(m_engineCurrent);

	m_engineCurrent = ui.engineList->currentIndex().row();

	if (m_engineCurrent > -1) {
		ui.engineEditWidget->setEnabled(true);
		EngineData data = engineList[m_engineCurrent];
		// Fill edit fields with data for selected engine
		ui.engineName->setText(data.name);
		ui.engineCommand->setText(data.command);
		ui.engineOptions->setText(data.options);
		ui.engineDirectory->setText(data.directory);
		if (data.protocol == EngineData::WinBoard)
			ui.engineProtocolWinBoard->setChecked(true);
		else ui.engineProtocolUCI->setChecked(true);
	} else if (!engineList.count()) {
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
	// Restore size
	AppSettings->layout(this);

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
	ui.pieceEffect->setCurrentIndex(AppSettings->value("pieceEffect", 2).toInt());
	QString boardTheme = AppSettings->value("boardTheme", "slate").toString();
	AppSettings->endGroup();

	QString themeDir(AppSettings->dataPath() + "/themes");
	QStringList themes = QDir(themeDir).entryList(QStringList("*.png"));
	for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it) {
		(*it).truncate((*it).length() - 4);
		ui.pieceThemeCombo->addItem(*it);
	}

	themes = QDir(themeDir + "/boards").entryList(QStringList("*.png"));
	QStringListIterator it(themes);
	while (it.hasNext()) {
		QString trim(it.next());
		ui.boardThemeCombo->addItem(trim.left(trim.length() - 4));
	}
	ui.boardThemeCombo->addItem(tr("[plain colors]"));

	selectInCombo(ui.pieceThemeCombo, pieceTheme);
	selectInCombo(ui.boardThemeCombo, boardTheme);

	// Read Players settings
	//AppSettings->beginGroup("/Players/");
	//AppSettings->endGroup();

	// Read Engine settings
	engineList.restore();
	updateEngineList(0);

	// Read Advanced settings
	ui.limitSpin->setValue(AppSettings->value("/General/EditLimit", 10).toInt());

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
	AppSettings->setValue("pieceEffect", ui.pieceEffect->currentIndex());
	if (ui.boardThemeCombo->currentIndex() != ui.boardThemeCombo->count() - 1)
		AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
	else	AppSettings->setValue("boardTheme", QString());
	QStringList colorNames;
	colorNames << "lightColor" << "darkColor" << "highlightColor" << "frameColor";
	saveColorList(ui.boardColorsList, colorNames);
	AppSettings->endGroup();
	//AppSettings->beginGroup("/Players/");
	//AppSettings->setValue("rating", ui.playersRatingsCheck->isChecked());
	//AppSettings->endGroup();

	// Save engine settings
	updateEngineData(ui.engineList->currentIndex().row());  // Make sure current edits are saved
	engineList.save();

	AppSettings->setValue("/General/EditLimit", ui.limitSpin->value());

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

