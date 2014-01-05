/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef OPENINGTREEWIDGET_H
#define OPENINGTREEWIDGET_H

#include "board.h"
#include "filter.h"
#include <QAbstractItemModel>
#include <QWidget>

namespace Ui
{
class OpeningTreeWidget;
}

class BoardView;
class OpeningTree;

class OpeningTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OpeningTreeWidget(QWidget *parent = 0);
    ~OpeningTreeWidget();
    OpeningTree* m_openingTree;

    QString move(QModelIndex index) const;
    Board board() const;
    bool updateFilter(Filter& f, const Board& b, bool bEnd);

public slots:
    void cancel(bool bVisible);
    /** Store current configuration. */
    void saveConfig();
    /** Restore current configuration. */
    void slotReconfigure();

protected slots:
    /** Indicate end of tree update */
    void slotTreeUpdate();
    /** Show progress bar for open file. */
    void slotOperationProgress(int value);
    /** Indicate start of tree update */
    void slotTreeUpdateStarted();

signals:
    void signalTreeUpdated();

private:
    Ui::OpeningTreeWidget *ui;
    BoardView* m_openingBoardView;
};

#endif // OPENINGTREEWIDGET_H
