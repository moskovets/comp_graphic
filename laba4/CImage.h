#ifndef CIMAGE_H
#define CIMAGE_H

#include "my_scene.h"
#include "errors.h"
#include "point.h"
#include <vector>
using namespace std;

struct tLine {
    vector <tPoint> line;
    int sizePixel;
    QColor color;
};

enum ALGORITHM {
    STANDART = 1,
    BRESENHAM, // алгоритм Брезенхема
    MID_POINT, // алгоритм средней точки
    CANON_EQ,  // алгоритм построения по каноническому уравнению
    PARAM_EQ,  // алгоритм построения по параметрическому уравнению

    CHANGE_FON,
    CHANGE_SCALE,
    CLEAR_SCENE
};
struct tPaintParam {
    int sizePixel;
    QColor color;
    QColor fon;
};

struct tDataEllipse {
    QPoint center;
    int a;
    int b;
    tPaintParam param;
};

struct tDataCircle {
    QPoint center;
    int radius;
    tPaintParam param;
};

typedef unsigned long long int tick_t;

struct tAnaliz {
    tick_t time;
    int max_step;
};

class tImage {
    private:
        QImage image;
        QColor fon;
        int scale;
        void printOnScene(tScene &scene);
        void addPixel(const tPoint &p,
                      const QColor &color);
    public:
        tImage() {} //todo
        tImage(int w, int h, int sizepixel); //todo
        tImage(tScene &scene, int sizepixel, const QColor &color); // add from param
        void changeFon(tScene &scene, const QColor &color);
        void changeScale(tScene &scene, int sizepixel);

        double draw_Algorithm(tScene &scene, const tDataEllipse &data, ALGORITHM alg);
        double draw_Algorithm(tScene &scene, const tDataCircle &data,  ALGORITHM alg);

};

#endif // CIMAGE_H
