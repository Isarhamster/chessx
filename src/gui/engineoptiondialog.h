/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ENGINEOPTIONDIALOG_H
#define ENGINEOPTIONDIALOG_H

#include <QDialog>

class Engine;

namespace Ui {
class EngineOptionDialog;
}

class EngineOptionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EngineOptionDialog(QWidget *parent = 0, int index = 0);
    ~EngineOptionDialog();
public slots:
    void accept();
protected slots:
    void engineActivated();
private:
    Ui::EngineOptionDialog *ui;
    Engine* m_engine;
    int m_index;
};


#endif // ENGINEOPTIONDIALOG_H
