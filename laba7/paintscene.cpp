#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paintFlag = false;
    status = ADD_RECT_FIRST;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint newPoint = QPoint(event->scenePos().x(), event->scenePos().y());
    if(status == ADD_RECT_FIRST || status == ADD_SEGMENT_FIRST) {
        paintFlag = true;
        previousPoint = firstVertex = newPoint;
        if(status == ADD_RECT_FIRST)
            status = ADD_RECT_SECOND;
        if(status == ADD_SEGMENT_FIRST)
            status = ADD_SEGMENT_SECOND;
    }
    else if(status == ADD_RECT_SECOND) {
        paintFlag = false;
        this->addRect(QRect(firstVertex, previousPoint), QPen(Qt::white, 1, Qt::SolidLine));
        this->addRect(QRect(firstVertex, newPoint), QPen(colorRect, 1, Qt::SolidLine));
        polynom = QRectF(QRect(firstVertex, newPoint));
        this->repaintScene();
        status = ADD_RECT_FIRST;
    }
    else if(status == ADD_SEGMENT_SECOND) {
        paintFlag = false;
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        segments.push_back(pair<tPoint, tPoint>(firstVertex, newPoint));
        //TODO Shift
        status = ADD_SEGMENT_FIRST;
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //TODO
}
void paintScene::addMyLine(tPoint a, tPoint b, const QColor &color)
{
    this->addLine(a.x, a.y, b.x, b.y, QPen(color, 1, Qt::SolidLine));
}
void paintScene::addMyLine(QPoint a, QPoint b, const QColor &color)
{
    this->addLine(a.x(), a.y(), b.x(), b.y(), QPen(color, 1, Qt::SolidLine));
}

void paintScene::repaintScene()
{
    for(size_t i = 0; i < segments.size(); i++) {
        this->addMyLine(segments[i].first, segments[i].secont, colorLine);
    }
    if(rectExist) {
        this->addRect(polynom, QPen(colorRect, 1, Qt::SolidLine));
    }
}

void paintScene::sleepFeature(int time)
{
    QTime end = QTime::currentTime().addMSecs(time);
    while(QTime::currentTime() < end) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    //this->repaintPolynom();
    return;
}

void paintScene::addPoint(QPoint &newPoint)
{
    //TODO
}
