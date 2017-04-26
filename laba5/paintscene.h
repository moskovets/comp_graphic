#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <math.h>

struct tVertex {
    QPointF p;
    int next = -1;
    int prev = -1;
    tVertex(const QPointF &a) {
        p = a;
    }
    tVertex(const QPointF &a, int pr, int n = -1) {
        p = a;
        next = n;
        prev = pr;
    }

};


using namespace std;
class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    void repaintPolynom();
    void sleepFeature(int time);
    void addPoint(QPoint &newPoint);

    vector<tVertex> polynom;
    vector<pair<int,int>> edges;
    bool paintFlag;
    QColor colorLine = Qt::black;

private:
    QPointF     previousPoint;      // Координаты предыдущей точки
    QPointF     previousVertex;
    int         previousPolynom;

private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


};

#endif // PAINTSCENE_H
