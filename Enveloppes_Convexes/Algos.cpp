#include "Algos.h"
#include <algorithm>
#include "Vec3.h"

Vertex FindBarycenter(std::vector<Vertex> S)
{
	size_t const size = S.size();
	if (size <= 0)
		return Vertex(0, 0, 0);
	
	double avg_x = 0;
	double avg_y = 0;
	double avg_z = 0;

	for(size_t i = 0; i < size; ++i)
	{
		avg_x += S[i].x;
		avg_y += S[i].y;
		avg_z += S[i].z;
	}

	avg_x /= size;
	avg_y /= size;
	avg_z /= size;
	
	return Vertex(avg_x, avg_y, avg_z);
}

ConvexEnvelope Jarvis(std::vector<Vertex>& S)
{
	int i0 = 0;
	float xmin = S[i0].x;
	float ymin = S[i0].y;

	//Recherche du point le plus a gauche
	for (size_t i = i0; i < S.size(); i++)
	{
		if (S[i].x < xmin || (S[i].x == xmin && S[i].y < ymin))
		{
			i0 = i;
			xmin = S[i0].x;
			ymin = S[i0].y;
		}
	}

	Vec3 v(0, -1, 0);	//Initialisation du vecteur directeur
	std::vector<Vertex*> P;	// Polygone de l'enveloppe convexe
	int i = i0; // Indice du 1e sommet
	int j = 0;

	do
	{
		//Ajout du point
		Vertex* vert = &S[i];
		vert->setColor(Color(1.0f, 0.0f, 0.0f));
		P.push_back(vert);

		//Initialisation des min avec le 1er point d'indice != de i
		if (i == 0)
			j = 1;
		else
			j = 0;

		Vec3 PiPj = S[j].GetPos() - S[i].GetPos();
		float alphaMin = v.Angle(PiPj);
		float lmax = PiPj.magnitude();
		int iNew = j;

		//Recherche du point le plus proche (en angle) de la droite
		for (size_t j = iNew + 1; j < S.size(); j++)
		{
			if (j != i)
			{
				Vec3 PiPj = S[j].GetPos() - S[i].GetPos();
				float alpha = v.Angle(PiPj);
				float l = PiPj.magnitude();

				if (alphaMin > alpha || (alphaMin == alpha && lmax < l))
				{
					alphaMin = alpha;
					lmax = l;
					iNew = j;
				}
			}
		}

		//Mise à jour du pivot et du vecteur directeur
		v = (S[iNew].GetPos() - S[i].GetPos());
		i = iNew;

	} while (i != i0);

	ConvexEnvelope envelope(P);
	return envelope;
}

ConvexEnvelope GrahamScan(std::vector<Vertex>& S)
{
	size_t const size = S.size();
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	float const pi = 3.14159265359f;
	Vec3 Ox = Vec3(1.0f, 0.0f, 0.0f); // vecteur origine
	Vertex const B = FindBarycenter(S); // barycentre
	
	// Polygone de l'enveloppe convexe
	std::vector<Vertex*> P;
	std::vector<Vertex>& tmp = S;

	float angle_min = 999999999.0f;
	float dist_min = 999999999.0f;
	j = size - 1;
	size_t min = -1;

	// Tri selon l'angle orienté
	do
	{
		// Recherche de l'angle orienté le plus petit
		for(i = 0; i < tmp.size(); ++i)
		{
			Vec3 BPi = tmp[i].GetPos() - B.GetPos();
			float angle = Ox.AngleClockwise(BPi); // angle orienté à l'origine du vecteur BPi
			if (angle < 0) 
			{
				angle = 2 * pi + angle;
			}
			float dist = BPi.magnitude(); // distance du point Pi avec le barycentre B
			if (angle_min > angle || (abs(angle_min - angle) <= 0.01f && dist_min > dist))
			{
				angle_min = angle;
				dist_min = dist;
				min = i;
			}
		}

		// ajout du minimum et suppression de tmp
		P.push_back(&tmp[min]);
		tmp.erase(tmp.begin() + min);

		// reset des valeurs
		angle_min = 999999999.0f;
		dist_min = 999999999.0f;
		min = -1;
		
		j--;
	} while (j != -1);

	size_t count = 0;
	i = 0;
	
	do
	{
		j = i == 0 ? P.size() - 1 : i - 1; // i - 1
		k = (i + 1) % P.size(); // i + 1
		
		Vec3 PiPj = P[j]->GetPos() - P[i]->GetPos();
		Vec3 PiPk = P[k]->GetPos() - P[i]->GetPos();
		float angle = PiPj.AngleClockwise(PiPk);
		if (angle < 0)
		{
			angle = 2 * pi + angle;
		}
		
		//Suppression des points non convexes
		if (angle <= pi) 
		{
			P.erase(P.begin() + i);
			i = i == 0 ? P.size() - 1 : i - 1;
			count--;
		}
		else
		{
			i = (i + 1) % P.size();
			count++;
		}
	} while (count != P.size() - 1);
	
	ConvexEnvelope envelope(P);
	return envelope;
};

Vertex nearestVertex(Vertex v, std::vector<Vertex> tabV)
{
	Vertex vNear;
	float min = 10000.f;
	float distance = 5000.f;
	for (int i = 0; i < tabV.size(); ++i)
	{
		if (tabV[i] != v)
		{

			distance = v.Distance(tabV[i]);

			if (distance < min)
			{
				min = distance;
				vNear = tabV[i];
			}
		}
	}

	return vNear;
}

bool isVisible(Vertex v, std::vector<Vertex> tabV)
{
	return true;
}

std::vector<Tri> triangulateIncremental(std::vector<Vertex>& S)
{
	//									P               Q              R
	//vector<Vertex> sortPoints{ Vertex(0, 0, 0), Vertex(0, 0.3, 0), Vertex(0.5, 0, 0), Vertex(0,- 0.6, 0), Vertex(-0.6, 0, 0) };
	vector<Tri> triangulateTriTab;
	vector<Vertex*> vertexEnveloppeConvexe;
	vector<Vertex*> vertexInterior;

	//1 : sort vector by abscisse (utilise l'operator < dans Vertex.h)
	std::sort(S.begin(), S.end());
	
	//2 : enveloppe convexe
	vertexEnveloppeConvexe = Jarvis(S).GetPoints();
	
	//3 : remplir tableau de vertex interieur
	for (int i = 0; i < S.size(); i++)
	{
		bool dedans = false;
		for(int j = 0; j < vertexEnveloppeConvexe.size(); j++)
		{
			if (&S[i] == vertexEnveloppeConvexe[j])
				dedans = true;
				
		}
		if(dedans == false)
			vertexInterior.push_back(&S[i]);
	}

	//4 : faire un premier triangle
	if (S.size() > 2)
		triangulateTriTab.push_back(Tri(&S[0], &S[1], &S[2]));
	else
		std::cerr << "Pas assez de points pour faire un triangle ! " << std::endl;



	/*
	Vertex* neighor;
	for (int i = 0; i < vertexInterior.size(); i++)
	{
		//rajout du point le plus proche dans le vertex 
		vertexInterior[i]->addNeighborVertices(&nearestVertex(sortPoints[i], sortPoints));

		float cosAngle = 1.f;

		for (int j = 0; j < vertexInterior.size(); j++)
		{
			if (vertexInterior[j] != vertexInterior[i] && vertexInterior[j] != vertexInterior[i]->getNeighborVertices()[0])
			{
				Vec3 PQ = vertexInterior[i]->getNeighborVertices()[0]->GetPos() - vertexInterior[i]->GetPos();
				//entre P et R
				Vec3 Pj = vertexInterior[j]->GetPos() - vertexInterior[i]->GetPos();
				float determinant = PQ.getDeterminant(Pj);

				//sûr qu'on tourne vers la droite
				if (determinant < 0)
				{
					//RP
					Vec3 jP = vertexInterior[i]->GetPos() - vertexInterior[j]->GetPos();
					//RQ
					Vec3 jQ = vertexInterior[i]->getNeighborVertices()[0]->GetPos() - vertexInterior[j]->GetPos();
					if (cos(jP.Angle(jQ)) < cosAngle);
					{
						cosAngle = cos(jP.Angle(jQ));
						neighor = vertexInterior[j];
					}
				}
			}
		}
		vertexInterior[i]->addNeighborVertices(neighor);
	}
	*/
	/*
	for (int i = 0; i < sortPoints.size(); ++i)
	{
		int neighborSize = sortPoints[i].getNeighborVertices().size();
		for (int j = 0; j < neighborSize; ++j)
		{
			triangulateTriTab.push_back(Tri(&sortPoints[i], sortPoints[i].getNeighborVertices()[j % neighborSize], sortPoints[i].getNeighborVertices()[(j + 1) % neighborSize]));
		}
	}
	*/
	return triangulateTriTab;
}

//Check if point is in triangle
float sign(Vertex p1, Vertex p2, Vertex p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(Vertex pt, Vertex v1, Vertex v2, Vertex v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

//Delaunay algorithm
std::vector<Tri> triangulateDelaunay(std::vector<Vertex>& S)
{
	return std::vector<Tri>();
}
