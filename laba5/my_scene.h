#ifndef MY_SCENE_H
#define MY_SCENE_H
#include "paint.h"
#include <QGraphicsScene>

struct tScene {
    paintScene *scene = NULL;
    double x_center;
    double y_center;
};

#endif // MY_SCENE_H
