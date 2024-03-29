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

#ifndef COPYDIALOG_H_INCLUDED
#define COPYDIALOG_H_INCLUDED

#include "ui_copydialog.h"

/**  @ingroup GUI
  The CopyDialog class represents a dialog for choosing target database
for copied games.  */

class CopyDialog : public QDialog
{
    Q_OBJECT
public:
    enum SrcMode {SingleGame, Selection, Filter, AllGames};
    /** Create ChessX Preferences dialog */
    CopyDialog(QWidget* parent = nullptr,
               Qt::WindowFlags f = Qt::WindowFlags());
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
    /** Set the current games title. */
    void setCurrentGame(QString title, int selectedGames, int gamesInFilter, int gamesInDatabase);

protected slots:
  void accept();
  void reject();

private:
    Ui::CopyDialog ui;
};


#endif
