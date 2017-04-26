#include "mycontroller.h"
#include "ui_mycontroller.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <iostream>
#include <string.h>
#include <QColorDialog>
#include <QDebug>
//#include "my_scene.h"
#include "algo.h"
#define MIN_PAR    10
#define LEN        300
#define MAX_TIME   100

Text_Error LineEditError;

MyController::MyController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyController)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    par = parent;
    ui->setupUi(this);
//    Validator = new QRegExpValidator(QRegExp("^[+-]?[0-9]{0,5}(\\.|,|$)[0-9]{0,4}$"));
    Validator = new QIntValidator(0, 1000);
    ui->xEdit->setValidator(Validator);
    ui->yEdit->setValidator(Validator);
    ui->timeEdit->setValidator(Validator);
    QColor color = Qt::green;
    ui->fonlabel->setStyleSheet("background-color: " + GetColor(color));
    colorBrush = color;
    timePause = 0;

}
void MyController::GetScene(paintScene *scene1)
{
    this->scene = scene1;
 }

MyController::~MyController()
{
    delete Validator;
    delete ui;
}

//получение данных с выбранных полей
double * MyController::GetData(vector <QLineEdit*> &vec)
{
    QString str;
    double *data = new double[vec.size()];
    double x;
    QString mess;
    for(unsigned int i = 0; i < vec.size(); i++) {
        str = vec[i]->text();
        x = Analiz_Text(str);
        switch(LineEditError) {
        case EMPTY:
            mess = "Область текста пуста.\n Введите данные!";
            break;
        case E_SYMBOL:
            mess = "Ошибочный символ\n "
                   "Разрешается использование только цифр, '.'',' и знаков табуляции";
            break;
        }
        if(LineEditError != NO_ER) {
            QErrorMessage errorMessage;
            errorMessage.showMessage(mess);
            errorMessage.exec();
            break;
        }
        data[i] = x;
    }
   return data;
}
//анализ строки данных (проверка, явл-тся ли вещественным числом)
double Analiz_Text(QString str)
{
    LineEditError = NO_ER;
    if(str == "") {
        LineEditError = EMPTY;
        return 0;
    }
    str.replace(QString(","), QString("."));
    //str.replace(QString("\n"), QString(" "));

    QStringList list = str.split(' ', QString::SkipEmptyParts);
    if(list.size() > 1) {
        LineEditError = E_SYMBOL;
        return 0;
    }

    double x;
    bool ok = true;
    for(int i = 0; i < list.size(); i++) {
        x = list.at(i).toDouble(&ok);
        if(!ok) {
            LineEditError = E_SYMBOL;
            return 0;
        }
    }
    return x;
}


QString MyController::GetColor(QColor &color)
{
    QString r = QString::number(color.red());
    QString g = QString::number(color.green());
    QString b = QString::number(color.blue());
    QString res = "rgb(" + r + ", " + g + ", " + b + ")";
    return res;
}

void MyController::on_foncolorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (!color.isValid() ) {
     return;
    }
    ui->fonlabel->setStyleSheet("background-color: " + GetColor(color));
    colorBrush = color;
}

void MyController::on_linecolorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (!color.isValid() ) {
     return;
    }
    ui->linelabel->setStyleSheet("background-color: " + GetColor(color));
    scene->colorLine = color;
}

void MyController::on_clearButton_clicked()
{
    qDebug() << "clear:" << scene->height();
    scene->clear();
    scene->repaintPolynom();
    scene->paintFlag = false;
}


void MyController::on_drawButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->xEdit);
    edits.push_back(ui->yEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    QPoint newPoint = QPoint(arr[0], arr[1]);

    if(!(ValidPoint(newPoint))) {
        delete[] arr;
        return;
    }
    scene->addPoint(newPoint);

    delete[] arr;

}

bool MyController::ValidPoint(QPoint &p) {
    if(p.x() < 0 || p.x() > scene->width() ||
       p.y() < 0 || p.y() > scene->height())
    {
        QString mess = "Координаты точки должны быть неотрицательными целыми числами\n"
                       "максимальные координаты " +
                QString::number(round(scene->width())) + " " +
                QString::number(round(scene->height())) + "\n";
        QErrorMessage errorMessage;
        errorMessage.showMessage(mess);
        errorMessage.exec();
        return false;
    }
    return true;
}
void MyController::on_timeButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->timeEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;
    if(arr[0] >= 0 && arr[0] <= MAX_TIME) {
        timePause = arr[0];
    }
    else {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Время задержки должно находится в пределах [0,100]\n");
        errorMessage.exec();
    }
    delete[] arr;
}

void MyController::on_brushButton_clicked()
{
    SimpleAlgo(scene, colorBrush, timePause);
    scene->repaintPolynom();
}

void MyController::on_clearallButton_clicked()
{
    scene->clear();
    scene->polynom.clear();
    scene->edges.clear();
    scene->paintFlag = false;
}
