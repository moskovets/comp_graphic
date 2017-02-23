#include "work.h"
#include <iostream>
int compare_double(double a, double b)
{
    if(fabs(a - b) < EPS)
        return 0;
    if(a > b)
        return 1;
    return -1;
}
QPoint f(double a, double b, double t)
{
    double x, y;
    double c = (a + b) * t / a;
    x = (a + b) * cos(t) - a * cos(c);
    y = (a + b) * sin(t) - a * sin(c);
    return QPoint(x, y);
}

Answer* Create_figure(double a, double b)
{
   Answer *ans = new Answer;

   double side1 = (a + a + b); //y
   double side2 = (a + b + b); //x
   ans->rect << QPoint(side2, side1);
   ans->rect << QPoint(-side2, side1);
   ans->rect << QPoint(-side2, -side1);
   ans->rect << QPoint(side2, -side1);

   double final = 2 * M_PI * a;
   for(double i = 0.0; i < final; i += 0.001) {
       ans->graph << f(a, b, i);
   }
   return ans;
}

Answer* Rotate_figure(Answer* figure, double cx, double cy, double angle)
{
    Answer *ans = NULL;
    return ans;

}

Answer* Scale_figure(Answer* figure, double cx, double cy, double kx, double ky)
{
    Answer *ans = NULL;
    return ans;

}

Answer* Move_figure(Answer* figure, double dx, double dy)
{
    Answer *ans = NULL;
    return ans;

}



