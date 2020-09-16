#ifndef HTMLITEMDELEGATE_H
#define HTMLITEMDELEGATE_H

#include <QStyledItemDelegate>

class HTMLItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit HTMLItemDelegate(QObject *parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // HTMLITEMDELEGATE_H
