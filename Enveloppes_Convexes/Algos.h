#include "Tri.h"
#include "ConvexEnvelope.h"
#include "ConvexEnvelope3D.h"
#include "Triangulation.h"
#include "Tetrahedre.h"

Vertex FindBarycenter(std::vector<Vertex> S);

ConvexEnvelope Jarvis(std::vector<Vertex>& S);

ConvexEnvelope GrahamScan(std::vector<Vertex>& S);

ConvexEnvelope3D Envelope3D(std::vector<Vertex>& S);

Triangulation triangulateIncremental(std::vector<Vertex>& S);

float sign(Vertex p1, Vertex p2, Vertex p3);
bool PointInTriangle(Vertex pt, Vertex v1, Vertex v2, Vertex v3);

Triangulation triangulateDelaunay(std::vector<Vertex>& S);