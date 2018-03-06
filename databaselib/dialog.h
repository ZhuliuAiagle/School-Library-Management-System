#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include "mainwindow.h"
class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString getinput();
    void SetLabel(QString A);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
