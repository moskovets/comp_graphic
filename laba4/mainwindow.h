#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mycontroller.h"
#include "mytimeanaliz.h"
#include <mygraphicview.h>



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
    void SendingScene(tScene *my_scene);
private:
    Ui::MainWindow  *ui;
    MyGraphicView   *myPicture;     // Наш кастомный виджет
    MyController    *myController;
    MyTimeanaliz    *myTimeanaliz;
};


#endif // MAINWINDOW_H
