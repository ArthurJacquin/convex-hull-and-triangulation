#include "Algos.h"

Vertex FindBarycenter(std::vector<Vertex> S)
{
	int size = S.size();
	if (size <= 0)
		return Vertex(0.f, .0f, .0f);
	
	double avgX = 0;
	double avgY = 0;
	double avgZ = 0;

	for(int i = 0; i < size; ++i)
	{
		avgX += S[i].x;
		avgY += S[i].y;
		avgZ += S[i].z;
	}

	avgX /= size;
	avgY /= size;
	avgZ /= size;
	
	Vertex c(avgX, avgY, avgZ);

	return c;
}

ConvexEnvelope Jarvis(std::vector<Vertex> S)
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
	std::vector<Vertex> P;	// Polygone de l'enveloppe convexe
	int i = i0; // Indice du 1e sommet
	int j = 0;

	do
	{
		//Ajout du point
		Vertex vert = S[i];
		vert.setColor(Color(1.0f, 0.0f, 0.0f));
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

ConvexEnvelope GrahamScan(std::vector<Vertex> S)
{
	// Recherche du barycentre
	Vertex barycenter = FindBarycenter(S);
	
	// Le choisir comme point d'origine
	Vertex orig = barycenter;
	
	// Déterminer pour chaque point l'angle orienté avec l'origine
	
	
	// Les trier dans le sens horaire + si égalité avec la distance à l'origine
	// Former le polygone
	// Supprimer les points non convexes


	
	ConvexEnvelope envelope(S);
	return envelope;
};
