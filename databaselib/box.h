#ifndef BOX_H
#define BOX_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include "dialog.h"
#include "select.h"

bool JudgeBox();
bool JudgeBox(QString message);
void InforBox();
void ErrorBox(QString A);
#endif // BOX_H
