#ifndef DRAWLINE_H
#define DRAWLINE_H

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


//int Draw_line(tScene &scene, const tLine &line);

class tImage {
    private:
        QImage image;
        QColor fon;
        int scale;
        void printOnScene(tScene &scene);
        void addPixel(const tPoint &p,
                      const QColor &color);
    public:
        tImage(); //todo
        tImage(int w, int h, int sizepixel); //todo
        tImage(tScene &scene, int sizepixel, const QColor &color);
        void changeFon(tScene &scene, const QColor &color);
        void changeScale(tScene &scene, int sizepixel);
        void drawLine(tScene &scene, const tLine &line);
        void drawLine(tScene &scene, const QColor &c,
                      const QPoint &start, const QPoint &end);

};

#endif // DRAWLINE_H
