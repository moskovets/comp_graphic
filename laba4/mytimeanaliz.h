#ifndef MYTIMEANALIZ_H
#define MYTIMEANALIZ_H

#include <QWidget>
#include "CImage.h"
#include "qcustomplot.h"

class MyTimeanaliz : public QWidget
{
    Q_OBJECT
public:
    explicit MyTimeanaliz(QWidget *parent = 0);
private:
    static const int N = 20;
    CImage image;
    QCustomPlot* customPlot;
    double rcircle[5][N],  timecircle[5][N];
    double rellipse[5][N], timeellipse[5][N];

    ALGORITHM algo_arr[5] = { BRESENHAM, MID_POINT, STANDART, CANON_EQ, PARAM_EQ };


    double AnalizTimeCircle(tScene &scene, int R, ALGORITHM algo);
    double AnalizTimeEllipse(tScene &scene, int Rx, int Ry, ALGORITHM algo);

    void calculateVectors();
    void Analiz(double r[][N], double time[][N]);
public slots:
    void MySlotEl()
    {
        qDebug() << "asfdjh\n";
        Analiz(rellipse, timeellipse);
    }
    void MySlotC()
    {
           Analiz(rcircle, timecircle);
    }

signals:

public slots:

};

#endif // MYTIMEANALIZ_H
