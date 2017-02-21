#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QHBoxLayout>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout *layout = new QHBoxLayout;
    ui->centralWidget->setLayout(layout);
    this->setWindowTitle("Лабораторная работа№1");
    /* Инициализируем виджет с графикой */
    myPicture   = new MyGraphicView();
    /* и добавляем его на слой  */
  //ui->gridLayout->addWidget(myPicture);
    layout->addWidget(myPicture);
    //ui->gridLayout_2->alignment(Qt::AlignLeft);
    //ui->gridLayout->expandingDirections();
  //ui->gridLayout->setAlignment(Qt::AlignLeft);
    myController   = new MyController();
  //ui->gridLayout_2->addWidget(myController);
    layout->addWidget(myController);
    myController->setFixedWidth(240);
  //ui->gridLayout_2->setAlignment(Qt::AlignRight);
    //connect(myController, SIGNAL(chang()), this, SLOT(quit()));
    QObject::connect(myController, SIGNAL(AnswerChange(Answer*)),
            this, SLOT(answerChange(Answer*)));
}
void MainWindow::answerChange(Answer *ans)
{
    //cout << "slot:" << ans->p1.x << endl;
    PaintAnswer(ans);
}

MainWindow::~MainWindow()
{
    delete ui;
    //не нужно ли удалять мой рисунок
}
void MainWindow::PaintAnswer(Answer *ans)
{
    //cout << "window:" << ans->p1.x << endl;

    myPicture->Paint(ans);
    //QErrorMessage errorMessage;
    //errorMessage.showMessage("OK");
    //errorMessage.exec();
}
