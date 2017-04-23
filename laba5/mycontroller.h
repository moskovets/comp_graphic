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


    void on_foncolorButton_clicked();

    void on_linecolorButton_clicked();

    void on_clearButton_clicked();

    void on_drawButton_clicked();

    void on_timeButton_clicked();

    void on_brushButton_clicked();

    void on_clearallButton_clicked();

private:
    QString GetColor(QColor &color);
    double *GetData(vector <QLineEdit*> &vec);
    bool ValidPoint(QPoint &p);
    //ALGORITHM GetAlgorithm();
    void drawEllipse(QPoint &center, double rx, double ry);
    void drawCircle(QPoint &center,  double r);
    paintScene *scene;
    //tPaintParam  data;
    //CImage image;
    Ui::MyController *ui;
    QWidget *par;
    QColor colorLine;
    QColor colorBrush;
    int timePause;
//    QRegExpValidator *Validator;
    QIntValidator *Validator;

};

double Analiz_Text(QString str);
#endif // MYCONTROLLER_H
