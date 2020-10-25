#include "Tri.h"

Tri::Tri()
{
	points.resize(0);
	neighorTri.resize(0);

	center = Vertex(0, 0, 0) ;
	radius = 0;
}

Tri::Tri(Vertex* p1, Vertex* p2, Vertex* p3)
{
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);

	neighorTri.resize(0);
	center = Vertex(0, 0, 0);
	radius = 0;
}
