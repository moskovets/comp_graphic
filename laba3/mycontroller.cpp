#include "mycontroller.h"
#include "ui_mycontroller.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <iostream>
#include <string.h>
#include <QColorDialog>
#include <QDebug>

#define MIN_PAR 10
Text_Error LineEditError;


MyController::MyController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyController)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    par = parent;
    ui->setupUi(this);
    Validator = new QRegExpValidator(QRegExp("^[+-]?[0-9]{0,5}(\\.|,|$)[0-9]{0,4}$"));
    ui->dEdit->setValidator(Validator);
    ui->xstartEdit->setValidator(Validator);
    ui->xendEdit->setValidator(Validator);
    ui->ystartEdit->setValidator(Validator);
    ui->yendEdit->setValidator(Validator);
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
    if(ui->standartButton->isChecked())
        return STANDART;
    if(ui->brDoubleButton->isChecked())
        return BR_DOUBLE;
    if(ui->brIntButton->isChecked())
        return BR_INT;
    if(ui->brModButton->isChecked())
        return BR_SMOOTH;
    if(ui->cdaButton->isChecked())
        return CDA;
    if(ui->wuButton->isChecked())
        return WU;
    std::cout << "oops\n";
    return STANDART;
}

void MyController::on_dwawlineButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->xstartEdit);
    edits.push_back(ui->ystartEdit);
    edits.push_back(ui->xendEdit);
    edits.push_back(ui->yendEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    data.start = QPoint(arr[0], arr[1]);
    data.end   = QPoint(arr[2], arr[3]);

    if(ui->fonButton->isChecked()) {
        data.color = data.fon;
    }
    else {
        data.color = colorLine;
    }

    draw_Line(scene, data, GetAlgorithm());

    delete[] arr;
}



void MyController::on_sizePixelButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->sizeEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;
    data.sizePixel = arr[0];
    draw_Line(scene, data, CHANGE_SCALE);

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
    draw_Line(scene, data, CHANGE_FON);
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
    draw_Line(scene, data, CLEAR_SCENE);
}
void Rotate(QPoint &p, QPoint center, double angle)
{
    angle *= M_PI / 180;
    QPoint buff = p;

    //p.setX(center.x() + (buff.x() - center.x())*cos(angle));
    //p.setY(center.y() + (buff.y() - center.y())*sin(angle));

    p.setX(center.x() + (double)(buff.x() - center.x())*cos(angle) - (double)(buff.y() - center.y())*sin(angle));
    p.setY(center.y() + (double)(buff.x() - center.x())*sin(angle) +  (double)(buff.y() - center.y())*cos(angle));
}

void MyController::on_dwawSunButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->dEdit);

    double *arr = GetData(edits);

    if(LineEditError != NO_ER)
        return;


    if(ui->fonButton->isChecked()) {
        data.color = data.fon;
    }
    else {
        data.color = colorLine;
    }
    double teta = 0;
    double dteta = arr[0];
    qDebug() << "я тут";
//    QPoint start(scene.x_center, scene.y_center);
//    QPoint end(scene.x_center+50, scene.y_center+50);

    QPoint start(100, 100);
    data.start = start;
    QPoint end(150, 100);
    while(teta < 360) {
        data.end = end;
        Rotate(data.end, start, teta);
        qDebug() << data.end;
        draw_Line(scene, data, GetAlgorithm());
        teta += dteta;
    }

//TODO
    /*нужен последовательный вызов для каждого отрезка
     * data.start = QPointF(arr[0], arr[1]);
    data.end   = QPointF(arr[2], arr[3]);

    draw_Line(scene, data, GetAlgorithm());
    */

    delete[] arr;

}
