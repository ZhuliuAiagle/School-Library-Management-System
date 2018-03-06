#ifndef USERS_H
#define USERS_H
#include "dialog.h"
#include "ui_users.h"
class Users : public QDialog
{
    Q_OBJECT
public:
    explicit Users(QWidget *parent = 0);
    ~Users();
    QString* getinput();
private:
    Ui::Users *ui;
};
#endif // USERS_H
