/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "matchparameterdlg.h"
#include "settings.h"
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
    dlg.ui->timeBonus->setValue(par.ms_bonus/1000);
    dlg.ui->timeInc->setValue(par.ms_increment/1000);
    dlg.ui->cbMode->setCurrentIndex(par.tm);
    dlg.ui->annotateEgt->setChecked(par.annotateEgt);
    dlg.ui->cbAllowBook->setChecked(par.allowBook);

    if (dlg.exec())
    {
        par.ms_totalTime = -dlg.ui->baseTime->time().msecsTo(QTime(0,0));
        par.ms_bonus     = 1000*dlg.ui->timeBonus->value();
        par.ms_increment = 1000*dlg.ui->timeInc->value();
        par.tm           = (EngineParameter::TimeModus) dlg.ui->cbMode->currentIndex();
        par.ms_white     = par.ms_totalTime;
        par.ms_black     = par.ms_totalTime;
        par.annotateEgt  = dlg.ui->annotateEgt->isChecked();
        par.allowBook    = dlg.ui->cbAllowBook->isChecked();

        AppSettings->beginGroup("/Match/");
        AppSettings->setValue("Mode", (int) par.tm);
        AppSettings->setValue("TotalTime",par.ms_totalTime);
        AppSettings->setValue("UserBonus",par.ms_bonus);
        AppSettings->setValue("Increment",par.ms_increment);
        AppSettings->setValue("AnnotateEgt",par.annotateEgt);
        AppSettings->setValue("AllowBook",par.allowBook);
        AppSettings->endGroup();

        return true;
    }
    return false;
}
