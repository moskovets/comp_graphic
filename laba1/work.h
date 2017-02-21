#ifndef WORK_H
#define WORK_H
#include <math.h>
#include <algorithm>
#include <limits.h>
#include <stdlib.h>
#include <QString>
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

class Answer {
public:
    Point p1;
    Point p2;
    Point p3;
    int diff;
    Point center;
    double R;
    void Scale(double scale) {
        p1.x *= scale;
        p2.x *= scale;
        p3.x *= scale;
        p1.y *= scale;
        p2.y *= scale;
        p3.y *= scale;
        for(int i = 0; i < arr.size(); i++) {
            arr[i].x *= scale;
            arr[i].y *= scale;
        }
    }
    vector<Point> arr;

};

// нахождение пересечения прямых ax+by+c=0
Point IntersectionsLines(double a1, double b1, double c1,
                         double a2, double b2, double c2);

int compare_double(double a, double b);

// центр окружности, проходящей через точки 1,2,3
Point FindCircumCircle(Point *p1, Point *p2, Point *p3);

// знаковая площадь треугольника 123
double SignSquare(Point *p1, Point *p2, Point *p3);

// количество точек в окружности (с контуром)
int CountPointInCircle(Point *center, double R, Point *arr, int N);

// количество точек в треугольнике (c контуром)
int CountPointInTriangle(Point *p1, Point *p2, Point *p3,
                         Point *arr, int N);

// поиск разницы точек внутри треугольника и
// за пределами треугольника внутри описанной окружности
int DifferencePointForTriangle(Point *p1, Point *p2, Point *p3,
                               Point *arr, int N);

// поиск треугольника с минимальной разницей
Answer* FindMinTriangleWithPoints(Point *arr, int N);

double LenSegment(Point *p1, Point *p2);



#endif // WORK_H
