#ifndef CHESSBROWSER_H
#define CHESSBROWSER_H

#include <QWebEngineView>
#include <QAction>
#include <QMenu>
#include <QGestureEvent>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QBuffer>

#include "editaction.h"
#include "gameid.h"

class ChessBrowser : public QWebEngineView
{
    Q_OBJECT

    class MemoryHandler : public QWebEngineUrlSchemeHandler
    {
    public:
        QByteArray htmlData;

        void requestStarted(QWebEngineUrlRequestJob *job) override
        {
            QBuffer *buffer = new QBuffer(job);
            buffer->setData(htmlData);
            buffer->open(QIODevice::ReadOnly);

            job->reply("text/html; charset=UTF-8", buffer);
        }
    };

public:
    explicit ChessBrowser(QWidget *parent = nullptr);

    QStringList getAnchors(const QList<MoveId> &list);
    QMap<QAction*, EditAction> m_actions;

    void loadAtMove(QString html, int moveId);

public slots:
    void showMove(int id);
    void selectMove(int id);
    void slotAction(QAction *action);
    void slotContextMenu(const QPoint &pos);

signals:
    void anchorClicked(const QUrl& url);
    void actionRequested(const EditAction &action);
    void queryActiveGame(const GameX **game);
    void signalMergeGame(GameId gameIndex, QString source);
    void swipeRight();
    void swipeLeft();

protected:
    // Gestures
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);

    // Drag & Drop
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void setupMenu();
    QAction* createAction(const QString &name, EditAction::Type type);
    QAction* createNagAction(const Nag &nag);
    void mergeGame(GameId gameIndex);

private:
    QAction* m_copyHtml;
    QAction* m_copyText;
    QAction* m_uncomment;
    QAction* m_remove;
    QAction* m_startComment;
    QAction* m_gameComment;
    QAction* m_gameComment2;
    QAction* m_addComment;
    QAction* m_removeVariation;
    QAction* m_promoteVariation;
    QAction* m_VariationUp;
    QAction* m_VariationDown;
    QAction* m_removePrevious;
    QAction* m_removeNext;
    QAction* m_addNullMove;
    QAction* m_addNullMove2;
    QAction* m_removeNags;
    QAction* m_enumerateVariations1;
    QAction* m_enumerateVariations2;
    QAction* m_copyTextSelection;


    QMenu *m_gameMenu = nullptr;
    QMenu *m_browserMenu = nullptr;
    QMenu *m_mainMenu = nullptr;

    int m_currentMove = -1;
    MemoryHandler *handler;
};

#endif // CHESSBROWSER_H
