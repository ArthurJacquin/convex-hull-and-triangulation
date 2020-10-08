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

	void updateBuffers();

	void Clear();

	void SetPoints(const vector<Vertex> p) { points = p; }

	uint32_t GetVBO()const { return VBO; }
	int GetPointsCount()const { return points.size(); }

};