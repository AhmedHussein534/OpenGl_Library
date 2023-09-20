#include <iostream>
#include <stdexcept>

#include "Square.hpp"

#include <cmath>

Square::Square(float x, float y, float length,
	float r, float g, float b, float a, float rotate,
    float rotateAxisX, float rotateAxisY, bool isDataNormalized) :  m_x(x),
                                                                    m_y(y),
                                                                    m_length(length),
                                                                    m_r(r),
                                                                    m_g(g),
                                                                    m_b(b),
                                                                    m_a(a),
                                                                    m_rotate(rotate),
                                                                    m_rotateAxisX(rotateAxisX),
                                                                    m_rotateAxisY(rotateAxisY),
                                                                    vertexBuffer(nullptr),
                                                                    indexBuffer(nullptr)
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
    constexpr float pi = 3.14159265359f;
    float angle_rad = m_rotate * pi / 180;

    float cos_angle = cos(angle_rad);
    float sin_angle = sin(angle_rad);

    float m_x_new = (m_x - m_rotateAxisX) * cos_angle - (m_y - m_rotateAxisY) * sin_angle;
    float m_y_new = (m_y - m_rotateAxisY) * cos_angle + (m_x - m_rotateAxisX) * sin_angle;

    vertexData->push_back((m_x_new)+m_rotateAxisX);
    vertexData->push_back((m_y_new)+m_rotateAxisY);
    vertexData->push_back((m_x_new + m_length * cos_angle) + m_rotateAxisX);
    vertexData->push_back((m_y_new + m_length * sin_angle) + m_rotateAxisY);
    vertexData->push_back((m_x_new + m_length * (cos_angle + sin_angle)) + m_rotateAxisX);
    vertexData->push_back((m_y_new + m_length * (sin_angle - cos_angle)) + m_rotateAxisY);
    vertexData->push_back((m_x_new + m_length * sin_angle) + m_rotateAxisX);
    vertexData->push_back((m_y_new - m_length * cos_angle) + m_rotateAxisY);

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



Square::Square(float x, float y, float length,
               float r, float g, float b, float a,
               float rotate, bool isDataNormalized) : Square(x, y, length, r, g, b, a, rotate, x + length/2, y - length/2, isDataNormalized)
{

}

void Square::bind()
{
    vertexBuffer->bind();
    indexBuffer->bind();
    shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    shader->bind();
    shader->setUniformValue("u_Color", m_r, m_g, m_b, m_a);
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