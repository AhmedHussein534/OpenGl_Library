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
                                vertexCpuBuffer(nullptr),
                                indexCpuBuffer(nullptr),
                                m_vertexBuffer(nullptr),
                                m_indexBuffer(nullptr)
    {
        glEnable(GL_DEPTH_TEST);
    }

    bool Renderer2D::addElement(uint32_t layoutKey, std::shared_ptr<IElement> e)
    {
        bool ret = false;
        if (sceneExists)
        {
            auto key = e->getElementType();
            if (elementMap[layoutKey].find(key) != elementMap[layoutKey].end())
            {
                elementMap[layoutKey][key].push_back(e);
            }
            else
            {
                elementMap[layoutKey][key] = std::vector<std::shared_ptr<IElement>>({e});
            }

            ret = true;
        }

        return ret;
    }

    std::shared_ptr<OrthographicCamera> Renderer2D::beginScene(const float &left, const float &right, const float &bot, const float &top)
    {
        m_layouts.clear();
        m_camera = std::make_shared<OrthographicCamera>(left, right, bot, top);
        vertexCpuBuffer = std::make_unique<uint8_t[]>(vertexCpuBufferSize);
        indexCpuBuffer = std::make_unique<uint8_t[]>(indexCpuBufferSize);
        m_vertexBuffer = std::make_unique<VertexBuffer>(vertexCpuBufferSize);
        m_indexBuffer = std::make_unique<IndexBuffer>(indexCpuBufferSize);
        sceneExists = true;
        return m_camera;
    }

    void Renderer2D::createAndBindShader(uint32_t layoutKey, std::shared_ptr<IElement> e, const std::string &vertexShaderText, const std::string &indexShaderText)
    {
        auto type = e->getElementType();
        if (type != currentShaderClass)
        {
            if (elementShaderMap[layoutKey].find(type) == elementShaderMap[layoutKey].end())
            {
                elementShaderMap[layoutKey][type] = std::make_shared<Shader>(vertexShaderText, indexShaderText);
            }

            elementShaderMap[layoutKey][type]->bind();
            e->setShaderData(*elementShaderMap[layoutKey][type], m_camera->GetViewProjectionMatrix());
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
            for (uint32_t i = 0; i < m_layouts.size(); i++)
            {
                m_layouts[i]->bind();
                for (auto& v : elementMap[i])
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

                        createAndBindShader(i, v.second[0], shaderText.first, shaderText.second);
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
                    reinterpret_cast<const void*>(offset));
                offset += element.count * static_cast<int>(element.getDataSize());
                index++;
            }

            /*
            std::cout << "DRAW CALL" << std::endl;
            std::cout << "VertexBuffer: ";
            for (int i = 0; i < vertexDataSize; i+=4)
            {
                float * data = reinterpret_cast<float*>(vertexCpuBuffer.get() + i);
                std::cout << *data << " ";
            }
            std::cout << std::endl;
            std::cout << "IndexBuffer: ";
            for (int i = 0; i < indexDataSize; i+=4)
            {
                uint32_t* data = reinterpret_cast<uint32_t*>(indexCpuBuffer.get() + i);
                std::cout << *data << " ";
            }
            std::cout << std::endl << std::endl;
            */

            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        }
    }

    void Renderer2D::endScene()
    {
        sceneExists = false;

        m_camera = nullptr;
        vertexCpuBuffer = nullptr;
        indexCpuBuffer = nullptr;
        m_vertexBuffer = nullptr;
        m_indexBuffer = nullptr;
        m_layouts.clear();
        elementMap.clear();
        elementShaderMap.clear();
        currentShaderClass = IElement::getDefaultElementId();
    }
}