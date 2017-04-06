#ifndef ALGO_H
#define ALGO_H
#include <vector>
#include <point.h>
#include <qpoint.h>

using namespace std;

int AnalizMaxStep(vector<tPoint> &vec, int step = 1);

int SomeAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
double CdaAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
double BresenhamDoubleAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
double BresenhamIntAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
double BresenhamSmoothAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
double WuAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);

#endif // ALGO_H
