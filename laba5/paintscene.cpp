#include "paintscene.h"
#include <QDebug>
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
    //qDebug() << this->height();
    if(paintFlag) {
        if(event->button() == Qt::LeftButton) {
            QPointF newPoint(event->scenePos().x(), event->scenePos().y());
            if(event->modifiers() == Qt::ShiftModifier) {
                if(fabs(event->scenePos().x() - previousVertex.x()) <=
                   fabs(event->scenePos().y() - previousVertex.y())) {
                    newPoint.setX(previousVertex.x());
                }
                else {
                    newPoint.setY(previousVertex.y());
                }
            }
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    newPoint.x(),
                    newPoint.y(),
                    QPen(colorLine,1,Qt::SolidLine));
            previousVertex = newPoint;
            polynom.push_back(previousVertex);
        }
        if(event->button() == Qt::RightButton) {
            // Рисуем финальное ребро
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    polynom[previousPolynom].x(),
                    polynom[previousPolynom].y(),
                    QPen(colorLine,1,Qt::SolidLine));
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
    for(unsigned int i = 0; i < polynom.size() - 1; i++) {
        addLine(polynom[i].x(),
                polynom[i].y(),
                polynom[i + 1].x(),
                polynom[i + 1].y(),
                QPen(colorLine,1,Qt::SolidLine));
    }
}
