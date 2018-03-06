#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "dialog.h"
#include "select.h"
#include "box.h"
#include "users.h"
#include "userlogin.h"
#include <QDebug>
#include "mode.h"
#include <QFileDialog>
#include <QAxObject>
//---------------------------------------------
bool OpenDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");   //数据库驱动类型为SQL Server
    qDebug()<<"ODBC driver?"<<db.isValid();
    QString dsn = QString::fromLocal8Bit("QTDB");      //数据源名称
    db.setHostName("localhost");                        //选择本地主机，127.0.1.1
    db.setDatabaseName(dsn);                            //设置数据源名称
    db.setUserName("逐流");                               //登录用户
    db.setPassword("");                           //密码
    if(!db.open())                                      //打开数据库
    {
        qDebug()<<db.lastError().text();
        QMessageBox::critical(0, QObject::tr("Database error"), db.lastError().text());
        return false;                                   //打开失败
    }
    else
        qDebug()<<"database open success!";
    return true;
}

QString admin = "ERRORACCESS";


void Login()
{
    while(1)
    {
        Userlogin userlogin;
        userlogin.modify_1("管理账号");
        userlogin.modify_2("密码:");
        userlogin.exec();
        if(userlogin.result() == Dialog::Rejected)
        {
            admin = "ERRORACCESS";
            ErrorBox("未登录！你将只能使用查询功能！");
            return;
        }
        QString *ret = userlogin.getinput();
        QSqlQuery query;
        qDebug() << ret[0] << ret[1];
        query.exec("select count(*) from admin where ID = \'" + ret[0] + "\' and password = \'" + ret[1] + "\'");
        while(query.next())
        {
            if(query.isActive())
            {
                if(query.value(0).toInt() == 0)
                {
                    ErrorBox("用户名或密码错误.");
                    admin = "ERRORACCESS";
                }
                else
                {
                    ::admin = ret[0];
                }
            }
        }
        if(admin == "ERRORACCESS") continue;

        ErrorBox("登陆成功");
        break;
    }

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpenDatabase();
    Login();
    if(admin == "ERRORACCESS") {ui->pushButton_11->setText("登录");ui->label_2->setText("[未登录]");}
    else {ui->pushButton_11->setText("退出登录");ui->label_2->setText("欢迎你！管理员"+admin);}

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select * from book order by bno");//这里直接设置SQL语句，忽略最后一个参数
    ui->tableView->setModel(model);


/*
            //隔行变色
            ui->tableView->setAlternatingRowColors(true);

            //设置行高
            int row_count = model->rowCount();
            for(int i =0; i < row_count; i++)
            {
                ui->tableView->setRowHeight(i, 20);
            }
*/
}

void MainWindow::on_pushButton_2_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    QSqlQuery query;
    Dialog dialog;
    dialog.SetLabel("请输入要删除的书籍编号：");
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
    bool ret2 = JudgeBox();
    QString q = "delete from book where bno = ";
    if(ret2 == true)
    {
        query.exec(q + ret);
    }

    else;
}

void MainWindow::on_pushButton_7_clicked()
{
    Dialog dialog;
    Mode mode;
    QSqlQuery query;
    mode.exec();
    if(mode.result() == Dialog::Rejected)
        return;
    int mode_case = mode.getinput();
    if(mode_case == -1)
    {
        ErrorBox("您没有选定任何模式！");
        return;
    }
    if(mode.result() == Dialog::Rejected)
        return;


    Mode mode2;
    QString order = "title";

    mode2.SetLabel("请输入您选定的排序依赖,默认依赖为书名:");
    mode2.exec();
    if(mode2.result() == Dialog::Rejected)
        return;
    int mode_case2 = mode2.getinput();
    if(mode_case2 == -1)
        ErrorBox("警告：没有选定排序依赖:(\n将执行基于书名的默认排序依赖");
    switch (mode_case2)
    {
            case 1:
                order = "category";
                break;
            case 2:case -1:
                order = "title";
            case 3:
                order = "press";
            case 4:
                order = "year";
            case 5:
                order = "author";
            case 6:
                order = "price";
            default:
                break;
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlError error;
    QString* back;
    Userlogin userlogin;
    QString ret;


    switch (mode_case)
    {
    case 1:
        dialog.SetLabel("请输入要查找的书籍元素：");
        dialog.exec();
        if(dialog.result() == Dialog::Rejected)
            return;
        ret = dialog.getinput();
        query.exec("select count(*) from book where category like \'%"+ret+"%\' ");
        error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("格式错误.");
                   return;
            }
        }
        while(query.next())
        {
               if(query.isActive())
               {
                   if(query.value(0).toInt() == 0)
                   {
                       ErrorBox("No data");
                       return;
                   }
               }
        }

        model->setQuery("select * from book where category like \'%"+ret+"%\' order by "+order);
        break;
    case 2:
        dialog.SetLabel("请输入要查找的书籍元素：");
        dialog.exec();
        if(dialog.result() == Dialog::Rejected)
            return;
        ret = dialog.getinput();
        query.exec("select count(*) from book where title like \'%"+ret+"%\' ");
        error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("格式错误.");
                   return;
            }
        }
        while(query.next())
        {
               if(query.isActive())
               {
                   if(query.value(0).toInt() == 0)
                   {
                       ErrorBox("No data");
                       return;
                   }
               }
        }
        model->setQuery("select * from book where title like \'%"+ret+"%\' order by "+order);
        break;
    case 3:
        dialog.SetLabel("请输入要查找的书籍元素：");
        dialog.exec();
        if(dialog.result() == Dialog::Rejected)
            return;
        ret = dialog.getinput();
        query.exec("select count(*) from book where press like \'%"+ret+"%\' ");
        error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("格式错误.");
                   return;
            }
        }
        while(query.next())
        {
               if(query.isActive())
               {
                   if(query.value(0).toInt() == 0)
                   {
                       ErrorBox("No data");
                       return;
                   }
               }
        }
        model->setQuery("select * from book where press like \'%"+ret+"%\' order by "+order);
        break;
    case 4:
        dialog.SetLabel("请输入要查找的书籍元素：");
        dialog.exec();
        if(dialog.result() == Dialog::Rejected)
            return;
        ret = dialog.getinput();
        query.exec("select count(*) from book where year = "+ret);
        error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("格式错误.");
                   return;
            }
        }
        while(query.next())
        {
               if(query.isActive())
               {
                   if(query.value(0).toInt() == 0)
                   {
                       ErrorBox("No data");
                       return;
                   }
               }
        }
        model->setQuery("select * from book where year = "+ret+" order by "+order);
        break;
    case 5:
        dialog.SetLabel("请输入要查找的书籍元素：");
        dialog.exec();
        if(dialog.result() == Dialog::Rejected)
            return;
        ret = dialog.getinput();
        query.exec("select count(*) from book where author like \'%"+ret+"%\'");
        error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("格式错误.");
                   return;
            }
        }
        while(query.next())
        {
               if(query.isActive())
               {
                   if(query.value(0).toInt() == 0)
                   {
                       ErrorBox("No data");
                       return;
                   }
               }
        }
        model->setQuery("select * from book where author like \'%"+ret+"%\' order by "+order);
        break;
    case 6:
        userlogin.modify_1("小于");
        userlogin.modify_2("大于");
        userlogin.exec();
        if(userlogin.result() == Dialog::Rejected)
        {
            return;
        }
        back = userlogin.getinput();
        query.exec("select count(*) from book where price <= "+back[0]+"and price >="+back[1]);
        error = query.lastError();
        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
                 case QSqlError::NoError:
                       break;
            default:
                   ErrorBox("格式错误.");
                   return;
            }
        }
        while(query.next())
        {
               if(query.isActive())
               {
                   if(query.value(0).toInt() == 0)
                   {
                       ErrorBox("No data");
                       return;
                   }
               }
        }
        model->setQuery("select * from book where price <= "+back[0]+"and price >="+back[1]+" order by " + order);
        break;
    default:
        break;
    }


    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_3_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    Select select;
    select.exec();
}

void MainWindow::on_pushButton_6_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    QSqlQuery query;
    Users users;
    users.exec();
    if(users.result() == Dialog::Rejected)
        return;
    QString* ret = users.getinput();
    query.exec("insert into card values(" + ret[0] + ",\'" + ret[1] + "\'" + \
            ",\'" + ret[2] + "\'" + \
            ",\'" + ret[3] + "\')");
    QSqlError error = query.lastError();
    if(error.isValid())//发生错误时isValid()返回true
    {
        switch (error.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.");
               return;
        }
    }
    InforBox();
}

void MainWindow::on_pushButton_8_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select * from card");
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    Userlogin userlogin;
    QSqlQuery query;
    userlogin.exec();
    if(userlogin.result() == Dialog::Rejected)
        return;
    QString* ret = userlogin.getinput();
    query.exec("select count(*) from card where cno = \'"+ret[0]+"\'");
    QSqlError error1 = query.lastError();
    if(error1.isValid())//发生错误时isValid()返回true
    {
        switch (error1.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.");
               return;
        }
    }
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("查无此人");
                return;
            }
        }
    }
    query.exec("select count(*) from book where bno = "+ret[1]);
    error1 = query.lastError();
    if(error1.isValid())//发生错误时isValid()返回true
    {
        switch (error1.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.");
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
    query.exec("select stock from book where bno = "+ret[1]);
    QString result;
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("此书借罄");
                query.exec("with A as (select * from borrow where bno  = +"+ret[1]+") select return_date from borrow where bno = "+ret[1]+" and return_date = (select min(return_date) from A);");
                while(query.next())
                {
                    if(query.isActive())
                    {
                        result = query.value(0).toString();
                    }
                }
                ErrorBox("最近还书时间："+result.section('T',0,0));
                return;
            }
        }
    }
    Userlogin dt;
    dt.modify_1("借书日期");
    dt.modify_2("还书日期");
    dt.exec();
    if(dt.result() == Dialog::Rejected)
        return;
    QString* date = dt.getinput();
    query.exec("insert into borrow values(\'"+ret[0]+"\',"+ret[1]+",\'"+date[0]+"\',\'"+date[1]+"\',\'"+::admin+"\')");
    QSqlError error = query.lastError();
    if(error.isValid())//发生错误时isValid()返回true
    {
        switch (error.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误或日期先后错误.");
               return;
        }
    }
    InforBox();
}

void MainWindow::on_pushButton_5_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    Userlogin userlogin;
    QSqlQuery query;
    userlogin.exec();
    if(userlogin.result() == Dialog::Rejected)
        return;
    QString* ret = userlogin.getinput();
    query.exec("select count(*) from borrow where cno = \'"+ret[0]+"\'");
    QSqlError error1 = query.lastError();
    if(error1.isValid())//发生错误时isValid()返回true
    {
        switch (error1.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.");
               return;
        }
    }
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("此人未借书");
                return;
            }
        }
    }
    query.exec("select count(*) from borrow where bno = "+ret[1]+"and cno = \'"+ret[0]+"\'");
    error1 = query.lastError();
    if(error1.isValid())//发生错误时isValid()返回true
    {
        switch (error1.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.");
               return;
        }
    }
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("此人无此书的借阅记录！");
                return;
            }
        }
    }
    query.exec("delete from borrow where cno = \'"+ret[0]+"\' and bno = "+ret[1]);
    QSqlError error = query.lastError();
    if(error.isValid())//发生错误时isValid()返回true
    {
        switch (error.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.");
               return;
        }
    }
    InforBox();

}

void MainWindow::on_pushButton_10_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    Dialog dialog;
    dialog.SetLabel("请输入借书卡号");
    dialog.exec();
    if(dialog.result() == Dialog::Rejected)
    {
        return;
    }
    QString ret = dialog.getinput();
    qDebug() << ret;
    if(ret == "")
    {
        ErrorBox("默认输出所有记录.");
        QSqlQueryModel *model2 = new QSqlQueryModel;
        model2->setQuery("select * from borrow order by borrow_date");
        ui->tableView->setModel(model2);
        return;
    }
    QSqlQuery query;
    query.exec("select count(*) from borrow where cno = \'" +ret+ "\'");
    QSqlError error1 = query.lastError();
    if(error1.isValid())//发生错误时isValid()返回true
    {
        switch (error1.type())
        {
             case QSqlError::NoError:
                   break;
        default:
               ErrorBox("格式错误.输出所有记录.");
               QSqlQueryModel *model2 = new QSqlQueryModel;
               model2->setQuery("select * from borrow order by borrow_date");
               ui->tableView->setModel(model2);
               return;
        }
    }
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("查无此卡或此人未借书");
                return;
            }
        }
    }
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select * from borrow where cno = \'" +ret+ "\'");
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_9_clicked()
{
    if(admin == "ERRORACCESS")
    {
        ErrorBox("未登录！你将只能使用查询功能！");
       return;
    }
    Dialog dialog;
    dialog.SetLabel("请输入要删除的用户名");
    dialog.exec();
    if(dialog.result() == Dialog::Rejected)
        return;
    QString ret = dialog.getinput();
    QSqlQuery query;
    query.exec("select count(cno) from card where cno = \'"+ret+"\'");
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() == 0)
            {
                ErrorBox("查无此人");
                return;
            }
        }
    }
    query.exec("select count(cno) from borrow where cno = \'"+ret+"\'");
    while(query.next())
    {
        if(query.isActive())
        {
            if(query.value(0).toInt() > 0)
            {
                ErrorBox("他还有书没还！你不能删除他！");
                return;
            }
        }
    }
    QString name;
    query.exec("select name from card where cno = \'"+ret+"\'");
    while(query.next())
    {
        if(query.isActive())
        {
            name =  query.value(0).toString();
        }
    }
    bool  todo = JudgeBox(name);
    if(todo == true)
    {
        query.exec("delete from card where cno = \'"+ret+"\'");
        InforBox();
    }

}

void MainWindow::on_pushButton_11_clicked()
{
    if(admin == "ERRORACCESS")
    {
        Login();
        if(admin != "ERRORACCESS")
        {
            ui->pushButton_11->setText("退出登录");
            ui->label_2->setText("欢迎你！管理员"+admin);

        }
    }
    else
    {
        ErrorBox("成功注销！"+admin);
        admin = "ERRORACCESS";
        ui->pushButton_11->setText("登录");
        ui->label_2->setText("[未登录]");
    }
}
