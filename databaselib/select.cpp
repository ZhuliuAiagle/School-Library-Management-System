#include "select.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "box.h"
#include "form.h"
#include <QFileDialog>
#include <QAxObject>
#include <QDesktopServices>
#include <QTextStream>
#include <iostream>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QProcess>

Select::Select(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Select)
{
    ui->setupUi(this);
}
Select::~Select(){delete ui;}

void Select::SetLabel(QString A)
{
    ui->label->setText(A);
}


void openExcel(QString &fileName)
{

    QFile file(fileName);
    if (file.exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}

void Select::on_pushButton_clicked()
{
    Form form;
    QSqlQuery query;
    Dialog dialog;
    dialog.SetLabel("请输入要添加的书籍种类数:");
    dialog.exec();
    if(dialog.result() == Dialog::Rejected)
        return;
    QString num = dialog.getinput();
    bool CONDITION;
    int time = num.toInt(&CONDITION);
    if(CONDITION == false)
    {
        ErrorBox("格式错误！");
        return;
    }
    for(int i = 0; i < time; i++)
    {
        form.exec();
        if(form.result() == Dialog::Rejected)
            return;
        QString* ret = form.getinput();
        query.exec("insert into book values(" + ret[0] + ",\'" + ret[1] + "\'" + \
                ",\'" + ret[2] + "\'" + \
                ",\'" + ret[3] + "\'" + \
                "," + ret[4] + \
                ",\'" + ret[5] + "\'" + \
                "," + ret[6] + \
                "," + ret[7] + \
                "," + ret[7] + ")");
        QSqlError error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("语法错误或被约束限制.");
                   --i;
                   continue;
            }
        }
        InforBox();
    }
}

void Select::on_pushButton_2_clicked()
{
    Dialog dialog;
    QSqlQuery query;
    dialog.SetLabel("请输入要增加的书籍编号：");
    dialog.exec();
    if(dialog.result() == Dialog::Rejected)
        return;
    QString ret = dialog.getinput();
    query.exec("select count(*) from book where bno = " + ret);
    QSqlError error = query.lastError();
    if(error.isValid())//发生错误时isValid()返回true
    {
        switch (error.type())
        {
        case QSqlError::NoError:
                   break;
        default:
               ErrorBox("查询格式错误.");
               return;
        }
    }
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("查无此书");
                return;
            }
        }
    }
    Dialog dialog2;
    dialog2.SetLabel("请输入要增加的数量：");
    dialog2.exec();
    if(dialog2.result() == Dialog::Rejected)
        return;
    QString ret2 = dialog2.getinput();
    query.exec("update book set total = total + " + ret2 + "where bno = " + ret);
    query.exec("update book set stock = stock + " + ret2 + "where bno = " + ret);
    InforBox();
}

void Select::on_pushButton_3_clicked()
{
    ErrorBox("本模式功能：从csv中导入数据\n注意,请确保对应csv中已经输入相关数据了");
    bool status = JudgeBox();
    if(status == false) return;
    QFile file("C:\\Users\\LENOVO\\Documents\\databaselib\\library.txt"); //打开csv文件
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          std::cerr << "Cannot open file for reading: "
                        << qPrintable(file.errorString()) << std::endl;
          return;
    }

    //将csv_utf8转为数据库中的表District保存

        QSqlQuery query;  //默认打开
        QStringList list;
        list.clear();
        QTextStream in(&file);  //QTextStream读取数据

        QSqlQuery transaction_start;
        QSqlQuery transaction_COMMIT;
        QSqlQuery transaction_ROLLBACK;

        QSqlQuery test_query;
        QSqlQuery Old_query;
        QSqlError error;
        transaction_start.exec("START TRANSACTION");

        while(!in.atEnd())
        {
           int mode = 0;
           QString fileLine = in.readLine();  //从第一行读取至下一行
           list = fileLine.split(",",QString::SkipEmptyParts);
           int bno = list.at(0).section('_',2,2).toInt();
           qDebug() << "---------------------------" << bno << "-------------------------";
           int year = list.at(4).toInt();
           int total = list.at(7).section(')',0,0).toInt();
           double price = list.at(6).toDouble();
           test_query.prepare("select count(*) from book where bno = :testbno");
           test_query.bindValue(":testbno",bno);
           test_query.exec();
           error = query.lastError();
           if(error.isValid())//发生错误时isValid()返回true
           {
               switch (error.type())
               {
                    case QSqlError::NoError:
                          break;
               default:
                      ErrorBox("查询格式错误.");
                      transaction_ROLLBACK.exec("ROLLBACK");
                      return;
               }
           }
           while(test_query.next())
           {
               if(test_query.isActive())
               {
                   if(test_query.value(0).toInt() > 0)
                   {
                       Old_query.prepare("update book set total = total + :testtotal where bno = :testbno");
                       Old_query.bindValue(":testbno",bno);
                       Old_query.bindValue(":testtotal",total);
                       Old_query.exec();
                       error = query.lastError();
                       if(error.isValid())//发生错误时isValid()返回true
                       {
                           switch (error.type())
                           {
                                case QSqlError::NoError:
                                      break;
                           default:
                                  ErrorBox("查询格式错误.");
                                  transaction_ROLLBACK.exec("ROLLBACK");
                                  return;
                           }
                       }
                       Old_query.prepare("update book set stock = stock + :testtotal where bno = :testbno");
                       Old_query.bindValue(":testbno",bno);
                       Old_query.bindValue(":testtotal",total);
                       Old_query.exec();
                       error = query.lastError();
                       if(error.isValid())//发生错误时isValid()返回true
                       {
                           switch (error.type())
                           {
                                case QSqlError::NoError:
                                      break;
                           default:
                                  ErrorBox("查询格式错误.");
                                  transaction_ROLLBACK.exec("ROLLBACK");
                                  return;
                           }
                       }
                       Old_query.clear();
                       test_query.clear();
                       mode = 1;
                   }
               }
           }
           if(mode == 1) continue;
           query.prepare("INSERT INTO book VALUES (:bno,:cata,:title,:press,:year,:author,:price,:total,:stock)"); //准备执行SQL查询
           query.bindValue(":bno", bno);   //绑定要插入的值
           query.bindValue(":cata", list.at(1));
           query.bindValue(":title", list.at(2));
           query.bindValue(":press", list.at(3));
           query.bindValue(":year", year);
           query.bindValue(":author", list.at(5));
           query.bindValue(":price", price);
           query.bindValue(":total",total);
           query.bindValue(":stock",total);
           query.exec();
           error = query.lastError();
           if(error.isValid())//发生错误时isValid()返回true
           {
               switch (error.type())
               {
                    case QSqlError::NoError:
                          break;
               default:
                      ErrorBox("查询格式错误.");
                      transaction_ROLLBACK.exec("ROLLBACK");
                      return;
               }
           }
        }
        query.clear();
        transaction_COMMIT.exec("COMMIT");
        InforBox();

}
