#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>

#define CIRCLE_R 10
#define CIRCLE_WIDTH 3

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paintFlag = false;
    status = ADD_POLYNOM_FIRST;

    polynomExist = polyForCutExist = nearVFlag = false;
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
        if(polyForCutExist) {
            this->addMyPolynomForCut(Qt::white);
            polyForCut.clear();
        }
        if(event->modifiers() == Qt::ControlModifier) {
            findPointOnBorder(newPoint);
        }
        polyForCut.push_back(newPoint);
        previousPoint = firstVertex = newPoint;
        status = ADD_SECOND;
        polyForCutExist = false;
    }
    else if(status == ADD_SECOND) {
        this->addMyPolynomForCut(previousPoint, Qt::white);
        int n = polyForCut.size() - 1;
        if(event->modifiers() == Qt::ShiftModifier) {
            if(fabs(event->scenePos().x() - polyForCut[n].x) <=
               fabs(event->scenePos().y() - polyForCut[n].y))
            {
                newPoint.x = polyForCut[n].x;
            }
            else {
                newPoint.y = polyForCut[n].y;
            }
        }
        if(event->modifiers() == Qt::ControlModifier) {
            findPointOnBorder(newPoint);
        }
        if(event->button() == Qt::RightButton) {
            polyForCutExist = true;
            polyForCut.push_back(polyForCut[0]);
            this->addMyPolynomForCut(colorLine);
            status = ADD_FIRST;
        }
        else {
            this->addMyPolynomForCut(newPoint, colorLine);
            polyForCut.push_back(newPoint);
            previousPoint = newPoint;
        }
        this->repaintScene();
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
    else {
        if(status == ADD_FIRST || status == ADD_SECOND) {
            tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
            if(event->modifiers() == Qt::ControlModifier) {
                findNearVertex(newPoint);
            }
            else {
                if(nearVFlag) {
                    drawCircle(QColor(Qt::white));
                    nearVFlag = false;
                }
            }
        }
        if(status == ADD_SECOND) {
            tPoint newPoint = tPoint(event->scenePos().x(), event->scenePos().y());
            int n = polyForCut.size() - 1;
            if(event->modifiers() == Qt::ShiftModifier) {
                if(fabs(event->scenePos().x() - polyForCut[n].x) <=
                        fabs(event->scenePos().y() - polyForCut[n].y))
                {
                    newPoint.x = polyForCut[n].x;
                }
                else {
                    newPoint.y = polyForCut[n].y;
                }
            }
            if(event->modifiers() == Qt::ControlModifier) {
                findPointOnBorder(newPoint);
            }
            this->addMyPolynomForCut(previousPoint, Qt::white);
            this->addMyPolynomForCut(newPoint, colorLine);
            previousPoint = newPoint;
            this->repaintScene();
        }
    }

}

double distanse(tPoint &a, tPoint &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int findDistanse(tPoint &a, tPoint &b, tPoint &p) {
    double ab;
    ab = distanse(a, b);
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

void paintScene::findPointOnBorder(tPoint &p)
{
    if(nearVFlag) {
        p = nearVertex;
        return;
    }
    findNearSide(p);
    tPoint s = nearSide.first;
    if(nearSide.first.x == nearSide.second.x) {
        p.x = s.x;
        return;
    }
    double k = (nearSide.first.y - nearSide.second.y) / (double)
               (nearSide.first.x - nearSide.second.x);
    if(k < 1) {
        p.y = round((p.x - s.x) * k) + s.y;
    }
    else {
        p.x = round((p.y - s.y) / k) + s.x;
    }
}

void paintScene::findNearVertex(tPoint &p)
{
    int tmpVertex = 0;
    int mindistance = distanse(polynom[0], p);
    int n = polynom.size() - 1;
    for(int i = 0; i < n; i++) {
        int tmp = distanse(polynom[i], p);
        if(tmp < mindistance) {
            mindistance = tmp;
            tmpVertex = i;
        }
    }
    if(mindistance > CIRCLE_R) {
        if(nearVFlag) {
            drawCircle(QColor(Qt::white));
            nearVFlag = false;
        }
    }
    else {
        nearVertex = polynom[tmpVertex];
        nearVFlag = true;
        drawCircle(Qt::black);
    }

}

void paintScene::drawCircle(QColor c)
{
   this->addEllipse(nearVertex.x - CIRCLE_R / 2, nearVertex.y - CIRCLE_R / 2,
                    CIRCLE_R, CIRCLE_R,
                    QPen(c, CIRCLE_WIDTH, Qt::SolidLine));
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

void paintScene::addMyPolynomForCut(tPoint &p, const QColor &color)
{
    int n = polyForCut.size() - 1;
    this->addLine(polyForCut[0].x, polyForCut[0].y, p.x, p.y, QPen(color, 1, Qt::SolidLine));
    if(n > 0) {
        this->addLine(polyForCut[n].x, polyForCut[n].y, p.x, p.y, QPen(color, 1, Qt::SolidLine));
    }
}

void paintScene::addMyPolynomForCut(const QColor &color)
{
    if(StateFlag) {
        int n = polyForCutSave.size() - 1;
        for(int i = 0; i < n; i++) {
            this->addLine(polyForCutSave[i].x,     polyForCutSave[i].y,
                          polyForCutSave[i + 1].x, polyForCutSave[i + 1].y,
                          QPen(color, 1, Qt::SolidLine));
        }
    }
    else {
        int n = polyForCut.size() - 1;
        for(int i = 0; i < n; i++) {
            this->addLine(polyForCut[i].x,     polyForCut[i].y,
                          polyForCut[i + 1].x, polyForCut[i + 1].y,
                          QPen(color, 1, Qt::SolidLine));
        }
    }
}

void paintScene::clearAll()
{
    this->clear();
    paintFlag = false;
    polynomExist = false;
    polynom.clear();
    polyForCut.clear();
}

void paintScene::SaveState()
{
    polyForCutSave = polyForCut;
    StateFlag = true;
}

void paintScene::ActivatePreviousState()
{
    polyForCut = polyForCutSave;
    StateFlag = false;
}

bool paintScene::IsExist()
{
    return polynomExist;
}

void paintScene::repaintScene()
{
    //this->clear();
    if(nearVFlag) {
        drawCircle(Qt::black);
    }
    this->addMyPolynom(colorPolynom);
    this->addMyPolynomForCut(colorLine);
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
        if(polyForCutExist) {
            this->addMyPolynomForCut(Qt::white);
            polyForCut.clear();
        }
        polyForCut.push_back(newPoint);
        previousPoint = firstVertex = newPoint;
        polyForCutExist = false;
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
        this->addMyPolynomForCut(previousPoint, Qt::white);
        this->addMyPolynomForCut(newPoint, colorLine);
        polyForCut.push_back(newPoint);
        previousPoint = newPoint;
        this->repaintScene();
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
