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
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "out vec4 posColor;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = projection * view * model * position;\n"
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

void Cube::bind()
{
    static bool isBind = false;
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

    vertexBuffer->bind();
    indexBuffer->bind();

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