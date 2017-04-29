#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paint.h"
#include "mycontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void SendingScene(paintScene *my_scene);
    void ChangePixel();
private:
    Ui::MainWindow  *ui;
    Paint   *myPicture;     // Наш кастомный виджет
    MyController    *myController;
    //MyTimeanaliz    *myTimeanaliz;
};


#endif // MAINWINDOW_H
