
#include <iostream>

#include "VertexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>


namespace GL_ENGINE
{
    VertexBuffer::VertexBuffer(std::shared_ptr<std::vector<float>> dataPtr) : renderId(0),
                                                                            isBindedBefore(false),
                                                                            m_data(dataPtr)
    {
        GLCall(glGenBuffers(1, &renderId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_data->size() * sizeof(float), m_data->data(), GL_STATIC_DRAW));
    }

    VertexBuffer::VertexBuffer(size_t size)
    {
        GLCall(glGenBuffers(1, &renderId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));
    }

    void VertexBuffer::setData(const void *data, size_t size)
    {
        uint32_t size_int = static_cast<uint32_t>(size);
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderId));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size_int, data));
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
}
