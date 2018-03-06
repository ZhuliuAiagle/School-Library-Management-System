#ifndef MODE_H
#define MODE_H
#include <QDialog>
#include "dialog.h"
#include "mainwindow.h"
#include "ui_mode.h"
class Mode : public QDialog
{
    Q_OBJECT
public:
    explicit Mode(QWidget *parent = 0);
    ~Mode();
    int getinput();
    void SetLabel(QString A);
private:
    Ui::Mode *ui;
};
#endif // MODE_H
