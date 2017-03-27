#include "algo.h"
#include <QDebug>
#include <math.h>
#define SIGN(x) ((int) (x > 0) - (x < 0))

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

int SomeAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    static int k = 0;
    for(int i = 0; i < 20; i++) {
        vec.push_back(tPoint(i+k, i+k, 1));
    }
    k += 30;
}

int CdaAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }
    double dx = end.x() - start.x();
    double dy = end.y() - start.y();

    double lx = fabs(dx);
    double ly = fabs(dy);

    double l;
    if(lx > ly)
        l = lx;
    else
        l = ly;

    dx /= l;
    dy /= l;

    double x = start.x();
    double y = start.y();
    for(int i = 1; i <= int (l+1); i++) {
        vec.push_back(tPoint(round(x), round(y), 1));
        //qDebug() << x << y;
        x += dx;
        y += dy;
    }
    if(vec[vec.size() - 1].x != end.x() || vec[vec.size() - 1].y != end.y()) {

        qDebug() << "Мимо! :)" << vec[vec.size() - 1].x << vec[vec.size() - 1].y;
        return 1;
    }
    return 0;
}
int BresenhamDoubleAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end)
{
    if(IsDegenerate(start, end)) {
        return DegenerateAlgo(vec, start);
    }
    double deltax = end.x() - start.x();
    double deltay = end.y() - start.y();

    double dx = SIGN(deltax);
    double dy = SIGN(deltay);

    double sx = fabs(deltax);
    double sy = fabs(deltay);

    int obmen = 0;

    if(sx <= sy) {
        obmen = 1;
        swap(sx, sy);
    }

    double m = sy / sx;
    double e = m - 0.5;


    double x = start.x();
    double y = start.y();
    for(int i = 1; i <= sx + 1; i++) {
        vec.push_back(tPoint(round(x), round(y), 1)); //can optim
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
    if(vec[vec.size() - 1].x != end.x() || vec[vec.size() - 1].y != end.y()) {

        qDebug() << "Мимо! :)" << vec[vec.size() - 1].x << vec[vec.size() - 1].y;
        return 1;
    }


    return 0;
}
