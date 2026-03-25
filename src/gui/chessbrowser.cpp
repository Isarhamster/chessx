/***************************************************************************
 *   (C) 2006 by Michal Rudolf mrudolf@kdewebdev.org                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "chessbrowserbridge.h"
#include "chessbrowser.h"
#include "chessbrowserpage.h"
#include "GameMimeData.h"

#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QtCore/qstringview.h>

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#include <QWebEngineContextMenuRequest>
#else
#include <QWebEngineContextMenuData>
#endif

ChessBrowser::ChessBrowser(QWidget *parent)
    : QWebEngineView(parent)
{
    setAcceptDrops(true);

    // Enable gestures
    grabGesture(Qt::TapAndHoldGesture);


    // Context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &ChessBrowser::slotContextMenu);

    auto *page = new ChessBrowserPage(this);
    setPage(page);

    handler = new MemoryHandler;
    page->profile()->installUrlSchemeHandler("mem", handler);

    ChessBrowserBridge* bridge = new ChessBrowserBridge(this);
    QWebChannel *channel = new QWebChannel(page);
    channel->registerObject("bridge", bridge);
    page->setWebChannel(channel);

    // Intercept navigation
    connect(bridge, &ChessBrowserBridge::moveSelected,
            this, &ChessBrowser::selectMove);

    // Sensible defaults
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
}

void ChessBrowser::loadAtMove(QString html, int moveId)
{
    handler->htmlData = html.toUtf8();

    connect(page(), &QWebEnginePage::loadFinished,
            this, [this, moveId](bool ok) {
                if (ok)
                    showMove(moveId);
            });

    page()->load(QUrl("mem://page"));
}


void ChessBrowser::selectMove(int id)
{
    QString url = QString("move:%1").arg(id);
    emit anchorClicked(url);
}

void ChessBrowser::showMove(int id)
{
    m_currentMove = id;
    // Check necessary in case no game is loaded (yet)
    page()->runJavaScript(QString("if (window.highlightMove) highlightMove(%1);").arg(id));
    page()->runJavaScript(QString("if (window.highlightPath) highlightPath(%1);").arg(id));
    //page()->runJavaScript(QString("document.body.classList.toggle(\"table-mainline\");"));
}

void ChessBrowser::setupMenu()
{
    m_gameMenu = new QMenu("Notation",this);
    m_browserMenu = new QMenu("Text",this);
    connect(m_gameMenu, SIGNAL(triggered(QAction*)), SLOT(slotAction(QAction*)));
    connect(m_browserMenu, SIGNAL(triggered(QAction*)), SLOT(slotAction(QAction*)));

    m_gameMenu->addAction((m_startComment = createAction(tr("Add start comment..."), EditAction::EditPrecomment)));
    m_gameMenu->addAction((m_gameComment = createAction(tr("Add game comment..."), EditAction::EditGameComment)));
    m_gameMenu->addAction((m_addComment = createAction(tr("Comment..."), EditAction::EditComment)));
    // Nag menus
    QMenu* nagMoveMenu = m_gameMenu->addMenu(tr("Add move symbol"));
    for(int n = MoveNagStart; n <= MoveNagEnd; ++n)
    {
        if(n != SingularMove)
        {
            nagMoveMenu->addAction(createNagAction(Nag(n)));
        }
    }
    QMenu* nagPositionMenu = m_gameMenu->addMenu(tr("Add evaluation symbol"));
    nagPositionMenu->addAction(createNagAction(DrawishPosition));
    for(int n = UnclearPosition; n <= BlackHasADecisiveAdvantage; ++n)
    {
        nagPositionMenu->addAction(createNagAction(Nag(n)));
    }
    nagPositionMenu->addAction(createNagAction(Nag(WhiteHasSufficientCompensationForMaterialDeficit)));
    nagPositionMenu->addAction(createNagAction(WhiteHasTheInitiative));
    nagPositionMenu->addAction(createNagAction(WhiteHasTheAttack));
    nagPositionMenu->addAction(createNagAction(WhiteHasModerateCounterplay));

    QMenu* nagSpecialMenu = m_gameMenu->addMenu(tr("Add other symbol"));
    QMenu* subMenu;
    nagSpecialMenu->addAction(createNagAction(NagDiagram));
    nagSpecialMenu->addAction(createNagAction(WithTheIdea));
    nagSpecialMenu->addAction(createNagAction(BetterMove));
    nagSpecialMenu->addAction(createNagAction(Novelty));
    nagSpecialMenu->addAction(createNagAction(WhiteIsInZugzwang));
    nagSpecialMenu->addAction(createNagAction(WeakPoint));
    nagSpecialMenu->addAction(createNagAction(EndGame));
    nagSpecialMenu->addAction(createNagAction(WhiteHasSevereTimeControlPressure));
    subMenu = nagSpecialMenu->addMenu(tr("Bishops"));
    subMenu->addAction(createNagAction(WhiteHasAPairOfBishops));
    subMenu->addAction(createNagAction(BishopsOfOppositeColor));
    subMenu->addAction(createNagAction(BishopsOfSameColor));

    m_gameMenu->addAction(m_removeNags = createAction(tr("Remove symbols"), EditAction::ClearNags));
    m_gameMenu->addSeparator();
    m_gameMenu->addAction((m_enumerateVariations1 = createAction(tr("Enumerate Variations A) B) C)"), EditAction::EnumerateVariations1)));
    m_gameMenu->addAction((m_enumerateVariations2 = createAction(tr("Enumerate Variations a) b) c)"), EditAction::EnumerateVariations2)));
    m_gameMenu->addSeparator();
    m_gameMenu->addAction((m_promoteVariation = createAction(tr("Promote to main line"), EditAction::PromoteVariation)));
    m_gameMenu->addAction((m_VariationUp = createAction(tr("Move variation up"), EditAction::VariationUp)));
    m_gameMenu->addAction((m_VariationDown = createAction(tr("Move variation down"), EditAction::VariationDown)));
    m_gameMenu->addSeparator();
    m_gameMenu->addAction((m_removeVariation = createAction(tr("Remove variation"), EditAction::RemoveVariation)));
    m_gameMenu->addAction((m_removePrevious = createAction(tr("Remove previous moves"), EditAction::RemovePreviousMoves)));
    m_gameMenu->addAction((m_removeNext = createAction(tr("Remove next moves"), EditAction::RemoveNextMoves)));
    m_gameMenu->addSeparator();
    m_gameMenu->addAction((m_addNullMove = createAction(tr("Insert threat"), EditAction::AddNullMove)));

    // Non-move oriented actions
    m_browserMenu->addAction((m_gameComment2 = createAction(tr("Add game comment..."), EditAction::EditGameComment)));
    m_browserMenu->addAction((m_addNullMove2 = createAction(tr("Insert threat"), EditAction::AddNullMove)));
    m_browserMenu->addAction((m_copyHtml = createAction(tr("Copy Html"), EditAction::CopyHtml)));
    m_browserMenu->addAction((m_copyText = createAction(tr("Copy Text"), EditAction::CopyText)));
    m_browserMenu->addAction((m_copyTextSelection = createAction(tr("Copy selected Text"), EditAction::CopyTextSelection)));
    m_browserMenu->addSeparator();
    QMenu* refactorMenu = m_browserMenu->addMenu(tr("Refactor"));
    refactorMenu->addAction((m_uncomment = createAction(tr("Uncomment"), EditAction::Uncomment)));
    refactorMenu->addAction((m_remove = createAction(tr("Remove Variations"), EditAction::RemoveVariations)));

    m_gameMenu->addMenu(m_browserMenu);
}

void ChessBrowser::slotContextMenu(const QPoint &pos)
{
    if (!m_gameMenu)
        setupMenu();

    const GameX* game = nullptr;
    emit queryActiveGame(&game);

    // Handle game browser
    if(!game)
    {
        return;
    }

    bool gameIsEmpty = game->isEmpty();

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    auto *req = lastContextMenuRequest();
    QString link = req ? req->linkUrl().toString() : QString();
#else
    auto data = contextMenuData();
    QString link = data.linkUrl().toString();
#endif

    bool hasGameComment = !game->annotation(0).isEmpty();

    m_gameComment2->setVisible(gameIsEmpty && !hasGameComment);
    m_addNullMove2->setVisible(gameIsEmpty);
    m_copyHtml->setVisible(!gameIsEmpty);
    m_copyText->setVisible(!gameIsEmpty);
    m_uncomment->setVisible(!gameIsEmpty);
    m_remove->setVisible(!gameIsEmpty);
    m_copyTextSelection->setVisible(!gameIsEmpty);

    // Convert to JS query
    QString js = QString(R"(
        (function() {
            var el = document.elementFromPoint(%1, %2);
            if (!el) return null;

            while (el && !el.dataset.moveId)
                el = el.parentElement;

            return el ? el.dataset.moveId : null;
        })();
    )").arg(pos.x()).arg(pos.y());

    page()->runJavaScript(js, [this, game, gameIsEmpty, pos](const QVariant &result) {
        QString link = result.toString();
        if(!link.isEmpty())
        {
            m_currentMove = link.toInt();

            bool isVariation = !game->isMainline(m_currentMove);
            bool atLineStart = game->atLineStart(m_currentMove);
            bool atGameStart = m_currentMove == 0 || game->atGameStart(m_currentMove - 1);
            bool hasComment = !game->annotation(m_currentMove).isEmpty();
            bool hasPrecomment = !game->annotation(m_currentMove, GameX::BeforeMove).isEmpty();
            bool hasNags = !game->nags().isEmpty();
            bool atLineEnd = game->atLineEnd(m_currentMove);

            m_startComment->setVisible(atLineStart && !hasPrecomment && !gameIsEmpty);
            m_gameComment->setVisible(gameIsEmpty && !hasComment);
            m_addComment->setVisible(true);
            m_enumerateVariations1->setVisible(isVariation);
            m_enumerateVariations2->setVisible(isVariation);
            m_promoteVariation->setVisible(isVariation);
            m_removeVariation->setVisible(isVariation);
            m_VariationUp->setVisible(isVariation && game->cursor().canMoveVariationUp(m_currentMove));
            m_VariationDown->setVisible(isVariation && game->cursor().canMoveVariationDown(m_currentMove));
            m_removeNext->setVisible(!atLineEnd);
            m_removePrevious->setVisible(!atGameStart);
            m_removeNags->setVisible(hasNags);
            m_addNullMove->setVisible(true);

            m_gameMenu->exec(mapToGlobal(pos));
            m_currentMove = CURRENT_MOVE;
        }
        else
        {
            m_browserMenu->exec(mapToGlobal(pos));
        }
    });
}

void ChessBrowser::slotAction(QAction* action)
{
    if(m_actions.contains(action))
    {
        EditAction editAction = m_actions[action];
        editAction.setMove(m_currentMove);
        emit actionRequested(editAction);
    }
}

QAction* ChessBrowser::createAction(const QString& name, EditAction::Type type)
{
    QAction* action = new QAction(name, this);
    m_actions[action] = EditAction(type);
    return action;
}

QAction* ChessBrowser::createNagAction(const Nag& nag)
{
    QAction* action = new QAction(NagSet::nagToMenuString(nag), this);
    m_actions[action] = EditAction(EditAction::AddNag, (int)nag);
    return action;
}

// ---------- Gesture Handling ----------------

bool ChessBrowser::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));

    return QWebEngineView::event(event);
}

bool ChessBrowser::gestureEvent(QGestureEvent *event)
{
    if (QGesture *g = event->gesture(Qt::TapAndHoldGesture))
    {
        auto *tap =
            static_cast<QTapAndHoldGesture*>(g);

        if (tap->state() == Qt::GestureFinished)
        {
            QPoint pos = tap->position().toPoint();
            slotContextMenu(mapFromGlobal(pos));
        }

        return true;
    }
    return false;
}

// ---------- Drag and Drop Handlers ----------------

void ChessBrowser::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);

    if(gameMimeData)
    {
        event->acceptProposedAction();
    }
}

void ChessBrowser::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void ChessBrowser::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void ChessBrowser::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
    if(gameMimeData)
    {
        foreach(GameId index, gameMimeData->m_indexList)
        {
            emit signalMergeGame(index, gameMimeData->source);
        }
    }

    event->acceptProposedAction();
}

// ---------- Conetext Menu Handlers ----------------

void ChessBrowser::mergeGame(GameId gameIndex)
{
    emit signalMergeGame(gameIndex, QString());
}
