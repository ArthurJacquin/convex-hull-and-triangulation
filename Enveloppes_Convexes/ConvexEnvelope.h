#pragma once
#include "Vertex.h"
#include "OpenGLcore.h"
#include <vector>

using namespace std;

class ConvexEnvelope 
{
	vector<Vertex> points;
	uint32_t VBO;

public:
	ConvexEnvelope();
	ConvexEnvelope(const vector<Vertex> points);

	//Utility
	void updateBuffers();
	void Clear();

	//Setters
	void SetPoints(const vector<Vertex> p) { points = p; }

	//Getters
	uint32_t GetVBO()const { return VBO; }
	int GetPointsCount()const { return points.size(); }

};