#pragma once

#include <memory>
#include <vector>
#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr);

	void bind();

	void unbind();

	~VertexBuffer();

private:
	unsigned int renderId;
	bool isBindedBefore;
	std::shared_ptr<std::vector<float>> m_data;
};