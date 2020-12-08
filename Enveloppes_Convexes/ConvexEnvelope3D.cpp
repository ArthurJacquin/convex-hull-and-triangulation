#include "ConvexEnvelope3D.h"
#include <iostream>

ConvexEnvelope3D::ConvexEnvelope3D()
{
	triangles.reserve(0);
}

ConvexEnvelope3D::ConvexEnvelope3D(const vector<Tri> tri)
{
	triangles = tri;
}

void ConvexEnvelope3D::Clear()
{
	triangles.clear();
}
