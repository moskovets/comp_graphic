#ifndef REMOVE_SEGMENTS_ALGO_H
#define REMOVE_SEGMENTS_ALGO_H
#include "base_algo.h"
#include <vector>
using namespace std;
void GetSegmentsFromVertex(vector<tSegment> &segments, vector<tPoint> &resPolynom);
void FindOverlappingSegments(vector<tSegment> &segments);


#endif // REMOVE_SEGMENTS_ALGO_H
