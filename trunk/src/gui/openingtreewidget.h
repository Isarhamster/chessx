/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef OPENINGTREEWIDGET_H
#define OPENINGTREEWIDGET_H

#include "board.h"
#include "filter.h"
#include <QAbstractItemModel>
#include <QPointer>
#include <QUndoCommand>
#include <QWidget>

namespace Ui
{
class OpeningTreeWidget;
}

class BoardView;
class OpeningTree;
class QUndoStack;

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
    bool doSetBoard(Filter& f, const Board& b, bool bEnd);
    int getFilterIndex(QString& name) const;
    void updateFilterIndex(QStringList);
    bool shouldAddMove() const;

public slots:
    void cancel();
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
    /** The source for the tree has changed */
    void slotSourceChanged();

signals:
    void signalTreeUpdated(bool);
    void signalSourceChanged();

private:
    Ui::OpeningTreeWidget *ui;
    BoardView* m_openingBoardView;
    QStringList m_filePaths;
    QUndoStack* m_UndoStack;
};

class BoardUndoCommand : public QUndoCommand
{
public:
    BoardUndoCommand(QObject* parent, Filter* f, const Board& b, bool bEnd, QString action) :
        QUndoCommand(action),
        m_openingTree((OpeningTreeWidget*)parent),
        m_filter(f), m_board(b), m_bEnd(bEnd)
        {
        }

    QPointer<OpeningTreeWidget> m_openingTree;
    Filter* m_filter;
    Board m_board;
    bool m_bEnd;

    virtual void undo() { m_openingTree->doSetBoard(*m_filter, m_board, m_bEnd); }
    virtual void redo() { m_openingTree->doSetBoard(*m_filter, m_board, m_bEnd); }
};

#endif // OPENINGTREEWIDGET_H
