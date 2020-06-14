#include "gametoolbar.h"

#include <QLCDNumber>

#include "chartwidget.h"

GameToolBar::GameToolBar(const QString& title, QWidget* parent)
    : QToolBar(title, parent)
    , m_clock1(nullptr)
    , m_clock2(nullptr)
    , m_chart(nullptr)
{
    setObjectName("GameToolBar");

    m_clock1 = new QLCDNumber(7, this);
    m_clock1->setSegmentStyle(QLCDNumber::Flat);
    m_clock1->setObjectName("Clock0");
    m_clock1->display("1:00:00");
    addWidget(m_clock1);

    m_chart = new ChartWidget(this);
    m_chart->setObjectName("ChartWidget");
    m_chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(m_chart);

    m_clock2 = new QLCDNumber(7, this);
    m_clock2->setSegmentStyle(QLCDNumber::Flat);
    m_clock2->setObjectName("Clock1");
    m_clock2->display("1:00:00");
    addWidget(m_clock2);
}
