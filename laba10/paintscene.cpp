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
}

void paintScene::repaintScene()
{
    tPoint center(this->width() / 2, this->height() / 2);
    for(int i = 0; i < graphic.size(); i++) {
        addMyLine(tPoint(graphic[i].first, center), tPoint(graphic[i].second, center), colorLine, 1);
    }
}
