#include "paint.h"
#include "ui_paint.h"
#include <QDebug>
Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    this->setMouseTracking(true);
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //ui->graphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scene = new paintScene();       // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    ui->graphicsView->setMouseTracking(true);
    //scene->setSceneRect(0,0, this->width() - 20, this->height() - 20);

    //qDebug() << "paint_constr: " << scene->height();
    //qDebug() << "paint_constr: " << this->height();


    timer = new QTimer();       // Инициализируем таймер
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(100);          // Запускаем таймер
}

Paint::~Paint()
{
    delete ui;
}

void Paint::slotTimer()
{
    /* Переопределяем размеры графической сцены в зависимости
     * от размеров окна
     * */
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void Paint::resizeEvent(QResizeEvent *event)
{
    //qDebug() << "here" << this->height() << " " << scene->height();
    timer->start(100);
    QWidget::resizeEvent(event);
}

void Paint::Connect()
{
    qDebug() << "paint: " << scene->sceneRect().height();
    emit SendScene(scene);
}
