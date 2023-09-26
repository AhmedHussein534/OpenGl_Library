#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <iostream>

class VertexBuffer
{
public:
	VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr);

	VertexBuffer(VertexBuffer &other);

	VertexBuffer(VertexBuffer &&other);

	VertexBuffer& operator+(const VertexBuffer &other);

	void bind();

	void unbind();

	~VertexBuffer();

private:
	unsigned int renderId;
	bool isBindedBefore;
	std::shared_ptr<std::vector<float>> m_data;
};