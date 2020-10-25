#pragma once
#include"Vertex.h"
#include <vector>

class Tri
{
private:
	std::vector<Vertex*> points;
	std::vector<Tri*> neighorTri;

	Vertex center;
	float radius;

public:
	Tri();
	Tri(Vertex* p1, Vertex* p2, Vertex* p3);

	const std::vector<Vertex*> getPoints() const { return points; }
	const std::vector<Tri*> getNeighorTri() const { return neighorTri; }
	const Vertex getCenter() const { return center; }
	const float getRadius() const { return radius; }

	void setPoints(Vertex* p1, Vertex* p2, Vertex* p3) { points.push_back(p1);
														 points.push_back(p2);
														 points.push_back(p3);}
							
};