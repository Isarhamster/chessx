/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "boardviewex.h"
#include "ui_boardviewex.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
