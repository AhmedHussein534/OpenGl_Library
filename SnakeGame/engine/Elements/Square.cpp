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
                                                                    vertexBuffer(std::make_unique<VertexBuffer>()),
                                                                    indexBuffer(std::make_unique<IndexBuffer>())
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

    constexpr float pi = 3.14159265359f;
    float angle_rad = m_rotate * pi / 180;

    float cos_angle = cos(angle_rad);
    float sin_angle = sin(angle_rad);

    float m_x_new = (m_x - m_rotateAxisX) * cos_angle - (m_y - m_rotateAxisY) * sin_angle;
    float m_y_new = (m_y - m_rotateAxisY) * cos_angle + (m_x - m_rotateAxisX) * sin_angle;

    position[0] = (m_x_new)+m_rotateAxisX;
    position[1] = (m_y_new)+m_rotateAxisY;
    position[2] = (m_x_new + m_length * cos_angle) + m_rotateAxisX;
    position[3] = (m_y_new + m_length * sin_angle) + m_rotateAxisY;
    position[4] = (m_x_new + m_length * (cos_angle + sin_angle)) + m_rotateAxisX;
    position[5] = (m_y_new + m_length * (sin_angle - cos_angle)) + m_rotateAxisY;
    position[6] = (m_x_new + m_length * sin_angle) + m_rotateAxisX;
    position[7] = (m_y_new - m_length * cos_angle) + m_rotateAxisY;

    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 2 * sizeof(float));

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
}



Square::Square(float x, float y, float length,
               float r, float g, float b, float a,
               float rotate, bool isDataNormalized) : Square(x, y, length, r, g, b, a, rotate, x + length/2, y - length/2, isDataNormalized)
{

}

void Square::bind()
{
    vertexBuffer->addVertexData(position.data(), sizeof(position));
    indexBuffer->addIndexData(indices.data(), sizeof(indices));
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