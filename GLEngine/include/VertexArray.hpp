
#pragma once

namespace GL_ENGINE
{
	class VertexArray
	{
	public:
		VertexArray();

		void bind();

		void unbind();

		~VertexArray();

	private:
		unsigned int renderId;
	};
}
