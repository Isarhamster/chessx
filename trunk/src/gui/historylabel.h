/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef QHISTORYLABEL_H
#define QHISTORYLABEL_H

#include <QFocusEvent>
#include <QLabel>
#include <QListView>
#include <QString>

class QStringListModel;
class QHideEvent;

class QHistoryListView : public QListView
{
    Q_OBJECT
public:
    explicit QHistoryListView(QWidget *parent = Q_NULLPTR) : QListView(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    }

protected:
    void focusOutEvent(QFocusEvent*)
    {
        close();
    }
};

class HistoryLabel : public QLabel
{
   Q_OBJECT
   Q_PROPERTY(QString text READ text WRITE setText)

public:
   HistoryLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);
   HistoryLabel(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);
   Q_SLOT void setText(const QString & text, bool dontStore=false);

protected:
   virtual void hideEvent(QHideEvent *);
   virtual void mouseDoubleClickEvent(QMouseEvent *e);

protected:
   QStringList m_history;
   QStringListModel* m_model;
   QHistoryListView* m_view;
   void init();
   void initView();
   Q_SLOT void showHistory();

};


#endif // QHISTORYLABEL_H
