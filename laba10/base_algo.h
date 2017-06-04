#ifndef BASE_ALGO_H
#define BASE_ALGO_H
#define SIGN(x) ((int) (x > 0) - (x < 0))
#include <QPoint>
struct tPoint
{
    double x;
    double y;
    double z;
    tPoint(double x1, double y1, double z1 = 0) {
        x = x1;
        y = y1;
        z = z1;
    }
    tPoint(tPoint &a, tPoint &center) {
        x = a.x + center.x;
        y = a.y + center.y;
        z = a.z;
    }
    tPoint(QPoint a) {
        x = a.x();
        y = a.y();
        z = 0;
    }
    tPoint() {}
    bool isEqual(tPoint p) {
        return x == p.x && y == p.y && z == p.z;
    }
};


struct tSegment
{
    bool empty = false;
    tPoint p1;
    tPoint p2;
    tSegment() {}
    tSegment(tPoint &a, tPoint &b) {
        if(a.isEqual(b)) //todo
            empty = true;
        if((a.x > b.x) || (a.x == b.x && a.y > b.y)) {
            p2 = a;
            p1 = b;
        }
        else {
            p1 = a;
            p2 = b;
        }
    }
    bool isEqual(tSegment s) {
        if(p1.isEqual(s.p1) && p2.isEqual(s.p2))
            return true;
        if(p1.isEqual(s.p2) && p2.isEqual(s.p1))
            return true;
        return false;
    }
};

struct tVector
{
    int x;
    int y;
    int z;
    tVector() {}
    tVector(int a, int b, int c = 0) {
        x = a;
        y = b;
        z = c;
    }
    tVector(tPoint end, tPoint start) {
        x = end.x - start.x;
        y = end.y - start.y;
        z = 0;
    }
    tVector(tSegment s) {
        x = s.p2.x - s.p1.x;
        y = s.p2.y - s.p1.y;
        z = 0;
    }
};

struct tFunction
{
    double (*f)(double, double);
    double xmin;
    double xmax;
    double dx;
    int n;
    double zmin;
    double zmax;
};

void VectorMult(tVector &a, tVector& b, tVector &res);

int ScalarMult(tVector &a, tVector& b);

#endif // BASE_ALGO_H
