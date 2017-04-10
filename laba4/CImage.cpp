#include <QPen>
#include "CImage.h"
#include <QGraphicsItem>
#include <QPainter>
#include <math.h>
#define EPS 0.1
#define SIGN(x) ((int) (x > 0) - (x < 0))

tick_t tick(void)
{
    tick_t d;
    __asm__ __volatile__
    (
        "rdtsc"
        : "=A" (d)
    );
    return d;
}

void CImage::printOnScene(tScene &scene)
{
    QImage image_tmp = image.scaled(QSize(image.width() * scale,
                       image.height() * scale));
    QPixmap pixmap;
    pixmap.convertFromImage(image_tmp);
    scene.scene->clear();
    scene.scene->addPixmap(pixmap);
}

void CImage::addPixel(const tPoint &p, const QColor &color)
{
    double i = p.intensity;
    QColor fonColor = image.pixelColor(QPoint(p.x, p.y));
    QColor res;
    res.setGreen(color.green() * i + fonColor.green() * (1 - i));
    res.setRed(color.red() * i + fonColor.red() * (1 - i));
    res.setBlue(color.blue() * i + fonColor.blue() * (1 - i));

    image.setPixelColor(QPoint(p.x, p.y), res);

}

CImage::CImage() {}

CImage::CImage(tScene &scene, int sizepixel, const QColor &color)
{
    image = QImage((int)scene.scene->width() / sizepixel, (int)scene.scene->height() / sizepixel, QImage::Format_ARGB32);
    image.fill(color);
    fon = color;
    scale = sizepixel;
    printOnScene(scene);
}
CImage::CImage(tScene &scene, tPaintParam &param)
{
    fon = param.fon;
    scale = param.sizePixel;
    image = QImage((int)scene.scene->width() / scale, (int)scene.scene->height() / scale, QImage::Format_ARGB32);
    image.fill(fon);
    printOnScene(scene);
}

void CImage::changeFon(tScene &scene, const QColor &color)
{
    image.fill(color);
    fon = color;
    printOnScene(scene);
}

void CImage::changeScale(tScene &scene, int sizepixel)
{
    image = QImage((int)scene.scene->width() / sizepixel, (int)scene.scene->height() / sizepixel, QImage::Format_RGB888);
    image.fill(fon);
    scale = sizepixel;
    printOnScene(scene);
}

double CImage::algoBresenham(tScene &scene, const tDataEllipse &data)
{

}

double CImage::algoMidPoint(tScene &scene, const tDataEllipse &data)
{

}

double CImage::algoCanonEq(tScene &scene, const tDataEllipse &data)
{
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;
    int rx2 = data.rx * data.rx;
    int ry2 = data.ry * data.ry;
    int rdel2 = round(rx2 / sqrt(rx2 + ry2));

    double m = (double) data.ry / data.rx;

    int x = 0, y = 0;

    for(x = 0; x <= rdel2; x++) {
        y = round(sqrt(rx2 - x*x) * m);

        xr = x + cx;
        yr = y + cy;

        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);

    }

    rdel2 = round(ry2 / sqrt(rx2 + ry2));
    m = 1 / m;

    for(y = 0; y <= rdel2; y++) {
        x = round(sqrt(ry2 - y*y) * m);

        xr = x + cx;
        yr = y + cy;

        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);

    }
    printOnScene(scene);

}

double CImage::algoParamEq(tScene &scene, const tDataEllipse &data)
{

}

double CImage::algoStandart(tScene &scene, const tDataEllipse &data)
{
    QPainter painter(&image);
    painter.setPen(data.param.color);
    painter.drawEllipse(data.center, data.rx, data.ry);
    painter.end();
    printOnScene(scene);
}

double CImage::algoBresenham(tScene &scene, const tDataCircle &data)
{

}

double CImage::algoMidPoint(tScene &scene, const tDataCircle &data)
{

}

double CImage::algoCanonEq(tScene &scene, const tDataCircle &data)
{
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;
    int r2 = data.radius * data.radius;
    int rdel2 = round(data.radius / sqrt(2));
    int x = 0, y = 0;

    for(x = 0; x <= rdel2; x++) {
        y = round(sqrt(r2 - x*x));

        xr = x + cx;
        yr = y + cy;

        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);

    }

    for(y = 0; y <= rdel2; y++) {
        x = round(sqrt(r2 - y*y));

        xr = x + cx;
        yr = y + cy;

        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);

    }
    printOnScene(scene);
}

double CImage::algoParamEq(tScene &scene, const tDataCircle &data)
{
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;
    double r = data.radius;
    double dt = 1 / r;
    double tend = M_PI_2 + EPS;
    int x = 0, y = 0;


    for(double t = 0; t <= tend; t += dt) {
        y = round(r * sin(t));
        x = round(r * cos(t));

        xr = x + cx;
        yr = y + cy;

        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);

    }

    printOnScene(scene);
}

double CImage::algoStandart(tScene &scene, const tDataCircle &data)
{
    tDataEllipse new_data;
    new_data.param = data.param;
    new_data.center = data.center;
    new_data.rx = new_data.ry = data.radius;
    return algoStandart(scene, new_data);
}


double CImage::draw_Algorithm(tScene &scene, const tDataEllipse &data, ALGORITHM alg)
{
    switch(alg) {
    case STANDART:
        return algoStandart(scene, data);
    case BRESENHAM:
        return algoBresenham(scene, data);
    case MID_POINT:
        return algoMidPoint(scene, data);
    case CANON_EQ:
        return algoCanonEq(scene, data);
    case PARAM_EQ:
        return algoParamEq(scene, data);
    default:
        return -1;
    }
}

double CImage::draw_Algorithm(tScene &scene, const tDataCircle &data, ALGORITHM alg)
{
    switch(alg) {
    case STANDART:
        return algoStandart(scene, data);
    case BRESENHAM:
        return algoBresenham(scene, data);
    case MID_POINT:
        return algoMidPoint(scene, data);
    case CANON_EQ:
        return algoCanonEq(scene, data);
    case PARAM_EQ:
        return algoParamEq(scene, data);
    default:
        return -1;
    }
}
