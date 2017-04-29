#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1200, 700);
    ui->centralWidget->setLayout(ui->mainLayout);
    QHBoxLayout *layout = new QHBoxLayout;
    ui->draw->setLayout(layout);
    this->setWindowTitle("Лабораторная работа №6");

    //this->setMouseTracking(true);

    myPicture   = new Paint();
    layout->addWidget(myPicture);
    //centralWidget()->setMouseTracking(true);


    myController   = new MyController();
    layout->addWidget(myController);
    myController->setFixedWidth(276); //magic :)

    QObject::connect(myPicture, SIGNAL(SendScene(paintScene*)),
            this, SLOT(SendingScene(paintScene*)));

    QObject::connect(myPicture, SIGNAL(NewPixel()),
                     this, SLOT(ChangePixel()));

    myPicture->Connect();
}
void MainWindow::SendingScene(paintScene* my_scene)
{
    std::cout << "window:" << my_scene->sceneRect().height() << endl;
    myController->GetScene(my_scene);
}

void MainWindow::ChangePixel()
{
    //std::cout << "pixel:\n";
    myController->ShowPixel();
}

MainWindow::~MainWindow()
{
    delete myPicture;
    delete myController;
    delete ui;
}
