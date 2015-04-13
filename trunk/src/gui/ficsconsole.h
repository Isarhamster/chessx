/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FICSCONSOLE_H
#define FICSCONSOLE_H

#include "square.h"
#include "board.h"

#include <QPointer>
#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class FicsConsole;
}
class FicsClient;
class QButtonGroup;
class QTimer;
class QTableWidgetItem;

enum
{
    TabMessage,
    TabHistory,
    TabGames,
    TabRelay,
    TabPuzzle,
    TabPlayers,
    TabSeeks
};

class FicsConsole : public QWidget
{
    Q_OBJECT

public:
    explicit FicsConsole(QWidget *parent = 0, FicsClient *ficsClient=0);
    ~FicsConsole();

public slots:
    void Terminate();
    void SendMove(QString m);
    void SendStoredMove(Square from, Square to);
    void SlotGameModeChanged(bool);

protected slots:
    void SendCommand();
    void HandleMessage(int, QString s);
    void HandleBoard(int cmd, QString s);
    void HandleObserveRequest(QListWidgetItem*);
    void HandleExamineRequest(QListWidgetItem*);
    void HandleRelayRequest(QListWidgetItem*);
    void HandleTacticsRequest(QListWidgetItem*);
    void CommandStarted(int cmd);
    void Disconnected();
    void SlotTabClicked(int tab);
    void SlotTabChanged(int tab);
    void HandleHistoryRequest(QTableWidgetItem *item);
    void HandleSeekRequest(QListWidgetItem* item);
    void SlotSeekTimeChanged(int);

    void SlotCountDownGameTime();

    void SlotSendAccept();
    void SlotSendDraw();
    void SlotSendDecline();
    void SlotSendAbort();
    void SlotSendResign();
    void SlotSendHint();
    void SlotSendUnexamine();
    void SlotSendSeek();
    void SlotSayMessage();
    void SlotAddNoPlay();

    void SlotContextMenu(const QPoint&);

signals:
    void ReceivedBoard(int cmd, QString s);
    void RequestNewGame();
    void RequestSaveGame();
    void RequestGameMode(bool);
    void RequestRemoveLastMove();
    void RequestAddTag(QString tag, QString value);
    void SignalGameResult(QString);
    void SignalPlayerIsBlack(bool);
    void RequestCloseFICS();
    /** User clicked source and destination squares */
    void RequestStoredMove(Square from, Square to);

protected:
    QString FormatTime(QString s) const;
    void SetPlayerListItemsFromLine(QString s);
private:
    Ui::FicsConsole *ui;
    QPointer<FicsClient> m_ficsClient;
    QString m_lastHistoryPlayer;
    QButtonGroup* btgSeek;
    bool gameMode;
    bool puzzleMode;
    QTimer* m_countDownTimer;
    bool m_bWhiteToMove;
    Char64Relation m_lastRelation;
    int m_prevTab;
    Square m_from;
    Square m_to;
};

#endif // FICSCONSOLE_H
