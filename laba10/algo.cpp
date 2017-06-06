#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"
#include <math.h>
#include <limits.h>
#include "base_algo.h"
#include "point_transf.h"

#define INF (INT_MAX-1)
#define EPS 0.001
#define BORDER 100
void FindYBroders(vector<pair<tPoint, tPoint>> &graph, double &ymax, double &ymin)
{
    ymax = ymin = graph[0].first.y;
    for(int i = 0; i < graph.size(); i++){
     // qDebug() << graph[i].first.z;

        if(graph[i].first.y > ymax)
            ymax = graph[i].first.y;
        if(graph[i].first.y < ymin)
            ymin = graph[i].first.y;
        if(graph[i].second.y > ymax)
            ymax = graph[i].second.y;
        if(graph[i].second.y < ymin)
            ymin = graph[i].second.y;
    }
}

void MovingGraphic(vector<pair<tPoint, tPoint>> &graph, double dx, double dy)
{
    for(int i = 0; i < graph.size(); i++){
        graph[i].first.x  += dx;
        graph[i].first.y  += dy;
        graph[i].second.x += dx;
        graph[i].second.y += dy;
    }
}
int VisiblePoint(tPoint &low, tPoint &high, tPoint &tmp)
{
    if(tmp.y > high.y)
        return 1;
    if(tmp.y < low.y)
        return -1;
    return 0;
}
tPoint IntersectionSegments(tPoint &p1, tPoint &p2, tPoint &p3, tPoint &p4)
{
    double k1 = (p2.y - p1.y) / (p2.x - p1.x);
    double b1 = (p2.x*p1.y - p1.x*p2.y) / (p2.x - p1.x);
    double k2 = (p4.y - p3.y) / (p4.x - p3.x);
    double b2 = (p4.x*p3.y - p3.x*p4.y) / (p4.x - p3.x);
    tPoint res;
    res.x = (b2 - b1) / (k1 - k2);
    res.y = k1 * res.x + b1;
    //res.z = p1.z;
    return res;
}
double Round(double x) //magic
{
    x *= 100000;
    x = round(x);
    x /= 100000;
    return x;
}

void GorisontAlgo(tFunction &func, vector<pair<tPoint, tPoint>> &Res)
{
    int maxHor = -INF;
    int minHor = INF;
    vector<tPoint> highHorisont;
    vector<tPoint> lowHorisont;
    for(double i = func.xmin; i <= func.xmax; i += func.dx) {
        highHorisont.push_back(tPoint(i, maxHor));
        lowHorisont.push_back (tPoint(i, minHor));
    }
    tPoint P, Q;
    bool flag = false;
    double zstep = (func.zmax - func.zmin) / (func.n - 1);
    zstep = Round(zstep);
    int pointCount = lowHorisont.size();
    for(double i = func.zmin; i <= func.zmax + EPS; i += zstep) {
        if(i > func.zmax)
            i = func.zmax;
        //боковые линии
        tPoint tmp = tPoint(lowHorisont[0].x, func.f(lowHorisont[0].x, i), i);
        if(flag) {
            Res.push_back(pair<tPoint, tPoint>(tmp, P));
        }
        P = tmp;
        tmp = tPoint(lowHorisont[pointCount-1].x, func.f(lowHorisont[pointCount-1].x, i), i);
        if(flag) {
            Res.push_back(pair<tPoint, tPoint>(tmp, Q));
        }
        Q = tmp;
        flag = true;

        tPoint first = tPoint(lowHorisont[0].x, func.f(lowHorisont[0].x, i), i);
        tPoint second, I;
        int flag1, flag2;
        flag1 = VisiblePoint(lowHorisont[0], highHorisont[0], first);
        for(int j = 1; j < pointCount; j++) {
            second = tPoint(lowHorisont[j].x, func.f(lowHorisont[j].x, i), i);
               // Res.push_back(pair<tPoint, tPoint>(first, second));
            flag2 = VisiblePoint(lowHorisont[j], highHorisont[j], second);
            if(flag1 == flag2) {
                if(flag2)
                    Res.push_back(pair<tPoint, tPoint>(first, second));
            }
            else if(flag2 && flag1) {
                I = IntersectionSegments(first, second, lowHorisont[j-1], lowHorisont[j]);
                I.z = i;
                if(flag2 < 0)
                    Res.push_back(pair<tPoint, tPoint>(I, second));
                else
                    Res.push_back(pair<tPoint, tPoint>(I, first));
                I = IntersectionSegments(first, second, highHorisont[j-1], highHorisont[j]);
                I.z = i;
                if(flag2 > 0)
                    Res.push_back(pair<tPoint, tPoint>(I, second));
                else
                    Res.push_back(pair<tPoint, tPoint>(I, first));
            }
            else {
                if(flag2 + flag1 > 0)
                    I = IntersectionSegments(first, second, highHorisont[j-1], highHorisont[j]);
                else
                    I = IntersectionSegments(first, second, lowHorisont[j-1], lowHorisont[j]);
                I.z = i;
                if(flag2)
                    Res.push_back(pair<tPoint, tPoint>(I, second));
                else
                    Res.push_back(pair<tPoint, tPoint>(I, first));
            }
            first = second;
            flag1 = flag2;
        }

        //корректировка массивов горизонтов
        for(int j = 0; j < pointCount; j++) {
            lowHorisont[j].y  = min(lowHorisont[j].y,  func.f(lowHorisont[j].x,  i));
            highHorisont[j].y = max(highHorisont[j].y, func.f(highHorisont[j].x, i));
        }
    }

}

int SimpleAlgo(paintScene *scene, tFunction func)
{
    scene->graphic.clear();
    //построение с помощью алгоритма плавающего горизонта
    GorisontAlgo(func, scene->graphic);

    //корректировка координат с учетом размеров сцены
    double ymax, ymin;
    FindYBroders(scene->graphic, ymax, ymin);

    tPoint center((func.xmax - func.xmin)/2 + func.xmin, (ymax - ymin)/2 + ymin,0);
    MovingGraphic(scene->graphic, -center.x, -center.y);

    double ky = (ymax - ymin)           / (scene->height() - BORDER * 2);
    double kx = (func.xmax - func.xmin) / (scene->width()  - BORDER * 2);
    double k = 1 / max(kx, ky);
    Scale scale {k, k, k};
    ScaleModel(scene->graphic, scale);

    scene->repaintScene();
    scene->flagGraphExist = true;
    return 0;
}

