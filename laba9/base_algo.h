#ifndef BASE_ALGO_H
#define BASE_ALGO_H
#define SIGN(x) ((int) (x > 0) - (x < 0))
#include <QPoint>
struct tPoint
{
    int x;
    int y;
    tPoint(int x1, int y1) {
        x = x1;
        y = y1;
    }
    tPoint(QPoint a) {
        x = a.x();
        y = a.y();
    }
    tPoint() {}
    bool isEqual(tPoint p) {
        return x == p.x && y == p.y;
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

void VectorMult(tVector &a, tVector& b, tVector &res);

int ScalarMult(tVector &a, tVector& b);

#endif // BASE_ALGO_H
