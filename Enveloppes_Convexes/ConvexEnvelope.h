#pragma once
#include "Vertex.h"
#include "OpenGLcore.h"
#include <vector>

using namespace std;

class ConvexEnvelope 
{
	vector<Vertex*> points;
	vector<Vertex> bufferPts;
	uint32_t VBO;

public:
	ConvexEnvelope();
	ConvexEnvelope(const vector<Vertex*> points);

	//Utility
	void updateBuffers();
	void updateBufferPoints();
	void Clear();

	//Setters
	void SetPoints(const vector<Vertex*> p) { points = p; }

	//Getters
	uint32_t GetVBO()const { return VBO; }
	int GetPointsCount()const { return points.size(); }
	vector<Vertex*> GetPoints()const { return points; }
};