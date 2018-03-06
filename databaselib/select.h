#ifndef SELECT_H
#define SELECT_H
#include "dialog.h"
#include "ui_select.h"
class Select : public QDialog
{
    Q_OBJECT
public:
    explicit Select(QWidget *parent = 0);
    ~Select();
    void SetLabel(QString A);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Select *ui;
};
#endif //SELECT_H
