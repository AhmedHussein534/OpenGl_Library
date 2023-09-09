
#include "VertexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>


VertexBuffer::VertexBuffer()
{
    GLCall(glGenBuffers(1, &renderId));
}

VertexBuffer::VertexBuffer(void* data, int size)
{
    GLCall(glGenBuffers(1, &renderId));
    addVertexData(data, size);
}

void VertexBuffer::addVertexData(void* data, int size)
{
    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
}

void VertexBuffer::unbind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &renderId));
}