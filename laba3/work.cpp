#include "work.h"
#include "QGraphicsItem"
#include "drawLine.h"
#include "algo.h"
double draw_Algorithm(vector<tPoint> &vec, const tData &data, ALGORITHM alg)
{
    switch(alg) {
    case CDA:
        return CdaAlgo(vec, data.start, data.end);
        break;
    case BR_DOUBLE:
        return BresenhamDoubleAlgo(vec, data.start, data.end);
        break;
    case BR_INT:
        return BresenhamIntAlgo(vec, data.start, data.end);
        break;
    case BR_SMOOTH:
        return BresenhamSmoothAlgo(vec, data.start, data.end);
        break;
    case WU:
        return WuAlgo(vec, data.start, data.end);
        break;
    default:
        SomeAlgo(vec, data.start, data.end);
    }
    return 0;
}

int draw_Line(tScene &scene, const tData &data, ALGORITHM alg)
{
    tLine line;
    static tImage image(scene, data.sizePixel, data.fon);

    switch(alg) {
    //ALGORITHM PART

    case STANDART:
        image.drawLine(scene, data.color, data.start, data.end);
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
        image.changeScale(scene, data.sizePixel);
    case CLEAR_SCENE:
        image = tImage(scene, data.sizePixel, data.fon);
        break;
    default:
        return -1;
    }

    return 0;
}

int draw_Line(tAnaliz &ret, const tData &data, ALGORITHM alg)
{
    tLine line;

    int step = 1;
    //time_t t1, t2;

    switch(alg) {

    case STANDART:
        //TODO
        break;
    case WU:
        step = 2;
    case BR_DOUBLE:
    case BR_INT:
    case BR_SMOOTH:
    case CDA:
        //t1 = clock();
        ret.time = draw_Algorithm(line.line, data, alg);
        //t2 = clock();
        //ret.time = t2 - t1;
        ret.max_step = AnalizMaxStep(line.line, step);
        break;
   default:
        return -1;
    }

    return 0;
}

