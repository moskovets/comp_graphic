#include "paintscene.h"
#include <QPainter>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    status = NO_ACT;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void paintScene::addMyLine(tPoint &a, tPoint &b, QColor color, int width)
{
    this->addLine(a.x, a.y, b.x, b.y, QPen(color, width, Qt::SolidLine));
}
void paintScene::addMyLine(QPoint &a, QPoint &b, QColor color, int width)
{
    this->addLine(a.x(), a.y(), b.x(), b.y(), QPen(color, width, Qt::SolidLine));
}


void paintScene::clearAll()
{
    this->clear();
}

void paintScene::repaintScene()
{
}
