#include "ConvexEnvelope.h"
#include "Tri.h"

Vertex FindBarycenter(std::vector<Vertex> S);

ConvexEnvelope Jarvis(std::vector<Vertex>& S);

ConvexEnvelope GrahamScan(std::vector<Vertex>& S);

std::vector<Edge> triangulateIncremental(std::vector<Vertex>& S);

float sign(Vertex p1, Vertex p2, Vertex p3);
bool PointInTriangle(Vertex pt, Vertex v1, Vertex v2, Vertex v3);

std::vector<Tri> triangulateDelaunay(std::vector<Vertex>& S);