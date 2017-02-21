#include "mycontroller.h"
#include "ui_mycontroller.h"
#include <iostream>

MyController::MyController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyController)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    par = parent;
    ui->setupUi(this);
}

MyController::~MyController()
{
    delete ui;
}

void MyController::printLabel(Answer *ans)
{
    QString str = "Треугольник с вершинами в точках: ";
    str += QString::number(ans->p1.x) + " " + QString::number(ans->p1.y) + "; " +
           QString::number(ans->p2.x) + " " + QString::number(ans->p2.y) + "; " +
           QString::number(ans->p3.x) + " " + QString::number(ans->p3.y) + "  ";
    str += "имеет минимальную разность количеств точек, "
           "попавших внутрь треугольника и за его пределами, "
           "но внутри описанной окружности. Она равна ";
    str += QString::number(ans->diff);
    ui->label->setText(str);
}

void MyController::on_pushButton_clicked()
{
    QString str= ui->textEdit->toPlainText();
    Data* data = Analiz_Text(str);
    QString mess;
    switch(data->er)
    {
    case EMPTY:
        mess = "Область текста пуста.\n Введите данные!";
        break;
    case E_SYMBOL:
        mess = "Ошибочный символ\n "
               "Разрешается использование только цифр, '.'',' и знаков табуляции";
        break;
    case E_PAIR_DATA:
        mess = "Не хватает одной координаты";
        break;
    case E_FEW_POINT:
        mess = "Треугольник можно построить только из 3 и более точек!";
        break;
    }
    if(data->er != NO_ER) {
        ui->label->setText("");
        QErrorMessage errorMessage;
        errorMessage.showMessage(mess);
        errorMessage.exec();
    }
    else {
        Answer *ans = FindMinTriangleWithPoints(data->arr, data->N);
        if(ans->diff == INF) {
            ui->label->setText("Решений нет - треугольники вырождены");
            emit AnswerChange(NULL);
        }
        else {
            cout << "emit:" << ans->p1.x << endl;
            printLabel(ans);
            emit AnswerChange(ans);
        }
    }
}

Data* Analiz_Text(QString str)
{
    Data* data = new Data;
    if(str == "") {
        data->er = EMPTY;
        return data;
    }
    str.replace(QString(","), QString("."));
    str.replace(QString("\n"), QString(" "));

    QStringList list = str.split(' ', QString::SkipEmptyParts);

    QString tmp;
    double x;
    bool ok = true;
    vector <double> arr;

    for(int i = 0; i < list.size(); i++) {
        tmp = list.at(i);
        x = tmp.toDouble(&ok);
        //cout << tmp.toStdString() << " " << ok << " " << x << endl;
        if(ok) {
            arr.push_back(x);
        }
        else {
            data->er = E_SYMBOL;
            return data;
        }
    }
    data->N = arr.size();
    if(arr.size() % 2 == 1) {
        data->er = E_PAIR_DATA;
        return data;
    }
    data->N /= 2;
    if(data->N < 3) {
        data->er = E_FEW_POINT;
        return data;
    }
    data->arr = new Point [data->N];
    for(int i = 0; i < data->N; i++) {
       data->arr[i].x = arr[i * 2];
       data->arr[i].y = arr[i * 2 + 1];
    }
    data->er = NO_ER;
    return data;
}
