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
#include "downloadmanager.h"

#include <QAction>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QListWidget>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QTextStream>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

int PreferencesDialog::s_lastIndex = 0;

PreferencesDialog::PreferencesDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    ui.setupUi(this);

#ifndef Q_WS_WIN
    ui.engineProtocolWinBoard->setText(tr("XBoard"));
#endif

    QPushButton *addEngineButton = ui.buttonBoxEngines->addButton(tr("Add..."), QDialogButtonBox::ActionRole);
    QPushButton *deleteEngineButton = ui.buttonBoxEngines->addButton(tr("Remove"), QDialogButtonBox::ActionRole);
    QPushButton *engineUpButton = ui.buttonBoxEngines->addButton(tr("Up"), QDialogButtonBox::ActionRole);
    QPushButton *engineDownButton = ui.buttonBoxEngines->addButton(tr("Down"), QDialogButtonBox::ActionRole);

    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    connect(ui.engineList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            SLOT(slotSelectEngine(QListWidgetItem*, QListWidgetItem*)));
    connect(ui.engineName, SIGNAL(textChanged(const QString&)), SLOT(slotEngineNameChange(const QString&)));
    connect(addEngineButton, SIGNAL(clicked(bool)), SLOT(slotAddEngine()));
    connect(deleteEngineButton, SIGNAL(clicked(bool)), SLOT(slotDeleteEngine()));
    connect(engineUpButton, SIGNAL(clicked(bool)), SLOT(slotEngineUp()));
    connect(engineDownButton, SIGNAL(clicked(bool)), SLOT(slotEngineDown()));
    connect(ui.directoryButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineDirectory()));
    connect(ui.commandButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineCommand()));
    connect(ui.browsePathButton, SIGNAL(clicked(bool)), SLOT(slotSelectDataBasePath()));
    connect(ui.engineOptionMore, SIGNAL(clicked(bool)), SLOT(slotShowOptionDialog()));

    connect(ui.tbUK, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbGermany, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbFrance, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbPoland, SIGNAL(clicked()), SLOT(slotChangePieceString()));
    connect(ui.tbSymbolic, SIGNAL(clicked()), SLOT(slotChangePieceString()));

    connect(ui.btLoadLang, SIGNAL(clicked()), SLOT(slotLoadLanguageFile()));
    connect(ui.btExtToolPath, SIGNAL(clicked(bool)), SLOT(slotSelectToolPath()));

    connect (ui.guestLogin, SIGNAL(toggled(bool)), ui.passWord, SLOT(setDisabled(bool)));
    connect (ui.guestLogin, SIGNAL(toggled(bool)), ui.userName, SLOT(setDisabled(bool)));

    connect (ui.pieceEffect, SIGNAL(activated(int)), SLOT(SlotPieceEffectActivated(int)));
    restoreSettings();

    // Start off with no Engine selected
    ui.engineEditWidget->setEnabled(false);
    ui.tabWidget->setCurrentIndex(s_lastIndex);
    ui.btLoadLang->setEnabled(false);
    ui.cbLangServer->setEnabled(false);

    if(AppSettings->getValue("/General/onlineVersionCheck").toBool())
    {
        QUrl url = QUrl(QString("http://chessx.sourceforge.net/translations/dict.txt"));
        downloadManager = new DownloadManager(this);
        connect(downloadManager, SIGNAL(downloadError(QUrl)), this, SLOT(loadFileError(QUrl)), Qt::QueuedConnection);
        connect(downloadManager, SIGNAL(onDownloadFinished(QUrl, QString)), this, SLOT(slotFileLoaded(QUrl, QString)), Qt::QueuedConnection);
        QString path = AppSettings->getTempPath();
        downloadManager->doDownloadToPath(url, path + QDir::separator() + "dict.txt");
    }
    else
    {
        ui.labelLoadStatus->setText(tr("For updating translations online version checking needs to be enabled."));
    }
    QTimer::singleShot(0, this, SLOT(restoreLayout()));
}

void PreferencesDialog::restoreLayout()
{
    AppSettings->layout(this);
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::done(int r)
{
    AppSettings->setLayout(this);
    QDialog::done(r);
    close();
}

void PreferencesDialog::slotSelectEngineDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                  tr("Select engine directory"), ui.engineDirectory->text(),
                  QFileDialog::ShowDirsOnly);
    if(QDir(dir).exists())
    {
        ui.engineDirectory->setText(dir);
    }
}

void PreferencesDialog::slotSelectToolPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select external executable"),
                                                    ui.extToolPath->text());
    if(QFileInfo(fileName).exists())
    {
        ui.extToolPath->setText(fileName);
    }
}

void PreferencesDialog::slotSelectDataBasePath()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                  tr("Select databases folder"), ui.defaultDataBasePath->text(),
                  QFileDialog::ShowDirsOnly);
    if(QDir(dir).exists())
    {
        ui.defaultDataBasePath->setText(dir);
    }
}

void PreferencesDialog::slotAddEngine()
{
    QString command = selectEngineFile();
    if(command.isEmpty())
    {
        return;
    }
    QString name = EngineData::commandToName(command);
    if(name.isEmpty())
    {
        name = tr("New Engine");
    }
    EngineData data(name);
    data.command = command;
    engineList.append(data);
    ui.engineList->addItem(name);
    ui.engineList->setCurrentRow(engineList.count() - 1);
}

void PreferencesDialog::slotSelectEngineCommand()
{
    QString command = selectEngineFile(ui.engineCommand->text());
    if(!command.isEmpty())
    {
        ui.engineCommand->setText(command);
        ui.engineName->setText(EngineData::commandToName(command));
    }
}

void PreferencesDialog::slotDeleteEngine()
{
    int row = ui.engineList->currentRow();
    if(row >= 0)
    {
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
    if(ui.engineList->currentItem())
    {
        ui.engineList->currentItem()->setText(name);
        engineList[ui.engineList->currentIndex().row()].name = name;
    }
}

void PreferencesDialog::slotEngineUp()
{
    int index = ui.engineList->currentIndex().row();
    if(index > 0)
    {
        engineList.swap(index, index - 1);
        QListWidgetItem* item = ui.engineList->takeItem(index - 1);
        ui.engineList->insertItem(index, item);
    }
}

void PreferencesDialog::slotEngineDown()
{
    int index = ui.engineList->currentIndex().row();
    if(index < ui.engineList->count() - 1)
    {
        engineList.swap(index, index + 1);
        QListWidgetItem* item = ui.engineList->takeItem(index + 1);
        ui.engineList->insertItem(index, item);
    }
}

void PreferencesDialog::updateEngineData(int index)
{
    if(index < 0 || index >= engineList.count())
    {
        return;
    }
    engineList[index].name = ui.engineName->text();
    engineList[index].command = ui.engineCommand->text();
    engineList[index].options = ui.engineOptions->text();
    engineList[index].directory = ui.engineDirectory->text();
    engineList[index].logging = ui.logging->isChecked();
    engineList[index].sendHistory = ui.sendHistory->isChecked();
    engineList[index].protocol = ui.engineProtocolWinBoard->isChecked() ?
                                 EngineData::WinBoard : EngineData::UCI;
}

void PreferencesDialog::slotSelectEngine(QListWidgetItem* currentItem, QListWidgetItem* previousItem)
{
    int previous = ui.engineList->row(previousItem);
    int current = ui.engineList->row(currentItem);

    if(previous != -1)
    {
        updateEngineData(previous);
    }

    if(current != -1)
    {
        ui.engineEditWidget->setEnabled(true);
        // Fill edit fields with data for selected engine
        ui.engineName->setText(engineList[current].name);
        ui.engineCommand->setText(engineList[current].command);
        ui.engineOptions->setText(engineList[current].options);
        ui.engineDirectory->setText(engineList[current].directory);
        ui.logging->setChecked(engineList[current].logging);
        ui.sendHistory->setChecked(engineList[current].sendHistory);
        ui.engineOptionMore->setEnabled(true);
        if(engineList[current].protocol == EngineData::WinBoard)
        {
            ui.engineProtocolWinBoard->setChecked(true);
        }
        else
        {
            ui.engineProtocolUCI->setChecked(true);
        }
    }
    else
    {
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
    EngineOptionDialog dlg(this, engineList, index);
    if(dlg.exec() == QDialog::Accepted)
    {
        engineList[index].m_optionValues = dlg.GetResults();
    }
}

void PreferencesDialog::slotChangePieceString()
{
    QString pieceString;
    if((QToolButton*)sender() == ui.tbUK)
    {
        pieceString = " KQRBN";
    }
    else if((QToolButton*)sender() == ui.tbGermany)
    {
        pieceString = " KDTLS";
    }
    else if((QToolButton*)sender() == ui.tbFrance)
    {
        pieceString = " RDTFC";
    }
    else if((QToolButton*)sender() == ui.tbPoland)
    {
        pieceString = " KHWGS";
    }
    else if((QToolButton*)sender() == ui.tbSymbolic)
    {
        pieceString.clear();
    }
    else
    {
        pieceString = " KQRBN";
    }
    ui.pieceString->setText(pieceString);
}

void PreferencesDialog::slotLoadLanguageFile()
{
    if(!ui.cbLangServer->currentText().isEmpty())
    {
        QUrl url = QUrl(QString("http://chessx.sourceforge.net/translations/chessx_") + ui.cbLangServer->currentText() + ".qm");
        downloadManager->doDownloadToPath(url, AppSettings->dataPath() + QDir::separator() + "lang" + QDir::separator() + "chessx_" + ui.cbLangServer->currentText() + ".qm");
    }
}

void PreferencesDialog::loadFileError(QUrl url)
{
    if(url.toString().endsWith(".txt"))
    {
        ui.labelLoadStatus->setText(tr("Could not load server language file dictionary"));
    }
    else if(url.toString().endsWith(".qm"))
    {
        ui.labelLoadStatus->setText(tr("Could not load or install language pack"));
    }
}

void PreferencesDialog::slotFileLoaded(QUrl, QString name)
{
    if(name.endsWith(".qm"))
    {
        name.remove(QRegExp("[^_]*_"));
        name.remove(".qm");
        ui.cbLanguage->addItem(name);
        ui.labelLoadStatus->setText(tr("Translation file loaded - select added language above!"));
    }
    else if(name.endsWith(".txt"))
    {
        QFile dictFile(name);
        if(dictFile.open(QIODevice::ReadOnly))
        {
            QTextStream textStream(&dictFile);
            while(true)
            {
                QString line = textStream.readLine();
                if(line.isNull())
                {
                    break;
                }
                else
                {
                    bool notFound = true;
                    for(int i = 0; i < ui.cbLanguage->count(); ++i)
                    {
                        if(ui.cbLanguage->itemText(i) == line)
                        {
                            notFound = false;
                            break;
                        }
                    }
                    if(notFound)
                    {
                        ui.cbLangServer->addItem(line);
                    }
                    else
                    {
                        ui.labelLoadStatus->setText(tr("No further translations online available!"));
                    }
                }
            }

            ui.btLoadLang->setEnabled(ui.cbLangServer->count() > 0);
            ui.cbLangServer->setEnabled(ui.cbLangServer->count() > 0);
        }
    }
}

int PreferencesDialog::exec()
{
    int result = QDialog::exec();
    s_lastIndex = ui.tabWidget->currentIndex();
    if(result == QDialog::Accepted)
    {
        saveSettings();
        emit reconfigure();
    }
    return result;
}

void PreferencesDialog::slotReset()
{
    if(MessageDialog::yesNo(tr("Clear all application settings?"), tr("Warning")))
    {
        AppSettings->clear();
        AppSettings->setLayout(this);
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
    // Read Board settings
    AppSettings->beginGroup("/General/");
    ui.tablebaseCheck->setChecked(AppSettings->getValue("onlineTablebases").toBool());
    ui.versionCheck->setChecked(AppSettings->getValue("onlineVersionCheck").toBool());
    ui.automaticECO->setChecked(AppSettings->getValue("automaticECO").toBool());
    ui.useIndexFile->setChecked(AppSettings->getValue("useIndexFile").toBool());
    ui.cbAutoCommitDB->setChecked(AppSettings->getValue("autoCommitDB").toBool());
    QString lang = AppSettings->getValue("language").toString();
    AppSettings->endGroup();
    AppSettings->beginGroup("/Board/");
    ui.boardFrameCheck->setChecked(AppSettings->getValue("showFrame").toBool());
    ui.boardShowCoordinates->setChecked(AppSettings->getValue("showCoordinates").toBool());
    ui.boardColorCopy->setChecked(AppSettings->getValue("colorCopy").toBool());
    ui.fixedImageSize->setChecked(AppSettings->getValue("fixedImageSize").toBool());
    ui.editImageSize->setValue(AppSettings->getValue("copyImageSize").toInt());
    ui.editImageSize->setEnabled(AppSettings->getValue("fixedImageSize").toBool());
    ui.hilightCurrentMove->setCurrentIndex(AppSettings->getValue("showCurrentMove").toInt());
    ui.cbShowIndicator->setCurrentIndex(AppSettings->getValue("showMoveIndicator").toInt());
    ui.guessMoveCheck->setChecked(AppSettings->getValue("guessMove").toBool());
    ui.guessNextMove->setChecked(AppSettings->getValue("nextGuess").toBool());
    ui.minWheelCount->setValue(AppSettings->getValue("minWheelCount").toInt());
    ui.cbSaveAndContinue->setChecked(AppSettings->getValue("AutoSaveAndContinue").toBool());
    ui.cbBackwardAnalysis->setChecked(AppSettings->getValue("BackwardAnalysis").toBool());
    ui.cbPromoteToQueen->setChecked(AppSettings->getValue("AutoPromoteToQueen").toBool());
    ui.btNoHints->setChecked(AppSettings->getValue("noHints").toBool());
    ui.alwaysScale->setChecked(AppSettings->getValue("AlwaysScale").toBool());
    ui.editPlayerTurnBoard->setText(AppSettings->getValue("PlayerTurnBoard").toString());

    QString pieceTheme = AppSettings->getValue("pieceTheme").toString();
    ui.pieceEffect->setCurrentIndex(AppSettings->getValue("pieceEffect").toInt());
    QString boardTheme = AppSettings->getValue("boardTheme").toString();

    ui.boardColorsList->clear();
    restoreColorItem(ui.boardColorsList, tr("Light squares"), "lightColor");
    restoreColorItem(ui.boardColorsList, tr("Dark squares"), "darkColor");
    restoreColorItem(ui.boardColorsList, tr("Highlighted squares"), "highlightColor");
    restoreColorItem(ui.boardColorsList, tr("Frame"), "frameColor");
    restoreColorItem(ui.boardColorsList, tr("Current move"), "currentMoveColor");
    restoreColorItem(ui.boardColorsList, tr("Stored move"), "storedMoveColor");
    restoreColorItem(ui.boardColorsList, tr("Threat"), "threatColor");
    restoreColorItem(ui.boardColorsList, tr("Targets"), "targetColor");
    restoreColorItem(ui.boardColorsList, tr("Check"), "checkColor");
    restoreColorItem(ui.boardColorsList, tr("Wall"), "wallColor");
    restoreColorItem(ui.boardColorsList, tr("Underprotected"), "underprotectedColor");
    AppSettings->endGroup();

    SlotPieceEffectActivated(ui.pieceEffect->currentIndex());

    QStringList translations = AppSettings->getTranslations();
    ui.cbLanguage->addItem("Default");
    ui.cbLanguage->addItem("en");
    QStringListIterator it1(translations);
    while(it1.hasNext())
    {
        QString trim(it1.next());
        trim.remove(QRegExp("[^_]*_"));
        trim.remove(".qm");
        ui.cbLanguage->addItem(trim);
    }

    QStringList boards = AppSettings->getBoardList();
    QStringListIterator it(boards);
    while(it.hasNext())
    {
        QString trim(it.next());
        ui.boardThemeCombo->addItem(trim.left(trim.length() - 4));
    }
    ui.boardThemeCombo->addItem(tr("[plain colors]"));

    selectInCombo(ui.cbLanguage, lang);
    selectInCombo(ui.pieceThemeCombo, pieceTheme);
    selectInCombo(ui.boardThemeCombo, boardTheme);

    // Read Engine settings
    engineList.restore();
    ui.engineList->clear();
    ui.engineList->insertItems(0, engineList.names());

    // Read Advanced settings
    ui.limitSpin->setValue(AppSettings->getValue("/General/EditLimit").toInt());
    ui.spinBoxRecentFiles->setValue(AppSettings->getValue("/History/MaxEntries").toInt());

    QString dir = AppSettings->commonDataPath();
    ui.defaultDataBasePath->setText(dir);
    ui.spinBoxListFontSize->setValue(AppSettings->getValue("/General/ListFontSize").toInt());
    ui.verticalTabs->setChecked(AppSettings->getValue("/MainWindow/VerticalTabs").toBool());
    ui.iconsVisible->setChecked(AppSettings->getValue("/MainWindow/ShowMenuIcons").toBool());
    // Read Game List settings
    AppSettings->beginGroup("GameText");

    ui.notationColors->clear();
    restoreColorItem(ui.notationColors, tr("Main Line"), "MainLineMoveColor");
    restoreColorItem(ui.notationColors, tr("Variations"), "VariationColor");
    restoreColorItem(ui.notationColors, tr("Comments"), "CommentColor");
    restoreColorItem(ui.notationColors, tr("NAGs"), "NagColor");

    ui.gameTextFontSizeSpin->setValue(AppSettings->getValue("FontSize").toInt());
    ui.cbShowDiagrams->setChecked(AppSettings->getValue("ShowDiagrams").toBool());
    ui.cbColumnStyle->setChecked(AppSettings->getValue("ColumnStyle").toBool());
    ui.variationIndentLevel->setValue(AppSettings->getValue("VariationIndentLevel").toInt());
    ui.diagramSize->setValue(AppSettings->getValue("DiagramSize").toInt());
    ui.pieceString->setText(AppSettings->getValue("PieceString").toString());

    ui.fontText->setText(AppSettings->getValue("FontBrowserText").toString());
    ui.fontMove->setText(AppSettings->getValue("FontBrowserMove").toString());

    AppSettings->endGroup();

    ui.additionalTags->setText(AppSettings->getValue("/GameList/AdditionalTags").toString());

    AppSettings->beginGroup("Tools");
    ui.extToolPath->setText(AppSettings->getValue("Path1").toString());
    ui.extToolParameters->setText(AppSettings->getValue("CommandLine1").toString());
    AppSettings->endGroup();

    AppSettings->beginGroup("Web");
    ui.webFavorite->setText(AppSettings->getValue("Favorite1").toString());
    ui.autoNumber->setValue(AppSettings->getValue("AutoNumber1").toInt());
    AppSettings->endGroup();

    AppSettings->beginGroup("FICS");
    ui.userName->setText(AppSettings->getValue("userName").toString());
    ui.passWord->setText(AppSettings->getValue("passWord").toString());
    ui.guestLogin->setChecked(AppSettings->getValue("guestLogin").toBool());
    ui.btUseTimeseal->setChecked(AppSettings->getValue("useTimeseal").toBool());
    AppSettings->endGroup();

    AppSettings->beginGroup("Sound");
#ifdef USE_SOUND
    ui.cbSoundOn->setChecked(AppSettings->getValue("Move").toBool());
#else
    ui.cbSoundOn->setChecked(false);
    ui.cbSoundOn->setEnabled(false);
#endif
    AppSettings->endGroup();
}

void PreferencesDialog::saveSettings()
{
    AppSettings->beginGroup("/General/");
    AppSettings->setValue("onlineTablebases", QVariant(ui.tablebaseCheck->isChecked()));
    AppSettings->setValue("onlineVersionCheck", QVariant(ui.versionCheck->isChecked()));
    AppSettings->setValue("automaticECO", QVariant(ui.automaticECO->isChecked()));
    AppSettings->setValue("useIndexFile", QVariant(ui.useIndexFile->isChecked()));
    AppSettings->setValue("autoCommitDB", QVariant(ui.cbAutoCommitDB->isChecked()));
    AppSettings->setValue("language", QVariant(ui.cbLanguage->currentText()));
    AppSettings->endGroup();
    AppSettings->beginGroup("/Board/");
    AppSettings->setValue("showFrame", QVariant(ui.boardFrameCheck->isChecked()));
    AppSettings->setValue("showCoordinates", QVariant(ui.boardShowCoordinates->isChecked()));
    AppSettings->setValue("colorCopy", QVariant(ui.boardColorCopy->isChecked()));
    AppSettings->setValue("fixedImageSize", QVariant(ui.fixedImageSize->isChecked()));
    AppSettings->setValue("copyImageSize", QVariant(ui.editImageSize->value()));
    AppSettings->setValue("showCurrentMove", QVariant(ui.hilightCurrentMove->currentIndex()));
    AppSettings->setValue("showMoveIndicator", QVariant(ui.cbShowIndicator->currentIndex()));
    AppSettings->setValue("guessMove", QVariant(ui.guessMoveCheck->isChecked()));
    AppSettings->setValue("noHints", QVariant(ui.btNoHints->isChecked()));
    AppSettings->setValue("nextGuess", QVariant(ui.guessNextMove->isChecked()));
    AppSettings->setValue("minWheelCount", ui.minWheelCount->value());
    AppSettings->setValue("pieceTheme", ui.pieceThemeCombo->currentText());
    AppSettings->setValue("pieceEffect", ui.pieceEffect->currentIndex());
    AppSettings->setValue("AutoSaveAndContinue", QVariant(ui.cbSaveAndContinue->isChecked()));
    AppSettings->setValue("BackwardAnalysis", QVariant(ui.cbBackwardAnalysis->isChecked()));
    AppSettings->setValue("AutoPromoteToQueen", QVariant(ui.cbPromoteToQueen->isChecked()));
    AppSettings->setValue("AlwaysScale", QVariant(ui.alwaysScale->isChecked()));
    AppSettings->setValue("PlayerTurnBoard", ui.editPlayerTurnBoard->text());

    if(ui.boardThemeCombo->currentIndex() != ui.boardThemeCombo->count() - 1)
    {
        AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
    }
    else
    {
        AppSettings->setValue("boardTheme", QString());
    }
    QStringList colorNames;
    colorNames << "lightColor" << "darkColor" << "highlightColor"
               << "frameColor" << "currentMoveColor" << "storedMoveColor" << "threatColor"
               << "targetColor" << "checkColor" << "wallColor" << "underprotectedColor";
    saveColorList(ui.boardColorsList, colorNames);
    AppSettings->endGroup();

    // Save engine settings
    updateEngineData(ui.engineList->currentIndex().row());  // Make sure current edits are saved
    engineList.save();

    AppSettings->setValue("/General/EditLimit", ui.limitSpin->value());
    AppSettings->setValue("/History/MaxEntries", ui.spinBoxRecentFiles->value());
    AppSettings->setValue("/General/DefaultDataPath", ui.defaultDataBasePath->text());
    AppSettings->setValue("/General/ListFontSize", ui.spinBoxListFontSize->value());
    AppSettings->setValue("/MainWindow/VerticalTabs", ui.verticalTabs->isChecked());
    AppSettings->setValue("/MainWindow/ShowMenuIcons", ui.iconsVisible->isChecked());

    AppSettings->beginGroup("GameText");

    AppSettings->setValue("FontSize", ui.gameTextFontSizeSpin->value());
    AppSettings->setValue("ShowDiagrams", ui.cbShowDiagrams->isChecked());
    AppSettings->setValue("ColumnStyle", ui.cbColumnStyle->isChecked());
    AppSettings->setValue("VariationIndentLevel", ui.variationIndentLevel->value());
    AppSettings->setValue("DiagramSize", ui.diagramSize->value());
    AppSettings->setValue("PieceString", ui.pieceString->text());

    AppSettings->setValue("FontBrowserText", ui.fontText->text());
    AppSettings->setValue("FontBrowserMove", ui.fontMove->text());

    QStringList colorNamesNotation;
    colorNamesNotation << "MainLineMoveColor" << "VariationColor" << "CommentColor" << "NagColor";
    saveColorList(ui.notationColors, colorNamesNotation);

    AppSettings->endGroup();

    AppSettings->setValue("/GameList/AdditionalTags", ui.additionalTags->text());

    AppSettings->beginGroup("Tools");
    AppSettings->setValue("Path1", ui.extToolPath->text());
    AppSettings->setValue("CommandLine1", ui.extToolParameters->text());
    AppSettings->endGroup();

    AppSettings->beginGroup("Web");
    AppSettings->setValue("Favorite1", ui.webFavorite->text());
    AppSettings->setValue("AutoNumber1", ui.autoNumber->value());
    AppSettings->endGroup();

    AppSettings->beginGroup("FICS");
    AppSettings->setValue("userName", ui.userName->text());
    AppSettings->setValue("passWord", ui.passWord->text());
    AppSettings->setValue("guestLogin", ui.guestLogin->isChecked());
    AppSettings->setValue("useTimeseal", ui.btUseTimeseal->isChecked());
    AppSettings->endGroup();

    AppSettings->beginGroup("Sound");
    AppSettings->setValue("Move", ui.cbSoundOn->isChecked());
    AppSettings->endGroup();

    QDir().mkpath(ui.defaultDataBasePath->text());
}

bool PreferencesDialog::selectInCombo(QComboBox* combo, const QString& text)
{
    for(int i = 0; i < combo->count(); ++i)
    {
        if(combo->itemText(i) == text)
        {
            combo->setCurrentIndex(i);
            return true;
        }
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
    for(int i = 0; i < list->count(); ++i)
    {
        AppSettings->setValue(cfgnames[i], list->color(i));
    }
}

void PreferencesDialog::slotBtMoveFontClicked()
{
    bool ok;
    QFont currentFont = QFont(ui.fontMove->text());
    QFont font = QFontDialog::getFont(&ok, currentFont, this);
    if (ok)
    {
        QString fontFamily = font.family();
        ui.fontMove->setText(fontFamily);
    }
}

void PreferencesDialog::slotBtTextFontClicked()
{
    bool ok;
    QFont currentFont = QFont(ui.fontText->text());
    QFont font = QFontDialog::getFont(&ok, currentFont, this);
    if (ok)
    {
        QString fontFamily = font.family();
        ui.fontText->setText(fontFamily);
    }
}

void PreferencesDialog::setupIconInMenus(QObject* pObject)
{
    QList<QAction*> actions = pObject->findChildren<QAction*>();
    bool showIcons = AppSettings->getValue("/MainWindow/ShowMenuIcons").toBool();
    foreach (QAction* action, actions)
    {
        action->setIconVisibleInMenu(showIcons);
    }
}

void PreferencesDialog::buttonClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton sb = ui.buttonBox->standardButton(button);
    switch(sb)
    {
    case QDialogButtonBox::Ok:
        accept();
        break;
    case QDialogButtonBox::Apply:
        slotApply();
        break;
    case QDialogButtonBox::Reset:
        slotReset();
        break;
    case QDialogButtonBox::Cancel:
        reject();
        break;
    default:
        break;
    }
}

void PreferencesDialog::SlotPieceEffectActivated(int index)
{
    QStringList pathArray;
    pathArray << "." << "outline" << "shadow";
    QString path = pathArray.at(index);

    QStringList themes = AppSettings->getThemeList(path);
    QString currentTheme = ui.pieceThemeCombo->currentText();
    ui.pieceThemeCombo->clear();
    for(QStringList::Iterator it = themes.begin(); it != themes.end(); ++it)
    {
        (*it).truncate((*it).length() - 4);
        ui.pieceThemeCombo->addItem(*it);
    }
    if (!currentTheme.isEmpty())
    {
        ui.pieceThemeCombo->setCurrentText(currentTheme);
    }
}
