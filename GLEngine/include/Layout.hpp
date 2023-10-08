#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "IElement.hpp"
#include "ICamera.hpp"

namespace GL_ENGINE
{

	class Layout
	{
	public:
		Layout();

		void bind();

		void unbind();

	private:
		VertexArray vArray;
	};
}
