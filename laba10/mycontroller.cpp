#include "mycontroller.h"
#include "ui_mycontroller.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <iostream>
#include <string.h>
#include <QColorDialog>
#include <QDebug>
#include "algo.h"
#include "f.h"

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

    Validator = new QRegExpValidator(QRegExp("^[+-]?[0-9]{0,5}(\\.|,|$)[0-9]{0,4}$"));
    ui->xminEdit->setValidator(Validator);
    ui->xmaxEdit->setValidator(Validator);
    ui->dxEdit->setValidator(Validator);
    ui->zminEdit->setValidator(Validator);
    ui->zmaxEdit->setValidator(Validator);
    ui->nEdit->setValidator(new QIntValidator(0, 100));
    ui->tetaxEdit->setValidator(Validator);
    ui->tetayEdit->setValidator(Validator);
    ui->tetazEdit->setValidator(Validator);

    //xmin, xmax, dx,  n, zmin, zmax
    // -2,   2,  0.05, 41, -2,   2
    ui->xminEdit->setText("-2");
    ui->xmaxEdit->setText("2");
    ui->dxEdit->setText("0.05");
    ui->nEdit->setText("41");
    ui->zminEdit->setText("-2");
    ui->zmaxEdit->setText("2");

    for(int i = 0; i < FUNC_NUMBER; i++) {
        ui->functionBox->addItem(func.GetStr(i));
    }
    functionIndex = 0;
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



void MyController::on_drawButton_clicked()
{
    scene->clearAll();
    vector<QLineEdit*> edits;
    edits.push_back(ui->xminEdit);
    edits.push_back(ui->xmaxEdit);
    edits.push_back(ui->dxEdit);
    edits.push_back(ui->nEdit);
    edits.push_back(ui->zminEdit);
    edits.push_back(ui->zmaxEdit);

    double *data = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    //f, xmin, xmax, dx, n, zmin, zmax
    tFunction func1 { func.GetFunc(functionIndex), data[0], data[1], data[2],
                                                   data[3], data[4], data[5] } ;
    SimpleAlgo(scene, func1);
}

void MyController::on_rotateButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->tetaxEdit);
    edits.push_back(ui->tetayEdit);
    edits.push_back(ui->tetazEdit);

    double *data = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    if(!scene->flagGraphExist)
        return;

    scene->clear();
    scene->rotate(data[0], data[1], data[2]);
    scene->repaintScene();
}

void MyController::on_functionBox_activated(int index)
{
    functionIndex = index;
    qDebug() << index;
}

void MyController::on_clearallButton_clicked()
{
    scene->clearAll();
}
