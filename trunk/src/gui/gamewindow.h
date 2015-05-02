/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "game.h"
#include "nag.h"
#include "toolmainwindow.h"

class ChessBrowser;

namespace Ui {
class GameWindow;
}

class GameWindow : public ToolMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    ChessBrowser* browser();
    void showVariations(QList<MoveId>);

signals:
    void enterVariation(int index);
public slots:
    void saveConfig();
    void slotReconfigure();

protected:
    void setupSpacers();
protected slots:
    void variationClicked(QModelIndex index);
private:
    Ui::GameWindow *ui;
    void setupToolBox();
    void setupNagInToolBox(Nag nag, QAction *action);
    void addActionAtPage(int page, QAction *action);
};

#endif // GAMEWINDOW_H
