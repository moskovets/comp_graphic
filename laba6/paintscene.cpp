#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QWidget>
#include <QKeyEvent>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paintFlag = false;
    status = ADD_POLYNOM;
}

paintScene::~paintScene()
{

}

void paintScene::Save()
{
    image = QImage(this->width(), this->height(), QImage::Format_ARGB32);
    image.fill(NULL);
    QPainter painter(&image);
    this->render(&painter);
    currentImage = QImage(image);
}

void paintScene::ShowPreviousScene()
{
    QPixmap pixmap;
    pixmap.convertFromImage(image);
    this->clear();
    this->addPixmap(pixmap);
    paintFlag = false;
}


void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(status) {
    case ADD_POLYNOM:
        PolynomPressEvent(event);
        break;
    case ADD_PAINT:
        PaintPressEvent(event);
        break;
    case ADD_PIXEL:
        PixelPressEvent(event);
        break;
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(status == ADD_PAINT) {
        QPoint newPoint(event->scenePos().x(), event->scenePos().y());
        if(paintFlag) {
            addLine(previousPoint.x(),
                    previousPoint.y(),
                    newPoint.x(),
                    newPoint.y(),
                    QPen(colorLine,1,Qt::SolidLine));
            previousPoint = newPoint;
        }
        else {
            previousPoint = newPoint;
            paintFlag = true;
        }
    }
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(status == ADD_PAINT) {
        paintFlag = false;
    }
}

void paintScene::keyPressEvent(QKeyEvent *event)
{
    if(status == ADD_POLYNOM) {
        if(event->key() == Qt::Key_Escape) {
            paintFlag = false;
        }
    }
}

void paintScene::PolynomPressEvent(QGraphicsSceneMouseEvent *event)
{
    if(paintFlag) {
        if(event->button() == Qt::LeftButton) {
            QPoint newPoint(event->scenePos().x(), event->scenePos().y());
            if(event->modifiers() == Qt::ShiftModifier) {
                if(fabs(event->scenePos().x() - previousVertex.x()) <=
                   fabs(event->scenePos().y() - previousVertex.y())) {
                    newPoint.setX(previousVertex.x());
                }
                else {
                    newPoint.setY(previousVertex.y());
                }
            }
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    newPoint.x(),
                    newPoint.y(),
                    QPen(colorLine,1,Qt::SolidLine));
            previousVertex = newPoint;
        }
        if(event->button() == Qt::RightButton) {
            // Рисуем финальное ребро
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    startVertex.x(),
                    startVertex.y(),
                    QPen(colorLine,1,Qt::SolidLine));
            //previousVertex = QPoint(event->scenePos().x(), event->scenePos().y());
            paintFlag = false;
            qDebug();
        }
    }
    else {
        if(event->button() == Qt::LeftButton) {
            paintFlag = true;
            previousVertex = QPoint(event->scenePos().x(), event->scenePos().y());
            startVertex = previousVertex;
        }
    }

}

void paintScene::PixelPressEvent(QGraphicsSceneMouseEvent *event)
{
    pixel = QPoint(event->scenePos().x(), event->scenePos().y());
    qDebug() << pixel;
    emit NewPixel();
}

void paintScene::PaintPressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
            // Рисуем финальное ребро
            addLine(previousPoint.x(),
                    previousPoint.y(),
                    startVertex.x(),
                    startVertex.y(),
                    QPen(colorLine,1,Qt::SolidLine));
            //previousVertex = QPoint(event->scenePos().x(), event->scenePos().y());
            paintFlag = false;
    }
    else {
        previousPoint = QPoint(event->scenePos().x(), event->scenePos().y());
        startVertex = previousPoint;
        paintFlag = true;
    }

}

void paintScene::sleepFeature(int time)
{
    QTime end = QTime::currentTime().addMSecs(time);
    while(QTime::currentTime() < end) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    return;
}

void paintScene::addPoint(QPoint &newPoint)
{
    if(paintFlag) {
        addLine(previousVertex.x(),
                previousVertex.y(),
                newPoint.x(),
                newPoint.y(),
                QPen(colorLine,1,Qt::SolidLine));
        previousVertex = newPoint;
    }
    else {
        paintFlag = true;
        previousVertex = newPoint;
        startVertex = newPoint;
    }
}
QColor paintScene::getPixelColor(const QPoint &p)
{
    return currentImage.pixelColor(p);
}
QColor paintScene::getPixelColor(const tPoint &p)
{
    if(p.x <= 0 || p.x >= image.width() || p.y <= 0 || p.y >= image.height())
    {
        return colorLine;
    }
    return currentImage.pixelColor(p.x, p.y);
}
void paintScene::ChangeStatus(SCENE_STATUS st)
{
    status = st;
    if(st == NO_ACT) {
        this->Save();
    }
}

void paintScene::addPixelImage(const QPoint &p, const QColor &color)
{
    currentImage.setPixelColor(p, color);
    printCurrentImage();
}

void paintScene::addPixelImage(const tPoint &p, const QColor &color)
{
    currentImage.setPixelColor(p.x, p.y, color);
    if(cury != p.y) {
        printCurrentImage();
        cury = p.y;
        sleepFeature(timePause);
    }
}
void paintScene::addLineImage(const QPoint &p1, const QPoint &p2, const QColor &color)
{
    QPainter painter(&currentImage);
    painter.setPen(color);
    painter.drawLine(p1, p2);
    printCurrentImage();
}

void paintScene::printCurrentImage()
{
    QPixmap pixmap;
    pixmap.convertFromImage(currentImage);
    this->clear();
    this->addPixmap(pixmap);
}

void paintScene::clearAll()
{
    clear();
    Save();
    paintFlag = false;
}
