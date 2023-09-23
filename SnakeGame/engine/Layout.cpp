#include "Layout.hpp"

#include <type_traits>
#include <iostream>

Layout::Layout(std::shared_ptr<ICamera> camera) : m_camera(camera)
{
    glEnable(GL_DEPTH_TEST);
    vArray.bind();
}

Layout::~Layout()
{

}

void Layout::bind()
{
    vArray.bind();
}

void Layout::unbind()
{
    vArray.unbind();
}

void Layout::addElement(std::shared_ptr<IElement> e)
{
	elements.push_back(e);
}

void Layout::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float rotation = 0.0f;
    static float m_x = 0.0f;
    static float m_y = 0.0f;
    static float m_z = 0.0f;
    static float z_step = 0.01f;
    m_camera->SetPosition({m_x, m_y, m_z});
    m_camera->SetRotation(rotation, {0, 0.5, 0.5});
    vArray.bind();
    for (auto& e : elements)
    {
        e->bind(m_camera->GetViewProjectionMatrix());
        uint32_t index = 0;
        const auto& vertexElements = e->getVertexElements();
        int offset = 0;
        for (const auto& element : vertexElements)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index /* index of vertex array*/,
                element.count /* number of components per each vertex (x,y) */,
                element.getGlDataType() /* type of vertex data (float) */,
                (element.normalized ? GL_FALSE : GL_TRUE)/* data is already normalized so false for normalization */,
                element.stride /* stride i.e how many bytes to increment to move to next vertex */,
                (const void*) offset);
            offset += element.count * static_cast<int>(element.getDataSize());
            index++;
        }



       glDrawElements(GL_TRIANGLES, e->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
    }

    rotation += 0.1f;
}


