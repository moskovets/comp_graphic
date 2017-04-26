#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1200, 700);
    //QHBoxLayout *mainLayout = new QHBoxLayout;
    ui->centralWidget->setLayout(ui->mainLayout);
    //QTabWidget *tab = new QTabWidget();
    //mainLayout->addWidget(tab);
    QHBoxLayout *layout = new QHBoxLayout;
    //tab->addTab(layout, "draw");
    ui->draw->setLayout(layout);
    this->setWindowTitle("Лабораторная работа №5");

    this->setMouseTracking(true);

    myPicture   = new Paint();
    layout->addWidget(myPicture);
    centralWidget()->setMouseTracking(true);


    myController   = new MyController();
    layout->addWidget(myController);
    myController->setFixedWidth(276); //magic :)

    //qDebug() << "main_constr:" << myPicture->height();

    QObject::connect(myPicture, SIGNAL(SendScene(paintScene*)),
            this, SLOT(SendingScene(paintScene*)));

    myPicture->Connect();

    /*QHBoxLayout *layout2 = new QHBoxLayout;
    ui->analiz->setLayout(layout2);
    myTimeanaliz = new MyTimeanaliz;
    layout2->addWidget(myTimeanaliz);*/
}
void MainWindow::SendingScene(paintScene* my_scene)
{
    std::cout << "window:" << my_scene->sceneRect().height() << endl;
    myController->GetScene(my_scene);
}

MainWindow::~MainWindow()
{
    delete myPicture;
    delete myController;
//    delete myTimeanaliz;
    delete ui;
}
