#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "my_scene.h"
#include "CImage.h"


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
    void GetScene(tScene *scene);
signals:
    void SceneChange(QGraphicsScene *scene);
private slots:

    void on_sizePixelButton_clicked();

    void on_foncolorButton_clicked();

    void on_fonButton_clicked();

    void on_linecolorButton_clicked();

    void on_clearButton_clicked();

    void on_drawellipseButton_clicked();

private:
    QString GetColor(QColor &color);
    double *GetData(vector <QLineEdit*> &vec);
    bool ValidPoint(QPoint &p);
    ALGORITHM GetAlgorithm();
    void drawEllipse(QPoint &center, double rx, double ry);
    void drawCircle(QPoint &center,  double r);
    tScene scene;
    tPaintParam  data;
    CImage image;
    Ui::MyController *ui;
    QWidget *par;
    QColor colorLine;
//    QRegExpValidator *Validator;
    QIntValidator *Validator;

};

double Analiz_Text(QString str);
#endif // MYCONTROLLER_H
