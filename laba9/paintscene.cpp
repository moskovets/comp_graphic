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

    tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
    if(status == ADD_FIRST) {
        //previousPoint = firstVertex = newPoint;
        status = ADD_SECOND;
    }
    else if(status == ADD_SECOND) {
        //TODO
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
        int n = polynom.size() - 1;
        if(event->modifiers() == Qt::ShiftModifier) {
            if(fabs(event->scenePos().x() - polynom[n].x) <=
               fabs(event->scenePos().y() - polynom[n].y))
            {
                newPoint.x = polynom[n].x;
            }
            else {
                newPoint.y = polynom[n].y;
            }
        }
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
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(status == ADD_POLYNOM_SECOND) {
        tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
        int n = polynom.size() - 1;
        if(event->modifiers() == Qt::ShiftModifier) {
            if(fabs(event->scenePos().x() - polynom[n].x) <=
               fabs(event->scenePos().y() - polynom[n].y))
            {
                newPoint.x = polynom[n].x;
            }
            else {
                newPoint.y = polynom[n].y;
            }
        }
        this->addMyPolynom(previousPoint, Qt::white);
        this->addMyPolynom(newPoint, colorPolynom);
        previousPoint = newPoint;
        this->repaintScene();
    }
    else if(status == ADD_SECOND) {
        //TODO
    }
}

double distanse(tPoint &a, tPoint &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int findDistanse(tPoint &a, tPoint &b, tPoint &p) {
    double ab, bp, ap;
    ab = distanse(a, b);
    bp = distanse(p, b);
    ap = distanse(a, p);
    if(bp * bp + ab * ab < ap * ap) {
        return bp;
    }
    if(bp * bp > ab * ab + ap * ap) {
        return ap;
    }
    double d = fabs((b.y - a.y) * p.x - (b.x - a.x) * p.y + b.x * a.y - b.y * a.x) / ab;
    return d;
}

void paintScene::findNearSide(tPoint &p)
{
    int minside = 0;
    int mindistance = findDistanse(polynom[0], polynom[1], p);
    int n = polynom.size() - 1;
    for(int i = 0; i < n; i++) {
        int tmp = findDistanse(polynom[i], polynom[i+1], p);
        if(tmp < mindistance) {
            mindistance = tmp;
            minside = i;
        }
    }
    nearSide = pair<tPoint, tPoint>(polynom[minside], polynom[minside + 1]);
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
    //segments.clear();
}

bool paintScene::IsExist()
{
    return polynomExist;
}

void paintScene::repaintScene()
{
    this->addMyPolynom(colorPolynom);
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
    if(status == ADD_FIRST) {
        previousPoint = firstVertex = newPoint;
        //TODO
        status = ADD_SECOND;
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
    else if(status == ADD_SECOND) {
        //TODO
    }
}

void paintScene::SetStatus(SCENE_STATUS st)
{
    this->clear();
    if(status == ADD_POLYNOM_SECOND) {
        polynom.clear();
    }
    if(status == ADD_SECOND) {
        polyForCut.clear();
    }
    this->repaintScene();
    status = st;
}
