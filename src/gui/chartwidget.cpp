/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "chartwidget.h"

#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setUpdatesEnabled(true);
}

ChartWidget::~ChartWidget()
{
   m_values.clear();
   m_polygon.clear();
}

void ChartWidget::setValues(const QList<double>& values)
{
    m_values.clear();
    foreach(double d,values)
    {
        m_values<<d;
    }
    updatePolygon();
    repaint();
}

void ChartWidget::setPly(int ply)
{
    m_ply = ply;
    updatePly();
    repaint();
}

void ChartWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setWindow(0,0,width(),height());
    painter.eraseRect(0,0,width(),height());

    QPen pen0(Qt::SolidLine);
    pen0.setColor(palette().color(QPalette::Window));
    painter.setPen(pen0);
    painter.drawRect(0,0,width(),height());

    QPen pen1(Qt::DashDotLine);
    pen1.setColor(palette().color(QPalette::Mid));
    painter.setPen(pen1);
    painter.drawLine(0,height()/2,width(),height()/2);

    for (int i=10;i<m_polygon.count();i+=10)
    {
        painter.drawLine(m_polygon[i].x(),0,m_polygon[i].x(),height());
    }

    QPen pen2(Qt::SolidLine);
    pen2.setColor(palette().color(QPalette::Dark));
    painter.setPen(pen2);

    painter.drawEllipse(m_plyIndicator,height()/2-2,3,3);

    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawPolyline(m_polygon);
}

void ChartWidget::resizeEvent(QResizeEvent*)
{
    updatePly();
    updatePolygon();
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (width())
    {
#if QT_VERSION < 0x050000
        QPointF p = event->posF();
#else
        QPointF p = event->localPos();
#endif

        double x = 0.5 + (p.x() * (m_values.count()) / width());
        emit halfMoveRequested((int)x);
    }
}

void ChartWidget::updatePly()
{
    double multiplierW = ((double)width()) / (m_values.count()-1);
    m_plyIndicator = m_ply * multiplierW;
}

void ChartWidget::updatePolygon()
{
    m_polygon.clear();
    if (m_values.count()>1)
    {
        double max = 0;
        foreach(double d, m_values)
        {
            double absd = abs(d);
            if (absd > max) max = absd;
        }

        double multiplierH = (max != 0.0) ? ((double)height()) / (max*2.0) : 0.0;
        double multiplierW = ((double)width()) / (m_values.count()-1);

        int i = 0;
        foreach(double d, m_values)
        {
            m_polygon<<QPointF(multiplierW*i,-d*multiplierH+height()/2.0);
            ++i;
        }
    }
}

