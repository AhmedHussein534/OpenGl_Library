
#include "VertexArray.hpp"
#include "Renderer.hpp"

#include <external/glew/include/GL/glew.h>

namespace GL_ENGINE
{
    VertexArray::VertexArray()
    {
        GLCall(glGenVertexArrays(1, &renderId));
    }


    void VertexArray::bind()
    {
        GLCall(glBindVertexArray(renderId));
    }

    void VertexArray::unbind()
    {
        GLCall(glBindVertexArray(0));
    }

    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &renderId));
    }
}
