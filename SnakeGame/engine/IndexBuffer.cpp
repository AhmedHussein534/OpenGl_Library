
#include "IndexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>


IndexBuffer::IndexBuffer()
{
    GLCall(glGenBuffers(1, &renderId));
}

IndexBuffer::IndexBuffer(void* data, int size)
{
    GLCall(glGenBuffers(1, &renderId));
    addIndexData(data, size);
}

void IndexBuffer::addIndexData(void* data, int size)
{
    bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void IndexBuffer::bind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId));
}

void IndexBuffer::unbind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &renderId));
}