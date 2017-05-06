#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"
struct tCut {
    int xl;
    int xr;
    int yb;
    int yt;
    tCut() {}
    tCut(int xleft, int xright, int ytop, int ybottom) {
        xl = xleft;
        xr = xright;
        yt = ytop;
        yb = ybottom;
    }
    int operator[] (int n) const {
        switch(n) {
            case 1:
                return xl;
            case 2:
                return xr;
            case 3:
                return yb;
            case 4:
                return yt;
            default:
                qDebug() << "ooops";
                return -1;
        }
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
int SegmentVisible(const tCut &rect, const tPoint &p1, const tPoint &p2, int &t1i, int &t2i, int i)
{
    int t[2][5] = { 0 };
    tPoint points[2] = { p1, p2 };
    int s[2] = { 0 };
    for(int j = 0; j < 2; j++) {
        t[j][1] = points[j].x >= rect.xl ? 0 : 1;
        t[j][2] = points[j].x <= rect.xr ? 0 : 1;
        t[j][3] = points[j].y >= rect.yb ? 0 : 1;
        t[j][4] = points[j].y <= rect.yt ? 0 : 1;
        for(int k = 1; k <= 4; k++) {
            s[j] += t[j][k];
        }
    }
    t1i = t[0][i];
    t2i = t[1][i];
    if(s[0] == 0 && s[1] == 0)
        return 1; //visible all
    int p = 0;
    for(int j = 1; j <= 4; j++) {
        p += t[0][j] * t[1][j];
    }
    if(p != 0)
        return -1; //unvisible all;
    return 0; // visible part
}

int CutSegment(const tCut &rect, tPoint &p1, tPoint &p2, bool &visible)
{
    short int fl = 0;
    double m = 0;
    if(p1.x == p2.x) {
        fl = -1;
    }
    else {
        m = (p2.y - p1.y) / (double) (p2.x - p1.x);
        if(p2.y == p1.y) {
            fl = 1;
        }
    }
    int tmp, t1, t2;
    for(int i = 1; i <= 4; i++) {
        tmp = SegmentVisible(rect, p1, p2, t1, t2, i);
        if(tmp == 1) {
            visible = true;
            return 0;
        }
        if(tmp == -1) {
            visible = false;
            return 0;
        }
        if(t1 != t2) {
            if(t1 == 0) {
                swap(p1.x, p2.x);
                swap(p1.y, p2.y);
            }
            if(fl == -1) {
                p1.y = rect[i];
            }
            else {
                if(i <= 2) {
                    p1.y = round(m * (rect[i] - p1.x)) + p1.y;
                    p1.x = rect[i];
                }
                else {
                    p1.x = round((rect[i] - p1.y) / m) + p1.x;
                    p1.y = rect[i];
                }
            }
        }
    }
    visible = true;
    return 0;
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
        CutSegment(rect, s.p1, s.p2, visible);
        if(visible)
            scene->addMyLine(s.p1, s.p2, colorBrush, 3);
    }
    return 0;
}

