#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <stddef.h>

namespace GL_ENGINE
{
	class VertexBuffer
	{
	public:
		VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr);

		VertexBuffer(size_t size);

		void setData(const void *data, size_t size);

		void bind();

		void unbind();

		~VertexBuffer();

	private:
		uint32_t renderId;
		bool isBindedBefore;
		std::shared_ptr<std::vector<float>> m_data;
	};
}
