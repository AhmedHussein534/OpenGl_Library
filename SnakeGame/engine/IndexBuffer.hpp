#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>
class IndexBuffer
{
public:
	IndexBuffer(std::shared_ptr<std::vector<uint32_t>> dataPtr);

	IndexBuffer(IndexBuffer &other)
	{
		renderId = other.renderId;
		isBindedBefore = false;
		m_data = std::make_shared<std::vector<uint32_t>>(*(other.m_data));
	}

	IndexBuffer(IndexBuffer &&other)
	{
		renderId = other.renderId;
		isBindedBefore = false;
		m_data = other.m_data;
	}

	IndexBuffer& operator+(IndexBuffer &other)
	{
		auto currentSize = m_data->size();
		uint32_t currentMaxIndex = *max_element(std::begin(*m_data), std::end(*m_data));
		auto currentEdge = m_data->begin() + (m_data->size() - 1);
		m_data->insert(m_data->end(), other.m_data->begin(), other.m_data->end());
		std::transform(m_data->begin() + currentSize,
		               m_data->end(),
					   m_data->begin() + currentSize,
					   [currentMaxIndex](uint32_t x) {return x + currentMaxIndex + 1;});
		std::cout << "index Buffer size: " << m_data->size() << std::endl;
		std::cout << "index buffer: ";
		for (auto &it : *m_data)
		{
			std::cout << it << " ";
		}
		std::cout << std::endl;
		return *this;
	}


	void bind();

	void unbind();

	~IndexBuffer();

private:
	unsigned int renderId;
	bool isBindedBefore;
	std::shared_ptr<std::vector<uint32_t>> m_data;
};