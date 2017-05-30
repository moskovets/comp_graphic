#include "base_algo.h"

void VectorMult(tVector &a, tVector& b, tVector &res)
{
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
}

int ScalarMult(tVector &a, tVector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
