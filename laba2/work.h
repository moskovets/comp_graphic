#ifndef WORK_H
#define WORK_H
#include <math.h>
#include <algorithm>
#include <limits.h>
#include <stdlib.h>
#include <QString>
#include <QPolygon>

enum error_status { OK, NoSolution, Identity };

#define EPS 0.00001
#define INF INT_MAX

using namespace std;

class Point {
public:
    double x;
    double y;
    error_status status = OK;
    Point(double a, double b, error_status er) {
        x = a; y = b; status = er;
    }
    Point(double a, double b) {
        x = a; y = b; status = OK;
    }
    Point() {}
    QString ToQString() {
        QString str = "(" + QString::number(x) + "; " +
                      QString::number(y) + ")";
        //cout << str << endl;
        return str;
    }
};

struct Answer {
    QPolygon rect;
    QPolygon graph;
};


Answer* Create_figure(double a, double b);
Answer* Rotate_figure(Answer* figure, double cx, double cy, double angle);
Answer* Scale_figure(Answer* figure, double cx, double cy, double kx, double ky);
Answer* Move_figure(Answer* figure, double dx, double dy);


#endif // WORK_H
