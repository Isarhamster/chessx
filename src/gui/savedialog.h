/***************************************************************************
                          Savedialog  -  dialog for saving game
                             -------------------
    begin                : Mon 17 Apr 2006
    copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef __SAVEDIALOG_H__
#define __SAVEDIALOG_H__

#include "ui_savedialog.h"
#include "common.h"

class Database;
class Game;
class QLineEdit;

/** @ingroup GUI
The SaveDialog class is a simple dialog to edit and save game header. */
class SaveDialog : public QDialog
{
	Q_OBJECT
public:
    static const int Discard = 2;

	SaveDialog(QWidget* parent = 0);
	virtual ~SaveDialog();
    virtual int save(Database* database, Game& game);
    virtual void accept();
protected slots:
    void discardChanges();
private:
    void setLineEdit(QLineEdit* edit, Database* database, const QString& tagName);
	Ui::SaveDialog ui;
};


#endif

