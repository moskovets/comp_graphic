#include "algo.h"
#include <QDebug>
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
    for(int i = 1; i < l + 1; i++) {
        vec.push_back(tPoint(round(x), round(y), 1));
        qDebug() << x << y;
        x += dx;
        y += dy;
    }
    return 0;
}
