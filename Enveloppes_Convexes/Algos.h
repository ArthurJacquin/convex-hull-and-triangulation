#include "ConvexEnvelope.h"
#include "Tri.h"

Vertex FindBarycenter(std::vector<Vertex> S);

ConvexEnvelope Jarvis(std::vector<Vertex> S);

ConvexEnvelope GrahamScan(std::vector<Vertex> S);

std::vector<Tri> triangulateIncremental(std::vector<Vertex> S);