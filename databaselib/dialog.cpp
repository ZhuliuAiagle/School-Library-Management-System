#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}
Dialog::~Dialog(){delete ui;}
QString Dialog::getinput()
{
    QString s = ui->lineEdit->text();
    return s;
}

void Dialog::SetLabel(QString A)
{
    ui->label->setText(A);
}
void Dialog::on_buttonBox_accepted()
{

}

void Dialog::on_buttonBox_rejected()
{

}
