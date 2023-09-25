#include "Layout.hpp"


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
            e->bind(camera->GetViewProjectionMatrix());
            e->getVertexBuffer()->bind();
            e->getIndexBuffer()->bind();
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
}

void Layout::drawBatched(std::shared_ptr<ICamera> camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (!batchCached)
    {
        for (auto& v : elementMap)
        {
            std::shared_ptr<VertexBuffer> batchVertexBuffer = nullptr;
            std::shared_ptr<IndexBuffer> batchIndexBuffer = nullptr;
            std::shared_ptr<IElement> batchVertexElement = nullptr;
            for (auto e : v.second)
            {
                if (batchVertexElement == nullptr)
                {
                    batchVertexElement = e;
                }

                if (batchVertexBuffer == nullptr)
                {
                    batchVertexBuffer = std::make_shared<VertexBuffer>(*(e->getVertexBuffer()));
                    batchIndexBuffer = std::make_shared<IndexBuffer>(*(e->getIndexBuffer()));
                }
                else
                {
                    *batchVertexBuffer + *(e->getVertexBuffer());
                    *batchIndexBuffer + *(e->getIndexBuffer());
                }
            }

            if (batchVertexBuffer != nullptr && batchIndexBuffer != nullptr)
            {
                std::cout << "Got a batch" << std::endl;
                elementBatchBufferMap[v.first] = {batchVertexBuffer, batchIndexBuffer, batchVertexElement};
            }

            batchCached = true;
        }
    }

    vArray.bind();
    for (auto &it : elementBatchBufferMap)
    {
        auto vertexPtr = it.second.batchVertexBuffer;
        auto indexPtr = it.second.batchIndexBuffer;
        auto elementPtr = it.second.batchVertexElement;
        uint32_t index = 0;
        int offset = 0;
        vertexPtr->bind();
        indexPtr->bind();
        elementPtr->bind(camera->GetViewProjectionMatrix());
        const auto& vertexElements = elementPtr->getVertexElements();
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


