#include "Tri.h"

Tri::Tri()
{
	points.resize(0);
	neighorTri.resize(0);
	triEdge.resize(0);

	center = Vertex(0, 0, 0) ;
	radius = 0;
}

Tri::Tri(Vertex* p1, Vertex* p2, Vertex* p3)
{
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	updateBuffers();

	neighorTri.resize(0);
	center = Vertex(0, 0, 0);
	radius = 0;

	//les edges
	triEdge.push_back(Edge(p1, p2));
	triEdge.push_back(Edge(p2, p3));
	triEdge.push_back(Edge(p3, p1));

}

void Tri::updateBuffers()
{
	updateBufferPoints();
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * bufferPts.size(), bufferPts.data());
}

void Tri::updateBufferPoints()
{
	for (size_t i = 0; i < points.size(); i++)
	{
		bufferPts.push_back(*points[i]);
	}
}