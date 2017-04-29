#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QTimer>
#include <QResizeEvent>

#include <paintscene.h>
//#include "my_scene.h"

namespace Ui {
class Paint;
}

class Paint : public QWidget
{
    Q_OBJECT

public:
    explicit Paint(QWidget *parent = 0);
    ~Paint();
    void Connect();

    signals:
    void SendScene(paintScene* my_scene);
    void NewPixel();

private:
    Ui::Paint *ui;
    QTimer *timer;      /* Определяем таймер для подготовки актуальных размеров
                         * графической сцены
                         * */
    paintScene *scene;  // Объявляем кастомную графическую сцену

private:
    /* Переопределяем событие изменения размера окна
     * для пересчёта размеров графической сцены
     * */
    void resizeEvent(QResizeEvent * event);

private slots:
    void slotTimer();
    void changeScenePixel();

};

#endif // PAINT_H
