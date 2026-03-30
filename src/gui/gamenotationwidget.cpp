#include "gamenotationwidget.h"

#include <QHBoxLayout>

#include "chessbrowser.h"
#include "settings.h"
#include "output.h"
#include "boardview.h"

GameNotationWidget::GameNotationWidget(QWidget* parent)
    : QWidget(parent)
    , m_browser(nullptr)
    , m_output(nullptr)
{
    m_browser = new ChessBrowser(nullptr);

    // setup layout
    auto layout = new QHBoxLayout();
    layout->addWidget(m_browser);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    configureFont();

    connect(m_browser, &ChessBrowser::anchorClicked, this, &GameNotationWidget::anchorClicked);
    connect(m_browser, &ChessBrowser::actionRequested, this, &GameNotationWidget::actionRequested);
    connect(m_browser, &ChessBrowser::queryActiveGame, this, &GameNotationWidget::queryActiveGame);
    connect(m_browser, &ChessBrowser::signalMergeGame, this, &GameNotationWidget::signalMergeGame);
}

GameNotationWidget::~GameNotationWidget()
{
    delete m_output;
}

void GameNotationWidget::getHtml(
    std::function<void(const QString&)> callback) const
{
    m_browser->page()->toHtml(
        [callback](const QString &html)
        {
            callback(html);
        });
}

void GameNotationWidget::getText(
    std::function<void(const QString&)> callback) const
{
    m_browser->page()->toPlainText(
        [callback](const QString &result)
        {
            callback(result);
        });
}

QString GameNotationWidget::getTextSelection() const
{
    return m_browser->page()->selectedText();
}

QString GameNotationWidget::generateText(const GameX &game, bool trainingMode)
{
    return m_output->output(&game, trainingMode);
}

void GameNotationWidget::reload(const GameX& game, bool trainingMode)
{
    auto text = m_output->output(&game, trainingMode);
    m_browser->loadAtMove(text, game.currentMove());
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

    delete m_output;
    m_output = new Output(Output::NotationWidget, &BoardView::renderImageForBoard);
}

void GameNotationWidget::showMove(int id)
{
    m_browser->showMove(id);
}

void GameNotationWidget::toggleMode()
{
    m_browser->toggleMode();
}

int GameNotationWidget::mode()
{
    return m_browser->mode();
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
