#pragma once
#include <vector>
#include "Vertex.h"
#include "Tri.h"

using namespace std;

class Tetrahedre {

	vector<Vertex*> points;
	vector<Tri> triangles;

public:
	Tetrahedre();
	Tetrahedre(Vertex* p1, Vertex* p2, Vertex* p3, Vertex* p4);

	vector<Vertex*> getPoints()const { return points; }
	vector<Tri> getTriangle()const { return triangles; }

	int containsTri(Tri t);
};