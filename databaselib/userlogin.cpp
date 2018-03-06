#include "userlogin.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "box.h"
Userlogin::Userlogin(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Userlogin)
{
    ui->setupUi(this);
}
Userlogin::~Userlogin(){delete ui;}
QString* Userlogin::getinput()
{
    QString* result = new QString[3];
    result[0] = ui->lineEdit->text();
    result[1] = ui->lineEdit_2->text();
    return result;
}
void Userlogin::modify_1(QString A)
{
    ui->label->setText(A);
}
void Userlogin::modify_2(QString A)
{
    ui->label_2->setText(A);
}
