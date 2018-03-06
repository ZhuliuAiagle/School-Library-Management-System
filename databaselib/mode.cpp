#include "mode.h"
#include "ui_mode.h"

Mode::Mode(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Mode)
{
    ui->setupUi(this);
}
Mode::~Mode(){delete ui;}
int Mode::getinput()
{
    if(ui->radioButton->isChecked()) return 1;
    else if(ui->radioButton_2->isChecked()) return 2;
    else if(ui->radioButton_3->isChecked()) return 3;
    else if(ui->radioButton_4->isChecked()) return 4;
    else if(ui->radioButton_5->isChecked()) return 5;
    else if(ui->radioButton_6->isChecked())return 6;
    else return -1;
}
void Mode::SetLabel(QString A)
{
    ui->label->setText(A);
}
