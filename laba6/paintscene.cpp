#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paintFlag = false;
    status = ADD_POLYNOM;
}

paintScene::~paintScene()
{

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
}

void paintScene::PaintPressEvent(QGraphicsSceneMouseEvent *event)
{
    previousPoint = QPoint(event->scenePos().x(), event->scenePos().y());
    paintFlag = true;

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
