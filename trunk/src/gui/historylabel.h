/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef QHISTORYLABEL_H
#define QHISTORYLABEL_H

#include <QLabel>
#include <QString>
class QStringListModel;
class QListView;
class QHideEvent;

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
   QListView* m_view;
   void init();
   void initView();
   Q_SLOT void showHistory();

};


#endif // QHISTORYLABEL_H
