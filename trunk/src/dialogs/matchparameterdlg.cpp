/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "matchparameterdlg.h"
#include "ui_matchparameterdlg.h"

MatchParameterDlg::MatchParameterDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatchParameterDlg)
{
    ui->setupUi(this);

    QPoint pos = mapFromGlobal(QCursor::pos());
    pos.setX(pos.x() - size().width());
    pos.setY(pos.y() - size().height());
    move(pos);
}

MatchParameterDlg::~MatchParameterDlg()
{
    delete ui;
}

bool MatchParameterDlg::getParameters(EngineParameter& par)
{
    MatchParameterDlg dlg;

    dlg.ui->baseTime->setTime(QTime::fromMSecsSinceStartOfDay(par.ms_totalTime));
    dlg.ui->cbMode->setCurrentIndex(par.tm);
    dlg.ui->moveToDo->setValue(par.movesToDo);

    if (dlg.exec())
    {
        par.ms_totalTime = -dlg.ui->baseTime->time().msecsTo(QTime(0,0));
        par.tm           = (EngineParameter::TimeModus) dlg.ui->cbMode->currentIndex();
        par.movesToDo    = dlg.ui->moveToDo->value();
        return true;
    }
    return false;
}
