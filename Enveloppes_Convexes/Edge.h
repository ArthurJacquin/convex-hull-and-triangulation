#pragma once
#include <vector>
#include "Vertex.h"
#include "Vec3.h"
#include "OpenGLcore.h"

class Edge
{
	std::vector<Vertex*> edgePoints;
	Vertex middleEdge;
	bool exterior;
	Vec3 normale;
	std::vector<Vertex> bufferPts;

	uint32_t VBO;

public:
	Edge();
	Edge(Vertex* v1, Vertex* v2);

	std::vector<Vertex*>getEdgePoints() const { return edgePoints; }
	Vertex getMiddleEdgePoint() const { return middleEdge; }
	bool getExterior() const { return exterior; }
	Vec3 getNormale() const { return normale; }

	bool operator!=(const Edge e) { return !(*this == e); }
	bool operator==(const Edge e)
	{ 
		return (edgePoints[0] == e.edgePoints[0] && edgePoints[1] == e.edgePoints[1]) || (edgePoints[0] == e.edgePoints[1] && edgePoints[1] == e.edgePoints[0]);
	}

	void setInterior() { exterior = false; }
	void setExterior() { exterior = true; }
	void reCalculateNormale() { normale = Vec3(edgePoints[1]->x - edgePoints[0]->x, -(edgePoints[1]->y - edgePoints[0]->y), 0); }
	void calculateMiddle();
	
	void updateBuffers();
	uint32_t GetVBO()const { return VBO; }
	void updateBufferPoints();

};