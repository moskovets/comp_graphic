#include "paintscene.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    //this->setMouseTracking(true);
    paintFlag = false;
    previousPolynom = 0;

}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(paintFlag) {
        if(event->button() == Qt::LeftButton) {
            // Рисуем финальное ребро
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    event->scenePos().x(),
                    event->scenePos().y(),
                    QPen(Qt::black,1,Qt::SolidLine));
            previousVertex = event->scenePos();
            polynom.push_back(previousVertex);
        }
        if(event->button() == Qt::RightButton) {
            // Рисуем финальное ребро
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    polynom[previousPolynom].x(),
                    polynom[previousPolynom].y(),
                    QPen(Qt::black,1,Qt::SolidLine));
            previousVertex = event->scenePos();
            paintFlag = false;
        }
    }
    else {
        if(event->button() == Qt::LeftButton) {
            paintFlag = true;
            previousVertex = event->scenePos();
            polynom.push_back(previousVertex);
            previousPolynom = polynom.size() - 1;
        }
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void paintScene::repaintPolynom()
{
    for(int i = 0; i < polynom.size() - 1; i++) {
        addLine(polynom[i].x(),
                polynom[i].y(),
                polynom[i + 1].x(),
                polynom[i + 1].y(),
                QPen(Qt::black,1,Qt::SolidLine));
    }
}
