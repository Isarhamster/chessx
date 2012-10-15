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
protected slots:
    void engineActivated();
private:
    Ui::EngineOptionDialog *ui;
    Engine* m_engine;
};


#endif // ENGINEOPTIONDIALOG_H
