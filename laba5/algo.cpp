#include "algo.h"
#include <vector>
#include <QThread>
class Sleeper: public QThread
{
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};

struct tPoint {
    int x;
    int y;
    tPoint(int x1, int y1) {
        x = x1;
        y = y1;
    }
};
struct tPair {
    tPoint first;
    tPoint second;
/*    tPair(int x1, int y1, int x2, int y2) {
        first  = tPoint(x1, y1);
        second = tPoint(x2, y2);
    }*/
};

bool CheckLok(const vector<QPointF> &polynom, const vector<pair<int,int>> &edges, int index, int i)
{
    int dy1, dy2;
    int second = edges[i].first;
    if(second == index) {
        second = edges[i].second;
    }
    dy1 = polynom[index].y() - polynom[second].y();


    int new_index = -1;
    /*if(i > 0) {
        if(edges[i-1].first == index) {
            new_index = edges[i-1].second;
        }
        else if(edges[i-1].first == index) {
            new_index = edges[i-1].first;
        }
    }
    if(new_index == -1) {
        if(i < edges.size() - 1) {
            if(edges[i+1].first == index) {
                new_index = edges[i-1].second;
            }
            else if(edges[i+1].first == index) {
                new_index = edges[i-1].first;
            }
        }
        else {
            if(second < index) {
                new_index = second + 1;
            }
            else if(second > index) {
                new_index = index + 1;
            }
        }
    }*/

    if(new_index == -1) {
        for(int j = 0; j < edges.size(); j++) {
            if(j != i) {
                if(edges[j].first == index) {
                    new_index = edges[j].second;
                }
                else if(edges[j].second == index) {
                    new_index = edges[j].first;
                }
            }
        }
    }

    dy2 = polynom[new_index].y() - polynom[index].y();
    return dy2 * dy1 < 0;

}

bool CheckLok1(const vector<QPointF> &polynom, const vector<pair<int,int>> &edges, int i)
{
    int dy1, dy2;
    int p1 = -1, p2 = -1;
    int new_index;

    for(int j = 0; j < edges.size(); j++) {
        if(edges[j].first == i) {
            if(p1 == -1)
                p1 = edges[j].second;
            else
                p2 = edges[j].second;
        }
        else if(edges[j].second == i) {
            if(p1 == -1)
                p1 = edges[j].first;
            else
                p2 = edges[j].first;
        }
    }

    dy1 = polynom[i].y() - polynom[p1].y();


    dy2 = polynom[p2].y() - polynom[i].y();
    return dy2 * dy1 < 0;

}

bool CheckLok_horisont(const vector<QPointF> &polynom, const vector<pair<int,int>> &edges, int i1, int i2, int i_edge)
{
    int dy1, dy2;
    int p1 = -1, p2 = -1;
    int new_index;

    for(int j = 0; j < edges.size(); j++) {
        if(j != i_edge) {
            if(edges[j].first == i1) {
                p1 = edges[j].second;
            }
            else if(edges[j].second == i1) {
                p1 = edges[j].first;
            }
            if(edges[j].first == i2) {
                p2 = edges[j].second;
            }
            else if(edges[j].second == i2) {
                p2 = edges[j].first;
            }
        }
    }

    dy1 = polynom[i1].y() - polynom[p1].y();

    dy2 = polynom[p2].y() - polynom[i1].y();
    return dy2 * dy1 < 0;

}

int FindPairPoints(vector<tPoint> &vect, const vector<QPointF> &polynom, const vector<pair<int,int>> &edges)
{
    double x1, x2, y1, y2;
    bool vertex[polynom.size()];
    for(int i = 0; i < polynom.size(); i++) { vertex[i] = false; }
    double x, y;
    double m;
    bool flagSwap = false;
    for(unsigned int i = 0; i < edges.size(); i++)
    {
        x1 = polynom[edges[i].first].x();
        x2 = polynom[edges[i].second].x();
        y1 = polynom[edges[i].first].y();
        y2 = polynom[edges[i].second].y();
        if(y1 == y2) {
            vect.push_back(tPoint(x1, y1));
            if(CheckLok_horisont(polynom, edges, edges[i].first, edges[i].second, i))
                vect.push_back(tPoint(x2, y2));
        }
        else {
            if(y1 > y2) {
                swap(y1, y2);
                swap(x1, x2);
                flagSwap = true;
            }
            m = (x2 - x1) / (y2 - y1);
            y = y1 + 1;
            x = x1 + m;
            for(; y < y2; y += 1) {
                vect.push_back(tPoint(round(x), round(y)));
                x += m;
            }
        }

    }
    for(unsigned i = 0; i < polynom.size(); i++) {
            // check for lok max or min
        x1 = polynom[i].x();
        y1 = polynom[i].y();

        if(CheckLok1(polynom, edges, i))
        {
            vect.push_back(tPoint(x1, y1));
        }
        vect.push_back(tPoint(x1, y1));
    }

}
bool MyCompare(tPoint a, tPoint b) {
    if(a.y > b.y) {
        return true;
    }
    if(a.y == b.y && a.x < b.x) {
        return true;
    }
    return false;
}
void DrawLine(paintScene *scene, tPoint &a, tPoint &b, const QColor &colorBrush, int timePause)
{
    scene->addLine(a.x, a.y, b.x, b.y, QPen(colorBrush, 1));
    if(timePause != 0) {
        scene->sleepFeature(timePause);
    }
}

int SimpleAlgo(paintScene *scene, const QColor &colorBrush, int timePause)
{
    vector<tPoint> points;

    FindPairPoints(points, scene->polynom, scene->edges);
    std::sort(points.begin(), points.end(), MyCompare);


    for(int i = 0; i < points.size(); i += 2) {

        DrawLine(scene, points[i], points[i+1], colorBrush, timePause);
        //scene->addLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y, QPen(colorBrush, 1));

    }
    return 0;

}
