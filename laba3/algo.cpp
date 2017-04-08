#include "algo.h"
#include <QDebug>
#include <math.h>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <ctime>
#define EPS 0.00001
#define SIGN(x) ((int) (x > 0) - (x < 0))

tick_t tick(void)
{
    tick_t d;
    __asm__ __volatile__
    (
        "rdtsc"
        : "=A" (d)
    );
    return d;
}

int AnalizMaxStep(vector<tPoint> &vec, int step)
{
    int res = 1;
    int tmp = 1;
    for(unsigned int i = step; i < vec.size(); i += step) {
        if(vec[i].x == vec[i-step].x || vec[i].y == vec[i-step].y) {
            tmp++;
            if(tmp > res)
                res = tmp;
        }
        else
           tmp = 1;
    }
    return res;
}

int IsDegenerate(const QPoint &start, const QPoint &end)
{
    if(start.x() == end.x() && start.y() == end.y())
        return 1;
    return 0;
}

int DegenerateAlgo(vector<tPoint> &vec, const QPoint &p)
{
    vec.push_back(tPoint(p.x(), p.y(), 1));
    return 0;
}

tick_t CdaAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();
    tick_t res_time = 0;
    tick_t t1 = tick();

    double lx = fabs(dx);
    double ly = fabs(dy);

    double l; // ERR not int
    if(lx > ly)
        l = lx;
    else
        l = ly;

    dx /= l;
    dy /= l;
    res_time = 0;

    double x = start.x();
    double y = start.y();
    int xi = 0;
    int yi = 0;
    for(int i = 1; i <= int(l+1); i++) {
        xi = round(x);
        yi = round(y);
        res_time += tick() - t1;
        vec.push_back(tPoint(xi, yi, 1));
        t1 = tick();
        //qDebug() << x << y;
        x += dx;
        y += dy;
    }
    res_time += tick() - t1;

    if(vec[vec.size() - 1].x != end.x() || vec[vec.size() - 1].y != end.y()) {
        qDebug() << "Мимо! :)" << vec[vec.size() - 1].x << vec[vec.size() - 1].y;
        return -1;
    }
    return res_time;
}
tick_t BresenhamDoubleAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }
    int deltax = end.x() - start.x();
    int deltay = end.y() - start.y();

    tick_t res_time = 0;
    tick_t t1 = tick();

    int dx = SIGN(deltax);
    int dy = SIGN(deltay);

    int sx = fabs(deltax);
    int sy = fabs(deltay);

    int obmen = 0;

    if(sx <= sy) {
        obmen = 1;
        swap(sx, sy);
    }

    double m = double(sy) / sx;
    double e = m - 0.5;

    res_time = 0;

    int x = start.x();
    int y = start.y();
    for(int i = 1; i <= sx + 1; i++) {
        res_time += tick() - t1;
        vec.push_back(tPoint(x, y, 1));
        t1 = tick();
        //qDebug() << round(x) << round(y);
        if(e >= 0) {
            if(obmen)
                x += dx;
            else
                y += dy;
            e -= 1;
        }

        if(obmen)
            y += dy;
        else
            x += dx;
        e += m;
    }
    res_time += tick() - t1;

    if(vec[vec.size() - 1].x != end.x() || vec[vec.size() - 1].y != end.y()) {

        qDebug() << "Мимо! :)" << vec[vec.size() - 1].x << vec[vec.size() - 1].y;
        return -1;
    }

    return res_time;
}
tick_t BresenhamSmoothAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }
    int deltax = end.x() - start.x();
    int deltay = end.y() - start.y();

    tick_t res_time = 0;
    tick_t t1 = tick();

    int dx = SIGN(deltax);
    int dy = SIGN(deltay);

    int sx = fabs(deltax);
    int sy = fabs(deltay);

    int obmen = 0;

    if(sx <= sy) {
        obmen = 1;
        swap(sx, sy);
    }

    double m = double(sy) / sx;
    double I = 1; //max intensivity
    double e = I * 0.5;
    m *= I;
    double w = I - m;


    int x = start.x();
    int y = start.y();
    res_time = 0;

    double tmp;
    for(int i = 0; i <= sx; i++) {
        tmp = 1 - e;
        res_time += tick() - t1;
        vec.push_back(tPoint(x, y, tmp));
        t1 = tick();
        if(e >= w) {
            if(obmen)
                x += dx;
            else
                y += dy;
            e -= I;
        }
        if(obmen)
            y += dy;
        else
            x += dx;
        e += m;

    }
    res_time += tick() - t1;
    if(vec[vec.size() - 1].x != end.x() || vec[vec.size() - 1].y != end.y()) {

        qDebug() << "Мимо! :)" << vec[vec.size() - 1].x << vec[vec.size() - 1].y;
        return -1;
    }
    return res_time;
}

tick_t BresenhamIntAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }
    int deltax = end.x() - start.x();
    int deltay = end.y() - start.y();

    tick_t res_time = 0;
    tick_t t1 = tick();

    int dx = SIGN(deltax);
    int dy = SIGN(deltay);

    int sx = abs(deltax);
    int sy = abs(deltay);

    int obmen = 0;

    if(sx <= sy) {
        obmen = 1;
        swap(sx, sy);
    }

    int e = 2*sy - sx;
    int d2x = 2*sx;
    int d2y = 2*sy;
    res_time = 0;


    int x = start.x();
    int y = start.y();
    for(int i = 1; i <= sx + 1; i++) {
        res_time += tick() - t1;
        vec.push_back(tPoint(x, y, 1));
        t1 = tick();
        //qDebug() << round(x) << round(y);
        if(e >= 0) {
            if(obmen)
                x += dx;
            else
                y += dy;
            e -= d2x;
        }

        if(obmen)
            y += dy;
        else
            x += dx;
        e += d2y;
    }
    res_time += tick() - t1;
    if(vec[vec.size() - 1].x != end.x() || vec[vec.size() - 1].y != end.y()) {

        qDebug() << "Мимо! :)" << vec[vec.size() - 1].x << vec[vec.size() - 1].y;
        return -1;
    }
    return res_time;
}
double mantissa(double a) {
    return a - trunc(a);
}
tick_t WuAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }

    double x1 = start.x();
    double y1 = start.y();
    double x2 = end.x();
    double y2 = end.y();

    tick_t res_time = 0;
    tick_t t1 = tick();

    int obmen = 0;

    if(fabs(y2 - y1) > fabs(x2 - x1)) {
        obmen = 1;
        swap(x1, y1);
        swap(x2, y2);
    }

    if(x2 < x1) {
        swap(x1, x2);
        swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;

    double m = double(dy) / dx;

    double e = y1;
    int re = 0;
    int re1 = 0;
    int me = 0;
    int me1 = 0;
    res_time = 0;
    for(int x = x1; x < x2; x++) {
        re = round(e);
        re1 = re - 1;
        me = mantissa(e);
        me1 = 1 - me;

        res_time += tick() - t1;

        if(obmen) {
            vec.push_back(tPoint(re, x, me1));
            vec.push_back(tPoint(re1, x, me));
        }
        else {
            vec.push_back(tPoint(x, re, me1));
            vec.push_back(tPoint(x, re1, me));
        }
        t1 = tick();
        e += m;
    }
    res_time += tick() - t1;

    if(obmen) {
        vec.push_back(tPoint(y2, x2, 1));
        vec.push_back(tPoint(y2 - 1, x2, 0));
    }
    else {
        vec.push_back(tPoint(x2, y2, 1));
        vec.push_back(tPoint(x2, y2 - 1, 0));
    }

    return res_time;
}
