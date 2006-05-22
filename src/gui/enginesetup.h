/***************************************************************************
                          enginesetup.h  -  engine setup dialog
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

#include "enginesetupbase.h"

class EngineSetupDialog : public EngineSetupDialogBase
{
	Q_OBJECT
	
	public:
		EngineSetupDialog(QWidget* parent = 0);
		~EngineSetupDialog();

	public slots:
		void slotListChanged(QListBoxItem*);
		void slotNameChanged(const QString& name);
		void slotCommandChanged(const QString& command);
		void slotProtocolChanged(int);
		void slotBrowse();
		void slotDelete();
		void slotNew();

	private:
		typedef enum { Winboard, UCI } Protocol;	

		void removeEntry();
		void writeEntry();
		void setControlsEnabled(bool enabled);
		void updateControls();

		int m_engine;
		QString m_name;
		QString m_command;
		Protocol m_protocol;
};
