#include "mycontroller.h"
#include "ui_mycontroller.h"
#include <QRegExpValidator>
#include <QIntValidator>

#include <iostream>
#define MIN_PAR 10
Text_Error LineEditError;


MyController::MyController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyController)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    par = parent;
    ui->setupUi(this);
    ui->aEdit->setValidator(new QIntValidator(11, 1000));
    ui->bEdit->setValidator(new QIntValidator(11, 1000));
    ui->angleEdit->setValidator(new QRegExpValidator(QRegExp("^[+-]?[0-9]{0,5}(\\.|,|$)[0-9]{0,4}$")));
    ui->backButton->setEnabled(false);
}

MyController::~MyController()
{
    delete ui;
}

//получение данных с выбранных полей
double * MyController::GetData(vector <QLineEdit*> &vec)
{
    QString str;
    double *data = new double[vec.size()];
    double x;
    QString mess;
    for(int i = 0; i < vec.size(); i++) {
        str = vec[i]->text();
        x = Analiz_Text(str);
        switch(LineEditError)
        {
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
        //cout << tmp.toStdString() << " " << ok << " " << x << endl;
        if(!ok) {
            LineEditError = E_SYMBOL;
            return 0;
        }
    }
    return x;
}

void MyController::on_paint_new_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->aEdit);
    edits.push_back(ui->bEdit);

    double *data = GetData(edits);
    if(LineEditError != NO_ER)
        return;
    if(data[0] <= MIN_PAR || data[1] <= MIN_PAR) {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Параметры должны быть больше 10!");
        errorMessage.exec();
        return;
    }

    Answer* ans = Create_figure(data[0], data[1]);

    history.push(ans);
    if(!ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }
    emit AnswerChange(ans);
}

void MyController::on_rotateButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->cxEdit);
    edits.push_back(ui->cyEdit);
    edits.push_back(ui->angleEdit);

    double *data = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    Answer* ans = Rotate_figure(history.top(), data[0], data[1], data[2]);

    history.push(ans);
    if(!ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }

    emit AnswerChange(ans);
}

void MyController::on_scaleButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->cxEdit);
    edits.push_back(ui->cyEdit);
    edits.push_back(ui->kxEdit);
    edits.push_back(ui->kyEdit);

    double *data = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    Answer* ans = Scale_figure(history.top(),
                                data[0], data[1], data[2], data[3]);

    history.push(ans);
    if(!ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }

    emit AnswerChange(ans);

}

void MyController::on_moveButton_clicked()
{
    vector<QLineEdit*> edits;
    edits.push_back(ui->dxEdit);
    edits.push_back(ui->dyEdit);

    double *data = GetData(edits);

    if(LineEditError != NO_ER)
        return;

    Answer* ans = Move_figure(history.top(),
                                data[0], data[1]);

    history.push(ans);
    if(!ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }
    emit AnswerChange(ans);
}

void MyController::on_backButton_clicked()
{
    if(history.empty()) {
        QErrorMessage errorMessage;
        errorMessage.showMessage("Истории отрисовок пока нет");
        errorMessage.exec();
        return;
    }

    Answer *ans = history.top();
    history.pop();

    if(history.empty()) {
        history.push(ans);
        ui->backButton->setEnabled(false);
    }
    else {
        ans = history.top();
    }
    emit AnswerChange(ans);
}
