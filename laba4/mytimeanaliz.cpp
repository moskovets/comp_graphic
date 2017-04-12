#include "mytimeanaliz.h"
#include <QDebug>
#include <iostream>
MyTimeanaliz::MyTimeanaliz(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);
    this->customPlot = new QCustomPlot(this);
    layout->addWidget(this->customPlot);
    QRadioButton *circle = new QRadioButton("Окружность");
    circle->setChecked(true);
    QRadioButton *ellipse = new QRadioButton("Эллипс");
    calculateVectors();
    QObject::connect(ellipse, SIGNAL(clicked(bool)),
            this, SLOT(MySlotEl()));
    QObject::connect(circle, SIGNAL(clicked(bool)),
            this, SLOT(MySlotC()));

    //ellipse->setChecked(false);
    layout->addWidget(circle);
    layout->addWidget(ellipse);

    Analiz(rcircle, timecircle);
}

void MyTimeanaliz::calculateVectors()
{
    tScene scene;
    scene.scene = new QGraphicsScene(0, 0,2000, 2000);
    scene.x_center = 1000;
    scene.y_center = 1000;
    image = CImage(scene, 1, Qt::white);

    for(int i = 0; i < 5; i++) {
        int R = 20;
        int Ry = 10;
        int dR = 20;
        int dRy = 10;
        for (int j = 0; j < 20; ++j)
        {
          rcircle[i][j] = R;
          timecircle[i][j] = AnalizTimeCircle(scene, R, algo_arr[i]);
          rellipse[i][j] = R;
          timeellipse[i][j] = AnalizTimeEllipse(scene, R, Ry, algo_arr[i]);
          //qDebug() << timecircle[i][j] << " " << timeellipse[i][j];
          Ry += dRy;
          R += dR;
        }
    }
    qDebug() << "Анализ времени работы\n";
    qDebug() << "BRESENHAM";
    qDebug() << AnalizTimeCircle(scene, 100, BRESENHAM);
    qDebug() << "\nMID_POINT";
    qDebug() << AnalizTimeCircle(scene, 100, MID_POINT);
    qDebug() << "\nSTANDART";
    qDebug() << AnalizTimeCircle(scene, 100, STANDART);
    qDebug() << "\nCANON_EQ";
    qDebug() << AnalizTimeCircle(scene, 100, CANON_EQ);
    qDebug() << "\nPARAM_EQ";
    qDebug() << AnalizTimeCircle(scene, 100, PARAM_EQ);

}

double MyTimeanaliz::AnalizTimeCircle(tScene &scene, int R, ALGORITHM algo)
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
double MyTimeanaliz::AnalizTimeEllipse(tScene &scene, int Rx, int Ry, ALGORITHM algo)
{
    int iter = 20;
    double sum_time = 0;
    tDataEllipse data;
    data.rx = Rx;
    data.ry = Ry;
    data.center = QPoint(1000, 1000);
    data.param.color = Qt::black;

    for(int i = 0; i < iter; i++) {
        sum_time += image.draw_Algorithm(scene, data, algo);
    }
    sum_time /= iter;

    return sum_time;
}
void MyTimeanaliz::Analiz(double r[][N], double time[][N])
{
/*    tScene scene;
    scene.scene = new QGraphicsScene(0, 0,2000, 2000);
    scene.x_center = 1000;
    scene.y_center = 1000;
    image = CImage(scene, 1, Qt::white);
*/
   // this->customPlot = new QCustomPlot(this);
    customPlot->xAxis->setRange(0, 1);
    customPlot->yAxis->setRange(0, 1);

    customPlot->clearGraphs();

    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
    QStringList lineNames;
    lineNames << "Брезенхем" << "Средняя точнка" << "Стандартный" << "Канонический"
              << "Параметрический";
    // add graphs with different line styles:
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
      //int R = 20;
      //int dR = 20;
      for (int j=0; j < 20; ++j)
      {
        //x[j] = R;
        //y[j] = AnalizTime(scene, R, algo_arr[i]);
          x[j] = r[i][j];
          y[j] = time[i][j];

      //  R += dR;
      }
      customPlot->graph()->setData(x, y);
      //qDebug() << y;
      customPlot->graph()->rescaleAxes(true);
    }
    // zoom out a bit:
    customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
    customPlot->xAxis->scaleRange(1.1, customPlot->xAxis->range().center());
    // set blank axis lines:
    customPlot->xAxis->setTicks(true);
    customPlot->yAxis->setTicks(true);
    customPlot->xAxis->setTickLabels(true);
    customPlot->yAxis->setTickLabels(true);
    // make top right axes clones of bottom left axes:
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->replot();

//    out.open("results.txt");
}
