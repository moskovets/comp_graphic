#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1200, 700);
    QHBoxLayout *layout = new QHBoxLayout;
    ui->centralWidget->setLayout(layout);
    this->setWindowTitle("Лабораторная работа №4");

    myPicture   = new MyGraphicView();
    layout->addWidget(myPicture);

    myController   = new MyController();
    layout->addWidget(myController);
    myController->setFixedWidth(347); //magic :)

    QObject::connect(myPicture, SIGNAL(SendScene(tScene*)),
            this, SLOT(SendingScene(tScene*)));
    myPicture->Connect();
}
void MainWindow::SendingScene(tScene* my_scene)
{
    std::cout << "window:" << my_scene->x_center << endl;
    myController->GetScene(my_scene);
}

MainWindow::~MainWindow()
{
    delete myPicture;
    delete myController;
    delete ui;
}
