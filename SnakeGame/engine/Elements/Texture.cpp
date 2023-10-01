#include <list>

#include "Texture.hpp"
#include "Renderer.hpp"
#include <GL/glew.h>


namespace
{

	const std::string vertexShader =
		"#version 410 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 texCoord;\n"
		"layout(location = 3) in float texIndex;\n"
		"out vec2 v_TexCoord;\n"
		"out vec2 v_TexIndex;\n"
		"uniform mat4 projectionview;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projectionview * position;\n"
		"    v_TexCoord = texCoord;\n"
		"    v_TexIndex = texIndex;\n"
		"}\n";

	const std::string fragmentShader =
		"#version 410 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec2 v_TexCoord;\n"
		"in float v_TexIndex;\n"
		"sampler2D u_Texture;\n"
		"void main()\n"
		"{\n"
		"    vec4 texColor = texture(u_Texture, v_TexCoord);\n"
		"    color = texColor;\n"
		"}\n";

	struct TextureVertex
    {
        glm::vec4 pos;
        glm::vec2 texCoord;
		float slot;
    };

    struct TextureIndex
    {
        uint32_t index;
    };
}


namespace GL_ENGINE
{
	Texture::Texture(std::shared_ptr<TextureAsset> texAsset, float x, float y, float length) : m_x(x),
	                                                                                           m_y(y),
																				               m_center(x+length/2.0f,y-length/2.0f,0.0f)
	{
		vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 4 * sizeof(float));
		vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 4 * sizeof(float));
	}

	std::pair<std::string, std::string> Texture::getShaderText()
	{
		return {vertexShader, fragmentShader};
	}


	glm::vec3 Texture::getCenter()
	{
		return *m_model * glm::vec4(m_center, 1.0f);
	}

	const std::vector<VertexElement>& Texture::getVertexElements()
	{
		return vertexElements;
	}

	size_t Texture::getIndicesSize() const
    {
        return 6 * sizeof(TextureIndex);
    }

    size_t Texture::getVerticesSize() const
    {
        return 4 * sizeof(TextureVertex);
    }

    size_t Texture::getIndicesCount() const
    {
        return 6;
    }


    bool Texture::fillVertices(void* v_ptr, int &size)
    {
        TextureVertex* vertex = reinterpret_cast<TextureVertex*>(v_ptr);
        vertex->pos = *m_model * glm::vec4{m_x, m_y - m_length, 0.0f, 1.0f};
        vertex->texCoord = glm::vec2{0.0f, 0.0f};
        size += sizeof(TextureVertex);
        vertex++;

        vertex->pos = *m_model * glm::vec4{m_x + m_length, m_y - m_length, 0.0f, 1.0f};
        vertex->texCoord = glm::vec2{1.0f, 0.0f};
        size += sizeof(TextureVertex);
        vertex++;

		vertex->pos = *m_model * glm::vec4{m_x + m_length, m_y, 0.0f, 1.0f};
        vertex->texCoord = glm::vec2{1.0f, 1.0f};
        size += sizeof(TextureVertex);
        vertex++;

		vertex->pos = *m_model * glm::vec4{m_x, m_y, 0.0f, 1.0f};
        vertex->texCoord = glm::vec2{0.0f, 1.0f};
        size += sizeof(TextureVertex);
        vertex++;
		return true;
    }

    bool Texture::fillIndices(void* v_ptr, int &offset, int &count)
    {
        TextureIndex* index = reinterpret_cast<TextureIndex*>(v_ptr);
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
        count += 6 * sizeof(TextureVertex);
		return true;
    }


	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &m_rendererId));
	}




}
