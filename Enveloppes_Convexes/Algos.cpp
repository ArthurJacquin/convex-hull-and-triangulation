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


//-------------------------------------Enveloppe Convex------------------------------------------
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

	// Tri selon l'angle orienté (tri à bulles)
	for(i = 0; i < tmp.size(); ++i)
	{
		bool sorted = true;

		// Recherche de l'angle orienté le plus petit
		for(k = 0; k < tmp.size() - 1; ++k)
		{
			Vec3 BPk = tmp[k].GetPos() - B.GetPos();
			float angle = Ox.AngleClockwise(BPk); // angle orienté à l'origine du vecteur BPk
			if (angle < 0)
			{
				angle = 2 * pi + angle;
			}
			float dist = BPk.magnitude(); // distance du point Pk avec le barycentre B

			// pareil pour le point à l'index k + 1
			Vec3 BPkPlusOne = tmp[k + 1].GetPos() - B.GetPos();
			float  angle_next = Ox.AngleClockwise(BPkPlusOne);
			if (angle_next < 0)
			{
				angle_next = 2 * pi + angle_next;
			}
			float dist_next = BPkPlusOne.magnitude();

			// comparaison des valeurs pour déterminer le plus petit
			if (angle_next < angle || (abs(angle_next - angle) <= 0.01f && dist_next < dist))
			{
				iter_swap(tmp.begin() + k + 1, tmp.begin() + k);
				sorted = false;
			}
		}
		if (sorted) break;
	}

	for(j = 0; j < size; ++j)
	{
		// ajout des points triés
		P.push_back(&tmp[j]);
	}

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
}

//-------------------------------------Enveloppe Convex 3D------------------------------------------
int checkIfTriExist(Tri t, vector<Tri> tris)
{
	for (size_t i = 0; i < tris.size(); i++)
	{
		if (t == tris[i])
			return i;
	}

	return -1;
}

ConvexEnvelope3D Envelope3D(std::vector<Vertex>& S)
{
	std::vector<Tri> triangles;
	vector<Tetrahedre> tetras;

	//1 : sort vector by abscisse (utilise l'operator < dans Vertex.h)
	std::sort(S.begin(), S.end());

	//2 : faire un premier tetrahedre
	if (S.size() > 4)
	{
		tetras.push_back(Tetrahedre(&S[0], &S[1], &S[2], &S[3]));

		for (size_t i = 0; i < tetras[0].getTriangle().size(); i++)
		{
			triangles.push_back(tetras[0].getTriangle()[i]);
		}
	}
	else
		std::cerr << "Pas assez de points pour faire un tetrahedre ! " << std::endl;

	//3 : parcours des points suivants de S (sorted)
	for (int p = 4; p < S.size(); p++)
	{
		std::vector<Tri> newTriangles;

		//check visibilité par rapport à tous les triangles exterior
		for (int t = 0; t < triangles.size(); t++)
		{
			//Get le tetrahedre contenant le triangle
			Tetrahedre currentTetra;
			Tri currentTri;
			for (size_t i = 0; i < tetras.size(); i++)
			{
				int id = tetras[i].containsTri(triangles[t]);
				if (id != -1)
				{
					currentTetra = tetras[i];
					currentTri = tetras[i].getTriangle()[id];
					break;
				}
			}
			
			Vec3 AS = S[p].GetPos() - currentTri.getPoints()[0]->GetPos();

			//Si visible depuis le triangle
			if (currentTri.getNormal().dot(AS) > 0)
			{
				//Nouveau tetrahedre
				Tetrahedre newTetra(currentTri.getPoints()[0], currentTri.getPoints()[1], currentTri.getPoints()[2], &S[p]);
				tetras.push_back(newTetra);

				//Mise a jour des triangles exterieur
				for (size_t i = 1; i < 4; i++)
				{
					int id = checkIfTriExist(newTetra.getTriangle()[i], newTriangles);
					if (id != -1)
						newTriangles.erase(newTriangles.begin() + id);
					else
						newTriangles.push_back(newTetra.getTriangle()[i]);

				}

				//Suppression de triangle
				triangles.erase(triangles.begin() + t);
				t--;
			}
		}

		//Ajout des nouveaux triangles
		for (size_t i = 0; i < newTriangles.size(); i++)
		{
			triangles.push_back(newTriangles[i]);
		}
	}

	return ConvexEnvelope3D(triangles);

}

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

//-------------------------------------Triangulation------------------------------------------

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

Triangulation triangulateIncremental(std::vector<Vertex>& S)
{
	Triangulation laTri;
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
			if (isVisible(S[p], edgeTriTab[e]) && edgeTriTab[e].getExterior())
			{
				//triangle avec le nouveau point
				triangulateTriTab.push_back(Tri(edgeTriTab[e].getEdgePoints()[0],
											&S[p], 
											edgeTriTab[e].getEdgePoints()[1]));

				checkIfEdgeExist(Edge(edgeTriTab[e].getEdgePoints()[0], &S[p]), edgeTriTab);
				checkIfEdgeExist(Edge(&S[p], edgeTriTab[e].getEdgePoints()[1]), edgeTriTab);

				//l'edge n'est plus un bord
				edgeTriTab[e].setInterior();
			}
		}
	}

	laTri.edge = edgeTriTab;
	laTri.tri = triangulateTriTab;

	return laTri;
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

Vertex projectPoint(Vertex A, Vertex B, int factor)
{
	Vec3 projectPoint = Vec3(A.x, A.y, A.z);
	Vec3 referencePt = Vec3(B.x, B.y, B.z);

	Vec3 dir = referencePt - projectPoint;
	Vec3 result = projectPoint + dir * factor;

	return Vertex(result.x, result.y, 0, 0, 1, 0);
}

//check si le troisième point est dans le cercle circonscrit
bool critereDelaunay(Tri t1, Tri t2)
{
	bool exterior = false;
	//std::cerr << "---------CRITERE-----------" << std::endl;
	for (int i = 0; i < t2.getPoints().size(); i++)
	{
		float dist = (t2.getPoints()[i]->GetPos() - t1.getCenter()).magnitude();
		
		//std::cerr << "distance avec " << t2.getPoints()[i]->GetPos() << " = " << dist << std::endl;
		//std::cerr << "radius :  " << t1.getRadius() << std::endl;
		//std::cerr << "----" << std::endl;

		if (dist > t1.getRadius() + FLT_EPSILON)
			exterior = true;
	}

	if (exterior)
		return true;
	else
		return false;
}

//Delaunay algorithm
Triangulation triangulateDelaunay(std::vector<Vertex>& S)
{
	Triangulation laTri = triangulateIncremental(S);

	//flipping d'arete
	for (int i = 0; i < laTri.edge.size(); ++i)
	{
		if (laTri.edge[i].getExterior() == true)
			continue;

		std::vector<int> indexTri;
		std::vector<Vertex*> vertexNotInEdge;

		//parcours des triangles pour trouver le triangle avec son double
		for (int t = 0; t < laTri.tri.size(); t++)
		{
			for (int e = 0; e < laTri.tri[t].getEdge().size(); e++)
			{
				if (laTri.edge[i] == laTri.tri[t].getEdge()[e])
				{
					indexTri.push_back(t);
				}
			}
		}

		if (indexTri.size() > 1)
		{
			//parcours sur les triangles qui sont sur l'edge pour stocker les vertices différents de l'edge
			for(int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					if (laTri.tri[indexTri[j]].getPoints()[k] != laTri.edge[i].getEdgePoints()[0]
						&& laTri.tri[indexTri[j]].getPoints()[k] != laTri.edge[i].getEdgePoints()[1])
					{
						vertexNotInEdge.push_back(laTri.tri[indexTri[j]].getPoints()[k]);
						break;
					}
				}
			}


			//flipping d'arete
			if (critereDelaunay(laTri.tri[indexTri[0]], laTri.tri[indexTri[1]]) == false)
			{
				//New triangles
				laTri.tri[indexTri[0]] = Tri(vertexNotInEdge[0], vertexNotInEdge[1], laTri.edge[i].getEdgePoints()[0]);
				laTri.tri[indexTri[1]] = Tri(vertexNotInEdge[0], vertexNotInEdge[1], laTri.edge[i].getEdgePoints()[1]);

				//New edge
				Edge newEdge = Edge(vertexNotInEdge[0], vertexNotInEdge[1]);
				newEdge.setInterior();
				laTri.edge[i] = newEdge;

				//DEBUG : point en vert
				//newEdge.getEdgePoints()[0]->setColor(Color(0, 1, 0));
				//newEdge.getEdgePoints()[1]->setColor(Color(0, 1, 0));
			}
		}
	}

	return laTri;
}

//Voronoi from Delaunay algorithm
Triangulation voronoiDiagram(std::vector<Vertex>& S)
{	
	Triangulation delaunay = triangulateDelaunay(S);

	std::vector<Edge> voronoiEdge;
	
	for (int i = 0; i < delaunay.edge.size(); ++i)
	{
		if (delaunay.edge[i].getExterior() == true)
		{
			int indexTriangle = 0;
			
			//trouver le triangle qui utilise cette edge
			for(int t = 0; t < delaunay.tri.size(); ++t)
			{
				for(int e = 0; e < delaunay.tri[t].getEdge().size(); ++e)
				{
					if(delaunay.edge[i] == delaunay.tri[t].getEdge()[e])
					{
						indexTriangle = t;
						break;
					}
				}
			}
			
			S.emplace_back(Vertex(delaunay.tri[indexTriangle].getCenterCirclePoint()));
			if (delaunay.isPointInTriangulation(delaunay.tri[indexTriangle].getCenterCirclePoint()))
			{
				
				S.emplace_back(projectPoint(Vertex(delaunay.tri[indexTriangle].getCenterCirclePoint()), Vertex(delaunay.edge[i].getMiddleEdgePoint()),10));
			}
			else
			{
				//projection du milieu de l'edge de l'autre côté
				S.emplace_back(projectPoint(Vertex(delaunay.edge[i].getMiddleEdgePoint()), Vertex(delaunay.tri[indexTriangle].getCenterCirclePoint()), 10));
			}			
			
			Edge edge = Edge(&S[S.size() - 1], &S[S.size() - 2]);
			
			voronoiEdge.push_back(edge);
		}
		else
		{
			std::vector<int> indexTriangle;

			//trouver les deux triangles qui utilisent cette edge
			for (int t = 0; t < delaunay.tri.size(); ++t)
			{
				for (int e = 0; e < delaunay.tri[t].getEdge().size(); ++e)
				{
					if (delaunay.edge[i] == delaunay.tri[t].getEdge()[e])
					{
						indexTriangle.push_back(t);
					}
				}
			}

			if (indexTriangle.size() < 2) continue;

			S.emplace_back(Vertex(delaunay.tri[indexTriangle[0]].getCenterCirclePoint()));
			S.emplace_back(Vertex(delaunay.tri[indexTriangle[1]].getCenterCirclePoint()));
			
			Edge edge = Edge(&S[S.size() - 1], &S[S.size() - 2]);

			voronoiEdge.push_back(edge);
			
		}
	}

	for(int i = 0; i < voronoiEdge.size(); ++i)
	{
		delaunay.edge.emplace_back(voronoiEdge[i]);
	}

	Triangulation voronoi;
	voronoi.edge = voronoiEdge;
	return delaunay;
}
