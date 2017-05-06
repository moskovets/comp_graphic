#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"

struct tSegment {
    tPoint p1;
    tPoint p2;
    tSegment() {}
    tSegment(tPoint a, tPoint b) {
        p1 = a;
        p2 = b;
    }
};


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

