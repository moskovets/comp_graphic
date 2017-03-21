#ifndef POINT_H
#define POINT_H

struct tPoint {
    int x;
    int y;
    double intensity;
    tPoint(int a, int b, double i) {
        x = a;
        y = b;
        intensity = i;
    }


};

#endif // POINT_H
