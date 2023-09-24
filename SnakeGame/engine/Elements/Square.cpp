#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Square.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace
{
    const std::string vertexShader =
        "#version 410 core\n"
        "layout(location = 0) in vec4 position;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projectionview;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projectionview * model * position;\n"
        "}\n";

    const std::string fragmentShader =
        "#version 410 core\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform vec4 u_Color;\n"
        "void main()\n"
        "{\n"
        "    color = u_Color;\n"
        "}\n";
}

Square::Square(float x, float y, float length,
	float r, float g, float b, float a, bool isDataNormalized) :  m_x(x),
                                                                  m_y(y),
                                                                  m_length(length),
                                                                  m_r(r),
                                                                  m_g(g),
                                                                  m_b(b),
                                                                  m_a(a),
                                                                  vertexBuffer(nullptr),
                                                                  indexBuffer(nullptr),
                                                                  shader(std::make_unique<Shader>(vertexShader, fragmentShader))
{


    if (isDataNormalized)
    {
        if (x > 1.0f || x < -1.0f)
        {
            std::cout << "X point is not normalized: " << x << std::endl;
            throw std::length_error("SQUARE CONSTRUCTOR");
        }

        if (y > 1.0f || y < -1.0f)
        {
            std::cout << "Y point is not normalized: " << y << std::endl;
            throw std::length_error("SQUARE CONSTRUCTOR");
        }

        if ((x + length > 1.0) || (y - length < -1.0))
        {
            std::cout << "Length exceeds limit of normalized range: " << length << std::endl;
            throw std::length_error("SQUARE CONSTRUCTOR");
        }
    }

    std::shared_ptr<std::vector<float>> vertexData = std::make_shared<std::vector<float>>();
    std::shared_ptr<std::vector<uint32_t>> indexData = std::make_shared<std::vector<uint32_t>>();

    vertexData->push_back(m_x);
    vertexData->push_back(m_y);
    vertexData->push_back(m_x + m_length);
    vertexData->push_back(m_y);
    vertexData->push_back(m_x + m_length);
    vertexData->push_back(m_y - m_length);
    vertexData->push_back(m_x);
    vertexData->push_back(m_y - m_length);

    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 2 * sizeof(float));

    indexData->push_back(0);
    indexData->push_back(1);
    indexData->push_back(2);
    indexData->push_back(2);
    indexData->push_back(3);
    indexData->push_back(0);

    vertexBuffer = std::make_unique<VertexBuffer>(vertexData),
    indexBuffer = std::make_unique<IndexBuffer>(indexData);
}



void Square::bind(const glm::mat4 &viewProjection)
{
    vertexBuffer->bind();
    indexBuffer->bind();
    shader->bind();
    shader->setUniformValue("u_Color", m_r, m_g, m_b, m_a);
    shader->setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(viewProjection)));
    shader->setUniformValue("model", 1, false, const_cast<float*>(glm::value_ptr(*m_model)));
}

void Square::unbind()
{
    vertexBuffer->unbind();
    indexBuffer->unbind();
    shader->unbind();
}


unsigned int Square::getIndicesCount()
{
    return 6;
}

const std::vector<VertexElement>& Square::getVertexElements()
{
    return vertexElements;
}