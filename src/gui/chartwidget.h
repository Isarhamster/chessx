/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QPolygonF>

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    virtual ~ChartWidget();

    void setValues(int line, const QList<double> &values);
    void setPly(int ply);

signals:
    void halfMoveRequested(int);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    void updatePolygon(int line);
    void updatePly();
    void updatePolygons();

private:
    QVector<QPolygonF> m_polygon;
    QVector<QList<double>> m_values;
    int m_ply;
    double m_plyIndicator;
};

#endif // CHARTWIDGET_H
