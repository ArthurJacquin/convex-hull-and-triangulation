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

	std::vector<Edge> edge;
	//bool clockwise;

	Vertex circleCenter;
	double radius;

	Vec3 normal;

	uint32_t VBO;

public:
	Tri();
	Tri(Vertex* p1, Vertex* p2, Vertex* p3);

	const std::vector<Vertex*> getPoints() const { return points; }
	const std::vector<Tri*> getNeighorTri() const { return neighorTri; }
	std::vector<Edge>& getEdge() { return edge; }
	Vertex getCenterCirclePoint() const { return circleCenter; }
	const Vec3 getCenter() const { return circleCenter.GetPos(); }
	const double getRadius() const { return radius; }
	Vec3 getNormal()const { return normal; }

	void setPoints(Vertex* p1, Vertex* p2, Vertex* p3) { points.push_back(p1);
													  points.push_back(p2);
													  points.push_back(p3);}
	void setNormal(Vec3 n) { normal = n; }

	void updateBuffers();
	uint32_t GetVBO()const { return VBO; }
	void updateBufferPoints();

	void cercleCirconscrit();
	
	bool isPointInTriangle(Vertex p) const;
	
	bool operator==(Tri t);
};