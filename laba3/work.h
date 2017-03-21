#ifndef WORK_H
#define WORK_H
#include "QPoint"
#include "QColor"
#include "my_scene.h"

enum ALGORITHM {
    STANDART = 1,
    BR_DOUBLE,
    BR_INT,
    BR_SMOOTH,
    CDA,
    WU,
    CHANGE_FON,
    CHANGE_SCALE,
    CLEAR_SCENE
};

struct tData {
    QPoint start;
    QPoint end;
    int sizePixel;
    QColor color;
    QColor fon;
};

int draw_Line(tScene &scene, const tData &data, ALGORITHM alg);

#endif // WORK_H
