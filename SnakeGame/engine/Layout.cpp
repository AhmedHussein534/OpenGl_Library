#include "Layout.hpp"

#include <type_traits>
#include <iostream>

Layout::Layout()
{
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
    glClear(GL_COLOR_BUFFER_BIT);

    vArray.bind();
    for (auto& e : elements)
    {
        e->bind();
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
}


