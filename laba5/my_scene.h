#ifndef MY_SCENE_H
#define MY_SCENE_H
#include "paint.h"
#include <QGraphicsScene>

struct tScene {
    paintScene *scene = NULL;
    vector<QPointF> *poly;
};

#endif // MY_SCENE_H
