#pragma once
#include "Tri.h"
#include "OpenGLcore.h"
#include <vector>

using namespace std;

class ConvexEnvelope3D
{
	vector<Tri> triangles;

public:
	ConvexEnvelope3D();
	ConvexEnvelope3D(const vector<Tri> tri);

	//Utility
	void Clear();
	void CalculateNormals();

	//Setters
	void SetPoints(const vector<Tri> tri) { triangles = tri; }

	//Getters
	int GetTrianglesCount()const { return triangles.size(); }
	vector<Tri> GetTriangles()const { return triangles; }
};