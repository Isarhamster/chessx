/***************************************************************************
                          colorlistitem  -  QListWidget item to edit color
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
#ifndef COLORLISTITEM_H
#define COLORLISTITEM_H

class QListWidgetItem;

#include <QVariant>
#include <QWidget>

/**
	@ingroup GUI
	The ColorListItem class is a helper class to edit color in QListWidget.
*/
class ColorListItem : public QWidget
{
public:
	/** Standard constructor. */
	ColorListItem(QListWidgetItem* item);
	/** Standard constructor with initialization. */
	ColorListItem(QListWidgetItem* item, const QString& text, const QColor& color);
	/** Change color. */
	void setColor(const QColor& color);
	/** @return current color. */
	QColor color() const;
	/** Change text. */
	void setText(const QString& text);
	/** @return current text. */
	QColor text() const;
	/** Change data. */
	void setData(const QVariant& dats);
	/** @return current data. */
	QVariant data() const;
public slots:
	void edit();
protected:
	virtual void paintEvent(QPaintEvent*);
private:
	QListWidgetItem* m_list;
	QString m_text;
	QColor m_color;
	QVariant m_data;
};

#endif
