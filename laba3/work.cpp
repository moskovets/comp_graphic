#include "work.h"
#include "QGraphicsItem"
#include "drawLine.h"
#include "algo.h"
int draw_Algorithm(vector<tPoint> &vec, const tData &data, ALGORITHM alg)
{
    //TODO
    return 0;
}

int draw_Line(tScene &scene, const tData &data, ALGORITHM alg)
{
    tLine line;
    static tImage image(scene, data.sizePixel, data.fon);

    switch(alg) {
    //ALGORITHM PART

    case STANDART:
        //TODO
        break;
    case BR_DOUBLE:
    case BR_INT:
    case BR_SMOOTH:
    case CDA:
    case WU:
        draw_Algorithm(line.line, data, alg);
        line.color = data.color;
        line.sizePixel = data.sizePixel;
        image.drawLine(scene, line);
        break;

    //IMAGE PART

    case CHANGE_FON:
        image.changeFon(scene, data.fon);
        break;
    case CHANGE_SCALE:
        image.changeFon(scene, data.sizePixel);
    case CLEAR_SCENE:
        image = tImage(scene, data.sizePixel, data.fon);
        break;
    default:
        return -1;
    }

    return 0;
}

