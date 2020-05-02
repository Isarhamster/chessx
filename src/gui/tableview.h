/****************************************************************************
* QTableView with some adjustments like storing header layout               *
* (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                            *
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QTimer>

/**
	The TableView class is a specialized version of QTableView with automatic
	header layout saving.

*/

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = nullptr);
    ~TableView();

    Qt::Alignment alignDecoration() const;
    void setAlignDecoration(const Qt::Alignment &alignDecoration);

    QStyleOptionViewItem::Position posDecoration() const;
    void setPosDecoration(const QStyleOptionViewItem::Position &posDecoration);

    QImage renderToImage() const;
    QString renderToHTML() const;

public slots:
    /** Store current configuration. */
    void saveConfig();
    /** Restore current configuration. */
    virtual void slotReconfigure();
    /** Show context menu on header view */
    virtual void ShowContextMenu(const QPoint&);
    void SlotDragTimer();
protected: // overrides
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

protected:
    QString renderHTMLitem(int row,int column) const;
    QString renderHTMLrow(int row) const;
    QString renderHeaderToHTML() const;
    QStyleOptionViewItem viewOptions() const;
    virtual int minRowHeight() const { return 0; }
    /** Handle mouse wheel events */
    virtual void wheelEvent(QWheelEvent* e);
private:
    void setFontSize(int fontSize);
    Qt::Alignment m_alignDecoration;
    QStyleOptionViewItem::Position m_posDecoration;
    QTimer m_dragTimer;
};

#endif
