#include "Edge.h"

Edge::Edge()
{
	edgePoints.reserve(0);
	exterior = true;
	normale = Vec3(0, 0, 0);
}

Edge::Edge(Vertex* v1, Vertex* v2)
{
	edgePoints.reserve(2);
	edgePoints.push_back(v1);
	edgePoints.push_back(v2);

	exterior = true;
	Vec3 e = Vec3(v2->x - v1->x, v2->y - v1->y);
	Vec3 e2 = Vec3(0, 0, 1);
	normale = e^e2;
	normale.normalise();
}
