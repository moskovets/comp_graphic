#include "algo.h"

int SomeAlgo(vector<tPoint> &vec, const QPointF &start, const QPointF &end) {
    static int k = 0;
    for(int i = 0; i < 20; i++) {
        vec.push_back(tPoint(i+k, i+k, 1));
    }
    k += 30;
}
