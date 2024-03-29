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


#ifndef SAVEDIALOG_H_INCLUDED
#define SAVEDIALOG_H_INCLUDED

#include "ui_savedialog.h"

class GameX;
class Database;
class QLineEdit;

/** @ingroup GUI
The SaveDialog class is a simple dialog to edit and save game header. */
class SaveDialog : public QDialog
{
    Q_OBJECT
public:
    static const int Discard = 2;

    SaveDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~SaveDialog();
    virtual int save(Database* database, GameX& game);

protected slots:
    void discardChanges();
    void restoreLayout();
    void accept();
    void reject();
    void buttonClicked(QAbstractButton*);
private:
    void setLineEdit(QLineEdit* edit, Database* database, const QString& tagName);
    Ui::SaveDialog ui;
    QButtonGroup* group;
};

#endif

