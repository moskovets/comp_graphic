#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <math.h>

enum SCENE_STATUS {
    ADD_POLYNOM,  //добавление многоугольника
    ADD_PAINT,    //обычное рисование
    ADD_PIXEL,    //добавление затравочного пиксля
    NO_ACT        //бездействовать
};

using namespace std;
class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    void Save();
    void ShowPreviousScene();
    void sleepFeature(int time);
    void addPoint(QPoint &newPoint);

    //vector<QPointF> polynom;
    //vector<pair<int,int>> edges;
    bool paintFlag;
    QColor colorLine = Qt::black;
    QPoint pixel;

    SCENE_STATUS status;

private:
    QPointF     previousPoint;      // Координаты предыдущей точки
    QPoint     previousVertex;
    QPoint     startVertex;
    //int         previousPolynom;
    QImage      image;

private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void PolynomPressEvent(QGraphicsSceneMouseEvent * event);
    void PixelPressEvent(QGraphicsSceneMouseEvent * event);
    void PaintPressEvent(QGraphicsSceneMouseEvent * event);

};

#endif // PAINTSCENE_H
