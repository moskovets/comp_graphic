#include "algo.h"
#include <vector>
#include <QThread>
#include "paintscene.h"

#define SIGN(x) ((int) (x > 0) - (x < 0))

struct tSegment {
    bool empty = false;
    tPoint p1;
    tPoint p2;
    tSegment() {}
    tSegment(tPoint &a, tPoint &b) {
        if(a.x == b.x && a.y > b.y) {
            p2 = a;
            p1 = b;
        }
        else {
            p1 = a;
            p2 = b;
        }
    }
    bool isEqual(tSegment s) {
        if(p1.isEqual(s.p1) && p2.isEqual(s.p2))
            return true;
        if(p1.isEqual(s.p2) && p2.isEqual(s.p1))
            return true;
        return false;
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
    tVector(tSegment s) {
        x = s.p2.x - s.p1.x;
        y = s.p2.y - s.p1.y;
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
bool CheckParal(tVector &a, tVector& b) {
    double aa = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    double bb = sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
    double sk = ScalarMult(a, b);
    double angle = sk / aa / bb;
    qDebug() << angle;

    return fabs(angle) < 0.05;
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
        //DebugPrintPolynom(polynom);
        //PrintResPolynom(scene, resPolynom, Qt::green);
        //scene->sleepFeature(1000);
        resPolynom.clear();

    }
    resPolynom = polynom;
}
void PrintResPolynom(paintScene *scene, vector<tSegment> &segments, const QColor &colorBrush) {
    for(int i = 0; i < segments.size(); i++) {
        scene->addMyLine(segments[i].p1,segments[i].p2, colorBrush, 3);
    }
}
void GetSegmentsFromVertex(vector<tSegment> &segments, vector<tPoint> &resPolynom)
{
    for(int i = 0; i < resPolynom.size() - 1; i++) {
        if(!resPolynom[i].isEqual(resPolynom[i+1])) {
            if(resPolynom[i].x <= resPolynom[i + 1].x) {
                segments.push_back(tSegment(resPolynom[i],resPolynom[i+1]));
            }
            else {
                segments.push_back(tSegment(resPolynom[i+1],resPolynom[i]));
            }
        }
    }
}
void DeleteDoubleAndNullSegments(vector<tSegment> &segments)
{
    vector<tSegment> resSegment;
    for(int i = 0; i < segments.size(); i++) {
        bool flag = true;
        if(segments[i].empty)
            continue;
        for(int j = 0; j < resSegment.size(); j++) {
            if(segments[i].isEqual(resSegment[j])) {
                flag = false;
                break;
            }
        }
        if(flag)
            resSegment.push_back(segments[i]);
    }
    segments = resSegment;
}
bool IsSegmentsOnLineAndHaveIntersection(tSegment &s1, tSegment &s2) {
    tVector v1(s1);
    tVector v2(s2);
    tVector h(v1.y, -v1.x);
    if(CheckParal(h, v2)) {
        tVector v3(s1.p1, s2.p1);
        qDebug() << "hey";
        if(CheckParal(h, v3)) {
            qDebug() << "lala";
            if(s1.p1.x == s1.p2.x) { //vertical segments
                if(s1.p1.y <= s2.p2.y && s1.p1.y >= s2.p1.y)
                    return true;
                if(s1.p2.y <= s2.p2.y && s1.p2.y >= s2.p1.y)
                    return true;
            }
            else {
                if(s1.p1.x <= s2.p2.x && s1.p1.x >= s2.p1.x)
                    return true;
                if(s1.p2.x <= s2.p2.x && s1.p2.x >= s2.p1.x)
                    return true;
            }
        }
    }
    return false;
}
void AddToListSegmentsResultsOfIntersection(vector<tSegment> &segments, tSegment &s1, tSegment &s2) {
    if(s1.p1.x == s2.p2.x || s2.p1.x == s1.p2.x)
        return;
    s1.empty = true;
    s2.empty = true;
    if(s1.p1.isEqual(s2.p1) &&  s1.p2.isEqual(s2.p2))
        return;
    bool flagchange = false;
    if(s1.p1.x == s1.p1.x) {
        if(s2.p1.y < s1.p1.y) {
            segments.push_back(tSegment(s2.p1, s1.p1));
        }
        else {
            segments.push_back(tSegment(s1.p1, s2.p1));
        }
        tPoint t, end;
        if(s2.p2.y <= s1.p2.y) {
            t = s2.p2;
            end = s1.p2;
        }
        else {
            t = s1.p2;
            end = s2.p2;
        }
        flagchange = true; //TODO;
         segments.push_back(tSegment(t, end));
         return;
    }

    if(s2.p1.x < s1.p1.x) {
        segments.push_back(tSegment(s2.p1, s1.p1));
    }
    else {
        segments.push_back(tSegment(s1.p1, s2.p1));
    }
    tPoint t, end;
    if(s2.p2.x <= s1.p2.x) {
        t = s2.p2;
        end = s1.p2;
    }
    else {
        t = s1.p2;
        end = s2.p2;
    }
    //segments.push_back(tSegment(s1.p1, t)); //del
    //segments.push_back(tSegment(s1.p1, t)); //del
    segments.push_back(tSegment(t, end));
}

void FindOverlappingSegments(vector<tSegment> &segments)
{
   // vector<tSegment> resSegment;
    for(int i = 0; i < segments.size(); i++) {
        if(!segments[i].empty) {
            for(int j = 0; j < segments.size(); j++) {
                //qDebug() << "hop";
                if(segments[j].empty || i == j)
                    continue;
                if(IsSegmentsOnLineAndHaveIntersection(segments[i], segments[j])) {
                    qDebug() << "ok";
                    AddToListSegmentsResultsOfIntersection(segments, segments[i], segments[j]);
                }
            }
        }
    }
  //  segments = resSegment;

    DeleteDoubleAndNullSegments(segments);
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
    vector<tSegment> segments;
    GetSegmentsFromVertex(segments, resPolynom);
    FindOverlappingSegments(segments);

    PrintResPolynom(scene, segments, colorBrush);

    return 0;
}

