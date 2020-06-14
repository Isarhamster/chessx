#include "gamenotationwidget.h"

#include <QHBoxLayout>

#include "chessbrowser.h"
#include "settings.h"

GameNotationWidget::GameNotationWidget(QWidget* parent)
    : QWidget(parent)
    , m_browser(nullptr)
{
    m_browser = new ChessBrowser(nullptr);

    // setup layout
    auto layout = new QHBoxLayout();
    layout->addWidget(m_browser);
    setLayout(layout);

    configureFont();

    connect(m_browser, &QTextBrowser::anchorClicked, this, &GameNotationWidget::anchorClicked);
    connect(m_browser, &ChessBrowser::actionRequested, this, &GameNotationWidget::actionRequested);
    connect(m_browser, &ChessBrowser::queryActiveGame, this, &GameNotationWidget::queryActiveGame);
    connect(m_browser, &ChessBrowser::signalMergeGame, this, &GameNotationWidget::signalMergeGame);
}

QString GameNotationWidget::getHtml() const
{
    return m_browser->toHtml();
}

QString GameNotationWidget::getText() const
{
    return m_browser->toPlainText();
}

void GameNotationWidget::setText(const QString& text)
{
    m_browser->setText(text);
}

QMap<Nag, QAction*> GameNotationWidget::nagActions() const
{
    QMap<Nag, QAction*> result;
    const auto& actions = m_browser->m_actions;
    for (auto it = actions.cbegin(); it != actions.cend(); ++it)
    {
        auto action = it.key();
        const auto& e = it.value();
        if (e.type() == EditAction::AddNag)
        {
            Nag nag = static_cast<Nag>(e.data().toInt());
            result[nag] = action;
        }
    }
    return result;
}

void GameNotationWidget::saveConfig()
{
    AppSettings->setLayout(this);
}

void GameNotationWidget::slotReconfigure()
{
    AppSettings->layout(this);
    configureFont();
}

void GameNotationWidget::showMove(int id)
{
    m_browser->showMove(id);
}

void GameNotationWidget::configureFont()
{
    QFont f = qApp->font();
    qreal r = AppSettings->getValue("/GameText/FontSize").toInt();
    f.setPointSize(r);
    QString fontFamily = AppSettings->getValue("/GameText/FontBrowserText").toString();
    if (!fontFamily.isEmpty())
    {
        f.setFamily(fontFamily);
    }
    setFont(f);
}
