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

#include <q3buttongroup.h>
#include <q3filedialog.h>
#include <qglobal.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <q3listbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

#include "settings.h"

#include "enginesetup.h"

EngineSetupDialog::EngineSetupDialog(QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

 //no current engine active
	m_engine = -1;
	m_name = QString();
	m_command = QString();
	m_protocol = Winboard;

	//read in engine list from app setting
	QStringList engines = AppSettings->subkeyList("/Engines/");
	ui.engineListBox->insertStringList(engines);

	//connect up controls
	connect(ui.engineListBox, SIGNAL(currentChanged(Q3ListBoxItem*)), SLOT(slotListChanged(Q3ListBoxItem*)));
	connect(ui.nameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotNameChanged(const QString&)));
	connect(ui.commandLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotCommandChanged(const QString&)));
	connect(ui.protocolButtonGroup, SIGNAL(clicked(int)), SLOT(slotProtocolChanged(int)));
	connect(ui.browsePushButton, SIGNAL(clicked()), SLOT(slotBrowse()));
	connect(ui.newPushButton, SIGNAL(clicked()), SLOT(slotNew()));
	connect(ui.deletePushButton, SIGNAL(clicked()), SLOT(slotDelete()));

	//select first engine
	if(ui.engineListBox->count()) {
		ui.engineListBox->setSelected(0, true);
	}
}

EngineSetupDialog::~EngineSetupDialog()
{
}

void EngineSetupDialog::slotListChanged(Q3ListBoxItem*)
{
  //check it really has changed
  int engine = ui.engineListBox->currentItem();
  if (engine == m_engine)
    return;

  m_engine = engine;
  
  //update controls as required
  if(m_engine != -1) {
    m_name = ui.engineListBox->text(m_engine);
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
  ui.engineListBox->changeItem(m_name, m_engine);
}

void EngineSetupDialog::slotCommandChanged(const QString& command)
{
  m_command = command;
  writeEntry();
}

void EngineSetupDialog::slotProtocolChanged(int)
{
  m_protocol =  ui.winboardRadioButton->isOn() ? Winboard : UCI;
  writeEntry();
}

void EngineSetupDialog::slotBrowse()
{
  QString command = Q3FileDialog::getOpenFileName("","",this,"","Select a chess engine executable");
  if(command != "") {
    ui.commandLineEdit->setText(command);
  }
}

void EngineSetupDialog::slotDelete()
{
  removeEntry();
  ui.engineListBox->removeItem(m_engine);

  if (ui.engineListBox->count() == 0) 
  {
    m_name = "";
    m_command = "";
    m_protocol = Winboard;
    updateControls();
  }
  else 
  {
    if (m_engine > (signed int)ui.engineListBox->count() - 1) {
      m_engine = ui.engineListBox->count() - 1;
    }
    m_name = ui.engineListBox->text(m_engine);
    m_command = AppSettings->readEntry("/Engines/" + m_name + "/Command", "", NULL);
    m_protocol = ("UCI" == AppSettings->readEntry("/Engines/" + m_name + "/Protocol", "", NULL)) ? UCI : Winboard;
    updateControls();
    ui.engineListBox->setSelected(m_engine, true);
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
	m_engine = ui.engineListBox->count();
	m_name = engineName;
	m_command = engineName;
	m_protocol = Winboard;
	
	writeEntry();
	ui.engineListBox->insertItem(m_name);
	ui.engineListBox->setSelected(m_engine, true);
	
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
  ui.nameLabel->setEnabled(enabled);
  ui.nameLineEdit->setEnabled(enabled);
  ui.commandLabel->setEnabled(enabled);	
  ui.commandLineEdit->setEnabled(enabled);
  ui.browsePushButton->setEnabled(enabled);
  ui.protocolButtonGroup->setEnabled(enabled);
  ui.deletePushButton->setEnabled(enabled);
}

void EngineSetupDialog::updateControls()
{
  ui.nameLineEdit->setText(m_name);
  ui.commandLineEdit->setText(m_command);
  if (m_protocol == Winboard)
    ui.winboardRadioButton->setChecked(true);
  else
    ui.uciRadioButton->setChecked(true);
}
