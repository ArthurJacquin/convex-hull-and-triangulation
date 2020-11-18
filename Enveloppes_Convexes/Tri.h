#pragma once
#include"Vertex.h"
#include "OpenGLcore.h"
#include <vector>
#include "Edge.h"

class Tri
{
private:
	std::vector<Vertex*> points;
	std::vector<Tri*> neighorTri;
	std::vector<Vertex> bufferPts;

	std::vector<Edge> triEdge;
	//bool clockwise;

	Vertex center;
	float radius;


	uint32_t VBO;

public:
	Tri();
	Tri(Vertex* p1, Vertex* p2, Vertex* p3);

	const std::vector<Vertex*> getPoints() const { return points; }
	const std::vector<Tri*> getNeighorTri() const { return neighorTri; }
	const std::vector<Edge> getEdge() const { return triEdge; }
	const Vertex getCenter() const { return center; }
	const float getRadius() const { return radius; }

	void setPoints(Vertex* p1, Vertex* p2, Vertex* p3) { points.push_back(p1);
													  points.push_back(p2);
													  points.push_back(p3);}
	void updateBuffers();
	uint32_t GetVBO()const { return VBO; }
	void updateBufferPoints();

};