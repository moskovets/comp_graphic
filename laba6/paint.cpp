#include "paint.h"
#include "ui_paint.h"
#include <QDebug>
#define TIMER_TIME 100
Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    //this->setMouseTracking(true);
    scene = new paintScene();       // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    //ui->graphicsView->setMouseTracking(true);

    QObject::connect(scene, SIGNAL(NewPixel()),
                     this, SLOT(changeScenePixel()));

    timer = new QTimer();       // Инициализируем таймер
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(TIMER_TIME);          // Запускаем таймер


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

void Paint::changeScenePixel()
{
    emit NewPixel();
}

void Paint::resizeEvent(QResizeEvent *event)
{
    timer->start(TIMER_TIME);
    QWidget::resizeEvent(event);
}

void Paint::Connect()
{
    qDebug() << "paint: " << scene->sceneRect().height();
    emit SendScene(scene);
}
