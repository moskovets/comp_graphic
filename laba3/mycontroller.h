#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "my_scene.h"

#include <QWidget>
#include <QtGui>
#include <QErrorMessage>
#include <QString>
#include <QStringList>
#include <string>
#include <stdio.h>
#include <vector>
#include <QLineEdit>
#include <QGraphicsScene>

using namespace std;
enum Text_Error { EMPTY, E_SYMBOL, NO_ER };


namespace Ui {
class MyController;
}

class MyController : public QWidget
{
    Q_OBJECT

public:
    explicit MyController(QWidget *parent = 0);
    ~MyController();
    void GetScene(My_Scene *scene);
signals:
    void SceneChange(QGraphicsScene *scene);
private slots:

private:

    double *GetData(vector <QLineEdit*> &vec);
    My_Scene scene;
    Ui::MyController *ui;
    QWidget *par;
    QRegExpValidator *Validator;

};

double Analiz_Text(QString str);
#endif // MYCONTROLLER_H
