#include "gamenotationwidget.h"

#include <QHBoxLayout>

#include "chessbrowser.h"

GameNotationWidget::GameNotationWidget(QWidget* parent)
    : QWidget(parent)
    , m_browser(nullptr)
{
    m_browser = new ChessBrowser(nullptr);

    // setup layout
    auto layout = new QHBoxLayout();
    layout->addWidget(m_browser);
    setLayout(layout);
}
