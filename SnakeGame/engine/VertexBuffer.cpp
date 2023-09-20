
#include "VertexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>



VertexBuffer::VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr) : renderId(0),
                                                                          isBindedBefore(false),
                                                                          m_data(dataPtr)
{
    GLCall(glGenBuffers(1, &renderId));
}


void VertexBuffer::bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
    if (!isBindedBefore)
    {
        isBindedBefore = true;
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_data->size() * sizeof(float), m_data->data(), GL_STATIC_DRAW));
        m_data = nullptr;
    }
}

void VertexBuffer::unbind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &renderId));
}