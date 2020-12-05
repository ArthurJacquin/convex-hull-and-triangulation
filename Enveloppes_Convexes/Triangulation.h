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

	bool isPointInTriangulation(Vertex p)
	{
		for (size_t i = 0; i < tri.size(); ++i)
		{
			if (tri[i].isPointInTriangle(p)) return true;
		}
		return false;
	}

	int isPointInTriangulationIndex(Vertex p)
	{
		for (size_t i = 0; i < tri.size(); ++i)
		{
			//au cas ou si c'est sur une arete : que faire ?? 
			if (tri[i].isPointInTriangle(p)) return i;
		}
		return -1;
	}

	int GetIndexEdge(Edge e)
	{
		for (size_t i = 0; i < edge.size(); i++)
		{
			if (e == edge[i]) return i;
		}
		return -1;
	}

	std::vector<int> GetEdgeIndexByTriangle(Tri triangle)
	{
		std::vector<int> index;
		
		for (size_t i = 0; i < triangle.getEdge().size(); i++)
		{
			for (size_t j = 0; j < edge.size(); j++)
			{
				if (triangle.getEdge()[i] == edge[j])
				{
					index.push_back(j);
					break;
				}
			}
		}

		return index;
	}

	int GetTriangleIndexByEdge(Edge edge)
	{
		for (int t = 0; t < tri.size(); ++t)
		{
			for (int e = 0; e < tri[t].getEdge().size(); ++e)
			{
				if (edge == tri[t].getEdge()[e])
				{
					return t;
				}
			}
		}
		return -1;
	}
	
	void clear()
	{
		edge.clear();
		tri.clear();
	}
};