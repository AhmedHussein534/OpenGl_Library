#include "Renderer2D.hpp"

namespace GL_ENGINE
{
    namespace
    {
        const size_t vertexCpuBufferSize = 12800;
        const size_t indexCpuBufferSize = 2400;
    }

    Renderer2D::Renderer2D() :  sceneExists(false),
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
        return m_camera;
    }

    void Renderer2D::createAndBindShader(const ElementType &type, const std::string &vertexShaderText, const std::string &indexShaderText)
    {
        if (type != currentShaderClass)
        {
            if (elementShaderMap.find(type) == elementShaderMap.end())
            {
                elementShaderMap[type] = std::make_shared<Shader>(vertexShaderText, indexShaderText);
            }

            elementShaderMap[type]->bind();
            elementShaderMap[type]->setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(m_camera->GetViewProjectionMatrix())));
            currentShaderClass = type;
        }
    }

    void Renderer2D::drawIndexedAndFlush(const std::vector<VertexElement>& vertexElements)
    {
        drawElementsIndexed(vertexElements, indexDataSize / sizeof(uint32_t));
        vOffset = 0;
        iOffset = 0;
        maxIndex = 0;
        vertexDataSize = 0;
        indexDataSize = 0;
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
                auto elements = v.second[0]->getVertexElements();
                vOffset = 0;
                iOffset = 0;
                maxIndex = 0;
                vertexDataSize = 0;
                indexDataSize = 0;

                if (elementCount > 0)
                {
                    auto elementType = v.second[0]->getElementType();
                    auto shaderText = v.second[0]->getShaderText();
                    auto vElementSize = v.second[0]->getVerticesSize();
                    auto iElementSize = v.second[0]->getIndicesSize();
                    if ((vElementSize > vertexCpuBufferSize) || (iElementSize > indexCpuBufferSize))
                    {
                        std::cout << "ERROR: Renderer Buffers are not enough to render this element: "
                                  << v.second[0]->getElementName()
                                  << std::endl;
                        break;
                    }

                    createAndBindShader(v.second[0]->getElementType(), shaderText.first, shaderText.second);
                    for (auto e : v.second)
                    {
                        bool dataFilled = true;
                        dataFilled = e->fillVertices(vertexCpuBuffer.get() + vOffset, vOffset);
                        dataFilled = dataFilled && e->fillIndices(indexCpuBuffer.get() + iOffset, maxIndex, iOffset);
                        vertexDataSize += vElementSize;
                        indexDataSize += iElementSize;
                        if (!dataFilled)
                        {
                            drawIndexedAndFlush(elements);
                            dataFilled = e->fillVertices(vertexCpuBuffer.get() + vOffset, vOffset);
                            dataFilled = dataFilled && e->fillIndices(indexCpuBuffer.get() + iOffset, maxIndex, iOffset);
                            vertexDataSize += vElementSize;
                            indexDataSize += iElementSize;
                            if (!dataFilled)
                            {
                                std::cout << "Error filling vertex and index buffers" << std::endl;
                                return false;
                            }
                        }
                        else if (((vertexDataSize + vElementSize) > vertexCpuBufferSize) ||
                            ((indexDataSize + iElementSize ) > indexCpuBufferSize))
                        {
                            drawIndexedAndFlush(elements);
                            continue;
                        }
                    }

                    drawElementsIndexed(elements, indexDataSize / sizeof(uint32_t));
                }
            }

            ret = true;
        }

        return ret;
    }

    void Renderer2D::drawElementsIndexed(const std::vector<VertexElement>& vertexElements, uint32_t indexCount)
    {
        if (indexCount > 0)
        {
            m_vertexBuffer->setData(vertexCpuBuffer.get(), vertexDataSize);
            m_indexBuffer->setData(indexCpuBuffer.get(), indexDataSize);
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
    }

    void Renderer2D::endScene()
    {
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