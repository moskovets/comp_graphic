#include "mytimeanaliz.h"

MyTimeanaliz::MyTimeanaliz(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    this->diagram = new QCustomPlot(this);
    layout->addWidget(this->diagram);
    Analiz();
}

void MyTimeanaliz::Analiz()
{

}
