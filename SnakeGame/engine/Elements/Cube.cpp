#include <iostream>
#include <stdexcept>

#include "Cube.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <array>
Cube::Cube(float x, float y, float length,
    float r, float g, float b, float a, float rotate,
    float rotateAxisX, float rotateAxisY, bool isDataNormalized) : m_x(x),
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
    vertexElements.emplace_back(3, ElementDataType::FLOAT, true, 3 * sizeof(float));

    position = std::array<float, CUBE_DATA::positionCount>({
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    });

    indices = std::array<unsigned int, CUBE_DATA::indexCount>({
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
        7, 3, 0, 0, 4, 7, 6, 2, 1, 1, 5, 6,
        0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3
    });

    shader = std::make_unique<Shader>(vertexShader, fragmentShader);

}



Cube::Cube(float x, float y, float length,
    float r, float g, float b, float a,
    float rotate, bool isDataNormalized) : Cube(x, y, length, r, g, b, a, rotate, x + length / 2, y - length / 2, isDataNormalized)
{

}

void Cube::bind()
{
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
    vertexBuffer->addVertexData((void*)position.data(), position.size() * sizeof(float));
    indexBuffer->addIndexData(indices.data(), indices.size() * sizeof(unsigned int));

    shader->bind();
    shader->setUniformValue("model", 1, false, glm::value_ptr(model));
    shader->setUniformValue("view", 1, false, glm::value_ptr(view));
    shader->setUniformValue("projection", 1, false, glm::value_ptr(projection));
}

void Cube::unbind()
{
    vertexBuffer->unbind();
    indexBuffer->unbind();
    shader->unbind();
}


unsigned int Cube::getIndicesCount()
{
    return 36;
}

const std::vector<VertexElement>& Cube::getVertexElements()
{
    return vertexElements;
}