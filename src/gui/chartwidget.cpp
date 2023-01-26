/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "chartwidget.h"

#include <QColor>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QSharedPointer>
#include <cmath>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent),
    m_ply(0),
    m_plyIndicator(0),
    m_lastSentIndicator(0)
{
    setAutoFillBackground(true);
    setUpdatesEnabled(true);
}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::setValues(int line, const QList<double>& values)
{
    if (line >= m_values.size())
    {
        QList<double> l;
        m_values.insert(line, l);
    }
    m_values[line].clear();
    foreach(double d,values)
    {
        m_values[line]<<d;
    }
    updatePolygon(line);
}

void ChartWidget::setPly(int ply)
{
    m_ply = ply;
    updatePly();
    update();
}

void ChartWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setWindow(0,0,width(),height());
    painter.eraseRect(0,0,width(),height());

    QPen pen0(Qt::SolidLine);
    pen0.setColor(palette().color(QPalette::Mid));
    QColor midHalf = palette().color(QPalette::Mid);
    midHalf.setAlpha(50);

    QPen pen1(Qt::DashLine);
    pen1.setColor(midHalf);
    QPen pen2(Qt::DotLine);
    pen2.setColor(midHalf);

    painter.setPen(pen0);
    painter.drawRect(0,0,width(),height());
    painter.setPen(pen1);

    for (int i=0;i<m_polygon.count();++i)
    {
        const QPolygonF& polygon = m_polygon.at(i);

        if (i==0)
        {
            for (int j=10;j<polygon.count();j+=10)
            {
                painter.drawLine(polygon[j].x(),0,polygon[j].x(),height());
            }
        }

        QColor pcol = palette().color(i==0 ? QPalette::BrightText : QPalette::Midlight);
        QColor col = pcol;
        pcol.setAlpha(100.0*1.0/(i+1));
        col.setAlpha(50.0*1.0/(i+1));

        QPen pen3(Qt::SolidLine);
        pen3.setColor(pcol);
        painter.setPen(pen3);

        QBrush brush(Qt::SolidPattern);
        brush.setColor(col);
        painter.setBrush(brush);

        painter.setRenderHints(QPainter::Antialiasing);
        painter.drawPolygon(polygon);
    }

    QPen pen4(Qt::SolidLine);
    QColor colPly = palette().color(QPalette::BrightText);
    pen4.setColor(colPly);
    pen4.setWidth(2);
    painter.setPen(pen4);
    painter.drawLine(m_plyIndicator,3*height()/4,m_plyIndicator,height()/4);

    painter.setPen(pen2);
    painter.drawLine(0,height()/2,width(),height()/2);
}

void ChartWidget::resizeEvent(QResizeEvent*)
{
    updatePly();
    updatePolygons();
}

void ChartWidget::handleMouseEvent(QMouseEvent *event)
{
    if (width() && m_values.size() && (m_values[0].count()>1))
    {
    #if QT_VERSION < 0x050000
        QPointF p = event->posF();
    #else
        QPointF p = event->localPos();
    #endif

        double multiplierW = ((double)width()) / (m_values[0].count()-1);
        double x = 0.5 + (p.x() / multiplierW);
        if (m_lastSentIndicator!=(int)x)
        {
            emit halfMoveRequested((int)x);
            m_lastSentIndicator = (int)x;
        }
    }
}

#if QT_VERSION < 0x060000
void ChartWidget::enterEvent(QEvent *event)
#else
void ChartWidget::enterEvent(QEnterEvent *event)
#endif
{
    setCursor(QCursor(Qt::SplitHCursor));
    QWidget::enterEvent(event);
    m_lastSentIndicator = m_ply;
}

void ChartWidget::leaveEvent(QEvent *event)
{
    unsetCursor();
    QWidget::leaveEvent(event);
}

void ChartWidget::mousePressEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
    QWidget::mousePressEvent(event);
}

void ChartWidget::mouseMoveEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
    QWidget::mouseMoveEvent(event);
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
    QWidget::mouseReleaseEvent(event);
}

void ChartWidget::updatePly()
{
    if (m_values.size() && (m_values[0].count()>1))
    {
        double multiplierW = ((double)width()) / (m_values[0].count()-1);
        m_plyIndicator = m_ply * multiplierW;
    }
    else
    {
        m_plyIndicator = 0;
    }
}

void ChartWidget::updatePolygon(int line)
{
    if (line >= m_polygon.size())
    {
        m_polygon.insert(line, *new QPolygonF());
    }
    setUpdatesEnabled(false);
    QList<double>& values = m_values[line];
    QPolygonF& polygon = m_polygon[line];
    polygon.clear();
    if (values.count()>1)
    {
        double max = 0;
        foreach(double d, values)
        {
            double absd = std::abs(d);
            if (absd > max) max = absd;
        }

        double multiplierH = (max != 0.0) ? ((double)height()) / (max*2.0) : 0.0;
        double multiplierW = ((double)width()) / (values.count()-1);

        polygon<<QPointF(0.0, height()/2.0);
        int i = 0;
        foreach(double d, values)
        {
            polygon<<QPointF(multiplierW*i,-d*multiplierH+height()/2.0);
            ++i;
        }
        if (i)
        {
            polygon<<QPointF(multiplierW*i, height()/2.0);
        }
    }
    setUpdatesEnabled(true);
}

void ChartWidget::updatePolygons()
{
    for (int i=0; i<m_values.count();++i)
    {
        updatePolygon(i);
    }
}
