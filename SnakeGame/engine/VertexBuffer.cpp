
#include "VertexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>



VertexBuffer::VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr) : renderId(0),
                                                                          isBindedBefore(false),
                                                                          m_data(dataPtr)
{
}

VertexBuffer::VertexBuffer(VertexBuffer &other) : isBindedBefore(false)
{
    renderId = other.renderId;
    m_data = std::make_shared<std::vector<float>>(other.m_data->begin(), other.m_data->end());
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) : isBindedBefore(false)
{
    renderId = other.renderId;
    m_data = other.m_data;
}

VertexBuffer& VertexBuffer::operator+(const VertexBuffer &other)
{
    m_data->insert(m_data->end(), other.m_data->begin(), other.m_data->end());
    return *this;
}

void VertexBuffer::bind()
{
    if (!isBindedBefore)
    {
        isBindedBefore = true;
        GLCall(glGenBuffers(1, &renderId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_data->size() * sizeof(float), m_data->data(), GL_STATIC_DRAW));
    }
    else
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
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