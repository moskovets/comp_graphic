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
    void addMyLine(tPoint a, tPoint b, QColor color = Qt::black, int width = 1);
    void addMyLine(QPoint &a, QPoint &b, QColor color = Qt::black, int width = 1);
    void clearAll();
    void rotate(double tetax, double tetay, double tetaz);

    QColor colorLine = Qt::black;

    vector<pair<tPoint, tPoint>> graphic;
    bool flagGraphExist;

    signals:
    void ChangeCursor(QCursor &c);


private:
    QPointF previousPoint;
    SCENE_STATUS status;


private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PAINTSCENE_H
