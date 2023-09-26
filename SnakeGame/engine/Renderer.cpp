#include "Renderer.hpp"

#include <GL/glew.h>
#include <iostream>

namespace GL_ENGINE
{
	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		bool ret = true;
		while (GLenum error = glGetError())
		{
			std::cout << "[OPENGL Error] (" << error << "): " << function
				<< " " << file << ":" << line << std::endl;
			ret = false;
		}

		return ret;
	}
}
