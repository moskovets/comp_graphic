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

void GorisontAlgo(tFunction &func, vector<pair<tPoint, tPoint>> &Res)
{
    int maxHor = -INF;
    int minHor = INF;
    vector<tPoint> highHorisont;
    vector<tPoint> lowHorisont;
    for(int i = func.xmin; i <= func.xmax; i += func.dx) {
        highHorisont.push_back(tPoint(i, maxHor));
        lowHorisont.push_back (tPoint(i, minHor));
    }
    tPoint P, Q;
    bool flag = false;
    double zstep = (func.zmax - func.zmin) / (func.n - 1);
    int pointCount = lowHorisont.size();

    for(double i = func.zmin; i <= func.zmax + EPS; i += zstep) {
        //боковые линии
        tPoint tmp = tPoint(lowHorisont[0].x, func.f(lowHorisont[0].x, i));
        if(flag) {
            Res.push_back(pair<tPoint, tPoint>(tmp, P));
        }
        P = tmp;
        tmp = tPoint(lowHorisont[pointCount-1].x, func.f(lowHorisont[pointCount-1].x, i));
        if(flag) {
            Res.push_back(pair<tPoint, tPoint>(tmp, Q));
        }
        Q = tmp;
        flag = true;
        //

        //корректировка массивов горизонтов
        for(int j = 0; j < pointCount - 1; j++) {
            lowHorisont[j].y  = min(lowHorisont[j].y,  func.f(lowHorisont[j].x,  i));
            highHorisont[j].y = max(highHorisont[j].y, func.f(highHorisont[j].x, i));
        }
    }
}

int SimpleAlgo(paintScene *scene, tFunction func)
{
    //построение с помощью алгоритма плавающего горизонта
    GorisontAlgo(func, scene->graphic);
    //корректировка координат с учетом размеров сцены



    return 0;
}

