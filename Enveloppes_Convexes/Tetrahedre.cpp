#include "Tetrahedre.h"

Tetrahedre::Tetrahedre()
{
	points.reserve(0);
	triangles.reserve(0);
}

Tetrahedre::Tetrahedre(Vertex* p0, Vertex* p1, Vertex* p2, Vertex* p3)
{
	points.push_back(p0);
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);

	triangles.push_back(Tri(p0, p1, p2));
	triangles.push_back(Tri(p0, p1, p3));
	triangles.push_back(Tri(p0, p3, p2));
	triangles.push_back(Tri(p1, p3, p2));

	//check normal
	for (size_t t = 0; t < triangles.size(); t++)
	{
		//get le point qui n'est pas dans le triangle
		Vertex* p = nullptr;
		for (size_t i = 0; i < points.size(); i++)
		{
			if (triangles[t].getPoints()[0] != points[i] && triangles[t].getPoints()[1] != points[i] && triangles[t].getPoints()[2] != points[i])
			{
				p = points[i];
				break;
			}
		}

		if (p == nullptr)
			return;

		if (triangles[t].getNormal().dot(p->GetPos() - triangles[t].getPoints()[0]->GetPos()) > 0)
			triangles[t].setNormal(-triangles[t].getNormal());
	}
}

int Tetrahedre::containsTri(Tri t)
{
	for (size_t i = 0; i < triangles.size(); i++)
	{
		if (triangles[i] == t)
			return i;
	}

	return -1;
}
