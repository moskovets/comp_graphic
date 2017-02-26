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

bool krat(double a, int k)
{
    double b = round(a / k);
    b *= k;
    return compare_double(a, b) == 0;
}

QPointF f(double a, double b, double t)
{
    double x, y;
    double c = (a + b) * t / a;
    x = (a + b) * cos(t) - a * cos(c);
    y = (a + b) * sin(t) - a * sin(c);
    return QPointF(x, y);
}

Answer* Create_figure(double a, double b)
{
   Answer *ans = new Answer;

   double side1 = (a + a + b) + RECT_BORDER; //y
   double side2 = (a + b + (a > b ? a : b)) + RECT_BORDER; //x
   ans->brush.InitBrush(side2, side1);
  // vector
   ans->rect << QPointF(side2, side1);
   ans->rect << QPointF(-side2, side1);
   ans->rect << QPointF(-side2, -side1);
   ans->rect << QPointF(side2, -side1);

   double final = 2 * M_PI * a;
   for(double i = 0.0; i < final; i += 0.001) {
       QPointF tmp = f(a, b, i);
       ans->graph << tmp;
   //    ans->brush.add(tmp);
   }
   ans->brush.add(ans->graph);
   ans->brush.sort_and_clear();
   return ans;
}

void Mult(double a[3][3], double vec[3])
{
    double res[3] = {0};
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            res[i] += a[i][j] * vec[j];
        }
    }
    for(int i = 0; i < 3; i++) {
        vec[i] = res[i];
    }
}

void Rotate_polygon(QPolygonF &poly, double angle) {
    double a[3][3] = { { cos(angle), -sin(angle), 0 },
                       { sin(angle), cos(angle), 0 },
                       { 0, 0, 1 } };
    double vec[3];
    for(int i = 0; i < poly.size(); i++) {
        vec[0] = poly.at(i).x();
        vec[1] = poly.at(i).y();
        vec[2] = 1;
        Mult(a, vec);
        poly[i].setX(vec[0]);
        poly[i].setY(vec[1]);
    }
}

void Rotate_polygon(QPolygonF &poly, double cx, double cy, double angle)
{
    for(int i = 0; i < poly.size(); i++) {
        poly[i].setX(poly.at(i).x() - cx);
        poly[i].setY(poly.at(i).y() - cy);
    }
    double angle_rad = angle * M_PI / 180;

    Rotate_polygon(poly, angle_rad);

    for(int i = 0; i < poly.size(); i++) {
        poly[i].setX(poly.at(i).x() + cx);
        poly[i].setY(poly.at(i).y() + cy);
    }
}

void Rotate_brush(MyBrush &brush, double angle) {
    double a[3][3] = { { cos(angle), -sin(angle), 0 },
                       { sin(angle), cos(angle), 0 },
                       { 0, 0, 1 } };
    double vec[3];
    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            vec[0] = brush.vec[i][j].x();
            vec[1] = brush.vec[i][j].y();
            vec[2] = 1;
            Mult(a, vec);
            brush.vec[i][j].setX(vec[0]);
            brush.vec[i][j].setY(vec[1]);
        }
    }
}
void Rotate_brush(MyBrush &brush, double cx, double cy, double angle)
{
    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            brush.vec[i][j].setX(brush.vec[i][j].x() - cx);
            brush.vec[i][j].setY(brush.vec[i][j].y() - cy);
        }
    }
    double angle_rad = angle * M_PI / 180;

    Rotate_brush(brush, angle_rad);
    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            brush.vec[i][j].setX(brush.vec[i][j].x() + cx);
            brush.vec[i][j].setY(brush.vec[i][j].y() + cy);
        }
    }
}



Answer* Rotate_figure(Answer* figure, double cx, double cy, double angle)
{
    Answer *ans = new Answer(figure);
    Rotate_polygon(ans->rect, cx, cy, angle);
    Rotate_polygon(ans->graph, cx, cy, angle);
    Rotate_brush(ans->brush, cx, cy, angle);
    return ans;
}


void Scale_polygon(QPolygonF &poly, double kx, double ky) {
    double a[3][3] = { { kx, 0, 0 },
                       { 0, ky, 0 },
                       { 0, 0, 1 } };
    double vec[3];
    for(int i = 0; i < poly.size(); i++) {
        vec[0] = poly.at(i).x();
        vec[1] = poly.at(i).y();
        vec[2] = 1;
        Mult(a, vec);
        poly[i].setX(vec[0]);
        poly[i].setY(vec[1]);
    }
}

void Scale_polygon(QPolygonF &poly, double cx, double cy, double kx, double ky)
{
    for(int i = 0; i < poly.size(); i++) {
        poly[i].setX(poly.at(i).x() - cx);
        poly[i].setY(poly.at(i).y() - cy);
    }

    Scale_polygon(poly, kx, ky);

    for(int i = 0; i < poly.size(); i++) {
        poly[i].setX(poly.at(i).x() + cx);
        poly[i].setY(poly.at(i).y() + cy);
    }
}

void Scale_brush(MyBrush &brush, double kx, double ky) {
    double a[3][3] = { { kx, 0, 0 },
                       { 0, ky, 0 },
                       { 0, 0, 1 } };
    double vec[3];
    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            vec[0] = brush.vec[i][j].x();
            vec[1] = brush.vec[i][j].y();
            vec[2] = 1;
            Mult(a, vec);
            brush.vec[i][j].setX(vec[0]);
            brush.vec[i][j].setY(vec[1]);
        }
    }
}

void Scale_brush(MyBrush &brush, double cx, double cy, double kx, double ky)
{
    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            brush.vec[i][j].setX(brush.vec[i][j].x() - cx);
            brush.vec[i][j].setY(brush.vec[i][j].y() - cy);
        }
    }

    Scale_brush(brush, kx, ky);

    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            brush.vec[i][j].setX(brush.vec[i][j].x() + cx);
            brush.vec[i][j].setY(brush.vec[i][j].y() + cy);
        }
    }
}
void Move_polygon(QPolygonF &poly, double dx, double dy)
{
    for(int i = 0; i < poly.size(); i++) {
        poly[i].setX(poly.at(i).x() + dx);
        poly[i].setY(poly.at(i).y() + dy);
    }
}
void Move_brush(MyBrush &brush, double dx, double dy)
{
    for(int i = 0; i < brush.vec.size(); i++) {
        for(int j = 0; j < brush.vec[i].size(); j++) {
            brush.vec[i][j].setX(brush.vec[i][j].x() + dx);
            brush.vec[i][j].setY(brush.vec[i][j].y() + dy);
        }
    }
}

Answer* Scale_figure(Answer* figure, double cx, double cy, double kx, double ky)
{
    Answer *ans = new Answer(figure);
    Scale_polygon(ans->rect, cx, cy, kx, ky);
    Scale_polygon(ans->graph, cx, cy, kx, ky);
    Scale_brush(ans->brush, cx, cy, kx, ky);
    return ans;
}

Answer* Move_figure(Answer* figure, double dx, double dy)
{
    Answer *ans = new Answer(figure);
    Move_polygon(ans->rect, dx, dy);
    Move_polygon(ans->graph, dx, dy);
    Move_brush(ans->brush, dx, dy);

    return ans;
}



