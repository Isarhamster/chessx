/***************************************************************************
                          enginesetup.cpp  -  engine setup dialog
                             -------------------
    begin                : 16 April 2006
    copyright            : (C) 2006 William Hoggarth
                           <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qbuttongroup.h>
#include <qfiledialog.h>
#include <qglobal.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

#include "settings.h"

#include "enginesetup.h"

EngineSetupDialog::EngineSetupDialog(QWidget* parent) : EngineSetupDialogBase(parent)
{
	//no current engine active
	m_engine = -1;
	m_name = "";
	m_command = "";
	m_protocol = Winboard;

	//read in engine list from app setting
	QStringList engines = AppSettings->subkeyList("/Engines/");
	engineListBox->insertStringList(engines);

	//connect up controls
	connect(engineListBox, SIGNAL(currentChanged(QListBoxItem*)), SLOT(slotListChanged(QListBoxItem*)));
	connect(nameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotNameChanged(const QString&)));
	connect(commandLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotCommandChanged(const QString&)));
	connect(protocolButtonGroup, SIGNAL(clicked(int)), SLOT(slotProtocolChanged(int)));
	connect(browsePushButton, SIGNAL(clicked()), SLOT(slotBrowse()));
	connect(newPushButton, SIGNAL(clicked()), SLOT(slotNew()));
	connect(deletePushButton, SIGNAL(clicked()), SLOT(slotDelete()));

	//select first engine
	if(engineListBox->count()) {
		engineListBox->setSelected(0, true);
	}
}

EngineSetupDialog::~EngineSetupDialog()
{
}

void EngineSetupDialog::slotListChanged(QListBoxItem*)
{
	//check it really has changed
	int engine = engineListBox->currentItem();
	if(engine == m_engine) {
		return;
	}
	m_engine = engine;
	
	//update controls as required
	if(m_engine != -1) {
		m_name = engineListBox->text(m_engine);
		m_command = AppSettings->readEntry("/Engines/" + m_name + "/Command", "", NULL);
		m_protocol = ("UCI" == AppSettings->readEntry("/Engines/" + m_name + "/Protocol", "", NULL)) ? UCI : Winboard;
		updateControls();
		setControlsEnabled(true);
	} else {
		m_name = "";
		m_command = "";
		m_protocol = Winboard;
		updateControls();
		setControlsEnabled(false);
	}
}

void EngineSetupDialog::slotNameChanged(const QString& name)
{
	//check it really has changed
	if(name == m_name) {
		return;
	}
	
	//update entry and list box
	removeEntry();
	m_name = name;
	writeEntry();
	engineListBox->changeItem(m_name, m_engine);
}

void EngineSetupDialog::slotCommandChanged(const QString& command)
{
	m_command = command;
	writeEntry();
}

void EngineSetupDialog::slotProtocolChanged(int)
{
	m_protocol =  winboardRadioButton->isOn() ? Winboard : UCI;
	writeEntry();
}

void EngineSetupDialog::slotBrowse()
{
	QString command = QFileDialog::getOpenFileName("","",this,"","Select a chess engine executable");
	if(command != "") {
		commandLineEdit->setText(command);
	}
}

void EngineSetupDialog::slotDelete()
{
	removeEntry();
	engineListBox->removeItem(m_engine);
	
	if(engineListBox->count() == 0) {
		m_name = "";
		m_command = "";
		m_protocol = Winboard;
		updateControls();
	} else {
		if (m_engine > (signed int)engineListBox->count() - 1) {
			m_engine = engineListBox->count() - 1;
		}
		m_name = engineListBox->text(m_engine);
		m_command = AppSettings->readEntry("/Engines/" + m_name + "/Command", "", NULL);
		m_protocol = ("UCI" == AppSettings->readEntry("/Engines/" + m_name + "/Protocol", "", NULL)) ? UCI : Winboard;
		updateControls();
		engineListBox->setSelected(m_engine, true);
	}
}

void EngineSetupDialog::slotNew()
{
	//create unique name
	QString engineName = "New Engine";
	bool exists;
	AppSettings->readEntry("/Engines/" + engineName + "/Name", "", &exists);
	int number = 2;
	while(exists) {
		engineName = "New Engine " + QString::number(number++);
		AppSettings->readEntry("/Engines/" + engineName + "/Name", "", &exists);
	}

	//create the entry	
	m_engine = engineListBox->count();
	m_name = engineName;
	m_command = engineName;
	m_protocol = Winboard;
	
	writeEntry();
	engineListBox->insertItem(m_name);
	engineListBox->setSelected(m_engine, true);
	
	//update controls
	updateControls();
	setControlsEnabled(true);
}

void EngineSetupDialog::removeEntry()
{
	AppSettings->removeEntry("/Engines/" + m_name + "/Name");
	AppSettings->removeEntry("/Engines/" + m_name + "/Command");
	AppSettings->removeEntry("/Engines/" + m_name + "/Protocol");
}

void EngineSetupDialog::writeEntry()
{
	AppSettings->writeEntry("/Engines/" + m_name + "/Name", m_name);
	AppSettings->writeEntry("/Engines/" + m_name + "/Command", m_command);
	if(m_protocol == Winboard) {
		AppSettings->writeEntry("/Engines/" + m_name + "/Protocol", "Winboard");
	} else {
		AppSettings->writeEntry("/Engines/" + m_name + "/Protocol", "UCI");
	}
}

void EngineSetupDialog::setControlsEnabled(bool enabled)
{
	nameLabel->setEnabled(enabled);
	nameLineEdit->setEnabled(enabled);
	commandLabel->setEnabled(enabled);	
	commandLineEdit->setEnabled(enabled);
	browsePushButton->setEnabled(enabled);
	protocolButtonGroup->setEnabled(enabled);
	deletePushButton->setEnabled(enabled);
}

void EngineSetupDialog::updateControls()
{
	nameLineEdit->setText(m_name);
	commandLineEdit->setText(m_command);
	if(m_protocol == Winboard) {
		winboardRadioButton->setChecked(true);
	} else {
		uciRadioButton->setChecked(true);
	}
}
