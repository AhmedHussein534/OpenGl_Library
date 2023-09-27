#include "Layout.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace GL_ENGINE
{
    Layout::Layout() : batchCached(false), isShaderBinded(false), currentShaderClass(typeid(IElement))
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

    void Layout::draw(std::shared_ptr<ICamera> camera)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        vArray.bind();
        for (auto& v : elementMap)
        {
            for (auto e : v.second)
            {
                auto elementType = e->getElementType();
                if (!isShaderBinded || (elementType != currentShaderClass))
                {
                    std::cout << "Binding Shader: " << e->getElementName() << std::endl;
                    isShaderBinded = true;
                    e->getShader()->bind();
                    currentShaderClass = elementType;
                }

                e->bind(camera->GetViewProjectionMatrix(), *e->getModel());
                e->getVertexBuffer()->bind();
                e->getIndexBuffer()->bind();
                uint32_t index = 0;
                int offset = 0;
                const auto& vertexElements = e->getVertexElements();
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
    }

}