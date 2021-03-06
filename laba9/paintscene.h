#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <vector>
#include <math.h>
#include "base_algo.h"

enum SCENE_STATUS {
    ADD_POLYNOM_FIRST,  //добавление первой вершины прямоугольника
    ADD_POLYNOM_SECOND,    //добавление второй вершины
    ADD_FIRST,    // добавление отрезка
    ADD_SECOND,    // добавление отрезка
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
    void addMyPolynomForCut(tPoint &newPoint, const QColor &color);
    void addMyPolynomForCut(const QColor &color);
    void clearAll();
    void SaveState();
    void ActivatePreviousState();
    bool IsExist();

    vector<tPoint> polynom; // at the end push start vertex
    vector<tPoint> polyForCut;
    bool paintFlag;
    QColor colorLine = Qt::black;
    QColor colorPolynom = Qt::black;

    bool stepFlag;

    signals:
    void ChangeCursor(QCursor &c);

private:
    SCENE_STATUS status;
    tPoint     previousPoint;      // Координаты предыдущей точки
    tPoint     firstVertex;

    vector<tPoint> polyForCutSave;
    bool StateFlag = false;

    bool       polynomExist;
    bool       polyForCutExist;

    pair<tPoint, tPoint> nearSide;
    tPoint     nearVertex;
    bool       nearVFlag;

private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void findNearSide(tPoint &p);
    void findPointOnBorder(tPoint &p);
    void findNearVertex(tPoint &p);
    void drawCircle(QColor c);
};

#endif // PAINTSCENE_H
