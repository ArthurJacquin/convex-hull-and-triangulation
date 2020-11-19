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

//test si un point est visible par un edge
bool isVisible(Vertex v, Edge e)
{
	Vec3 v1 = Vec3(v.x - e.getEdgePoints()[0]->x, v.y - e.getEdgePoints()[0]->y, 0);
	float dot = v1.dot(e.getNormale());
	
	//std::cerr << "produit scalaire entre : " << e.getEdgePoints()[0]->GetPos().x << "/" << e.getEdgePoints()[1]->GetPos().x << " et " << v1.x << "/" << v1.y << "====>" << dot << std::endl;
	//std::cerr << "la normale :" << e.getNormale() << std::endl;

	if (dot > FLT_EPSILON)
	{
		return true;
	}
	else
		return false;
}

void checkIfEdgeExist(Edge e, vector<Edge>& edges)
{
	auto it = std::find(edges.begin(), edges.end(), e);
	if (it != edges.end())
		it->setInterior();
	else
		edges.push_back(e);
}

std::vector<Edge> triangulateIncremental(std::vector<Vertex>& S)
{
	vector<Vertex> testPoints{ Vertex(0, 0, 0), Vertex(-0.2, 0.5, 0), Vertex(-0.5, -0.2, 0), Vertex(0.5, 0.5, 0)};
	//S = testPoints;
	vector<Tri> triangulateTriTab;
	vector<Edge> edgeTriTab;

	//1 : sort vector by abscisse (utilise l'operator < dans Vertex.h)
	std::sort(S.begin(), S.end());

	//2 : faire un premier triangle
	if (S.size() > 2)
	{
		triangulateTriTab.push_back(Tri(&S[0], &S[1], &S[2]));

		for (int i = 0; i < 3; i++)
		{
			edgeTriTab.push_back(triangulateTriTab[0].getEdge()[i]);
		}
	}
	else
		std::cerr << "Pas assez de points pour faire un triangle ! " << std::endl;

	//3 : parcours des points suivants de S (sorted)
	for (int p = 3; p < S.size(); p++)
	{
		//check visibilité par rapport à tous les edges exterior
		for (int e = 0; e < edgeTriTab.size(); e++)
		{
			//si le point est visible et que la normale de l'edge est extérieure
			if (isVisible(S[p], edgeTriTab[e]) && edgeTriTab[e].getSide())
			{
				//std::cerr << S[p] << "visible par : " << triangulateTriTab[t].getEdge()[e].getEdgePoints()[0]->GetPos() << "|" << triangulateTriTab[t].getEdge()[e].getEdgePoints()[1]->GetPos() << std::endl;
				//triangle avec le nouveau point
				//triangulateTriTab.push_back(Tri(triangulateTriTab[t].getEdge()[e].getEdgePoints()[0], 
				//							&S[p], 
				//							triangulateTriTab[t].getEdge()[e].getEdgePoints()[1]));

				checkIfEdgeExist(Edge(edgeTriTab[e].getEdgePoints()[0], &S[p]), edgeTriTab);
				checkIfEdgeExist(Edge(&S[p], edgeTriTab[e].getEdgePoints()[1]), edgeTriTab);

				//triangulateTriTab[triangulateTriTab.size() - 1].getEdge()[2].setInterior();
				//l'edge n'est plus un bord
				edgeTriTab[e].setInterior();
				std::cerr << "laboucle" << std::endl;
			}
		}
	}

	return edgeTriTab;
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
