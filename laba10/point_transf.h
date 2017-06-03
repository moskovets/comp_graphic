#ifndef POINT_TRANSF_H
#define POINT_TRANSF_H

#include "base_algo.h"
#include <vector>

using namespace std;

struct Rotate {
    double x_angle;
    double y_angle;
    double z_angle;
};
struct Scale {
    double kx;
    double ky;
    double kz;
};

int RotateModel(vector<pair<tPoint, tPoint>> &vect, const Rotate &act);
int ScaleModel (vector<pair<tPoint, tPoint>> &vect, const Scale &act);

#endif // POINT_TRANSF_H
