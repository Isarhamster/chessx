#include "loadquery.h"
#include "ui_loadquery.h"

LoadQuery::LoadQuery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadQuery)
{
    ui->setupUi(this);
}

LoadQuery::~LoadQuery()
{
    delete ui;
}
