/***************************************************************************
                          quicksearch.h  -  Quick Tag search dialog
                             -------------------
    begin                : Sun 6 May 2007
    copyright            : (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUICKSEARCH_H
#define QUICKSEARCH_H

#include "ui_quicksearch.h"

class QuickSearchDialog : public QDialog
{
    Q_OBJECT
public:
    /** Create Quick search dialog */
    QuickSearchDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
    /** Destroy the dialog */
    virtual ~QuickSearchDialog();
    /** Set default tag */
    void setTag(int tag);
    /** Fill the tag combo box */
    void addTag(QString tag);
    /** @return current tag */
    int tag() const;
    /** Set default mode */
    void setMode(int index);
    /** @return current mode for combining searches. */
    int mode() const;
    /** @return current value. */
    QString value() const;

public slots:
    /** Execute dialog, save options on exit */
    int exec();
protected slots:
    void reject();
    void accept();
private:
    Ui::QuickSearchDialog ui;
};

#endif

