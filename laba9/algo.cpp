#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"

#define SIGN(x) ((int) (x > 0) - (x < 0))

struct tSegment {
    tPoint p1;
    tPoint p2;
    tSegment() {}
    tSegment(tPoint a, tPoint b) {
        p1 = a;
        p2 = b;
    }
};
struct tVector {
    int x;
    int y;
    int z;
    tVector() {}
    tVector(int a, int b, int c = 0) {
        x = a;
        y = b;
        z = c;
    }
    tVector(tPoint end, tPoint start) {
        x = end.x - start.x;
        y = end.y - start.y;
        z = 0;
    }
};

void VectorMult(tVector &a, tVector& b, tVector &res) {
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
}
int ScalarMult(tVector &a, tVector& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int IsConvexPolygon(vector<tPoint> &polynom) {
    tVector a(polynom[1], polynom[0]);
    tVector b;
    int n = polynom.size();
    tVector tmp;
    int res = 0;
    for(int i = 1; i < n - 1; i++) {
        b = tVector(polynom[i + 1], polynom[i]);
        VectorMult(a, b, tmp);
        if(res == 0)
            res = SIGN(tmp.z);
        if(tmp.z && res != SIGN(tmp.z)) {
            return 0;
        }
        a = b;
    }
    return res;
}
int findNormVectorsToSide(vector<tPoint> &polynom, int obhod, vector<tVector> &normVect) {
    int n = polynom.size() - 1;
    tVector b;
    for(int i = 0; i < n; i++) {
        b = tVector(polynom[i+1], polynom[i]);
        if(obhod == -1)
            normVect.push_back(tVector(b.y, -b.x));
        else
            normVect.push_back(tVector(-b.y, b.x));
    }
}
tPoint P(double t, tPoint &p1, tPoint &p2) {
    tPoint tmp;
    tmp.x = p1.x + round((p2.x - p1.x) * t);
    tmp.y = p1.y + round((p2.y - p1.y) * t);
    return tmp;
}
//проверка пересечения векторов
bool FindIntersection(tVector segment, tPoint &p1, tPoint &p2, tVector &norm)
{

}
bool VisibleVertex(tPoint &vertex, tPoint &p1, tVector &norm) {
    tVector v1(vertex, p1);
    double mult = ScalarMult(v1, norm);
    return mult > 0;
}
void PrintResPolynom(paintScene *scene, vector<tPoint> &resPolynom, const QColor &colorBrush) {
    if(resPolynom.size() == 0) {
        qDebug() << "cut all";

        return;
    }
    resPolynom.push_back(resPolynom[0]);
    for(int i = 0; i < resPolynom.size() - 1; i++) {
        scene->addMyLine(resPolynom[i],resPolynom[i+1], colorBrush, 3);
    }
}
void DebugPrintPolynom(vector<tPoint> &polynom) {
    qDebug() << "-------------------";
    for(int i = 0; i < polynom.size(); i++) {
        qDebug() << polynom[i].x << polynom[i].y;
    }
    qDebug() << "-------------------";
}

int CutSegment(vector<tPoint> &Cut, vector<tVector> &normVect,
               vector<tPoint> &polynom, vector<tPoint> &resPolynom, paintScene *scene)
{
    int n = Cut.size() - 1;
    //int m = polynom.size() - 1;
    tPoint F, S, I;
    double Dsk, Wsk, t;DebugPrintPolynom(Cut);DebugPrintPolynom(polynom);
    for(int i = 0; i < n; i++) {
        int m = polynom.size();
        for(int j = 0; j < m; j++) {
            if(j == 0)
                F = polynom[j];
            else {
                tVector D(polynom[j], S);
                Dsk = ScalarMult(D, normVect[i]);
                if(Dsk != 0) {
                    tVector W(S, Cut[i]);
                    Wsk = ScalarMult(W, normVect[i]);
                    t = -Wsk / Dsk;
                    if(t >= 0 && t <= 1) {
                        I = P(t, S, polynom[j]);
                        resPolynom.push_back(I);
                    }
                }
            }
            S = polynom[j];
            if(VisibleVertex(S, Cut[i], normVect[i]))
                resPolynom.push_back(S);
        }

        //PrintResPolynom(scene, polynom, Qt::white);
        polynom = resPolynom;
        polynom.push_back(polynom[0]);
        DebugPrintPolynom(polynom);
        //PrintResPolynom(scene, resPolynom, Qt::green);
        //scene->sleepFeature(1000);
        resPolynom.clear();

    }
    resPolynom = polynom;
}

int SimpleAlgo(paintScene *scene, const QColor &colorBrush)
{
    int obhod = IsConvexPolygon(scene->polynom);
    if(!obhod)
        return 1;
    vector<tVector> normVect;
    findNormVectorsToSide(scene->polynom, obhod, normVect);
    vector<tPoint> resPolynom;
    CutSegment(scene->polynom, normVect, scene->polyForCut, resPolynom, scene);
    PrintResPolynom(scene, resPolynom, colorBrush);
    return 0;
}

