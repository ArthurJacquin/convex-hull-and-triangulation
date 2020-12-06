#include "Algos.h"
#include <algorithm>
#include "Vec3.h"


#pragma region Enveloppe Convex

/// <summary>
/// Find the barycenter of all the given points
/// </summary>
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

/// <summary>
/// Generate the convex envelope using Jarvis algorithme
/// </summary>
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

/// <summary>
/// Generate the convex envelope using Graham Scan algorithme
/// </summary>
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

#pragma endregion


#pragma region Enveloppe Convex 3D

/// <summary>
/// Check if the triangle "t" is in the vector "tris"
/// </summary>
int checkIfTriExist(Tri t, vector<Tri> tris)
{
	for (size_t i = 0; i < tris.size(); i++)
	{
		if (t == tris[i])
			return i;
	}

	return -1;
}

/// <summary>
/// Find the nearest vertex of "v" in "tabV"
/// </summary>
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

/// <summary>
/// Generate the 3D convex envelope of the given points
/// </summary>
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

#pragma endregion


#pragma region Triangulation

/// <summary>
/// Test if the point "v" is visible from the edge "e" THE BEST
/// </summary>
bool isVisible(Vertex v, Edge e)
{
	Vec3 v1 = Vec3(v.x - e.getEdgePoints()[0]->x, v.y - e.getEdgePoints()[0]->y, 0);
	
	float dot = v1.dot(e.getNormale());
	
	return dot > FLT_EPSILON;
	
}

/// <summary>
/// Check if the edge "e" is in the vector "edges"
/// </summary>
void checkIfEdgeExist(Edge e, vector<Edge>& edges)
{
	auto it = std::find(edges.begin(), edges.end(), e);
	if (it != edges.end())
		it->setInterior();
	else
		edges.push_back(e);
}

/// <summary>
/// Generate an incremental triangulation with the given points
/// </summary>
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

/// <summary>
/// Check if the 2 triangles respect the Delaunay's criteria
/// </summary>
bool critereDelaunay(Tri t1, Tri t2)
{
	bool exterior = false;
	
	for (int i = 0; i < t2.getPoints().size(); i++)
	{
		double dist = (t2.getPoints()[i]->GetPos() - t1.getCenter()).magnitude();

		if (dist > t1.getRadius() + 0.00001)
			exterior = true;
	}

	for (int i = 0; i < t1.getPoints().size(); i++)
	{
		double dist = (t1.getPoints()[i]->GetPos() - t2.getCenter()).magnitude();
		if (dist > t2.getRadius() + 0.00001)
			exterior = true;
	}

	return exterior;
}

/// <summary>
/// Generate a Delaunay's triangulation by flipping edge of an incremental triangulation
/// </summary>>
Triangulation triangulateDelaunay(std::vector<Vertex>& S)
{
	Triangulation laTri = triangulateIncremental(S);

	bool done = false;
	do
	{
		done = false;
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
				for (int j = 0; j < 2; ++j)
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

					done = true;
					//DEBUG : point en vert
					//newEdge.getEdgePoints()[0]->setColor(Color(0, 1, 0));
					//newEdge.getEdgePoints()[1]->setColor(Color(0, 1, 0));
				}
			}
		}
	} while (done);

	return laTri;
}

/// <summary>
/// Generate a Delaunay's triangulation using Delaunay's cores
/// </summary>>
Triangulation coreDelaunay(std::vector<Vertex>& S)
{
	Triangulation coreDelaunay;
	
	if (S.size() <= 1)
		return coreDelaunay;
	
	
	if (S.size() == 2)
	{
		Edge edge1 = Edge(&S[0], &S[1]);
		coreDelaunay.edge.push_back(edge1);
		return coreDelaunay;
	}

	Tri triangle1 = Tri(&S[0], &S[1], &S[2]);

	for (size_t i = 0; i <3; i++)
	{
		coreDelaunay.edge.push_back(triangle1.getEdge()[i]);
	}
	
	coreDelaunay.tri.push_back(triangle1);
	
	if (S.size() == 3)
		return coreDelaunay;

	std::vector<Edge> L;
	
	for (size_t p = 3; p < S.size(); ++p)
	{

		//initialisation de L avec toutes les edge
		L.clear();
		int triIndex = coreDelaunay.isPointInTriangulationIndex(S[p]);
		if(triIndex != -1)
		{
			std::vector<int> indexEdges = coreDelaunay.GetEdgeIndexByTriangle(coreDelaunay.tri[triIndex]);
			for (size_t idx = 0; idx < indexEdges.size(); idx++)
			{
				L.push_back(coreDelaunay.edge[indexEdges[idx]]);
			}
			coreDelaunay.tri.erase(coreDelaunay.tri.begin() + triIndex);
		}
		else
		{
			for (size_t edgeIdx = 0; edgeIdx < coreDelaunay.edge.size(); edgeIdx++)
			{
				bool inTriangle = coreDelaunay.GetTriangleIndexByEdge(coreDelaunay.edge[edgeIdx]) != -1;

				if (!inTriangle) 
					L.push_back(coreDelaunay.edge[edgeIdx]);
				else if(isVisible(S[p], coreDelaunay.edge[edgeIdx]) && coreDelaunay.edge[edgeIdx].getExterior())
				{
					L.push_back(coreDelaunay.edge[edgeIdx]);
				}
			}
		}

		while(L.size() > 0)
		{
			Edge currentEdge = L[L.size() - 1];
			L.erase(L.end() - 1);
			
			int indexTriangle = coreDelaunay.GetTriangleIndexByEdge(currentEdge);
			int currentEdgeIdx = coreDelaunay.GetIndexEdge(currentEdge);
			
			//cas ou l'edge appartient a un triangle et le point est dans son cercle circonscrit
			if (indexTriangle != -1 && coreDelaunay.tri[indexTriangle].isPointInCircle(S[p]))
			{
				coreDelaunay.edge.erase(coreDelaunay.edge.begin() + currentEdgeIdx);
				for (size_t e = 0; e < 3; e++)
				{
					if (coreDelaunay.tri[indexTriangle].getEdge()[e] != currentEdge)
					{
						coreDelaunay.tri[indexTriangle].getEdge()[e].setExterior();
						L.push_back(coreDelaunay.tri[indexTriangle].getEdge()[e]);
					}
				}
				coreDelaunay.tri.erase(coreDelaunay.tri.begin() + indexTriangle);
			}
			else
			{
				//triangle avec le nouveau point
				Tri newTriangle = Tri(currentEdge.getEdgePoints()[0],
					&S[p],
					currentEdge.getEdgePoints()[1]);
				coreDelaunay.tri.push_back(newTriangle);

				for (size_t edge = 0; edge < 3; edge++)
				{
					if(currentEdge != newTriangle.getEdge()[edge])
						checkIfEdgeExist(newTriangle.getEdge()[edge], coreDelaunay.edge);
				}
				
				if(indexTriangle != -1)
					coreDelaunay.edge[currentEdgeIdx].setInterior();
			}
		}
	}
	
	return coreDelaunay;
}

void removeCoreDelaunay(Triangulation& T, Vertex* p)
{
	p->setColor(Color(1, 1, 1));

	//Si on a un edge, on veut supprimer cette edge et le point p
	if (T.edge.size() < 2)
	{
		T.edge.clear();
		return;
	}
	else if (T.edge.size() == 2)
	{
		std::vector<int> leEdge = T.GetEdgeFromPoint(p);
		std::vector<Vertex*> lesV;
		if (leEdge.size() > 1)
		{
			for (int i = 0; i < leEdge.size(); ++i)
			{
				if (T.edge[leEdge[i]].getEdgePoints()[0] != p)
					lesV.push_back(T.edge[leEdge[i]].getEdgePoints()[0]);
				else if (T.edge[leEdge[i]].getEdgePoints()[1] != p)
					lesV.push_back(T.edge[leEdge[i]].getEdgePoints()[1]);
			}

			T.edge.clear();
			T.edge.push_back(Edge(lesV[0], lesV[1]));
		}
		else
		{
			for (int i = 0; i < leEdge.size(); ++i)
			{
				if (T.edge[leEdge[i]].getEdgePoints()[0] == p || T.edge[leEdge[i]].getEdgePoints()[1] == p)
				{
					T.edge.erase(T.edge.begin() + i);
				}
			}
		}
		return;
	}

	//index des triangles et edge à supprimer
	std::vector<int> indexEdgeToRemove = T.GetEdgeFromPoint(p);
	std::vector<int> indexTriToRemove = T.GetTriFromPoint(p);

	//stock des edges du tri différents du point
	std::vector<Edge> edgeToKeep;
	for (int i = 0; i < indexTriToRemove.size(); ++i)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (T.tri[indexTriToRemove[i]].getEdge()[j].getEdgePoints()[0] != p && T.tri[indexTriToRemove[i]].getEdge()[j].getEdgePoints()[1] != p)
				edgeToKeep.push_back(T.tri[indexTriToRemove[i]].getEdge()[j]);
		}
	}

	std::vector<int> indexEdgeToKeep;
	//stock les edges de la triangulation différents du point
	for (int i = 0; i < T.edge.size(); ++i)
	{
		for (int j = 0; j < edgeToKeep.size(); ++j)
		{
			if (T.edge[i] == edgeToKeep[j])
			{
				indexEdgeToKeep.push_back(i);
				break;
			}
		}
	}

	std::vector<Vertex*> pointToTest;

	//suppression des edges de la tri
	std::sort(indexEdgeToRemove.begin(), indexEdgeToRemove.end(), greater<int>());
	for (int i = 0; i < indexEdgeToRemove.size(); ++i)
	{
		if (T.edge[indexEdgeToRemove[i]].getEdgePoints()[0] != p)
		{
			pointToTest.push_back(T.edge[indexEdgeToRemove[i]].getEdgePoints()[0]);
		}
		else
		{
			pointToTest.push_back(T.edge[indexEdgeToRemove[i]].getEdgePoints()[1]);
		}
		T.edge.erase(T.edge.begin() + indexEdgeToRemove[i]);
	}

	//suppression des tris de la tri
	std::sort(indexTriToRemove.begin(), indexTriToRemove.end(), greater<int>());
	for (int i = 0; i < indexTriToRemove.size(); ++i)
	{
		T.tri.erase(T.tri.begin() + indexTriToRemove[i]);
	}

	//check polygon fermé
	bool ferme = true;
	for (int i = 0; i < pointToTest.size(); ++i)
	{
		int compteur = 0;
		for (int j = 0; j < edgeToKeep.size(); ++j)
		{
			if (pointToTest[i] == edgeToKeep[j].getEdgePoints()[0] || pointToTest[i] == edgeToKeep[j].getEdgePoints()[1])
			{
				compteur++;
			}
		}

		if (compteur != 2)
		{
			ferme = false;
			break;
		}
	}

	if (ferme)
	{
		while (edgeToKeep.size() > 3)
		{
			bool createTri = false;

			for (int i = 0; i < edgeToKeep.size(); ++i)
			{
				for (int j = 0; j < edgeToKeep.size(); ++j)
				{
					if (i == j)
						continue;

					//les tests des indices pour le nouveau triangle
					Tri newTri;
					if (edgeToKeep[i].getEdgePoints()[0] == edgeToKeep[j].getEdgePoints()[0])
					{
						newTri = Tri(edgeToKeep[i].getEdgePoints()[0], edgeToKeep[j].getEdgePoints()[1], edgeToKeep[i].getEdgePoints()[1]);
					}
					else if (edgeToKeep[i].getEdgePoints()[0] == edgeToKeep[j].getEdgePoints()[1])
					{
						newTri = Tri(edgeToKeep[i].getEdgePoints()[0], edgeToKeep[j].getEdgePoints()[0], edgeToKeep[i].getEdgePoints()[1]);
					}
					else if (edgeToKeep[i].getEdgePoints()[1] == edgeToKeep[j].getEdgePoints()[0])
					{
						newTri = Tri(edgeToKeep[i].getEdgePoints()[1], edgeToKeep[i].getEdgePoints()[0], edgeToKeep[j].getEdgePoints()[1]);
					}
					else
					{
						continue;
					}

					bool inCircle = false;
					for (int p = 0; p < pointToTest.size(); ++p)
					{
						//check si delaunay
						if (!newTri.isPointInTriangle(pointToTest[p]) && newTri.isPointInCircle(*pointToTest[p]))
						{
							inCircle = true;
							break;
						}
					}
					if (!inCircle)
					{
						createTri = true;

						//ajout du nouveau tri si criteredelaunay
						T.tri.push_back(newTri);
						for (int e = 0; e < 3; ++e)
						{
							if (edgeToKeep[i] != newTri.getEdge()[e] && edgeToKeep[j] != newTri.getEdge()[e])
							{
								//ajout de la troisieme edge du nouveau tri
								T.edge.push_back(newTri.getEdge()[e]);
								edgeToKeep.push_back(newTri.getEdge()[e]);
							}
						}
						//suppression arêtes a1 et a2
						if (j < i)
						{
							edgeToKeep.erase(edgeToKeep.begin() + i);
							edgeToKeep.erase(edgeToKeep.begin() + j);
						}
						else
						{
							edgeToKeep.erase(edgeToKeep.begin() + j);
							edgeToKeep.erase(edgeToKeep.begin() + i);
						}

						//suppression du point p dans le tableau de tous les points 
						for (int l = 0; l < pointToTest.size(); l++)
						{
							if (newTri.getPoints()[0] == pointToTest[l])
							{
								pointToTest.erase(pointToTest.begin() + l);
								break;
							}
						} 
						break;
					} 
				}

				if (createTri)
					break;
			}
		}
		T.tri.push_back(Tri(pointToTest[0], pointToTest[1], pointToTest[2]));
	}
	else 
	{
		bool allGood = false;
		while (pointToTest.size() > 2 || allGood == false)
		{
			//recherche du point sans amis
			for (int i = 0; i < pointToTest.size(); ++i)
			{
				int compteur = 0;
				for (int j = 0; j < edgeToKeep.size(); ++j)
				{
					if (pointToTest[i] == edgeToKeep[j].getEdgePoints()[0] || pointToTest[i] == edgeToKeep[j].getEdgePoints()[1])
					{
						compteur++;
					}
				}

				if (compteur != 2)
				{
					for (int e = 0; e < edgeToKeep.size(); ++e)
					{
						if (T.GetTriangleIndexByEdge(edgeToKeep[e]) == -1 || isVisible(*pointToTest[i], T.GetTriangleEdgeByEdge(edgeToKeep[e])))
						{
							//triangle avec le nouveau point
							T.tri.push_back(Tri(edgeToKeep[e].getEdgePoints()[0],
								pointToTest[i],
								edgeToKeep[e].getEdgePoints()[1]));

							Edge newEdge = Edge(edgeToKeep[e].getEdgePoints()[0], pointToTest[i]);
							Edge newEdge1 = Edge(pointToTest[i], edgeToKeep[e].getEdgePoints()[1]);

							checkIfEdgeExist(newEdge, T.edge);
							checkIfEdgeExist(newEdge1, T.edge);

							checkIfEdgeExist(newEdge, edgeToKeep);
							checkIfEdgeExist(newEdge1, edgeToKeep);

							//l'edge n'est plus un bord
							T.edge[T.GetIndexEdge(edgeToKeep[e])].setInterior();

							edgeToKeep.erase(edgeToKeep.begin() + e);
							e--;
						}

					}
					pointToTest.erase(pointToTest.begin() + i);
					break;
				}
			}
			
			allGood = true;
		}
	}

}

/// <summary>
/// Generate a voronoi diagram from a Delaunay's triangulation
/// </summary>
Triangulation voronoiDiagram(std::vector<Vertex>& S)
{	
	Triangulation delaunay = coreDelaunay(S);

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

			Vec3 pos = delaunay.edge[i].getMiddleEdgePoint().GetPos() + delaunay.edge[i].getNormale() * 10;
			S.emplace_back(Vertex(pos.x, pos.y, pos.z, 1, 0, 1));

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

#pragma endregion