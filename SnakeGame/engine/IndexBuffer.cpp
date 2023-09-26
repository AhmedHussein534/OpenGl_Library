
#include "IndexBuffer.hpp"
#include "Renderer.hpp"

#include <GL/glew.h>


IndexBuffer::IndexBuffer(std::shared_ptr<std::vector<uint32_t>> data): renderId(0),
                                                                      isBindedBefore(false),
                                                                      m_data(data)
{

}

IndexBuffer::IndexBuffer(IndexBuffer &other) : isBindedBefore(false)
{
    renderId = other.renderId;
    m_data = std::make_shared<std::vector<uint32_t>>(other.m_data->begin(), other.m_data->end());
}

IndexBuffer::IndexBuffer(IndexBuffer &&other) : isBindedBefore(false)
{
    renderId = other.renderId;
    m_data = other.m_data;
}

IndexBuffer& IndexBuffer::operator+(IndexBuffer &other)
{
    auto currentSize = m_data->size();
    uint32_t currentMaxIndex = *max_element(std::begin(*m_data), std::end(*m_data));
    auto currentEdge = m_data->begin() + (m_data->size() - 1);
    m_data->insert(m_data->end(), other.m_data->begin(), other.m_data->end());
    std::transform(m_data->begin() + currentSize,
                    m_data->end(),
                    m_data->begin() + currentSize,
                    [currentMaxIndex](uint32_t x) {return x + currentMaxIndex + 1;});
    return *this;
}

void IndexBuffer::bind()
{

    if (!isBindedBefore)
    {
        GLCall(glGenBuffers(1, &renderId));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data->size() * sizeof(uint32_t), m_data->data(), GL_STATIC_DRAW));
        m_data = nullptr;
        isBindedBefore = true;
    }
    else
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId));
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