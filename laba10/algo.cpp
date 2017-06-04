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
#define BORDER 20
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
    int pointCount = lowHorisont.size();

    for(double i = func.zmin; i <= func.zmax + EPS; i += zstep) {
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
        //заглушка
        tmp = tPoint(lowHorisont[0].x, func.f(lowHorisont[0].x, i), i);
        tPoint t;
        for(int j = 1; j < pointCount - 1; j++) {
            t = tPoint(lowHorisont[j].x, func.f(lowHorisont[j].x, i), i);
            Res.push_back(pair<tPoint, tPoint>(tmp, t));
            tmp = t;
        }


        //корректировка массивов горизонтов
        for(int j = 0; j < pointCount - 1; j++) {
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

    return 0;
}

