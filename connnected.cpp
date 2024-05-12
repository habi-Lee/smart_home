#include "connnected.h"
#include "ui_connnected.h"

connnected::connnected(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connnected)
{
    ui->setupUi(this);
    w = new Widget;
    e =new events;

}

connnected::~connnected()
{
    delete ui;
}

void connnected::on_pushButton_clicked()
{
    w->show();
    //this->hide();
}


void connnected::on_pushButton_2_clicked()
{
    e->show();
    //this->hide();
}


void connnected::on_connnected_customContextMenuRequested(const QPoint &pos)
{

}

