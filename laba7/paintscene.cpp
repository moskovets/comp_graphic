#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paintFlag = false;
    status = ADD_RECT_FIRST;
    rectExist = false;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint newPoint = QPoint(event->scenePos().x(), event->scenePos().y());
    if(status == ADD_RECT_FIRST || status == ADD_SEGMENT_FIRST) {
        if(rectExist) {
            this->addMyRect(polynom.first, polynom.second, Qt::white);
        }
        paintFlag = true;
        previousPoint = firstVertex = newPoint;
        if(status == ADD_RECT_FIRST)
            status = ADD_RECT_SECOND;
        if(status == ADD_SEGMENT_FIRST)
            status = ADD_SEGMENT_SECOND;
        rectExist = false;
    }
    else if(status == ADD_RECT_SECOND) {
        rectExist = true;
        paintFlag = false;
        this->addMyRect(firstVertex, previousPoint, Qt::white);
        this->addMyRect(firstVertex, newPoint, colorRect);
        polynom = pair<tPoint, tPoint>(firstVertex, newPoint);
        this->repaintScene();
        status = ADD_RECT_FIRST;
    }
    else if(status == ADD_SEGMENT_SECOND) {
        paintFlag = false;
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        segments.push_back(pair<tPoint, tPoint>(firstVertex, newPoint));
        //TODO Shift
        this->repaintScene();
        status = ADD_SEGMENT_FIRST;
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(status == ADD_RECT_SECOND) {
        QPoint newPoint = QPoint(event->scenePos().x(), event->scenePos().y());
        this->addMyRect(firstVertex, previousPoint, Qt::white);
        this->addMyRect(firstVertex, newPoint, colorRect);
        previousPoint = newPoint;
        this->repaintScene();
    }
    else if(status == ADD_SEGMENT_SECOND) {
        QPoint newPoint = QPoint(event->scenePos().x(), event->scenePos().y());
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        this->repaintScene();
        previousPoint = newPoint;
        //TODO Shift
    }
}

void paintScene::addMyLine(tPoint a, tPoint b, const QColor &color)
{
    this->addLine(a.x, a.y, b.x, b.y, QPen(color, 1, Qt::SolidLine));
}
void paintScene::addMyLine(QPoint a, QPoint b, const QColor &color)
{
    this->addLine(a.x(), a.y(), b.x(), b.y(), QPen(color, 1, Qt::SolidLine));
}
void paintScene::addMyRect(tPoint a, tPoint b, const QColor &color)
{
    this->addLine(a.x, a.y, a.x, b.y, QPen(color, 1, Qt::SolidLine));
    this->addLine(a.x, a.y, b.x, a.y, QPen(color, 1, Qt::SolidLine));
    this->addLine(a.x, b.y, b.x, b.y, QPen(color, 1, Qt::SolidLine));
    this->addLine(b.x, a.y, b.x, b.y, QPen(color, 1, Qt::SolidLine));
}

void paintScene::repaintScene()
{
    for(size_t i = 0; i < segments.size(); i++) {
        this->addMyLine(segments[i].first, segments[i].second, colorLine);
    }
    if(rectExist) {
        this->addMyRect(polynom.first, polynom.second, colorRect);
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
