#include "Tri.h"

Tri::Tri()
{
	points.resize(0);
	neighorTri.resize(0);
	edge.resize(0);

	circleCenter = Vertex(0, 0, 0, 1.0, 0, 1.0);
	radius = 0;

	normal = Vec3();
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

	cercleCirconscrit();

	normal = (points[1]->GetPos() - points[0]->GetPos()) ^ (points[2]->GetPos() - points[0]->GetPos()).normalise();

	updateBuffers();
	neighorTri.resize(0);

}

void Tri::cercleCirconscrit()
{
	double yDelta_a = points[1]->y - points[0]->y;
	double xDelta_a = points[1]->x - points[0]->x;
	double yDelta_b = points[2]->y - points[1]->y;
	double xDelta_b = points[2]->x - points[1]->x;

	if (fabs(xDelta_a) <= 0.00001 && fabs(yDelta_b) <= 0.00001) {
		double x = 0.5 * (points[1]->x + points[2]->x);
		double y = 0.5 * (points[0]->y + points[1]->y);
		circleCenter = Vertex(x, y, 0, 1.0, 0.0, 1.0);
		radius = (circleCenter.GetPos() - points[0]->GetPos()).magnitude();

		return;
	}

	double aSlope = yDelta_a / xDelta_a;
	double bSlope = yDelta_b / xDelta_b;
	if (fabs(aSlope - bSlope) <= 0.00001) {	// checking whether the given points are colinear. 	
		return;
	}

	// calc center
	double x = (aSlope * bSlope * (points[0]->y - points[2]->y) + bSlope * (points[0]->x + points[1]->x)
		- aSlope * (points[1]->x + points[2]->x)) / (2 * (bSlope - aSlope));
	double y = -1 * (x - (points[0]->x + points[1]->x) / 2) / aSlope + (points[0]->y + points[1]->y) / 2;

	circleCenter = Vertex(x, y, 0, 1.0, 0.0, 1.0);
	radius = (circleCenter.GetPos() - points[0]->GetPos()).magnitude();
}

bool Tri::isPointInTriangle(Vertex p) const
{
	float s = points[0]->y * points[2]->x - points[0]->x * points[2]->y + (points[2]->y - points[0]->y) * p.x + (points[0]->x - points[2]->x) * p.y;
	float t = points[0]->x * points[1]->y - points[0]->y * points[1]->x + (points[0]->y - points[1]->y) * p.x + (points[1]->x - points[0]->x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -points[1]->y * points[2]->x + points[0]->y * (points[2]->x - points[1]->x) + points[0]->x * (points[1]->y - points[2]->y) + points[1]->x * points[2]->y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}

bool Tri::isPointInCircle(Vertex p) const
{
	return (circleCenter.GetPos() - p.GetPos()).magnitude() <= radius + 0.000001;
}

bool Tri::operator==(Tri t)
{
	int same = 0;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (points[i] == t.points[j])
			{
				same++;
				break;
			}
		}

		if (same <= i)
			return false;
	}

	return true;
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
		bufferPts[i].setNormal(normal.normalise());
	}
}