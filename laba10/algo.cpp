#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"
#include <math.h>
#include "base_algo.h"
#define EPS 0.001
int SimpleAlgo(paintScene *scene, tFunction func)
{
    int maxHor = 0;
    int minHor = scene->height();
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
            scene->addMyLine(tmp, P);
        }
        P = tmp;
        tmp = tPoint(lowHorisont[pointCount-1].x, func.f(lowHorisont[pointCount-1].x, i));
        if(flag) {
            scene->addMyLine(tmp, Q);
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
    return 0;
}

