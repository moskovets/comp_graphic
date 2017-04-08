#ifndef ALGO_H
#define ALGO_H
#include <vector>
#include <point.h>
#include <qpoint.h>

typedef unsigned long long tick_t;

using namespace std;

int AnalizMaxStep(vector<tPoint> &vec, int step = 1);

tick_t CdaAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
tick_t BresenhamDoubleAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
tick_t BresenhamIntAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
tick_t BresenhamSmoothAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
tick_t WuAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);

#endif // ALGO_H
