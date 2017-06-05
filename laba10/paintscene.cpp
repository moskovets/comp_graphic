#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>
#include "point_transf.h"
#define SPEED 0.3
#define TO_RAD (M_PI/180)
paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    status = NO_ACT;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QCursor c = Qt::ClosedHandCursor;
    emit ChangeCursor(c);
    previousPoint = event->scenePos();
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int dx = event->scenePos().x() - previousPoint.x();
    int dy = event->scenePos().y() - previousPoint.y();

    this->clear();
    this->rotate(dy*SPEED, -dx*SPEED, 0);
    this->repaintScene();
    previousPoint = event->scenePos();
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QCursor c = Qt::ArrowCursor;
    emit ChangeCursor(c);
}

void paintScene::addMyLine(tPoint a, tPoint b, QColor color, int width)
{
    this->addLine(round(a.x), round(a.y), round(b.x), round(b.y), QPen(color, width, Qt::SolidLine));
}
void paintScene::addMyLine(QPoint &a, QPoint &b, QColor color, int width)
{
    this->addLine(a.x(), a.y(), b.x(), b.y(), QPen(color, width, Qt::SolidLine));
}


void paintScene::clearAll()
{
    this->clear();
    graphic.clear();
    flagGraphExist = false;
}

void paintScene::rotate(double tetax, double tetay, double tetaz)
{
    tetax *= TO_RAD;
    tetay *= TO_RAD;
    tetaz *= TO_RAD;

    Rotate rot { tetax, tetay, tetaz };
    RotateModel(graphic, rot);
}

void paintScene::repaintScene()
{
    tPoint center(this->width() / 2, this->height() / 2);
    for(int i = 0; i < graphic.size(); i++) {
        addMyLine(tPoint(graphic[i].first, center), tPoint(graphic[i].second, center), colorLine, 1);
    }
}
