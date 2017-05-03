#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"

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
        for(unsigned int j = 0; j < edges.size(); j++) {
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

bool CheckLok2(const vector<QPointF> &polynom, const vector<pair<int,int>> &edges, int i)
{
    int dy1, dy2;
    int p1 = -1, p2 = -1;

    for(unsigned int j = 0; j < edges.size(); j++) {
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
bool CheckLok1(const vector<tVertex> &polynom, const vector<pair<int,int>> &edges, int i)
{
    int dy1, dy2;
    int p1 = -1, p2 = -1;
    p1 = polynom[i].next;
    p2 = polynom[i].prev;

    dy1 = polynom[i].p.y() - polynom[p1].p.y();
    dy2 = polynom[p2].p.y() - polynom[i].p.y();

    return dy2 * dy1 < 0;
}

bool CheckLok_horisont(const vector<tVertex> &polynom, const vector<pair<int,int>> &edges, int i1, int i2, int i_edge)
{
    int dy1, dy2;
    int p1 = -1, p2 = -1;

    for(unsigned int j = 0; j < edges.size(); j++) {
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

    dy1 = polynom[i1].p.y() - polynom[p1].p.y();

    dy2 = polynom[p2].p.y() - polynom[i1].p.y();
    return dy2 * dy1 < 0;

}

int FindPairPoints(vector<tPoint> &vect, const vector<tVertex> &polynom, const vector<pair<int,int>> &edges)
{
    double x1, x2, y1, y2;
    double x, y;
    double m;

    for(unsigned int i = 0; i < edges.size(); i++)
    {
        x1 = polynom[edges[i].first].p.x();
        x2 = polynom[edges[i].second].p.x();
        y1 = polynom[edges[i].first].p.y();
        y2 = polynom[edges[i].second].p.y();
        if(y1 == y2) {
            vect.push_back(tPoint(x1, y1));
            if(CheckLok_horisont(polynom, edges, edges[i].first, edges[i].second, i))
                vect.push_back(tPoint(x2, y2));
        }
        else {
            if(y1 > y2) {
                swap(y1, y2);
                swap(x1, x2);
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
        x1 = polynom[i].p.x();
        y1 = polynom[i].p.y();

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
void DrawLine(paintScene *scene, tPoint a, tPoint b, const QColor &colorBrush, int timePause)
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

    int y = -1;
    for(unsigned int i = 0; i < points.size(); i += 2) {
        if(points[i].y == y) {
           DrawLine(scene, points[i], points[i+1], colorBrush, 0);
        }
        else {
            DrawLine(scene, points[i], points[i+1], colorBrush, timePause);
            y = points[i].y;
        }
    }
    return 0;

}
/*
struct tNode {
    int n_max;
    double x;
    double dx;
    int dy;
    tNode *next = nullptr;
    tNode() {}
    tNode(int n, double x0, double dx0, int dy0) {
        n_max = n;
        x = x0;
        dx = dx0;
        dy = dy0;
        next = nullptr;
    }
};

class ListEdges
{
public:
    ListEdges() {

    }
    void add(tNode *node) {
        if(size == 0) {
            list = node;
            size++;
        }
        else {
            tNode* prevNode = list;
            tNode* tmp = list;
            if(node->n_max >= list->n_max) {
                node->next = list;
                list = node;
                size++;
                return;
            }
            tmp = tmp->next;
            while(tmp) {
                if(tmp->n_max >= prevNode->n_max) {
                    prevNode->next = node;
                    node->next = tmp;
                    break;
                }
                prevNode = tmp;
                tmp = tmp->next;
            }
            if(!node->next) {
                prevNode->next = node;
            }
            size++;
        }
    }
    void correct(int tmpy) {
        tNode* tmp = list;
        tNode* prev = list;
        while(tmp) {
            if(tmp->n_max < tmpy) { break; }
            tmp->x += tmp->dx;
            tmp->n_max--;
            tmp->dy -= 1;
            if(tmp->dy < 0) {
                if(tmp == prev) {
                    prev = tmp->next;
                    //delete tmp;
                    tmp = prev;
                }
                else {
                    prev->next = tmp->next;
                    //delete tmp;
                    tmp = nullptr;
                }
            }
            if(tmp)
               tmp = tmp->next;
        }
    }
    void form(vector <int> &tmpx, int tmpy) {
        tNode* tmp = list;
        while(tmp) {
            if(tmp->n_max < tmpy) { break; }
            tmpx.push_back(tmp->x);
            tmp = tmp->next;
        }
    }

    void print() {
        tNode* tmp = list;
        while(tmp) {
            qDebug() << tmp->n_max << tmp->x << tmp->dx << tmp->dy;
            tmp = tmp->next;
        }
    }

    bool empty()
    {
        return !list;
    }



    int size = 0;
    tNode* list = nullptr;
};
int Algo(const vector<tVertex> &polynom, const vector<pair<int,int>> &edges,
            paintScene *scene, const QColor &colorBrush, int timePause)
{
    ListEdges mylist;
    vector<tPoint> points;
    tNode *node = nullptr;
    double x1, x2, y1, y2;
    double m;

    for(unsigned int i = 0; i < edges.size(); i++)
    {
        x1 = polynom[edges[i].first].p.x();
        x2 = polynom[edges[i].second].p.x();
        y1 = polynom[edges[i].first].p.y();
        y2 = polynom[edges[i].second].p.y();
        if(y1 == y2) {
            node = new tNode(y1, x1, 0, 1);
            mylist.add(node);
            if(CheckLok_horisont(polynom, edges, edges[i].first, edges[i].second, i)) {
                node = new tNode(y2, x2, 0, 1);
                mylist.add(node);
            }
        }
        else {
            bool swapflag = false;
            if(y1 > y2) {
                swap(y1, y2);
                swap(x1, x2);
                swapflag = true;
            }
            m = (x2 - x1) / (y2 - y1);
           // int flagLok;
            if(CheckLok1(polynom, edges, edges[i].first)) {
                if(swapflag) {
                    node = new tNode(y2, x2, 0, 1);
                }
                else {
                    node = new tNode(y1, x1, 0, 1);
                }
                mylist.add(node);
            }
            node = new tNode(y2, x2, -m, y2 - y1 + 1);
            mylist.add(node);
        }
    }
    mylist.print();
    //return 0;


    vector <int> tmpx;
    int tmpy;
    while(!mylist.empty()) {
        tmpy = mylist.list->n_max;
        mylist.form(tmpx, tmpy);
        mylist.correct(tmpy);
        for(unsigned int i = 0; i < tmpx.size(); i += 2) {
            DrawLine(scene, tPoint(round(tmpx[i]), tmpy), tPoint(round(tmpx[i+1]), tmpy), colorBrush, timePause);
        }
        tmpx.clear();
    }
    return 0;

}

int SimpleAlgo(paintScene *scene, const QColor &colorBrush, int timePause)
{
    Algo(scene->polynom, scene->edges, scene, colorBrush, timePause);
    return 0;

}
*/
