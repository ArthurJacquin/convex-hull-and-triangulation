#include "ConvexEnvelope.h"

ConvexEnvelope::ConvexEnvelope()
{
	points = {};
	VBO = 0;
}

ConvexEnvelope::ConvexEnvelope(vector<Vertex*> pts)
{
	points = pts;
	updateBufferPoints();
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * bufferPts.size(), bufferPts.data());
}

void ConvexEnvelope::updateBuffers()
{
	updateBufferPoints();
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * bufferPts.size(), bufferPts.data());
}

void ConvexEnvelope::Clear()
{
	points.clear();
	VBO = 0;
}

void ConvexEnvelope::updateBufferPoints()
{
	for (size_t i = 0; i < points.size(); i++)
	{
		bufferPts.push_back(*points[i]);
	}
}
