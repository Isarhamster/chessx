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

#include "colorlistitem.h"
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

PreferencesDialog::PreferencesDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
	connect(ui.boardColorsList, SIGNAL(itemActivated(QListWidgetItem*)),
		SLOT(slotColorItem(QListWidgetItem*)));
	connect(ui.boardColorsList, SIGNAL(itemClicked(QListWidgetItem*)),
			  SLOT(slotColorItem(QListWidgetItem*)));

	restoreSettings();
	AppSettings->beginGroup("/Board/");
	restoreColorItem(ui.boardColorsList, tr("Light squares"), "lightColor", "#a0a0a0");
	restoreColorItem(ui.boardColorsList, tr("Dark squares"), "darkColor", "#d0d0d0");
	restoreColorItem(ui.boardColorsList, tr("Highlighted squares"), "highlightColor", "#ffff00");
	restoreColorItem(ui.boardColorsList, tr("Frame"), "frameColor", "#000000");
	AppSettings->endGroup();
}

PreferencesDialog::~PreferencesDialog()
{}

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
	AppSettings->beginGroup("/Board/");
	ui.boardFrameCheck->setChecked(AppSettings->value("showFrame", true).toBool());
	ui.guessMoveCheck->setChecked(AppSettings->value("guessMove", true).toBool());
	QString pieceTheme = AppSettings->value("pieceTheme", "default").toString();
	QString boardTheme = AppSettings->value("boardTheme", "default").toString();
	AppSettings->endGroup();

	QStringList themes = QDir(AppSettings->dataPath() + "/themes").entryList(QStringList("*.png"));
	for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it) {
		(*it).truncate((*it).length() - 4);
		ui.pieceThemeCombo->addItem(*it);
		ui.boardThemeCombo->addItem(*it);
	}
	ui.boardThemeCombo->addItem(tr("[plain colors]"));
	selectInCombo(ui.pieceThemeCombo, pieceTheme);
	selectInCombo(ui.boardThemeCombo, boardTheme);

	// Read Players settings
	AppSettings->beginGroup("/Players/");
	ui.playersRatingsCheck->setChecked(AppSettings->value("rating", true).toBool());
	AppSettings->endGroup();
}

void PreferencesDialog::saveSettings()
{
	AppSettings->beginGroup("/General/");
	AppSettings->setValue("confirmQuit", ui.quitCheck->isChecked());
	AppSettings->setValue("onlineTablebases", ui.tablebaseCheck->isChecked());
	AppSettings->endGroup();
	AppSettings->beginGroup("/Board/");
	AppSettings->setValue("showFrame", ui.boardFrameCheck->isChecked());
	AppSettings->setValue("guessMove", ui.guessMoveCheck->isChecked());
	AppSettings->setValue("pieceTheme", ui.pieceThemeCombo->currentText());
	if (ui.boardThemeCombo->currentIndex() != ui.boardThemeCombo->count() - 1)
		AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
	else
		AppSettings->setValue("boardTheme", QString());
	saveColorList(ui.boardColorsList);
	AppSettings->endGroup();
	AppSettings->beginGroup("/Players/");
	AppSettings->setValue("rating", ui.playersRatingsCheck->isChecked());
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

void PreferencesDialog::slotColorItem(QListWidgetItem* item)
{
	ColorListItem* c = qobject_cast<ColorListItem*>(item->listWidget()->itemWidget(item));
	if (c)
		c->edit();
}

void PreferencesDialog::restoreColorItem(QListWidget* list, const QString& text, const QString& cfgname,
		const QColor& cfgcolor)
{
	QListWidgetItem* item = new QListWidgetItem(list);
	QColor color = AppSettings->value(cfgname, cfgcolor).value<QColor>();
	ColorListItem* citem = new ColorListItem(item, text, color);
	citem->setData(cfgname);
	list->setItemWidget(item, citem);
}

void PreferencesDialog::saveColorList(QListWidget* list)
{
	for (int i = 0; i < list->count(); i++) {
		ColorListItem* c = qobject_cast<ColorListItem*>(list->itemWidget(list->item(i)));
		if (c)
			AppSettings->setValue(c->data().toString(), c->color());
	}
}
