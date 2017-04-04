#include <QPen>
#include "drawLine.h"
#include <QGraphicsItem>

const QPen penBlack(Qt::black); // Задаём чёрную кисть

/*int Draw_line(tScene &scene, const tLine &line)
{
    if(!scene.scene) {
        return SCENE_NOT_FOUND;
    }
    static int i = 0;

    static QImage image = QImage((int)scene.scene->width(), (int)scene.scene->height(), QImage::Format_RGB888);
    if(i == 0)
        image.fill(QColor(255, 255, 255));
    i++;
    for(int i = 0; i < line.line.size(); i++) {
        image.setPixelColor(QPoint(line.line[i].x, line.line[i].y), line.color);
    }
    image = image.scaled(QSize(image.width() * line.sizePixel,
                       image.height() * line.sizePixel));
    QPixmap pixmap;
    pixmap.convertFromImage(image);
    scene.scene->addPixmap(pixmap);
}*/

void tImage::printOnScene(tScene &scene)
{
    QImage image_tmp = image.scaled(QSize(image.width() * scale,
                       image.height() * scale));
    QPixmap pixmap;
    pixmap.convertFromImage(image_tmp);
    scene.scene->clear();
    scene.scene->addPixmap(pixmap);
}

void tImage::addPixel(const tPoint &p, const QColor &color)
{
    double i = p.intensity;
    QColor fonColor = image.pixelColor(QPoint(p.x, p.y));
    QColor res;
    res.setGreen(color.green() * i + fonColor.green() * (1 - i));
    res.setRed(color.red() * i + fonColor.red() * (1 - i));
    res.setBlue(color.blue() * i + fonColor.blue() * (1 - i));

    image.setPixelColor(QPoint(p.x, p.y), res);

}

tImage::tImage(tScene &scene, int sizepixel, const QColor &color)
{
    image = QImage((int)scene.scene->width() / sizepixel, (int)scene.scene->height() / sizepixel, QImage::Format_ARGB32);
    image.fill(color);
    fon = color;
    scale = sizepixel;
    printOnScene(scene);
}

void tImage::changeFon(tScene &scene, const QColor &color)
{
    image.fill(color);
    fon = color;
    printOnScene(scene);
}

void tImage::changeScale(tScene &scene, int sizepixel)
{
    image = QImage((int)scene.scene->width() / sizepixel, (int)scene.scene->height() / sizepixel, QImage::Format_RGB888);
    image.fill(fon);
    scale = sizepixel;
    printOnScene(scene);
}

void tImage::drawLine(tScene &scene, const tLine &line)
{
    for(int i = 0; i < line.line.size(); i++) {
        this->addPixel(line.line[i], line.color);
    }
    printOnScene(scene);
}
