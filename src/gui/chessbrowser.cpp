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

ChessBrowser::ChessBrowser(QWidget *parent)
    : QWebEngineView(parent)
{
    setAcceptDrops(true);

    // Enable gestures
    grabGesture(Qt::SwipeGesture);

    // Context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &ChessBrowser::slotContextMenu);

    auto *page = new ChessBrowserPage(this);
    setPage(page);

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
    setHtml(html);
    connect(page(), &QWebEnginePage::loadFinished,
            this, [this, moveId](bool ok) {
                if (ok)
                    showMove(moveId);
            });
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

}

void ChessBrowser::slotContextMenu(const QPoint &pos)
{
    if (!m_mainMenu)
        setupMenu();

    if (m_mainMenu)
        m_mainMenu->exec(mapToGlobal(pos));
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
    if (auto *gesture = static_cast<QSwipeGesture*>(
            event->gesture(Qt::SwipeGesture))) {

        if (gesture->horizontalDirection() == QSwipeGesture::Left)
            emit swipeLeft();
        else if (gesture->horizontalDirection() == QSwipeGesture::Right)
            emit swipeRight();

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
