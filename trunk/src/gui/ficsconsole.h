/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FICSCONSOLE_H
#define FICSCONSOLE_H

#include <QPointer>
#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class FicsConsole;
}
class FicsClient;

class FicsConsole : public QWidget
{
    Q_OBJECT

public:
    explicit FicsConsole(QWidget *parent = 0, FicsClient *ficsClient=0);
    ~FicsConsole();

    void Terminate();

protected slots:
    void SendCommand();
    void PrintMessage(int, QString s);
    void HandleBoard(int cmd, QString s);
    void HandleObserveRequest(QListWidgetItem*);
    void HandleExamineRequest(QListWidgetItem*);
    void CommandStarted(int cmd);
    void CommandDone(int cmd);
    void Disconnected();
    void SlotTabChanged(int tab);

    void HandleHistoryRequest(QListWidgetItem *item);
signals:
    void ReceivedBoard(int cmd, QString s);

private:
    Ui::FicsConsole *ui;
    QPointer<FicsClient> m_ficsClient;
    QString m_lastHistoryPlayer;
    QStringList m_transportList;
};

#endif // FICSCONSOLE_H
