#ifndef USERLOGIN_H
#define USERLOGIN_H
#include "dialog.h"
#include <QString>
#include "ui_userlogin.h"
class Userlogin : public QDialog
{
    Q_OBJECT
public:
    explicit Userlogin(QWidget *parent = 0);
    ~Userlogin();
    QString* getinput();
    void modify_1(QString A);
    void modify_2(QString A);
private:
    Ui::Userlogin *ui;
};
#endif // USERLOGIN_H
