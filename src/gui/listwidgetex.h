/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef LISTWIDGETEX_H
#define LISTWIDGETEX_H

#include <QListWidget>

class ListWidgetEx : public QListWidget
{
    Q_OBJECT
public:
    ListWidgetEx(QWidget *parent = 0);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    /** Handle mouse wheel events */
    virtual void myWheelEvent(QWheelEvent* e);

private:
    void setFontSize(int fontSize);
};

#endif // LISTWIDGETEX_H
