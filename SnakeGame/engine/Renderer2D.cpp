#include "Renderer2D.hpp"

namespace GL_ENGINE
{
    Renderer2D::Renderer2D() :  isShaderBinded(false),
                                sceneExists(false),
                                currentShaderClass(IElement::getDefaultElementId()),
                                m_layout(nullptr)


    {

    }

    bool Renderer2D::addElement(std::shared_ptr<IElement> e)
    {
        bool ret = false;
        if (sceneExists)
        {
            auto key = e->getElementType();
            if (elementMap.find(key) != elementMap.end())
            {
                elementMap[key].push_back(e);
            }
            else
            {
                elementMap[key] = std::vector<std::shared_ptr<IElement>>({e});
            }

            ret = true;
        }

        return ret;
    }

    std::shared_ptr<OrthographicCamera> Renderer2D::beginScene(const float &left, const float &right, const float &bot, const float &top)
    {
        m_layout = std::make_shared<Layout>();
        m_camera = std::make_shared<OrthographicCamera>(left, right, bot, top);
        sceneExists = true;
        return m_camera;
    }

    bool Renderer2D::drawScene()
    {
        bool ret = false;
        if (sceneExists)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_layout->bind();
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

                    e->bind(m_camera->GetViewProjectionMatrix(), *e->getModel());
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

            ret = true;
        }

        return ret;
    }

    void Renderer2D::endScene()
    {
        isShaderBinded = false;
		currentShaderClass = IElement::getDefaultElementId();
		elementMap.clear();
		m_layout = nullptr;
        m_camera = nullptr;
    }
}