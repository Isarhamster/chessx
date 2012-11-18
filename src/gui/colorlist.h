/***************************************************************************
                          colorlist  -  QListWidget item to edit colors
                             -------------------
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
#ifndef COLORLIST_H
#define COLORLIST_H

#include <QtGui/QListWidget>

/**
	@ingroup GUI
	The ColorList class is a list of editable colors.
*/
class ColorList : public QListWidget
{
	Q_OBJECT
public:
	/** Standard constructor. */
	ColorList(QWidget* parent);
	/** Standard constructor. */
	virtual ~ColorList();
	/** @return color of given item. */
	QColor color(QListWidgetItem*) const;
	/** @return color of given item. */
	QColor color(int index) const;
	/** Create new item. */
	virtual void addItem(const QString& text, const QColor& color);
protected:
	/** Edit color on left click. */
	virtual void mousePressEvent(QMouseEvent* event);
	/** Edit color on Enter. */
	virtual void keyPressEvent(QKeyEvent* event);
	/** Set item color. */
	void setItemColor(QListWidgetItem* item, const QColor& color);
	/** Edit item color. */
	void editItemColor(QListWidgetItem*);
};

#endif
