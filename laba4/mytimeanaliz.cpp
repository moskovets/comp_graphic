#include "mytimeanaliz.h"
#include <QDebug>
#include <iostream>
MyTimeanaliz::MyTimeanaliz(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    this->customPlot = new QCustomPlot(this);
    layout->addWidget(this->customPlot);
    //Analiz();
}
double MyTimeanaliz::AnalizTime(tScene &scene, int R, ALGORITHM algo)
{
    int iter = 20;
    double sum_time = 0;
    tDataCircle data;
    data.radius = R;
    data.center = QPoint(1000, 1000);
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
    scene.scene = new QGraphicsScene(0, 0,2000, 2000);
    scene.x_center = 1000;
    scene.y_center = 1000;
    image = CImage(scene, 1, Qt::white);


    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
    QStringList lineNames;
    lineNames << "Брезенхем" << "Средняя точнка" << "Стандартный" << "Канонический"
              << "Параметрический";
    // add graphs with different line styles:
    ALGORITHM algo_arr[] = { BRESENHAM, MID_POINT, STANDART, CANON_EQ, PARAM_EQ };
    QColor colors[] = { Qt::black, Qt::red, Qt::green, Qt::blue, Qt::magenta };
    int N = 5;
    for(int i = 0; i < 5; i++) {
      customPlot->addGraph();
      pen.setColor(colors[i]);
      customPlot->graph()->setPen(pen);
      customPlot->graph()->setName(lineNames.at(i));
      customPlot->graph()->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsLine);
      customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
      // generate data:
      QVector<double> x(20), y(20);
      int R = 20;
      int dR = 20;
      for (int j=0; j < 20; ++j)
      {
        x[j] = R;
        y[j] = AnalizTime(scene, R, algo_arr[i]);
        R += dR;
      }
      customPlot->graph()->setData(x, y);
      customPlot->graph()->rescaleAxes(true);
    }
    // zoom out a bit:
    customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
    customPlot->xAxis->scaleRange(1.1, customPlot->xAxis->range().center());
    // set blank axis lines:
    customPlot->xAxis->setTicks(false);
    customPlot->yAxis->setTicks(true);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(true);
    // make top right axes clones of bottom left axes:
    customPlot->axisRect()->setupFullAxesBox();

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
