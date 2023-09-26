#pragma once

#include <memory>
#include <array>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


constexpr size_t positionCount = 8;
constexpr size_t indexCount = 6;

class Square : public IElement
{
public:
	Square(float x, float y, float length,
		   float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool isDataNormalized = true);

	void bind(const glm::mat4 &viewProjection, const glm::mat4 &model) override;

	void unbind() override;

    const std::vector<VertexElement>& getVertexElements() override;

    unsigned int getIndicesCount() override;

    virtual glm::vec3 getCenter() override;


private:
    float m_x;
    float m_y;
    float m_length;
    float m_r;
    float m_g;
    float m_b;
    float m_a;
    glm::vec3 m_center;
    std::vector<VertexElement> vertexElements;
};