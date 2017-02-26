#ifndef WORK_H
#define WORK_H
#include <math.h>
#include <algorithm>
#include <limits.h>
#include <stdlib.h>
#include <QString>
#include <QPolygonF>
#include <iostream>

enum error_status { OK, NoSolution, Identity };

#define EPS 0.1
#define INF INT_MAX
#define POINT_COUNT 500
#define RECT_BORDER 10
#define STEP_BRUSH 15
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
/*class Matrix {
public:
    int n;
    int m;
    double** a;
    Matrix(double x, double y, double z = 1)
    {
        a = new double * [1];
        a[1] = new double[3];
        a[0][0] = x;
        a[0][1] = y;
        a[0][2] = z;
    }

    Matrix(double x, double y, double z = 1)
    {
        a = new double * [1];
        a[1] = new double[3];
        a[0][0] = x;
        a[0][1] = y;
        a[0][2] = z;
    }

};*/
bool krat(double a, int k);

struct MyBrush {

    QPointF start;
    int step = STEP_BRUSH;
    //int count;
    vector<vector<QPointF>> vec;
    void InitBrush(double polSidex, double polSidey) {
        start = QPointF(-polSidex - 2 * polSidey, -polSidey);
        step = STEP_BRUSH;

        int size = (polSidex - start.x()) / STEP_BRUSH;
        vec.resize(size);
        double x0 = -polSidex;
        double y0 = -polSidey;
        double y1 = polSidey;
        double x1 = polSidex;
        double h = polSidey * 2;
        double endx = x1 - h;
        int j;
        for(double i = start.x(), j = 0; i <= polSidex; i += STEP_BRUSH, j++)
        {
            if(j == vec.size()) {
                vec.resize(j+1);
            }
            vec[j].push_back(i < x0 ? QPointF(x0, y0 + x0 - i) :  QPointF(i, y0));
            vec[j].push_back(i < endx ? QPointF(i + h, y1) :  QPointF(x1, y0 + x1 - i));
        }
    }
    void add(QPointF &p) {
        double tmp = p.x() - (p.y() - start.y()) - start.x();
        if(krat(tmp, step)) {
            int j = tmp / step;
            vec[j].push_back(p);
        }
    }
    void add(QPolygonF &p) {

        for(int i = 1; i < p.size() - 1; i++) {
            double tmp2 = p[i+1].x() - (p[i+1].y() - start.y()) - start.x();
            double tmp1 = p[i].x() - (p[i].y() - start.y()) - start.x();
            double tmp0 = p[i-1].x() - (p[i-1].y() - start.y()) - start.x();
            int j = tmp1 / step;
            double comp = j * step;
            if((abs(tmp1 - comp) <= abs(tmp0 - comp)) && (abs(tmp1 - comp) <= abs(tmp2 - comp)) ){
            //if((tmp1 - comp) * (tmp0 - comp) < 0) {
                if(abs(tmp1 - comp) <= EPS) {

                //int j = tmp / step;
                vec[j].push_back(p[i]);
                }
            }

        }
    }
    void sort_vec(vector<QPointF> &arr) {
        for(int i = 0; i < arr.size(); i++) {
            for(int j = 1; j < arr.size(); j++) {
                if(arr[j].x() < arr[j - 1].x()) {
                    swap(arr[j], arr[j - 1]);
                }
            }
        }

    }

    void sort_and_clear()
    {
       cout << "ko" << endl;
        for(int i = 0; i < vec.size(); i++) {
           //cout << "ok";
           cout << vec[i].size() << " ";

           sort_vec(vec[i]);
           int j = vec[i].size() - 1;
           if(j > 3) {
               vec[i][2] = vec[i][j - 1];
               vec[i][3] = vec[i][j];
           }
           for(;j > 3; j--) { vec[i].pop_back(); }
       }
        cout << endl;
    }
};

struct Answer {
    QPolygonF rect;
    QPolygonF graph;
    MyBrush brush;
    Answer(Answer *obj) {
        rect = obj->rect;
        graph = obj->graph;
        brush = obj->brush;
    }
    Answer() {}
};


Answer* Create_figure(double a, double b);
Answer* Rotate_figure(Answer* figure, double cx, double cy, double angle);
Answer* Scale_figure(Answer* figure, double cx, double cy, double kx, double ky);
Answer* Move_figure(Answer* figure, double dx, double dy);


#endif // WORK_H
