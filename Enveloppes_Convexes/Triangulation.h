#pragma once
#include "Edge.h"
#include "Tri.h"

struct Triangulation
{
	std::vector<Edge> edge;
	std::vector<Tri> tri;

	Triangulation() {
		edge.reserve(0);
		tri.reserve(0);
	}

	Triangulation(std::vector<Edge>e, std::vector<Tri>t) {
		edge = e;
		tri = t;
	}

	void clear()
	{
		edge.clear();
		tri.clear();
	}
};