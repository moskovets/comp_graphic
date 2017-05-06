#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <math.h>


struct tPoint {
    int x;
    int y;
    tPoint(int x1, int y1) {
        x = x1;
        y = y1;
    }
    tPoint(QPoint a) {
        x = a.x();
        y = a.y();
    }
    tPoint() {}
};

enum SCENE_STATUS {
    ADD_RECT_FIRST,  //добавление первой вершины прямоугольника
    ADD_RECT_SECOND,    //добавление второй вершины
    ADD_SEGMENT_FIRST,    // добавление отрезка
    ADD_SEGMENT_SECOND,    // добавление отрезка
    NO_ACT        //бездействовать
};

using namespace std;
class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    void repaintScene();
    void sleepFeature(int time);
    void addPoint(QPoint &newPoint);
    void SetStatus(SCENE_STATUS st);
    void addMyLine(tPoint a, tPoint b, const QColor &color, int width = 1);
    void addMyLine(QPoint a, QPoint b, const QColor &color, int width = 1);
    void addMyRect(tPoint a, tPoint b, const QColor &color);
    void clearAll();
    bool IsExist();

    pair<tPoint, tPoint> polynom;
    vector<pair<tPoint,tPoint>> segments;
    bool paintFlag;
    QColor colorLine = Qt::black;
    QColor colorRect = Qt::black;

private:
    SCENE_STATUS status;
    tPoint     previousPoint;      // Координаты предыдущей точки
    tPoint     firstVertex;

    bool       rectExist;

private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


};

#endif // PAINTSCENE_H
