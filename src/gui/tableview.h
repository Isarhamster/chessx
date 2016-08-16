/****************************************************************************
* QTableView with some adjustments like storing header layout               *
* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                            *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QtCore>
#include <QTableView>

/**
	The TableView class is a specialized version of QTableView with automatic
	header layout saving.

*/

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = 0);
    ~TableView();

    Qt::Alignment alignDecoration() const;
    void setAlignDecoration(const Qt::Alignment &alignDecoration);

    QStyleOptionViewItem::Position posDecoration() const;
    void setPosDecoration(const QStyleOptionViewItem::Position &posDecoration);

public slots:
    /** Store current configuration. */
    void saveConfig();
    /** Restore current configuration. */
    virtual void slotReconfigure();
    /** Show context menu on header view */
    virtual void ShowContextMenu(const QPoint&);
protected:
    QStyleOptionViewItem viewOptions() const;
    virtual int minRowHeight() const { return 0; }
private:
    Qt::Alignment m_alignDecoration;
    QStyleOptionViewItem::Position m_posDecoration;
};

#endif
