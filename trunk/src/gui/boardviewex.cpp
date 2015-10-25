/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "boardviewex.h"
#include "ui_boardviewex.h"

BoardViewEx::BoardViewEx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardViewEx)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

BoardViewEx::~BoardViewEx()
{
    delete ui;
}

BoardView* BoardViewEx::boardView()
{
    return ui->boardView;
}

QObject* BoardViewEx::dbIndex()
{
    return ui->boardView->dbIndex();
}
