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
    //TODO
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //TODO
}

void paintScene::repaintPolynom()
{
    //TODO
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
