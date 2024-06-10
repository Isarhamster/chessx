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

    void setValues(int line, const QList<double> &values);
    void setPly(int ply);

signals:
    void halfMoveRequested(int);

protected:
    void handleMouseEvent(QMouseEvent *event, bool deduplicate = true);
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
#if QT_VERSION < 0x060000
    virtual void enterEvent(QEvent *event);
#else
    virtual void enterEvent(QEnterEvent *event);
#endif
    virtual void leaveEvent(QEvent *event);

    void updatePolygon(int line);
    void updatePly();
    void updatePolygons();

private:
    QVector<QPolygonF> m_polygon;
    QVector<QList<double>> m_values;
    int m_ply;
    double m_plyIndicator;
    int m_lastSentIndicator;
};

#endif // CHARTWIDGET_H
