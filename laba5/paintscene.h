#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <math.h>

using namespace std;
class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();

    vector<QPointF> polynom;
    bool paintFlag;
    QColor colorLine = Qt::black;

private:
    QPointF     previousPoint;      // Координаты предыдущей точки
    QPointF  previousVertex;
    int previousPolynom;

private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void repaintPolynom();


};

#endif // PAINTSCENE_H
