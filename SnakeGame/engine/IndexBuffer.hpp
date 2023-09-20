#pragma once

#include <memory>
#include <vector>
#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(std::shared_ptr<std::vector<uint32_t>> dataPtr);

	void bind();

	void unbind();

	~IndexBuffer();

private:
	unsigned int renderId;
	bool isBindedBefore;
	std::shared_ptr<std::vector<uint32_t>> m_data;
};