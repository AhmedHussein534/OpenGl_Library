#include "Renderer2D.hpp"

namespace GL_ENGINE
{
    namespace
    {
        const size_t vertexCpuBufferSize = 1024000;
        const size_t indexCpuBufferSize = 256000;
    }

    Renderer2D::Renderer2D() :  isShaderBinded(false),
                                sceneExists(false),
                                currentShaderClass(IElement::getDefaultElementId()),
                                m_layout(nullptr),
                                vertexCpuBuffer(nullptr),
                                indexCpuBuffer(nullptr),
                                m_vertexBuffer(nullptr),
                                m_indexBuffer(nullptr)
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
        vertexCpuBuffer = std::make_unique<uint8_t[]>(vertexCpuBufferSize);
        indexCpuBuffer = std::make_unique<uint8_t[]>(indexCpuBufferSize);
        m_vertexBuffer = std::make_unique<VertexBuffer>(vertexCpuBufferSize);
        m_indexBuffer = std::make_unique<IndexBuffer>(indexCpuBufferSize);
        sceneExists = true;
        isShaderBinded = false;
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
                auto elementCount = v.second.size();
                int vOffset = 0;
                int iOffset = 0;
                int maxIndex = 0;
                if (elementCount > 0)
                {
                    const size_t vertexDataSize = elementCount * v.second[0]->getVerticesSize();
                    const size_t indexDataSize = elementCount * v.second[0]->getIndicesSize();
                    auto elementType = v.second[0]->getElementType();
                    if (!isShaderBinded || (elementType != currentShaderClass))
                    {
                        if (elementShaderMap.find(elementType) == elementShaderMap.end())
                        {
                            auto shaderString = v.second[0]->getShaderText();
                            elementShaderMap[elementType] = std::make_shared<Shader>(shaderString.first, shaderString.second);
                            elementShaderMap[elementType]->bind();
                        }

                        elementShaderMap[elementType]->bind();
                        elementShaderMap[elementType]->setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(m_camera->GetViewProjectionMatrix())));
                        currentShaderClass = elementType;
                        isShaderBinded = true;
                    }



                    v.second[0]->bind();
                    for (auto e : v.second)
                    {
                        e->fillVertices(vertexCpuBuffer.get() + vOffset, vOffset);
                        e->fillIndices(indexCpuBuffer.get() + iOffset, maxIndex, iOffset);
                    }

                    m_vertexBuffer->setData(vertexCpuBuffer.get(), vertexDataSize);
                    m_indexBuffer->setData(indexCpuBuffer.get(), indexDataSize);
                    drawElementsIndexed(v.second[0]->getVertexElements(), indexDataSize / sizeof(uint32_t));
                }
            }

            ret = true;
        }

        return ret;
    }

    void Renderer2D::drawElementsIndexed(const std::vector<VertexElement>& vertexElements, uint32_t indexCount)
    {
        uint32_t index = 0;
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

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer2D::endScene()
    {
        isShaderBinded = false;
        sceneExists = false;
		m_layout = nullptr;
        m_camera = nullptr;
        vertexCpuBuffer = nullptr;
        indexCpuBuffer = nullptr;
        m_vertexBuffer = nullptr;
        m_indexBuffer = nullptr;
        elementMap.clear();
        elementShaderMap.clear();
        currentShaderClass = IElement::getDefaultElementId();
    }
}