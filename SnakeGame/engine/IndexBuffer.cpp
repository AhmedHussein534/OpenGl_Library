
#include "IndexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>


IndexBuffer::IndexBuffer(std::shared_ptr<std::vector<uint32_t>> data): renderId(0),
                                                                      isBindedBefore(false),
                                                                      m_data(data)
{
    GLCall(glGenBuffers(1, &renderId));
}

void IndexBuffer::bind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId));
    if (!isBindedBefore)
    {
        isBindedBefore = true;
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data->size() * sizeof(uint32_t), m_data->data(), GL_STATIC_DRAW));
        m_data = nullptr;
    }
}

void IndexBuffer::unbind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &renderId));
}