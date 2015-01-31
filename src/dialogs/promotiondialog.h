/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include "common.h"
#include <QDialog>

namespace Ui {
class PromotionDialog;
}

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(QWidget *parent, Color color);
    ~PromotionDialog();

    int getIndex();

protected slots:
    void setIndex();

protected:
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);

private:
    Ui::PromotionDialog *ui;
    int m_index;
    QPoint m_pos;
};

#endif // PROMOTIONDIALOG_H
