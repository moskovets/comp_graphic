#include "mytimeanaliz.h"
#include <QDebug>
#include <iostream>
MyTimeanaliz::MyTimeanaliz(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    this->diagram = new QCustomPlot(this);
    layout->addWidget(this->diagram);
    Analiz();
}
double MyTimeanaliz::AnalizTime(tScene &scene, int R, ALGORITHM algo)
{
    int iter = 20;
    double sum_time = 0;
    tDataCircle data;
    data.radius = R;
    data.center = QPoint(300, 300);
    data.param.color = Qt::black;


    for(int i = 0; i < iter; i++) {
        sum_time += image.draw_Algorithm(scene, data, algo);
    }
    sum_time /= iter;

    return sum_time;
}
void MyTimeanaliz::Analiz()
{
    tScene scene;
    scene.scene = new QGraphicsScene(0, 0,1000, 1000);
    scene.x_center = 500;
    scene.y_center = 500;
    image = CImage(scene, 1, Qt::white);


//    out.open("results.txt");
    qDebug() << "Анализ времени работы\n";
    qDebug() << "BRESENHAM";
    qDebug() << AnalizTime(scene, 100, BRESENHAM);
    qDebug() << "\nMID_POINT";
    qDebug() << AnalizTime(scene, 100, MID_POINT);
    qDebug() << "\nSTANDART";
    qDebug() << AnalizTime(scene, 100, STANDART);
    qDebug() << "\nCANON_EQ";
    qDebug() << AnalizTime(scene, 100, CANON_EQ);
    qDebug() << "\nPARAM_EQ";
    qDebug() << AnalizTime(scene, 100, PARAM_EQ);
}
