#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "work.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>


#define POINT_SIZE 3

namespace Ui {
class MyGraphicView;
}

class MyGraphicView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyGraphicView(QWidget *parent = 0);
    ~MyGraphicView();
    void Paint(Answer *ans);

private slots:
    void ChangeScene();
private:
    void resizeEvent(QResizeEvent *event);

    Ui::MyGraphicView *ui;

    Answer* scaling(Answer *ans, double h, double w);

    QGraphicsScene      *scene;     // Объявляем сцену для отрисовки
    QGraphicsItemGroup  *group_1;   // Объявляем первую группу элементов
    QGraphicsItemGroup  *group_2;   // Объявляем вторую группу элементов
    QGraphicsItemGroup  *group_3;   // Объявляем вторую группу элементов

    void deleteItemsFromGroup(QGraphicsItemGroup *group_1);

    Answer *ans_data = NULL;
};

#endif // MYGRAPHICVIEW_H
