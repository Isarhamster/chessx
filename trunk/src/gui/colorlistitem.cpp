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

#include "colorlistitem.h"

#include <QColorDialog>
#include <QListWidget>
#include <QPainter>
#include <QPaintEvent>

ColorListItem::ColorListItem(QListWidgetItem* list)	: QWidget(list->listWidget()), m_list(list)
{}

ColorListItem::ColorListItem(QListWidgetItem* list, const QString& text, const QColor& color)
		: QWidget(list->listWidget()), m_list(list), m_text(text), m_color(color)
{}

void ColorListItem::setColor(const QColor& color)
{
	m_color = color;
}

QColor ColorListItem::color() const
{
	return m_color;
}

void ColorListItem::setText(const QString& text)
{
	m_text = text;
}

QColor ColorListItem::text() const
{
	return m_text;
}

void ColorListItem::setData(const QVariant& data)
{
	m_data = data;
}

QVariant ColorListItem::data() const
{
	return m_data;
}

void ColorListItem::edit()
{
	QColor col = QColorDialog::getColor(color());
	if (col.isValid())
		setColor(col);
}

void ColorListItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (m_list == m_list->listWidget()->currentItem()) {
		painter.fillRect(event->rect(), palette().highlight());
		painter.setPen(palette().color(QPalette::HighlightedText));
	} else {
		painter.fillRect(event->rect(), palette().base());
		painter.setPen(palette().color(QPalette::Text));
	}
	painter.setBrush(m_color);
	painter.drawRect(2, 2, 100, height() - 5);
	painter.drawText(110, height() - 5, m_text);
}

