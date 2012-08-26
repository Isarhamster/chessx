/***************************************************************************
*   CopyDialog - dialog to choose target database when copying games       *
*   copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>  *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __COPYDIALOG_H__
#define __COPYDIALOG_H__

#include "ui_copydialog.h"

/**  @ingroup GUI
  The CopyDialog class represents a dialog for choosing target database
for copied games.  */

class CopyDialog : public QDialog
{
	Q_OBJECT
	public:
        enum SrcMode {SingleGame, Filter, AllGames};
		/** Create ChessX Preferences dialog */
		CopyDialog(QWidget* parent = 0);
		/** Destroy the dialog */
		virtual ~CopyDialog();
		/** Set current board */
		void setDatabases(const QStringList& databases);
		/** Get target database */
		int getDatabase() const;
		/** Get selected mode. */
		int getMode() const;
        /** Set the preferred mode. */
        void setMode(SrcMode mode);
	private:
		Ui::CopyDialog ui;
};


#endif
