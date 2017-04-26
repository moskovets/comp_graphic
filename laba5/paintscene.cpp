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
            edges.push_back(pair<int,int>(polynom.size() - 1, polynom.size()));
            polynom.push_back(previousVertex);
        }
        if(event->button() == Qt::RightButton) {
            // Рисуем финальное ребро
            addLine(previousVertex.x(),
                    previousVertex.y(),
                    polynom[previousPolynom].x(),
                    polynom[previousPolynom].y(),
                    QPen(colorLine,1,Qt::SolidLine));
            edges.push_back(pair<int,int>(previousPolynom, polynom.size() - 1));
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
    for(unsigned int i = 0; i < edges.size(); i++) {
        addLine(polynom[edges[i].first].x(),
                polynom[edges[i].first].y(),
                polynom[edges[i].second].x(),
                polynom[edges[i].second].y(),
                QPen(colorLine,1,Qt::SolidLine));
    }
}
#include <QPainter>
#include <QTime>
#include <QCoreApplication>

void paintScene::sleepFeature(int time)
{
    QTime end = QTime::currentTime().addMSecs(time);
    while(QTime::currentTime() < end) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }


    //QList<QRectF> l;
    //l.append(this->sceneRect());
    //this->changed(l);
/*    QImage image(this->width(), this->height(), QImage::Format_ARGB32);
    image.fill(NULL);
    QPainter painter(&image);

    this->render(&painter);
    QPixmap pixmap;
    pixmap.convertFromImage(image);

    this->clear();

    this->addPixmap(pixmap);
*/
    return;
}

void paintScene::addPoint(QPoint &newPoint)
{
    if(paintFlag) {
        addLine(previousVertex.x(),
                previousVertex.y(),
                newPoint.x(),
                newPoint.y(),
                QPen(colorLine,1,Qt::SolidLine));
        previousVertex = newPoint;
        edges.push_back(pair<int,int>(polynom.size() - 1, polynom.size()));
        polynom.push_back(previousVertex);
    }
    else {
        paintFlag = true;
        previousVertex = newPoint;
        polynom.push_back(previousVertex);
        previousPolynom = polynom.size() - 1;
    }

}
