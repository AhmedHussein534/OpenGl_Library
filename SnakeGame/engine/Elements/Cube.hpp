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
    Cube(float x, float y, float length,
        float r, float g, float b, float a,
        float rotate, float rotateAxisX, float rotateAxisY, bool isDataNormalized = true);

    Cube(float x, float y, float length,
        float r, float g, float b, float a,
        float rotate = 0.0f, bool isDataNormalized = true);


    void bind() override;

    void unbind() override;

    const std::vector<VertexElement>& getVertexElements() override;

    unsigned int getIndicesCount() override;


private:
    float m_x;
    float m_y;
    float m_length;
    float m_r;
    float m_g;
    float m_b;
    float m_a;
    float m_rotate;
    float m_rotateAxisX;
    float m_rotateAxisY;
    
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::vector<float> position;
    std::array<unsigned int, CUBE_DATA::indexCount> indices;
    std::vector<VertexElement> vertexElements;
    std::unique_ptr<Shader> shader;
    std::string vertexShader =
        "#version 410 core\n"
        "layout(location = 0) in vec4 position;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 410 core\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";


};