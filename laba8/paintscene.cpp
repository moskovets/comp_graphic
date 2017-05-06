#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paintFlag = false;
    status = ADD_POLYNOM_FIRST;
    polynomExist = false;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(paintFlag == false) {
        this->clear();
        this->repaintScene();
    }
    paintFlag = true;
    if(status == CHOOSE_SIDE || status == ADD_SEGMENT_PARAL_FIRST ||
       status == ADD_SEGMENT_PARAL_SECOND) {
        paralPress(event);
        return;
    }
    tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
    if(status == ADD_SEGMENT_FIRST) {
        previousPoint = firstVertex = newPoint;
        status = ADD_SEGMENT_SECOND;
    }
    else if(status == ADD_POLYNOM_FIRST) {
        if(polynomExist) {
            this->addMyPolynom(Qt::white);
            polynom.clear();
        }
        polynom.push_back(newPoint);
        previousPoint = firstVertex = newPoint;
        status = ADD_POLYNOM_SECOND;
        polynomExist = false;
    }
    else if(status == ADD_POLYNOM_SECOND) {
        this->addMyPolynom(previousPoint, Qt::white);
        if(event->button() == Qt::RightButton) {
            polynomExist = true;
            polynom.push_back(polynom[0]);
            this->addMyPolynom(colorPolynom);
            status = ADD_POLYNOM_FIRST;
        }
        else {
            this->addMyPolynom(newPoint, colorPolynom);
            polynom.push_back(newPoint);
            previousPoint = newPoint;
        }
        this->repaintScene();
    }
    else if(status == ADD_SEGMENT_SECOND) {
        if(event->modifiers() == Qt::ShiftModifier) {
            if(fabs(event->scenePos().x() - firstVertex.x) <=
               fabs(event->scenePos().y() - firstVertex.y))
            {
                newPoint.x = firstVertex.x;
            }
            else {
                newPoint.y = firstVertex.y;
            }
        }
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        segments.push_back(pair<tPoint, tPoint>(firstVertex, newPoint));
        this->repaintScene();
        status = ADD_SEGMENT_FIRST;
    }

}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(status == ADD_SEGMENT_PARAL_SECOND) {
        paralMove(event);
        return;
    }
    if(status == ADD_POLYNOM_SECOND) {
        tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
        this->addMyPolynom(previousPoint, Qt::white);
        this->addMyPolynom(newPoint, colorPolynom);
        previousPoint = newPoint;
        this->repaintScene();
    }
    else if(status == ADD_SEGMENT_SECOND) {
        tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
        if(event->modifiers() == Qt::ShiftModifier) {
            if(fabs(event->scenePos().x() - firstVertex.x) <=
               fabs(event->scenePos().y() - firstVertex.y))
            {
                newPoint.x = firstVertex.x;
            }
            else {
                newPoint.y = firstVertex.y;
            }
        }
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        this->repaintScene();
        previousPoint = newPoint;
    }
}

void paintScene::paralPress(QGraphicsSceneMouseEvent *event)
{
    tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
    if(status == CHOOSE_SIDE) {
        QCursor c = Qt::ClosedHandCursor;
        emit ChangeCursor(c);
        findNearSide(newPoint);
        status = ADD_SEGMENT_PARAL_FIRST;
        c = Qt::ArrowCursor;
        emit ChangeCursor(c);
    }
    else if(status == ADD_SEGMENT_PARAL_FIRST) {
        previousPoint = firstVertex = newPoint;
        status = ADD_SEGMENT_PARAL_SECOND;
    }
    else if(status == ADD_SEGMENT_PARAL_SECOND) {
        calculateParalPoint(firstVertex, newPoint);
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        segments.push_back(pair<tPoint, tPoint>(firstVertex, newPoint));
        this->repaintScene();
        status = CHOOSE_SIDE;
        QCursor c = Qt::OpenHandCursor;
        emit ChangeCursor(c);
    }
}

void paintScene::paralMove(QGraphicsSceneMouseEvent *event)
{
    if(status == ADD_SEGMENT_PARAL_SECOND) {
        tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
        calculateParalPoint(firstVertex, newPoint);
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        this->repaintScene();
        previousPoint = newPoint;
    }
}

void paintScene::findNearSide(tPoint &p)
{
    nearSide = pair<tPoint, tPoint>(polynom[0], polynom[1]);
}

void paintScene::calculateParalPoint(const tPoint &start, tPoint &p)
{

}

void paintScene::addMyLine(tPoint &a, tPoint &b, const QColor &color, int width)
{
    this->addLine(a.x, a.y, b.x, b.y, QPen(color, width, Qt::SolidLine));
}
void paintScene::addMyLine(QPoint &a, QPoint &b, const QColor &color, int width)
{
    this->addLine(a.x(), a.y(), b.x(), b.y(), QPen(color, width, Qt::SolidLine));
}

void paintScene::addMyPolynom(tPoint &p, const QColor &color)
{
    int n = polynom.size() - 1;
    this->addLine(polynom[0].x, polynom[0].y, p.x, p.y, QPen(color, 1, Qt::SolidLine));
    if(n > 0) {
        this->addLine(polynom[n].x, polynom[n].y, p.x, p.y, QPen(color, 1, Qt::SolidLine));
    }
}

void paintScene::addMyPolynom(const QColor &color)
{
    int n = polynom.size() - 1;
    for(int i = 0; i < n; i++) {
        this->addLine(polynom[i].x,     polynom[i].y,
                      polynom[i + 1].x, polynom[i + 1].y,
                      QPen(color, 1, Qt::SolidLine));
    }
}

void paintScene::clearAll()
{
    this->clear();
    paintFlag = false;
    polynomExist = false;
    polynom.clear();
    segments.clear();
}

bool paintScene::IsExist()
{
    return polynomExist;
}

void paintScene::repaintScene()
{
    for(size_t i = 0; i < segments.size(); i++) {
        this->addMyLine(segments[i].first, segments[i].second, colorLine);
    }

    //if(polynomExist) {
        this->addMyPolynom(colorPolynom);
    //}
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

void paintScene::addPoint(QPoint &nP)
{
    tPoint newPoint(nP);
    if(paintFlag == false) {
        this->clear();
        this->repaintScene();
    }
    paintFlag = true;
    if(status == ADD_SEGMENT_FIRST) {
        previousPoint = firstVertex = newPoint;
        status = ADD_SEGMENT_SECOND;
    }
    else if(status == ADD_POLYNOM_FIRST) {
        if(polynomExist) {
            this->addMyPolynom(Qt::white);
        }
        polynom.push_back(newPoint);
        previousPoint = firstVertex = newPoint;
        status = ADD_POLYNOM_SECOND;
        polynomExist = false;
    }
    else if(status == ADD_POLYNOM_SECOND) {
        this->addMyPolynom(previousPoint, Qt::white);
        this->addMyPolynom(newPoint, colorPolynom);
        polynom.push_back(newPoint);
        previousPoint = newPoint;
        this->repaintScene();
    }
    else if(status == ADD_SEGMENT_SECOND) {
        this->addMyLine(firstVertex, previousPoint, Qt::white);
        this->addMyLine(firstVertex, newPoint, colorLine);
        segments.push_back(pair<tPoint, tPoint>(firstVertex, newPoint));
        this->repaintScene();
        status = ADD_SEGMENT_FIRST;
    }
}

void paintScene::SetStatus(SCENE_STATUS st)
{
    status = st;
    if(st == CHOOSE_SIDE) {
        QCursor c = Qt::OpenHandCursor;
        emit ChangeCursor(c);
    }
}
