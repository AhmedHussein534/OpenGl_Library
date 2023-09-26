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

	IndexBuffer(IndexBuffer &other);

	IndexBuffer(IndexBuffer &&other);

	IndexBuffer& operator+(IndexBuffer &other);

	void bind();

	void unbind();

	~IndexBuffer();

private:
	unsigned int renderId;
	bool isBindedBefore;
	std::shared_ptr<std::vector<uint32_t>> m_data;
};