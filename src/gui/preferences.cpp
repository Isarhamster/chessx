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
#include "messagedialog.h"
#include "engineoptiondialog.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QListWidget>
#include <QPainter>
#include <QDir>
#include <QPushButton>
#include <QSpinBox>
#include <QFileDialog>
#include <QDesktopServices>

int PreferencesDialog::s_lastIndex = 0;

PreferencesDialog::PreferencesDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	ui.setupUi(this);

#ifndef Q_WS_WIN
	ui.engineProtocolWinBoard->setText(tr("XBoard"));
#endif

	connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
	connect(ui.engineList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		SLOT(slotSelectEngine(QListWidgetItem*, QListWidgetItem*)));
	connect(ui.engineName, SIGNAL(textChanged(const QString&)), SLOT(slotEngineNameChange(const QString&)));
	connect(ui.addEngineButton, SIGNAL(clicked(bool)), SLOT(slotAddEngine()));
	connect(ui.deleteEngineButton, SIGNAL(clicked(bool)), SLOT(slotDeleteEngine()));
	connect(ui.engineUpButton, SIGNAL(clicked(bool)), SLOT(slotEngineUp()));
	connect(ui.engineDownButton, SIGNAL(clicked(bool)), SLOT(slotEngineDown()));
	connect(ui.directoryButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineDirectory()));
	connect(ui.commandButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineCommand()));
    connect(ui.browsePathButton, SIGNAL(clicked(bool)), SLOT(slotSelectDataBasePath()));
    connect(ui.engineOptionMore, SIGNAL(clicked(bool)), SLOT(slotShowOptionDialog()));

    connect(ui.tbUK, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbGermany, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbFrance, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbPoland, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbSymbolic, SIGNAL(clicked()), SLOT(slotChangePieceString()));
	restoreSettings();

	// Start off with no Engine selected
	ui.engineEditWidget->setEnabled(false);
    ui.tabWidget->setCurrentIndex(s_lastIndex);
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::done( int r ) {
    QDialog::done( r );
    close();
}
void PreferencesDialog::closeEvent(QCloseEvent*)
{
	AppSettings->setLayout(this);
}

void PreferencesDialog::slotSelectEngineDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this,
					tr("Select engine directory"), ui.engineDirectory->text(),
					QFileDialog::ShowDirsOnly);
    if (QDir(dir).exists())
		ui.engineDirectory->setText(dir);
}

void PreferencesDialog::slotSelectDataBasePath()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                    tr("Select databases folder"), ui.defaultDataBasePath->text(),
                    QFileDialog::ShowDirsOnly);
    if (QDir(dir).exists())
        ui.defaultDataBasePath->setText(dir);
}

void PreferencesDialog::slotAddEngine()
{
	QString command = selectEngineFile();
	if (command.isEmpty())
		return;
	QString name = EngineData::commandToName(command);
    if (name.isEmpty()) name = tr("New Engine");
	EngineData data(name);
	data.command = command;
	engineList.append(data);
	ui.engineList->addItem(name);
	ui.engineList->setCurrentRow(engineList.count() - 1);
}

void PreferencesDialog::slotSelectEngineCommand()
{
	QString command = selectEngineFile(ui.engineCommand->text());
	if (!command.isEmpty()) {
		ui.engineCommand->setText(command);
		ui.engineName->setText(EngineData::commandToName(command));
	}
}

void PreferencesDialog::slotDeleteEngine()
{
	int row = ui.engineList->currentRow();
	if (row >= 0) {
		// Looks like it should crash, but it first removes
		// the item, then it updates it by slotSelectEngine (which is obsolete)
		// and only then it deletes it and removes the data
		QListWidgetItem *del = ui.engineList->takeItem(row);
		delete del;
		engineList.removeAt(row);
	}
}

void PreferencesDialog::slotEngineNameChange(const QString& name)
{
	if (ui.engineList->currentItem()) {
		ui.engineList->currentItem()->setText(name);
		engineList[ui.engineList->currentIndex().row()].name = name;
	}
}

void PreferencesDialog::slotEngineUp()
{
	int index = ui.engineList->currentIndex().row();
	if (index > 0) {
		engineList.swap(index, index - 1);
		QListWidgetItem* item = ui.engineList->takeItem(index - 1);
		ui.engineList->insertItem(index, item);
	}
}

void PreferencesDialog::slotEngineDown()
{
	int index = ui.engineList->currentIndex().row();
	if (index < ui.engineList->count() - 1) {
		engineList.swap(index, index + 1);
		QListWidgetItem* item = ui.engineList->takeItem(index + 1);
		ui.engineList->insertItem(index, item);
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
}

void PreferencesDialog::slotSelectEngine(QListWidgetItem* currentItem, QListWidgetItem* previousItem)
{
	int previous = ui.engineList->row(previousItem);
	int current = ui.engineList->row(currentItem);

	if (previous != -1)
		updateEngineData(previous);

	if (current != -1) {
		ui.engineEditWidget->setEnabled(true);
		// Fill edit fields with data for selected engine
		ui.engineName->setText(engineList[current].name);
		ui.engineCommand->setText(engineList[current].command);
		ui.engineOptions->setText(engineList[current].options);
		ui.engineDirectory->setText(engineList[current].directory);
        ui.engineOptionMore->setEnabled(true);
		if (engineList[current].protocol == EngineData::WinBoard)
			ui.engineProtocolWinBoard->setChecked(true);
		else ui.engineProtocolUCI->setChecked(true);
	} else {
		ui.engineName->clear();
		ui.engineCommand->clear();
		ui.engineOptions->clear();
		ui.engineDirectory->clear();
		ui.engineProtocolUCI->setChecked(true);
		ui.engineEditWidget->setEnabled(false);
        ui.engineOptionMore->setEnabled(false);
	}
}

QString PreferencesDialog::selectEngineFile(const QString& oldpath)
{
	return QFileDialog::getOpenFileName(this, tr("Select engine executable"),
					oldpath);
}

void PreferencesDialog::slotShowOptionDialog()
{
    int index = ui.engineList->currentIndex().row();
    updateEngineData(index);
    EngineOptionDialog dlg(0, engineList, index);
    if (dlg.exec() == QDialog::Accepted)
    {
        engineList[index].m_optionValues = dlg.GetResults();
    }
}

void PreferencesDialog::slotChangePieceString()
{
    QString pieceString;
    if ((QToolButton*)sender() == ui.tbUK)
    {
        pieceString = " KQRBN";
    }
    else if ((QToolButton*)sender() == ui.tbGermany)
    {
        pieceString = " KDTLS";
    }
    else if ((QToolButton*)sender() == ui.tbFrance)
    {
        pieceString = " RDTFC";
    }
    else if ((QToolButton*)sender() == ui.tbPoland)
    {
        pieceString = " KHWGS";
    }
    else if ((QToolButton*)sender() == ui.tbSymbolic)
    {
        pieceString.clear();
    }
    else
    {
        pieceString = " KQRBN";
    }
    ui.pieceString->setText(pieceString);
}

int PreferencesDialog::exec()
{
	int result = QDialog::exec();
    s_lastIndex = ui.tabWidget->currentIndex();
	if (result == QDialog::Accepted) {
		saveSettings();
		emit reconfigure();
	}
	return result;
}

void PreferencesDialog::slotReset()
{
    if (MessageDialog::yesNo(tr("Clear all application settings?"),tr("Warning")))
    {
        AppSettings->clear();
        restoreSettings();
        emit reconfigure();
    }
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
    ui.tablebaseCheck->setChecked(AppSettings->getValue("onlineTablebases").toBool());
    ui.versionCheck->setChecked(AppSettings->getValue("onlineVersionCheck").toBool());
    ui.automaticECO->setChecked(AppSettings->getValue("automaticECO").toBool());
    ui.useIndexFile->setChecked(AppSettings->getValue("useIndexFile").toBool());
    ui.cbAutoCommitDB->setChecked(AppSettings->getValue("autoCommitDB").toBool());
	AppSettings->endGroup();
    AppSettings->beginGroup("/Board/");
    ui.boardFrameCheck->setChecked(AppSettings->getValue("showFrame").toBool());
    ui.boardShowCoordinates->setChecked(AppSettings->getValue("showCoordinates").toBool());
    ui.hilightCurrentMove->setChecked(AppSettings->getValue("showCurrentMove").toBool());
    ui.guessMoveCheck->setChecked(AppSettings->getValue("guessMove").toBool());
    ui.guessNextMove->setChecked(AppSettings->getValue("nextGuess").toBool());
    ui.minWheelCount->setValue(AppSettings->getValue("minWheelCount").toInt());
    ui.autoPlayInterval->setValue(AppSettings->getValue("AutoPlayerInterval").toInt());
    ui.cbSaveAndContinue->setChecked(AppSettings->getValue("AutoSaveAndContinue").toBool());
    QString pieceTheme = AppSettings->getValue("pieceTheme").toString();
    ui.pieceEffect->setCurrentIndex(AppSettings->getValue("pieceEffect").toInt());
    QString boardTheme = AppSettings->getValue("boardTheme").toString();

    ui.boardColorsList->clear();
    restoreColorItem(ui.boardColorsList, tr("Light squares"), "lightColor");
    restoreColorItem(ui.boardColorsList, tr("Dark squares"), "darkColor");
    restoreColorItem(ui.boardColorsList, tr("Highlighted squares"), "highlightColor");
    restoreColorItem(ui.boardColorsList, tr("Frame"), "frameColor");
    restoreColorItem(ui.boardColorsList, tr("Current move"), "currentMoveColor");
    AppSettings->endGroup();

	QString themeDir(AppSettings->dataPath() + "/themes");

    if (!QFile::exists(themeDir))
        themeDir = QString(":/themes");

	QStringList themes = QDir(themeDir).entryList(QStringList("*.png"));
	for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it) {
		(*it).truncate((*it).length() - 4);
		ui.pieceThemeCombo->addItem(*it);
	}

    QString boardDir(AppSettings->dataPath() + "/themes/boards");

    if (!QFile::exists(boardDir))
        boardDir = QString(":/themes/boards");

    themes = QDir(boardDir).entryList(QStringList("*.png"));
	QStringListIterator it(themes);
	while (it.hasNext()) {
		QString trim(it.next());
		ui.boardThemeCombo->addItem(trim.left(trim.length() - 4));
	}
	ui.boardThemeCombo->addItem(tr("[plain colors]"));

	selectInCombo(ui.pieceThemeCombo, pieceTheme);
	selectInCombo(ui.boardThemeCombo, boardTheme);

	// Read Engine settings
	engineList.restore();
	ui.engineList->clear();
	ui.engineList->insertItems(0, engineList.names());

	// Read Advanced settings
    ui.limitSpin->setValue(AppSettings->getValue("/General/EditLimit").toInt());
    ui.spinBoxRecentFiles->setValue(AppSettings->getValue("/History/MaxEntries").toInt());

#if QT_VERSION < 0x050000
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/chessdata";
#else
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chessdata";
#endif
    ui.defaultDataBasePath->setText(AppSettings->value("/General/DefaultDataPath", dataPath).toString());

    // Read Game List settings
    ui.gameTextFontSizeSpin->setValue(AppSettings->getValue("/GameText/FontSize").toInt());
    ui.cbShowDiagrams->setChecked(AppSettings->getValue("/GameText/ShowDiagrams").toBool());
    ui.pieceString->setText(AppSettings->getValue("/GameText/PieceString").toString());
    ui.spinBoxListFontSize->setValue(AppSettings->getValue("/General/ListFontSize").toInt());
    ui.verticalTabs->setChecked(AppSettings->getValue("/MainWindow/VerticalTabs").toBool());
}

void PreferencesDialog::saveSettings()
{
    AppSettings->beginGroup("/General/");
    AppSettings->setValue("onlineTablebases", QVariant(ui.tablebaseCheck->isChecked()));
    AppSettings->setValue("onlineVersionCheck", QVariant(ui.versionCheck->isChecked()));
    AppSettings->setValue("automaticECO", QVariant(ui.automaticECO->isChecked()));
    AppSettings->setValue("useIndexFile",QVariant(ui.useIndexFile->isChecked()));
    AppSettings->setValue("autoCommitDB",QVariant(ui.cbAutoCommitDB->isChecked()));
	AppSettings->endGroup();
    AppSettings->beginGroup("/Board/");
    AppSettings->setValue("showFrame", QVariant(ui.boardFrameCheck->isChecked()));
    AppSettings->setValue("showCoordinates", QVariant(ui.boardShowCoordinates->isChecked()));
    AppSettings->setValue("showCurrentMove", QVariant(ui.hilightCurrentMove->isChecked()));
    AppSettings->setValue("guessMove", QVariant(ui.guessMoveCheck->isChecked()));
    AppSettings->setValue("nextGuess", QVariant(ui.guessNextMove->isChecked()));
    AppSettings->setValue("minWheelCount", ui.minWheelCount->value());
    AppSettings->setValue("AutoPlayerInterval", ui.autoPlayInterval->value());
	AppSettings->setValue("pieceTheme", ui.pieceThemeCombo->currentText());
	AppSettings->setValue("pieceEffect", ui.pieceEffect->currentIndex());
    AppSettings->setValue("AutoSaveAndContinue",QVariant(ui.cbSaveAndContinue->isChecked()));
	if (ui.boardThemeCombo->currentIndex() != ui.boardThemeCombo->count() - 1)
		AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
	else	AppSettings->setValue("boardTheme", QString());
	QStringList colorNames;
    colorNames << "lightColor" << "darkColor" << "highlightColor" << "frameColor" << "currentMoveColor";
	saveColorList(ui.boardColorsList, colorNames);
	AppSettings->endGroup();

	// Save engine settings
	updateEngineData(ui.engineList->currentIndex().row());  // Make sure current edits are saved
	engineList.save();

    AppSettings->setValue("/General/EditLimit", ui.limitSpin->value());
    AppSettings->setValue("/History/MaxEntries", ui.spinBoxRecentFiles->value());
    AppSettings->setValue("/General/DefaultDataPath", ui.defaultDataBasePath->text());
    AppSettings->setValue("/GameText/FontSize", ui.gameTextFontSizeSpin->value());
    AppSettings->setValue("/GameText/ShowDiagrams", ui.cbShowDiagrams->isChecked());
    AppSettings->setValue("/GameText/PieceString", ui.pieceString->text());
    AppSettings->setValue("/General/ListFontSize", ui.spinBoxListFontSize->value());
    AppSettings->setValue("/MainWindow/VerticalTabs", ui.verticalTabs->isChecked());

    QDir().mkpath(ui.defaultDataBasePath->text());
}

bool PreferencesDialog::selectInCombo(QComboBox* combo, const QString& text)
{
    for (int i = 0; i < combo->count(); ++i)
		if (combo->itemText(i) == text) {
			combo->setCurrentIndex(i);
			return true;
		}
	combo->setCurrentIndex(combo->count() - 1);
	return false;
}

void PreferencesDialog::restoreColorItem(ColorList* list, const QString& text, const QString& cfgname)
{
    QColor color = AppSettings->getValue(cfgname).value<QColor>();
	list->addItem(text, color);
}

void PreferencesDialog::saveColorList(ColorList* list, const QStringList& cfgnames)
{
    for (int i = 0; i < list->count(); ++i)
		AppSettings->setValue(cfgnames[i], list->color(i));
}
