#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <iostream>

class VertexBuffer
{
public:
	VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr);

	VertexBuffer(VertexBuffer &other)
	{
		renderId = other.renderId;
		isBindedBefore = false;
		m_data = std::make_shared<std::vector<float>>(*(other.m_data));
	}

	VertexBuffer(VertexBuffer &&other)
	{
		renderId = other.renderId;
		isBindedBefore = false;
		m_data = other.m_data;
	}

	VertexBuffer& operator+(VertexBuffer &other)
	{
		m_data->insert(m_data->end(), other.m_data->begin(), other.m_data->end());
		std::cout << "Vertex Buffer size: " << m_data->size() << std::endl;
		return *this;
	}

	void bind();

	void unbind();

	~VertexBuffer();

private:
	unsigned int renderId;
	bool isBindedBefore;
	std::shared_ptr<std::vector<float>> m_data;
};