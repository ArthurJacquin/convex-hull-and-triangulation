#include "Tri.h"

Tri::Tri()
{
	points.resize(0);
	neighorTri.resize(0);
	edge.resize(0);

	center = Vertex(0, 0, 0) ;
	radius = 0;
}

Tri::Tri(Vertex* p1, Vertex* p2, Vertex* p3)
{
	//test sens du triangle 
	Vec3 AB = Vec3(p2->x - p1->x, p2->y - p1->y, 0);
	Vec3 AC = Vec3(p3->x - p1->x, p3->y - p1->y, 0);
	float deter = AB.getDeterminant(AC);
	
	points.push_back(p1);
	if (deter > 0)
	{
		points.push_back(p2);
		points.push_back(p3);

		//les edges
		edge.push_back(Edge(p1, p2));
		edge.push_back(Edge(p2, p3));
		edge.push_back(Edge(p3, p1));
	}
	else
	{
		points.push_back(p3);
		points.push_back(p2);

		//les edges
		edge.push_back(Edge(p1, p3));
		edge.push_back(Edge(p3, p2));
		edge.push_back(Edge(p2, p1));
	}

	updateBuffers();
	neighorTri.resize(0);
	center = Vertex(0, 0, 0);
	radius = 0;
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