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
	Vec3 AB = Vec3(points[1]->x - points[0]->x, points[1]->y - points[0]->y, 0);
	Vec3 AC = Vec3(points[2]->x - points[0]->x, points[2]->y - points[0]->y, 0);

	Vec3 mediatriceAB = Vec3((points[0]->x + points[1]->x) / 2, (points[0]->y + points[1]->y) / 2, 0);
	Vec3 mediatriceAC = Vec3((points[0]->x + points[2]->x) / 2, (points[0]->y + points[2]->y) / 2, 0);

	float det = AB.x * AC.y - AB.y * AC.x;
	
	float centreX = (AC.y * (AB.x * mediatriceAB.x + AB.y * mediatriceAB.y) - AB.y * (AC.x * mediatriceAC.x + AC.y * mediatriceAC.y)) / det;
	float centreY = (-AC.x * (AB.x * mediatriceAB.x + AB.y * mediatriceAB.y) + AB.x * (AC.x * mediatriceAC.x + AC.y * mediatriceAC.y)) / det;
	
	circleCenter = Vertex(centreX, centreY, 0, 1.0, 0, 1.0);
	radius = (points[0]->GetPos() - circleCenter.GetPos()).magnitude();

	/*
	std::cerr << " ----" << std::endl;
	for (size_t i = 0; i < points.size(); i++)
	{
		std::cerr << (points[i]->GetPos() - center).magnitude() << std::endl;
	}
	*/
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