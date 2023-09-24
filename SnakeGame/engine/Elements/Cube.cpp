#include <iostream>
#include <stdexcept>

#include "Cube.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <type_traits>

namespace{
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
        "uniform vec4 inputColor;\n"
        "void main()\n"
        "{\n"
        "    color = inputColor ;\n"
        "}\n";
}



Cube::Cube(float x, float y, float z, float length,
           float r, float g, float b, float a, bool isDataNormalized) : m_x(x),
                                                                        m_y(y),
                                                                        m_z(z),
                                                                        m_length(length),
                                                                        m_r(r),
                                                                        m_g(g),
                                                                        m_b(b),
                                                                        m_a(a),
                                                                        m_center(x+length/2.0f,y-length/2.0f,z-length/2.0f),
                                                                        vertexBuffer(nullptr),
                                                                        indexBuffer(nullptr),
                                                                        shader(std::make_unique<Shader>(vertexShader, fragmentShader))
{
    vertexElements.emplace_back(3, ElementDataType::FLOAT, true, 3 * sizeof(float));

    std::shared_ptr<std::vector<float>> vertexPtr = std::make_shared<std::vector<float>>(std::initializer_list<float>({
        -x, -y, z - m_length,
         x, -y, z - m_length,
         x,  y, z - m_length,
        -x,  y, z - m_length,
        -x, -y,  z,
         x, -y,  z,
         x,  y,  z,
        -x,  y,  z,
    }));

    std::shared_ptr<std::vector<uint32_t>> indexPtr = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>({
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
        7, 3, 0, 0, 4, 7, 6, 2, 1, 1, 5, 6,
        0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3
    }));

    vertexBuffer = std::make_unique<VertexBuffer>(vertexPtr),
    indexBuffer = std::make_unique<IndexBuffer>(indexPtr);


}

void Cube::bind(const glm::mat4 &viewProjection)
{
    vertexBuffer->bind();
    indexBuffer->bind();

    shader->bind();
    shader->setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(viewProjection)));
    shader->setUniformValue("model", 1, false, const_cast<float*>(glm::value_ptr(*m_model)));
    shader->setUniformValue("inputColor", m_r, m_g, m_b, m_a);
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

glm::vec3 Cube::getCenter()
{
    return *m_model * glm::vec4(m_center, 1.0f);
}