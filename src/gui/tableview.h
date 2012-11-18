/****************************************************************************
* QTableView with some adjustments like storing header layout               *
* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                            *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QtCore>
#include <QtGui/QTableView>

/**
	The TableView class is a specialized version of QTableView with automatic
	header layout saving.

*/

class TableView : public QTableView
{
Q_OBJECT
public:
   TableView(QWidget *parent = 0);
	~TableView();
public slots:
	/** Store current configuration. */
	void saveConfig();
	/** Restore current configuration. */
    virtual void slotReconfigure();
    /** Show context menu on header view */
    void ShowContextMenu(const QPoint&);
};

#endif
