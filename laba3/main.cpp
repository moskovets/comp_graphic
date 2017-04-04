#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "analizalgo.h"

int main(int argc, char *argv[])
{
    Analize();
    //подключаем русский язык
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
