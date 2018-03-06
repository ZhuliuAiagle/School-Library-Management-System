#include "box.h"
bool JudgeBox()
{
    QMessageBox::StandardButton rb = \
            QMessageBox::question(NULL, "来自数据库的提示", "你确定执行该操作?", \
                         QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QMessageBox::information(NULL,"来自数据库的提示","操作成功!",QMessageBox::Yes,QMessageBox::Yes);
        return true;
    }
    else
    {
        QMessageBox::information(NULL,"from db","操作撤销",QMessageBox::Yes,QMessageBox::Yes);
        return false;
    }

}
void InforBox()
{
    QMessageBox::information(NULL,"来自数据库的提示","操作成功!",QMessageBox::Yes,QMessageBox::Yes);
}
void ErrorBox(QString A)
{
    QMessageBox::warning(NULL,"来自数据库的提示",A,QMessageBox::Yes,QMessageBox::Yes);
}

bool JudgeBox(QString Message)
{
    QMessageBox::StandardButton rb = \
            QMessageBox::question(NULL, "来自数据库的提示", "这是你要删除的用户的姓名:"+Message+"\n"+"你确定执行该操作?", \
                         QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QMessageBox::information(NULL,"来自数据库的提示","操作成功!",QMessageBox::Yes,QMessageBox::Yes);
        return true;
    }
    else
    {
        QMessageBox::information(NULL,"来自数据库的提示","操作撤销",QMessageBox::Yes,QMessageBox::Yes);
        return false;
    }

}
