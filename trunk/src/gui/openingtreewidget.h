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
    bool updateFilter(Filter& f, const Board& b, bool updateFilter, bool bEnd);

public slots:
    void cancel(bool bVisible);
    /** Store current configuration. */
    void saveConfig();
    /** Restore current configuration. */
    void slotReconfigure();


private:
    Ui::OpeningTreeWidget *ui;
    BoardView* m_openingBoardView;
};

#endif // OPENINGTREEWIDGET_H
