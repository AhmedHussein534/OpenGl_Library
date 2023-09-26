#include "Layout.hpp"
#include <glm/gtc/type_ptr.hpp>

Layout::Layout() : batchCached(false)
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

void Layout::drawBatched(std::shared_ptr<ICamera> camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    batchDataContainers.clear();

    if (!batchCached)
    {
        for (auto& v : elementMap)
        {
            batchDataContainers.emplace_back();
            BatchDataContainer& typeContainer = batchDataContainers.back();
            for (auto e : v.second)
            {
                std::cout << "ADDING" << std::endl;
                typeContainer.addElementData(e);
                std::cout << "ADDING - DONE" << std::endl;
            }
        }
    }

    vArray.bind();
    for (auto &it : batchDataContainers)
    {
        for (auto &d : it.batchVec)
        {
            std::cout << "DRAW CALL" << std::endl;
            auto vertexPtr = d->batchVertexBuffer;
            auto indexPtr = d->batchIndexBuffer;
            auto element = d->element;
            auto model = d->batchVertexModel;
            vertexPtr->bind();
            indexPtr->bind();
            element->bind(camera->GetViewProjectionMatrix());
            element->getShader()->setUniformValue("model", 1, false, const_cast<float*>(glm::value_ptr(*model)));
            uint32_t index = 0;
            int offset = 0;
            const auto& vertexElements = element->getVertexElements();
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

            glDrawElements(GL_TRIANGLES, 6*5, GL_UNSIGNED_INT, nullptr);
        }
    }
    std::cout << "drawBatched DONE." << std::endl;

}


