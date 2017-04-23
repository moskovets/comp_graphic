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
            vect.push_back(tPoint(x2, y2));
        }
        else {
            if(y1 > y2) {
                swap(y1, y2);
                swap(x1, x2);
                flagSwap = true;
            }
            x = x1 + m;
            y = y1 + 1;
            m = (x2 - x1) / (y2 - y1);
            for(; y < y2; y += 1) {
                vect.push_back(tPoint(round(x), round(y)));
                x += m;
            }
            // check for lok max or min

            if(!vertex[edges[i].first] && CheckLok(polynom, edges, edges[i].first, i))
            {
                if(flagSwap)
                    vect.push_back(tPoint(x2, y2));
                else
                    vect.push_back(tPoint(x1, y1));
            }
            if(!vertex[edges[i].second] && CheckLok(polynom, edges, edges[i].second, i))
            {
                if(flagSwap)
                    vect.push_back(tPoint(x1, y1));
                else
                    vect.push_back(tPoint(x2, y2));
            }
            if(!vertex[edges[i].first])
            {
                if(flagSwap)
                    vect.push_back(tPoint(x2, y2));
                else
                    vect.push_back(tPoint(x1, y1));
                vertex[edges[i].first] = true;
            }
            if(!vertex[edges[i].second])
            {
                if(flagSwap)
                    vect.push_back(tPoint(x1, y1));
                else
                    vect.push_back(tPoint(x2, y2));
                vertex[edges[i].second] = true;
            }
        }
    }

}
bool MyCompare(tPoint a, tPoint b) {
    if(a.y > b.y) {
        return true;
    }
    if(a.y == b.y && a.x < a.y) {
        return true;
    }
    return false;
}
void DrawLine(paintScene *scene, tPoint &a, tPoint &b, const QColor &colorBrush, int timePause)
{
    scene->addLine(a.x, a.y, b.x, b.y, QPen(colorBrush, 1));

    Sleeper::msleep(1);
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
