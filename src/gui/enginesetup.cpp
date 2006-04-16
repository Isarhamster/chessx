#include <qbuttongroup.h>
#include <qfiledialog.h>
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
	
	if(engine != -1) {
		QString engineName = engineListBox->text(engine);
		m_name = engineName;
		m_command = AppSettings->readEntry("/Engines/" + engineName + "/Command", "", NULL);
		m_protocol = ("UCI" == AppSettings->readEntry("/Engines/" + engineName + "/Protocol", "", NULL)) ? UCI : Winboard;
		nameLineEdit->setText(m_name);
		commandLineEdit->setText(m_command);
		if(m_protocol == Winboard) {
			winboardRadioButton->setChecked(true);
		} else {
			uciRadioButton->setChecked(true);
		}
		setControlsEnabled(true);
	} else {
		m_name = "";
		m_command = "";
		m_protocol = Winboard;
		nameLineEdit->setText("");
		commandLineEdit->setText("");
		winboardRadioButton->setChecked(true);
		setControlsEnabled(false);
	}
}

void EngineSetupDialog::slotNameChanged(const QString& name)
{
	//check it really has changed
	if(name == m_name) {
		return;
	}
	m_name = name;

	//update entry and list box
	int engine = engineListBox->currentItem();
	QString engineName = engineListBox->text(engine);

	removeEntry(engineName);
	writeEntry(m_name, m_command, m_protocol);
	engineListBox->changeItem(m_name, engine);
}

void EngineSetupDialog::slotCommandChanged(const QString& command)
{
	m_command = command;
	writeEntry(m_name, m_command, m_protocol);
}

void EngineSetupDialog::slotProtocolChanged(int)
{
	m_protocol =  winboardRadioButton->isOn() ? Winboard : UCI;
	writeEntry(m_name, m_command, m_protocol);
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
	int engine = engineListBox->currentItem();
	QString engineName = engineListBox->text(engine);

	removeEntry(engineName);
	engineListBox->removeItem(engine);
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
	writeEntry(engineName, engineName, Winboard);
	engineListBox->insertItem(engineName);
	engineListBox->setSelected(engineListBox->count() - 1, true);
}

void EngineSetupDialog::removeEntry(const QString& engineName)
{
	AppSettings->removeEntry("/Engines/" + engineName + "/Name");
	AppSettings->removeEntry("/Engines/" + engineName + "/Command");
	AppSettings->removeEntry("/Engines/" + engineName + "/Protocol");
}

void EngineSetupDialog::writeEntry(const QString& engineName, const QString& command, Protocol protocol)
{
	AppSettings->writeEntry("/Engines/" + engineName + "/Name", engineName);
	AppSettings->writeEntry("/Engines/" + engineName + "/Command", command);
	if(protocol == Winboard) {
		AppSettings->writeEntry("/Engines/" + engineName + "/Protocol", "Winboard");
	} else {
		AppSettings->writeEntry("/Engines/" + engineName + "/Protocol", "UCI");
	}
}

void EngineSetupDialog::setControlsEnabled(bool enabled)
{
	nameLabel->setEnabled(enabled);
	nameLineEdit->setEnabled(enabled);
	commandLabel->setEnabled(enabled);	
	commandLineEdit->setEnabled(enabled);
	protocolButtonGroup->setEnabled(enabled);
}
