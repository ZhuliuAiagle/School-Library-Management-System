#include "form.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "box.h"
Form::Form(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}
Form::~Form(){delete ui;}
QString* Form::getinput()
{
    QString* result = new QString[8];
    result[0] = ui->lineEdit->text();
    result[1] = ui->lineEdit_2->text();
    result[2] = ui->lineEdit_3->text();
    result[3] = ui->lineEdit_4->text();
    result[4] = ui->lineEdit_5->text();
    result[5] = ui->lineEdit_6->text();
    result[6] = ui->lineEdit_7->text();
    result[7] = ui->lineEdit_8->text();
    return result;
}
