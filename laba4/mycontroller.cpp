#include "mycontroller.h"
#include "ui_mycontroller.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <iostream>
#include <string.h>
#include <QColorDialog>
#include <QDebug>

#define MIN_PAR    10
#define LEN        300
#define MAX_PIXEL  100
#define MAX_RADIUS 100

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
    ui->drEdit->setValidator(Validator);
    ui->xEdit->setValidator(Validator);
    ui->yEdit->setValidator(Validator);
    ui->rxEdit->setValidator(Validator);
    ui->ryEdit->setValidator(Validator);
    ui->rstartEdit->setValidator(Validator);
    ui->nEdit->setValidator(Validator);
    ui->sizeEdit->setValidator(Validator);

    data.fon = QColor(255, 255, 255); //фон белый
    data.color = QColor(0, 0, 0); //отрезок черный
    colorLine = data.color;
    data.sizePixel = 1;

}
void MyController::GetScene(tScene *scene1)
{
    this->scene.x_center = scene1->x_center;
    this->scene.y_center = scene1->y_center;
    this->scene.scene = scene1->scene;
    image = CImage(scene, data);

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
ALGORITHM MyController::GetAlgorithm()
{
    switch(ui->algoComboBox->currentIndex())
    {
    case 0:
        return BRESENHAM;
    case 1:
        return MID_POINT;
    case 2:
        return CANON_EQ;
    case 3:
        return PARAM_EQ;
    default:
        return STANDART;
    }
}

bool MyController::ValidPoint(QPoint &p) {
    double scale = 2 / (double) data.sizePixel;
    qDebug() << scale;
    if(p.x() < 0 || p.x() > scene.x_center * scale ||
       p.y() < 0 || p.y() > scene.y_center * scale)
    {
        QString mess = "Координаты точки должны быть неотрицательными целыми числами\n"
                       "Для заданного размера пикселя максимальные координаты " +
                QString::number(round(scene.x_center * scale)) + " " +
                QString::number(round(scene.y_center * scale)) + "\n";
        QErrorMessage errorMessage;
        errorMessage.showMessage(mess);
        errorMessage.exec();
        return false;
    }
    return true;
}

void MyController::on_sizePixelButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->sizeEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;
    if(arr[0] >= 1 && arr[0] <= MAX_PIXEL) {
        data.sizePixel = arr[0];
        image.changeScale(scene, data.sizePixel);
    }
    else {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Размер пикселя должен находится в пределах [1,100]\n");
        errorMessage.exec();
    }
    delete[] arr;
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
    data.fon = color;
    image.changeFon(scene, data.fon);
}

void MyController::on_fonButton_clicked()
{
    if(ui->fonButton->isChecked()) {
        ui->linecolorButton->setEnabled(false);
    }
    else {
        ui->linecolorButton->setEnabled(true);
    }
}

void MyController::on_linecolorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (!color.isValid() ) {
     return;
    }
    ui->linelabel->setStyleSheet("background-color: " + GetColor(color));
    data.color = color;
    colorLine = color;
}

void MyController::on_clearButton_clicked()
{
    image = CImage(scene, data);
}

void Rotate(QPoint &p, QPoint center, double angle)
{
    angle *= M_PI / 180;
    QPoint buff = p;

    p.setX(center.x() + round((double)(buff.x() - center.x())*cos(angle) - (double)(buff.y() - center.y())*sin(angle)));
    p.setY(center.y() + round((double)(buff.x() - center.x())*sin(angle) +  (double)(buff.y() - center.y())*cos(angle)));
}
void MyController::drawEllipse(QPoint &center, double rx, double ry)
{
    tDataEllipse dataEllipse;
    dataEllipse.center = center;
    dataEllipse.param = this->data;
    dataEllipse.rx = rx;
    dataEllipse.ry = ry;
    image.draw_Algorithm(scene, dataEllipse, GetAlgorithm());
}

void MyController::drawCircle(QPoint &center, double r)
{
    tDataCircle dataCircle;
    dataCircle.center = center;
    dataCircle.param = this->data;
    dataCircle.radius = r;
    image.draw_Algorithm(scene, dataCircle, GetAlgorithm());
}

void MyController::on_drawellipseButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->xEdit);
    edits.push_back(ui->yEdit);
    edits.push_back(ui->rxEdit);
    edits.push_back(ui->ryEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    if(ui->fonButton->isChecked()) {
        data.color = data.fon;
    }
    else {
        data.color = colorLine;
    }

    QPoint center = QPoint(arr[0], arr[1]);

    if(!(ValidPoint(center))) {
        delete[] arr;
        return;
    }
    if(ui->ellipseButton->isChecked()) {
        drawEllipse(center, arr[2], arr[3]);
    }

    if(ui->circleButton->isChecked()) {
        drawCircle(center, arr[2]);
    }

    delete[] arr;
}

void MyController::on_ellipseButton_clicked()
{
    ui->ryEdit->setDisabled(false);
}

void MyController::on_circleButton_clicked()
{
    ui->ryEdit->setDisabled(true);
}
