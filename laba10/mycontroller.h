#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

//#include "my_scene.h"
//#include "CImage.h"


#include <QWidget>
#include <QtGui>
#include <QErrorMessage>
#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QGraphicsScene>

#include <string>
#include <stdio.h>
#include <vector>
#include <paintscene.h>
#include "f.h"

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
    void GetScene(paintScene *scene);
signals:
    void SceneChange(QGraphicsScene *scene);
private slots:

    void on_drawButton_clicked();

    void on_rotateButton_clicked();

    void on_functionBox_activated(int index);

    void on_clearallButton_clicked();

private:
    QString GetColor(QColor &color);
    double *GetData(vector <QLineEdit*> &vec);
    bool ValidPoint(QPoint &p);
    void drawEllipse(QPoint &center, double rx, double ry);
    void drawCircle(QPoint &center,  double r);
    paintScene *scene;
    Ui::MyController *ui;
    QWidget *par;
    QColor colorLine;
    QColor colorBrush;
    int timePause;
    QRegExpValidator *Validator;
    int functionIndex;
    CFunction func;
};

double Analiz_Text(QString str);
#endif // MYCONTROLLER_H
