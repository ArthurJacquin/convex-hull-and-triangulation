#include "ConvexEnvelope.h"

ConvexEnvelope::ConvexEnvelope()
{
	points = {};
	VBO = 0;
}

ConvexEnvelope::ConvexEnvelope(vector<Vertex> pts)
{
	points = pts;
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * points.size(), points.data());
}

void ConvexEnvelope::updateBuffers()
{
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * points.size(), points.data());
}

void ConvexEnvelope::Clear()
{
	points.clear();
	VBO = 0;
}
