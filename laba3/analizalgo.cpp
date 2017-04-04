#include "work.h"
#include "analizalgo.h"

#include <iostream>
#include <string.h>
#include <fstream>
#include <QDebug>
#include <QPoint>

#include <stdio.h>

#define LINE_LEN 100
using namespace std;

void Rotate(QPoint &p, QPoint &center, double angle)
{
    angle *= M_PI / 180;
    QPoint buff = p;

    p.setX(center.x() + round((double)(buff.x() - center.x())*cos(angle) - (double)(buff.y() - center.y())*sin(angle)));
    p.setY(center.y() + round((double)(buff.x() - center.x())*sin(angle) +  (double)(buff.y() - center.y())*cos(angle)));
}

void AnalizStep(ALGORITHM algo, ofstream &out)
{
    double len = LINE_LEN;
    double teta = 0;
    double dteta = 2;
    tAnaliz res;

    QPoint start(LINE_LEN, LINE_LEN);
    QPoint end(LINE_LEN + LINE_LEN, LINE_LEN);
    tData data;
    data.start = start;

    while(teta <= 90) {
        data.end = end;
        Rotate(data.end, start, teta);
        draw_Line(res, data, algo);
        out << res.max_step << endl;
        teta += dteta;
    }
}

void AnalizTime(ALGORITHM algo, ofstream &out)
{
    double len = LINE_LEN;
    double teta = 0;
    double dteta = 1;
    int iter = 50;
    double sum_time = 0;
    tAnaliz res;

    QPoint start(LINE_LEN, LINE_LEN);
    tData data;
    data.start = start;

    for(int i = 0; i < iter; i++) {

        QPoint end(LINE_LEN + LINE_LEN, LINE_LEN);

        while(teta <= 360) {
            data.end = end;
            Rotate(data.end, start, teta);
            draw_Line(res, data, algo);
            sum_time += (double)res.time;
            teta += dteta;
        }
    }
    sum_time /= iter;

    out << sum_time << endl;
}


void Analize()
{
    srand(time(0));
    ofstream out;
    out.open("results.txt");
    out << "Анализ времени работы\n";
    out << "BR_DOUBLE\n";
    AnalizTime(BR_DOUBLE, out);
    out << "BR_INT\n";
    AnalizTime(BR_INT, out);
    out << "BR_SMOOTH\n";
    AnalizTime(BR_SMOOTH, out);
    out << "CDA\n";
    AnalizTime(CDA, out);
    out << "WU\n";
    AnalizTime(WU, out);
    out << "\n\n";

    out << "Анализ максимальной ступеньки\n";
    out << "BR_DOUBLE\n\n";
    AnalizStep(BR_DOUBLE, out);
    out << "BR_INT\n\n";
    AnalizStep(BR_INT, out);
    out << "BR_SMOOTH\n\n";
    AnalizStep(BR_SMOOTH, out);
    out << "CDA\n\n";
    AnalizStep(CDA, out);
    out << "WU\n\n";
    AnalizStep(WU, out);
    out << "\n\n\n\n";

    out.close();

}
