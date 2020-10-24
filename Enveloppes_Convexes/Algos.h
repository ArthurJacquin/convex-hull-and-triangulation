#include "ConvexEnvelope.h"

Vertex FindBarycenter(std::vector<Vertex> S);

ConvexEnvelope Jarvis(std::vector<Vertex> S);

ConvexEnvelope GrahamScan(std::vector<Vertex> S);