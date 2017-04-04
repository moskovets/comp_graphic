#ifndef ALGO_H
#define ALGO_H
#include <vector>
#include <point.h>
#include <qpoint.h>

using namespace std;

int AnalizMaxStep(vector<tPoint> &vec, int step = 1);

int SomeAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
int CdaAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
int BresenhamDoubleAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
int BresenhamIntAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
int BresenhamSmoothAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);
int WuAlgo(vector<tPoint> &vec, const QPoint &start, const QPoint &end);

#endif // ALGO_H
