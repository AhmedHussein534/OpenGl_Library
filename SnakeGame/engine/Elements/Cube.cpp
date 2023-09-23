#include <iostream>
#include <stdexcept>

#include "Cube.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

namespace{
    const std::string vertexShader =
            "#version 410 core\n"
            "layout(location = 0) in vec4 position;\n"
            "uniform mat4 model;\n"
            "uniform mat4 projectionview;\n"
            "out vec4 posColor;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = projectionview * model * position;\n"
            "    posColor = (position + 1) * 0.5;\n"
            "}\n";

    const std::string fragmentShader =
        "#version 410 core\n"
        "layout(location = 0) out vec4 color;\n"
        "in vec4 posColor;\n"
        "void main()\n"
        "{\n"
        "    color = posColor ;\n"
        "}\n";
}



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
    vertexBuffer(nullptr),
    indexBuffer(nullptr),
    shader(std::make_unique<Shader>(vertexShader, fragmentShader))
{
    vertexElements.emplace_back(3, ElementDataType::FLOAT, true, 3 * sizeof(float));

    std::shared_ptr<std::vector<float>> vertexPtr = std::make_shared<std::vector<float>>(std::initializer_list<float>({
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    }));

    std::shared_ptr<std::vector<uint32_t>> indexPtr = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>({
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
        7, 3, 0, 0, 4, 7, 6, 2, 1, 1, 5, 6,
        0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3
    }));

    vertexBuffer = std::make_unique<VertexBuffer>(vertexPtr),
    indexBuffer = std::make_unique<IndexBuffer>(indexPtr);


}



Cube::Cube(float x, float y, float length,
    float r, float g, float b, float a,
    float rotate, bool isDataNormalized) : Cube(x, y, length, r, g, b, a, rotate, x + length / 2, y - length / 2, isDataNormalized)
{

}

void Cube::bind(const glm::mat4 &viewProjection, const glm::mat4 &model)
{
    static bool isBind = false;

    auto model2 = model;
    auto projectionview = viewProjection;
    vertexBuffer->bind();
    indexBuffer->bind();

    shader->bind();
    shader->setUniformValue("projectionview", 1, false, glm::value_ptr(projectionview));
    shader->setUniformValue("model", 1, false, glm::value_ptr(model2));
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