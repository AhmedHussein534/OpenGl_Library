
#include "VertexArray.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>


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