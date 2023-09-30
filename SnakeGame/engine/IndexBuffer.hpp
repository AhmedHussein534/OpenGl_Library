#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>
namespace GL_ENGINE
{
	class IndexBuffer
	{
	public:
		IndexBuffer(std::shared_ptr<std::vector<uint32_t>> dataPtr);

		IndexBuffer(size_t size);

		void setData(const void *data, size_t size);

		void bind();

		void unbind();

		~IndexBuffer();

	private:
		unsigned int renderId;
		std::shared_ptr<std::vector<uint32_t>> m_data;
	};
}
