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
    tick_t res_time = 0;
    tick_t t1 = tick();
    int cx = data.center.x();
    int cy = data.center.y();

    int rx2 = data.rx * data.rx;
    int ry2 = data.ry * data.ry;
    int r2y2 = 2 * ry2;
    int r2x2 = 2 * rx2;

    int xr;
    int yr;

    int x = 0, y = data.ry;

    int d = rx2 + ry2 - r2x2 * y;
    int d1, d2;

    while(y >= 0) {
        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

        if(d < 0) {
            d1 = 2 * d + r2x2 * y - 1;
            if(d1 > 0) {
                y -= 1;
                x += 1;
                d += r2y2 * x + ry2 + rx2 - r2x2 * y;
            }
            else {
                x += 1;
                d += r2y2 * x + ry2;
            }
        }
        else if(d == 0) {
            x += 1;
            y -= 1;
            d += r2y2 * x + ry2 + rx2 - r2x2 * y;
        }
        else {
            d2 = 2 * d - r2y2 * x - 1;
            if(d2 < 0) {
                y -= 1;
                x += 1;
                d += r2y2 * x + ry2 + rx2 - r2x2 * y;
            }
            else {
                y -= 1;
                d += rx2 - r2x2 * y;
            }
        }
    } //end while
    res_time += tick() - t1;

    printOnScene(scene);
    return res_time;

}

double CImage::algoMidPoint(tScene &scene, const tDataEllipse &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;
    int rx2 = data.rx * data.rx;
    int ry2 = data.ry * data.ry;
    int r2y2 = 2 * ry2;
    int r2x2 = 2 * rx2;
    int rdel2 = rx2 / sqrt(rx2 + ry2);

    int x = 0;
    int y = data.ry;

    int df = 0;
    int f = (ry2 - rx2 * y + 0.25 * rx2 + 0.5);

    int delta = -r2x2 * y;
    for(x = 0; x <= rdel2; x += 1) {
        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();
        if(f >= 0) {
            y -= 1;
            delta += r2x2;
            f += delta;
        }
        df += r2y2;;
        f  += df + ry2;
    }
    delta = r2y2 * x;
    f += -ry2 * (x + 0.75) - rx2 * (y - 0.75);
    df = -r2x2 * y;
    for(; y >= 0; y -= 1) {
        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

        if(f < 0) {
            x += 1;
            delta += r2y2;
            f += delta;
        }
        df += r2x2;
        f  += df + rx2;
    }
    res_time += tick() - t1;

    printOnScene(scene);
    return res_time;
}

double CImage::algoCanonEq(tScene &scene, const tDataEllipse &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
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
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

    }

    rdel2 = round(ry2 / sqrt(rx2 + ry2));
    m = 1 / m;

    for(y = 0; y <= rdel2; y++) {
        x = round(sqrt(ry2 - y*y) * m);

        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

    }
    res_time += tick() - t1;
    printOnScene(scene);
    return res_time;

}
//можно использовать и для алгоритма построения окружности...потом
double CImage::algoParamEq(tScene &scene, const tDataEllipse &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;
    double rx = data.rx;
    double ry = data.ry;
    double dt = 1 / max(rx, ry);
    double tend = M_PI_2 + EPS;
    int x = 0, y = 0;


    for(double t = 0; t <= tend; t += dt) {
        y = int(ry * sin(t) + 0.5);
        x = int(rx * cos(t) + 0.5);

        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();
    }

    res_time += tick() - t1;
    printOnScene(scene);
    return res_time;
}

double CImage::algoStandart(tScene &scene, const tDataEllipse &data)
{
    QPainter painter(&image);
    painter.setPen(data.param.color);
    tick_t res_time = 0;
    tick_t t1 = tick();
    painter.drawEllipse(data.center, data.rx, data.ry);
    res_time += tick() - t1;
    painter.end();
    printOnScene(scene);
    return res_time;
}

double CImage::algoBresenham(tScene &scene, const tDataCircle &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;

    int x = 0, y = data.radius;

    int d = 2 * (1 - data.radius);
    int d1, d2;

    while(y >= 0) {
        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

        if(d < 0) {
            d1 = 2 * d + 2 * y - 1;
            if(d1 > 0) {
                y -= 1;
                x += 1;
                d += 2 *(x - y + 1);
            }
            else {
                x += 1;
                d += 2 * x + 1;
            }
        }
        else if(d == 0) {
            x += 1;
            y -= 1;
            d += 2 *(x - y + 1);
        }
        else {
            d2 = 2 * d - 2 * x - 1;
            if(d2 < 0) {
                y -= 1;
                x += 1;
                d += 2 *(x - y + 1);
            }
            else {
                y -= 1;
                d += - 2 * y + 1;
            }
        }
    } //end while
    res_time += tick() - t1;

    printOnScene(scene);
    return res_time;
}

double CImage::algoMidPoint(tScene &scene, const tDataCircle &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
    int cx = data.center.x();
    int cy = data.center.y();

    int xr;
    int yr;
    int r2 = data.radius * data.radius;
    int r22 = 2 * r2;
    int rdel2 = round(data.radius / sqrt(2));

    int x = 0;
    int y = data.radius;

    int f = (r2 - r2 * y + 0.25 * r2 + 0.5);
    int df = 0;

    int delta = -r22 * y;

    while(x <= rdel2) {
        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

        x += 1;
        if(f >= 0) {
            y -= 1;
            delta += r22;
            f += delta;
        }
        df += r22;
        f  += df + r2;
    }
    delta = r22 * x;
    f += - r2 * (x + y);
    df = -r22 * y;
    while(y >= 0) {
        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();
        y -= 1;
        if(f < 0) {
            x += 1;
            delta += r22;
            f += delta;
        }
        df += r22;
        f  += df + r2;//
    }
    res_time += tick() - t1;

    printOnScene(scene);
    return res_time;
}

double CImage::algoCanonEq(tScene &scene, const tDataCircle &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
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
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

    }

    for(y = 0; y <= rdel2; y++) {
        x = round(sqrt(r2 - y*y));

        xr = x + cx;
        yr = y + cy;
        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();

    }
    res_time += tick() - t1;
    printOnScene(scene);
    return res_time;
}

double CImage::algoParamEq(tScene &scene, const tDataCircle &data)
{
    tick_t res_time = 0;
    tick_t t1 = tick();
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

        res_time += tick() - t1;
        addPixel(tPoint(xr, yr), data.param.color);
        addPixel(tPoint(cx - x, yr), data.param.color);
        addPixel(tPoint(xr, cy - y), data.param.color);
        addPixel(tPoint(cx - x, cy - y), data.param.color);
        t1 = tick();
    }
    res_time += tick() - t1;
    printOnScene(scene);
    return res_time;
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
