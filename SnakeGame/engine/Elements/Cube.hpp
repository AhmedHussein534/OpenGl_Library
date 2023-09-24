#pragma once

#include <memory>
#include <array>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


namespace CUBE_DATA
{
    constexpr size_t positionCount = 24;
    constexpr size_t indexCount = 36;
}

class Cube : public IElement
{
public:
    Cube(float x, float y, float z, float length,
        float r=1.0f, float g=1.0f, float b=1.0f, float a=1.0f, bool isDataNormalized = true);

    void bind(const glm::mat4 &viewProjection) override;

    void unbind() override;

    const std::vector<VertexElement>& getVertexElements() override;

    unsigned int getIndicesCount() override;

    virtual glm::vec3 getCenter() override;


private:
    float m_x;
    float m_y;
    float m_z;
    float m_length;
    float m_r;
    float m_g;
    float m_b;
    float m_a;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::vector<VertexElement> vertexElements;
    std::unique_ptr<Shader> shader;
};