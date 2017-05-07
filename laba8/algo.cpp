#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"

#define SIGN(x) ((int) (x > 0) - (x < 0))

struct tSegment {
    tPoint p1;
    tPoint p2;
    tSegment() {}
    tSegment(tPoint a, tPoint b) {
        p1 = a;
        p2 = b;
    }
};
struct tVector {
    int x;
    int y;
    int z;
    tVector() {}
    tVector(int a, int b, int c = 0) {
        x = a;
        y = b;
        z = c;
    }
    tVector(tPoint start, tPoint end) {
        x = end.x - start.x;
        y = end.y - start.y;
        z = 0;
    }
};

void VectorMult(tVector &a, tVector& b, tVector &res) {
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
}

int IsConvexPolygon(vector<tPoint> &polynom) {
    tVector a(polynom[1], polynom[0]);
    tVector b;
    int n = polynom.size();
    tVector tmp;
    int res = 0;
    for(int i = 1; i < n - 1; i++) {
        b = tVector(polynom[i + 1], polynom[i]);
        VectorMult(a, b, tmp);
        if(res == 0)
            res = SIGN(tmp.z);
        if(res != SIGN(tmp.z)) {
            return -1;
        }
        a = b;
    }
    return abs(res);
}

int CutSegment(vector<tPoint> &polynom, tPoint &p1, tPoint &p2, bool &visible)
{
    return 0;
}

int SimpleAlgo(paintScene *scene, const QColor &colorBrush)
{
    bool visible = true;
    for(unsigned int i = 0; i < scene->segments.size(); i++) {
        tSegment s(scene->segments[i].first, scene->segments[i].second);
        CutSegment(scene->polynom, s.p1, s.p2, visible);
        if(visible)
            scene->addMyLine(s.p1, s.p2, colorBrush, 3);
    }
    return 0;
}

