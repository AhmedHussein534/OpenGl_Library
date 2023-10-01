#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Rectangle.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace
{
    const std::string vertexShader =
        "#version 410 core\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "out vec4 m_color;\n"
        "uniform mat4 projectionview;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projectionview * position;\n"
        "    m_color = color;\n"
        "}\n";

    const std::string fragmentShader =
        "#version 410 core\n"
        "layout(location = 0) out vec4 color;\n"
        "in vec4 m_color;\n"
        "void main()\n"
        "{\n"
        "    color = m_color;\n"
        "}\n";

    struct RectangleVertex
    {
        glm::vec4 pos;
        glm::vec4 color;
    };

    struct RectangleIndex
    {
        uint32_t index;
    };
}


namespace GL_ENGINE
{
    Rectangle::Rectangle(float x, float y, float length, float width,
        float r, float g, float b, float a, bool isDataNormalized) :  m_x(x),
                                                                    m_y(y),
                                                                    m_length(length),
                                                                    m_width(width),
                                                                    m_r(r),
                                                                    m_g(g),
                                                                    m_b(b),
                                                                    m_a(a),
                                                                    m_center(x+length/2.0f,y-width/2.0f,0.0f)
    {
        std::shared_ptr<std::vector<float>> vertexData = std::make_shared<std::vector<float>>(std::initializer_list<float>({
                                                            (m_x)           ,     (m_y),
                                                            (m_x + m_length),     (m_y),
                                                            (m_x + m_length),     (m_y - m_width),
                                                            (m_x)           ,     (m_y - m_width)}));
        std::shared_ptr<std::vector<uint32_t>> indexData = std::make_shared<std::vector<uint32_t>>(std::initializer_list<uint32_t>({0, 1, 2, 2, 3, 0}));
        vertexElements.emplace_back(4, ElementDataType::FLOAT, true, 8 * sizeof(float));
        vertexElements.emplace_back(4, ElementDataType::FLOAT, true, 8 * sizeof(float));
    }

    std::pair<std::string, std::string> Rectangle::getShaderText()
    {
        return {vertexShader, fragmentShader};
    }

    size_t Rectangle::getIndicesSize() const
    {
        return 6 * sizeof(RectangleIndex);
    }

    size_t Rectangle::getVerticesSize() const
    {
        return 4 * sizeof(RectangleVertex);
    }

    size_t Rectangle::getIndicesCount() const
    {
        return 6;
    }


    bool Rectangle::fillVertices(void* v_ptr, int &size)
    {
        RectangleVertex* vertex = reinterpret_cast<RectangleVertex*>(v_ptr);
        vertex->pos = *m_model * glm::vec4{m_x, m_y, 0.0f, 1.0f};
        vertex->color = glm::vec4{m_r, m_g, m_b, m_a};
        size += sizeof(RectangleVertex);

        vertex++;
        vertex->pos = *m_model * glm::vec4{m_x + m_length, m_y, 0.0f, 1.0f};
        vertex->color = glm::vec4{m_r, m_g, m_b, m_a};
        size += sizeof(RectangleVertex);

        vertex++;
        vertex->pos = *m_model * glm::vec4{m_x + m_length, m_y - m_width, 0.0f, 1.0f};
        vertex->color = glm::vec4{m_r, m_g, m_b, m_a};
        size += sizeof(RectangleVertex);

        vertex++;
        vertex->pos = *m_model * glm::vec4{m_x, m_y - m_width, 0.0f, 1.0f};
        vertex->color = glm::vec4{m_r, m_g, m_b, m_a};
        size += sizeof(RectangleVertex);
        return true;
    }

    bool Rectangle::fillIndices(void* v_ptr, int &offset, int &count)
    {
        RectangleIndex* index = reinterpret_cast<RectangleIndex*>(v_ptr);
        index->index = offset + 0;
        index += 1;
        index->index = offset + 1;
        index += 1;
        index->index = offset + 2;
        index += 1;
        index->index = offset + 2;
        index += 1;
        index->index = offset + 3;
        index += 1;
        index->index = offset + 0;

        offset = offset + 3 + 1;
        count += 6 * sizeof(RectangleIndex);
        return true;
    }


    const std::vector<VertexElement>& Rectangle::getVertexElements()
    {
        return vertexElements;
    }

    glm::vec3 Rectangle::getCenter()
    {
        return *m_model * glm::vec4(m_center, 1.0f);
    }
}

