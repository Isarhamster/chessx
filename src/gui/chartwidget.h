/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPolygonF>

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = 0);
    virtual ~ChartWidget();

    void setValues(const QList<double> &values);
    void setPly(int ply);

signals:
    void halfMoveRequested(int);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    void updatePolygon();
    void updatePly();

private:
    QPolygonF m_polygon;
    QList<double> m_values;
    int m_ply;
    double m_plyIndicator;
};

#endif // CHARTWIDGET_H
