#include "users.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "box.h"
Users::Users(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Users)
{
    ui->setupUi(this);
}
Users::~Users(){delete ui;}
QString* Users::getinput()
{
    bool check;
    QString* result = new QString[4];
    result[0] = ui->lineEdit->text();
    result[1] = ui->lineEdit_2->text();
    result[2] = ui->lineEdit_3->text();
    check = ui->radioButton->isChecked();
    if(check == true)
        result[3] = "T";
    else
        result[3] = "S";
    return result;
}
