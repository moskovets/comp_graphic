#include "work.h"
#include <iostream>
int compare_double(double a, double b)
{
    if(fabs(a - b) < EPS)
        return 0;
    if(a > b)
        return 1;
    return -1;
}

Point IntersectionsLines(double a1, double b1, double c1,
                         double a2, double b2, double c2)
{
    double d = a1 * b2 - a2 * b1;
    if(compare_double(d, 0)) {
        double x = c2 * b1 - c1 * b2;
        double y = a2 * c1 - a1 * c2;
        x /= d;
        y /= d;
        return Point(x, y, OK);
    }
    else {
        if(compare_double(a1*c2, a2*c1) || compare_double(b1*c2, b2*c1)) {
            return Point(0, 0, NoSolution);
        }
        else {
            return Point(0, 0, Identity);
        }
    }
}


Point FindCircumCircle(Point *p1, Point *p2, Point *p3)
{
    double tmp = p1->x * p1->x + p1->y * p1->y;
    double c1 = tmp - p2->x * p2->x - p2->y * p2->y;
    c1 /= (-2);
    double c2 = tmp - p3->x * p3->x - p3->y * p3->y;
    c2 /= (-2);
    return IntersectionsLines(p1->x - p2->x, p1->y - p2->y, c1,
                              p1->x - p3->x, p1->y - p3->y, c2);
}

double SignSquare(Point *p1, Point *p2, Point *p3)
{
    double S = (p2->x - p1->x) * (p3->y - p1->y) - (p3->x - p1->x) * (p2->y - p1->y);
    //cout << "S:" << S << endl;
    return S / 2;
}

int CountPointInCircle(Point *center, double R, Point *arr, int N)
{
    double r = R * R;
    int Res = 0;
    for(int i = 0; i < N; i++) {
        double len = (arr[i].x - center->x) * (arr[i].x - center->x) +
                     (arr[i].y - center->y) * (arr[i].y - center->y);
        len = sqrt(len);
        if(compare_double(len, r) <= 0) { //= для включения контура
            Res++;
        }
    }
    return Res;
}


int CountPointInTriangle(Point *p1, Point *p2, Point *p3,
                         Point *arr, int N)
{
    int res = 0;
    double S = fabs(SignSquare(p1, p2, p3));
    double tmp;
    for(int i = 0; i < N; i++) {
        tmp = fabs(SignSquare(p1, p2, &arr[i])) + fabs(SignSquare(&arr[i], p2, p3)) +
              fabs(SignSquare(p1, &arr[i], p3));
        if(compare_double(tmp, S) == 0) {
           res++;
        }
    }
    return res;
}

int DifferencePointForTriangle(Point *p1, Point *p2, Point *p3,
                               Point *arr, int N)
{
    double S = SignSquare(p1, p2, p3);
    if(compare_double(S, 0) == 0)
        return INF;
    cout << p1->ToQString().toStdString() << " ";
    cout << p2->ToQString().toStdString() << " ";
    cout << p3->ToQString().toStdString() << " ";
    Point center = FindCircumCircle(p1, p2, p3);
    double R = LenSegment(p1, &center);
    int N1 = CountPointInTriangle(p1, p2, p3, arr, N);
    int N2 = CountPointInCircle(&center, R, arr, N);
    int in = N1;
    int out = N2 - N1 + 3;
    cout << in << " " << out << endl;
    return fabs(in - out);
}

Answer* FindMinTriangleWithPoints(Point *arr, int N)
{
    int tmp = 0;
    Answer *res = new Answer;
    res->diff = INF;
    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            for(int k = j + 1; k < N; k++) {
                tmp = DifferencePointForTriangle(&arr[i], &arr[j], &arr[k], arr, N);
                if(tmp < res->diff) {
                    res->p1 = arr[i];
                    res->p2 = arr[j];
                    res->p3 = arr[k];
                    res->diff = tmp;
                }
            }
        }
    }
    //cout << "find:" << res->p1.x << endl;
    if(res->diff == INF) {
        //cout << "o" << endl;
        return res;
    }
    res->center = FindCircumCircle(&res->p1, &res->p2, &res->p3);
    res->R = LenSegment(&res->p1, &res->center);
    for(int i = 0; i < N; i++) {
        if(LenSegment(&arr[i], &res->center) <= res->R + EPS) {
            res->arr.push_back(arr[i]);
        }
    }
    return res;
}

double LenSegment(Point *p1, Point *p2)
{
    return sqrt((p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y));
}
