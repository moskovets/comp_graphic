#ifndef MYTIMEANALIZ_H
#define MYTIMEANALIZ_H

#include <QWidget>
#include "CImage.h"
#include "qcustomplot.h"

class MyTimeanaliz : public QWidget
{
    Q_OBJECT
public:
    explicit MyTimeanaliz(QWidget *parent = 0);
private:
    CImage image;
    QCustomPlot* diagram;

    void Analiz();
signals:

public slots:
};

#endif // MYTIMEANALIZ_H