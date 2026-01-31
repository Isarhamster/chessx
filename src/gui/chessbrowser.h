#ifndef CHESSBROWSER_H
#define CHESSBROWSER_H

#include <QWebEngineView>
#include <QAction>
#include <QMenu>
#include <QGestureEvent>

#include "editaction.h"
#include "gameid.h"

class ChessBrowser : public QWebEngineView
{
    Q_OBJECT

public:
    explicit ChessBrowser(QWidget *parent = nullptr);

    QStringList getAnchors(const QList<MoveId> &list);
    QMap<QAction*, EditAction> m_actions;

public slots:
    void showMove(int id);
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
    // Navigation interception (pseudo-links replacement)
    void handleUrl(const QUrl &url);

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

    QMenu *m_gameMenu = nullptr;
    QMenu *m_browserMenu = nullptr;
    QMenu *m_mainMenu = nullptr;

    int m_currentMove = -1;
};

#endif // CHESSBROWSER_H
