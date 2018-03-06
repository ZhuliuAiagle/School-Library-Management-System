#ifndef FORM_H
#define FORM_H
#include "dialog.h"
#include "ui_form.h"
class Form : public QDialog
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    QString* getinput();
private:
    Ui::Form *ui;
};
#endif // FORM_H
