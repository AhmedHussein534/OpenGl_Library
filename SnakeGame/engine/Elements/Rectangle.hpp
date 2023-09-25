#pragma once

#include <memory>
#include <array>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


constexpr size_t positionCount = 8;
constexpr size_t indexCount = 6;

class Rectangle : public IElement
{
public:
	Rectangle(float x, float y, float length, float width,
		      float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool isDataNormalized = true);

	void bind(const glm::mat4 &viewProjection) override;

	void unbind() override;

    const std::vector<VertexElement>& getVertexElements() override;

    unsigned int getIndicesCount() override;

    virtual glm::vec3 getCenter() override;


private:
    float m_x;
    float m_y;
    float m_length;
    float m_width;
    float m_r;
    float m_g;
    float m_b;
    float m_a;
    glm::vec3 m_center;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
    std::vector<VertexElement> vertexElements;
    std::unique_ptr<Shader> shader;
};