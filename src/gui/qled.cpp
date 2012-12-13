/***************************************************************************
 *   Copyright (C) 2010 by P. Sereno                                       *
 *   http://www.sereno-online.com                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation              *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *   http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.               *
 ***************************************************************************/
  
#include <QColor>
#include <QtGlobal>
#include <QtGui>
#include <QPolygon>
#include <QtSvg>
#include <QSvgRenderer>

#include "qled.h"

/*!
  \brief QLed: this is the QLed constructor.
  \param parent: The Parent Widget
*/
QLed::QLed(QWidget *parent)
    : QWidget(parent)
{
   m_value=false;
   m_onColor=Red;
   m_offColor=Grey;
   m_shape=Circle;
   shapes << ":/images/circle_";
   colors << "red.svg" << "green.svg" << "yellow.svg" << "grey.svg" << "orange.svg" << "blue.svg" << "black.svg";
   renderer = new QSvgRenderer();
}

QLed::~QLed()
{
	delete renderer;
}

/*!
  \brief paintEvent: painting method
  \param QPaintEvent *
  \return void
*/
void QLed::paintEvent(QPaintEvent *)
{  
    QString ledShapeAndColor;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    ledShapeAndColor=shapes[m_shape];

    if(m_value)
        ledShapeAndColor.append(colors[m_onColor]);
    else
        ledShapeAndColor.append(colors[m_offColor]);

    renderer->load(ledShapeAndColor);
    renderer->render(&painter);
}

void QLed::resizeEvent(QResizeEvent*)
{
    setMinimumWidth(height());
}

/*!
  \brief setOnColor: this method allows to change the On color {Red,Green,Yellow,Grey,Orange,Purple,blue}
  \param ledColor newColor
  \return void
*/
void QLed::setOnColor(ledColor newColor)
{
   m_onColor=newColor;
   update();
}


/*!
  \brief setOffColor: this method allows to change the Off color {Red,Green,Yellow,Grey,Orange,Purple,blue}
  \param ledColor newColor
  \return void
*/
void QLed::setOffColor(ledColor newColor)
{
   m_offColor=newColor;
   update();
}


/*!
  \brief setShape: this method allows to change the led shape {Circle,Square,Triangle,Rounded rectangle}
  \param ledColor newColor
  \return void
*/
void QLed::setShape(ledShape newShape)
{
   m_shape=newShape;
   update();
}


/*!
  \brief setValue: this method allows to set the led value {true,false}
  \param ledColor newColor
  \return void
*/
void QLed::setValue(bool value)
{
   m_value=value;
   update();
}


/*!
  \brief toggleValue: this method toggles the led value
  \param ledColor newColor
  \return void
*/
void QLed::toggleValue()
{ 
	m_value=!m_value;
	update();
	return; 
}
