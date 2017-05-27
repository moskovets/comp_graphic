#include "remove_segments_algo.h"
#define EPS_PARAL 0.05
#include <QDebug>

// проверка параллельности векторов через поиск угла
bool CheckParal(tVector &a, tVector& b)
{
    double aa = sqrt(double(a.x * a.x + a.y * a.y + a.z * a.z));
    double bb = sqrt(double(b.x * b.x + b.y * b.y + b.z * b.z));
    double sk = ScalarMult(a, b);
    double angle = sk / aa / bb;
    return fabs(angle) < EPS_PARAL;
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

// проверка лежат ли отрезки на одной прямой и пересекаются
bool IsSegmentsOnLineAndHaveIntersection(tSegment &s1, tSegment &s2)
{
    tVector v1(s1);
    tVector v2(s2);
    tVector h(v1.y, -v1.x);
    if(CheckParal(h, v2)) // проверка параллельности
    {
        tVector v3;
        if(!s1.p1.isEqual(s2.p1))
            v3 = tVector(s1.p1, s2.p1);
        else if(!s1.p2.isEqual(s2.p1))
            v3 = tVector(s1.p2, s2.p1);
        else if(!s1.p2.isEqual(s2.p2))
            v3 = tVector(s1.p2, s2.p2);
        else
            v3 = tVector(s1.p1, s2.p2);

        if(CheckParal(h, v3)) { // проверка принадлежности одной прямой
            // проверка наличия пересечений
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

//разбиение пересекающихся отрезков на части, добавление "уникальных" отрезков в список, удаление исходных
void AddToListSegmentsResultsOfIntersection(vector<tSegment> &segments, tSegment &s1, tSegment &s2)
{
    if(s1.p1.x == s2.p2.x || s2.p1.x == s1.p2.x)
        return;
    s1.empty = true;
    s2.empty = true;
    if(s1.p1.isEqual(s2.p1) &&  s1.p2.isEqual(s2.p2)) // магия
        return;

    if(s1.p1.x == s1.p2.x) { //для вертикальных отрезков отдельно
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
        segments.push_back(tSegment(t, end));
        return;
    }
    if(s2.p1.x > s2.p2.x) {
        qDebug() << "aaaaaaa";
    }

    if(s2.p1.x < s1.p1.x) {
        segments.push_back(tSegment(s2.p1, s1.p1));
    }
    else if(s2.p1.x > s1.p1.x) {
        segments.push_back(tSegment(s1.p1, s2.p1));
    }
    tPoint t, end;
    if(s2.p2.x == s1.p2.x)
        return;
    if(s2.p2.x < s1.p2.x) {
        t = s2.p2;
        end = s1.p2;
    }
    else {
        t = s1.p2;
        end = s2.p2;
    }
    segments.push_back(tSegment(t, end));
}

void FindOverlappingSegments(vector<tSegment> &segments)
{
    for(int i = 0; i < segments.size(); i++) {
        if(!segments[i].empty) {
            for(int j = 0; j < segments.size(); j++) {
                if(segments[i].empty) break;
                if(segments[j].empty || i == j)
                    continue;
                if(IsSegmentsOnLineAndHaveIntersection(segments[i], segments[j])) {
                    AddToListSegmentsResultsOfIntersection(segments, segments[i], segments[j]);
                }
            }
        }
    }
    DeleteDoubleAndNullSegments(segments);
}
