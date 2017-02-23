#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include <QWidget>
#include <QtGui>
#include <QErrorMessage>
#include <QString>
#include <QStringList>
#include <string>
#include "work.h"
#include <stdio.h>
#include <vector>
#include <stack>
#include <QLineEdit>

enum Text_Error { EMPTY, E_SYMBOL, E_PAIR_DATA, E_FEW_POINT, NO_ER };
struct Data {
    Point* arr;
    int N;
    Text_Error er;
};

namespace Ui {
class MyController;
}

class MyController : public QWidget
{
    Q_OBJECT

public:
    explicit MyController(QWidget *parent = 0);
    ~MyController();
signals:
    void AnswerChange(Answer *ans);
private slots:
    void on_paint_new_clicked();

    void on_rotateButton_clicked();

    void on_scaleButton_clicked();

    void on_moveButton_clicked();

    void on_backButton_clicked();

private:
    double * GetData(vector <QLineEdit*> &vec);
    stack<Answer*> history;

    Ui::MyController *ui;
    QWidget *par;

};

double Analiz_Text(QString str);
#endif // MYCONTROLLER_H
