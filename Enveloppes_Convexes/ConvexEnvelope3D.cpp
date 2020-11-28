#include "ConvexEnvelope3D.h"
#include <iostream>

ConvexEnvelope3D::ConvexEnvelope3D()
{
	triangles.reserve(0);
}

ConvexEnvelope3D::ConvexEnvelope3D(const vector<Tri> tri)
{
	triangles = tri;
	//CalculateNormals();
}

void ConvexEnvelope3D::Clear()
{
	triangles.clear();
}

void ConvexEnvelope3D::CalculateNormals()
{
	for (size_t t = 0; t < triangles.size(); t++)
	{
		for (size_t p = 0; p < 3; p++)
		{
			Vec3 sum(0, 0, 0);
			int nbNormal = 0;

			for (size_t t2 = 0; t2 < triangles.size(); t2++)
			{
				for (size_t p2 = 0; p2 < 3; p2++)
				{
					if (triangles[t].getPoints()[p] == triangles[t2].getPoints()[p2])
					{
						sum += triangles[t].getNormal();
						nbNormal++;
						break;
					}
				}
			}

			triangles[t].getPoints()[p]->setNormal((sum / nbNormal).normalise());
		}
	}
}
