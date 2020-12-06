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

Triangulation triangulateDelaunay(std::vector<Vertex>& S);

Triangulation coreDelaunay(std::vector<Vertex>& S);

void removeCoreDelaunay(Triangulation& T, Vertex* p);

Triangulation voronoiDiagram(std::vector<Vertex>& S);