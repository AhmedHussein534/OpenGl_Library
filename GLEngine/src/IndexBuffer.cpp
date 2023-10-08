
#include "IndexBuffer.hpp"
#include "Renderer.hpp"

#include <external/glew/include/GL/glew.h>

namespace GL_ENGINE
{
    IndexBuffer::IndexBuffer(std::shared_ptr<std::vector<uint32_t>> data): renderId(0),
                                                                           m_data(data)
    {
        GLCall(glGenBuffers(1, &renderId));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data->size() * sizeof(uint32_t), m_data->data(), GL_STATIC_DRAW));
    }

    IndexBuffer::IndexBuffer(size_t size)
    {
        GLCall(glGenBuffers(1, &renderId));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    void IndexBuffer::setData(const void *data, size_t size)
    {
        bind();
        const uint32_t* dataI = static_cast<const uint32_t*>(data);
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data));
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
}
