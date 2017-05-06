#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QCursor>
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
    ADD_POLYNOM_FIRST,  //добавление первой вершины прямоугольника
    ADD_POLYNOM_SECOND,    //добавление второй вершины
    ADD_SEGMENT_FIRST,    // добавление отрезка
    ADD_SEGMENT_SECOND,    // добавление отрезка
    CHOOSE_SIDE,
    ADD_SEGMENT_PARAL_FIRST,
    ADD_SEGMENT_PARAL_SECOND,
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
    void addMyLine(tPoint &a, tPoint &b, const QColor &color, int width = 1);
    void addMyLine(QPoint &a, QPoint &b, const QColor &color, int width = 1);
    void addMyPolynom(tPoint &newPoint, const QColor &color);
    void addMyPolynom(const QColor &color);
    void clearAll();
    bool IsExist();

    vector<tPoint> polynom; // at the end push start vertex
    vector<pair<tPoint,tPoint>> segments;
    bool paintFlag;
    QColor colorLine = Qt::black;
    QColor colorPolynom = Qt::black;

    signals:
    void ChangeCursor(QCursor &c);

private:
    SCENE_STATUS status;
    tPoint     previousPoint;      // Координаты предыдущей точки
    tPoint     firstVertex;

    bool       polynomExist;

    pair<tPoint, tPoint> nearSide;
private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void paralPress(QGraphicsSceneMouseEvent * event);
    void paralMove(QGraphicsSceneMouseEvent * event);

    void findNearSide(tPoint &p);
    void calculateParalPoint(const tPoint &start, tPoint &p);
};

#endif // PAINTSCENE_H
