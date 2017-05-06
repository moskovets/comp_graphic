#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"
struct tCut {
    int xl;
    int xr;
    int yt;
    int yb;
    tCut() {}
    tCut(int xleft, int xright, int ytop, int ybottom) {
        xl = xleft;
        xr = xright;
        yt = ytop;
        yb = ybottom;
    }
};
struct tSegment {
    tPoint p1;
    tPoint p2;
    tSegment() {}
    tSegment(tPoint a, tPoint b) {
        p1 = a;
        p2 = b;
    }
};

int CutSegment(const tCut &rect, tSegment &s, bool &visible) {

}

int SimpleAlgo(paintScene *scene, const QColor &colorBrush)
{
    tCut rect;
    rect.xr = max(scene->polynom.first.x, scene->polynom.second.x);
    rect.xl = min(scene->polynom.first.x, scene->polynom.second.x);
    rect.yt = max(scene->polynom.first.y, scene->polynom.second.y);
    rect.yb = min(scene->polynom.first.y, scene->polynom.second.y);
    bool visible = true;
    for(unsigned int i = 0; i < scene->segments.size(); i++) {
        tSegment s(scene->segments[i].first, scene->segments[i].second);
        CutSegment(rect, s, visible);
        if(visible) {
            scene->addMyLine(s.p1, s.p2, colorBrush);
        }
    }
    return 0;
}

