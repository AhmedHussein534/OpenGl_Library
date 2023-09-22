#include "Texture.hpp"
#include "Renderer.hpp"
#include <GL/glew.h>

namespace
{

	const std::string vertexShader =
		"#version 410 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 texCoord;\n"
		"out vec2 v_TexCoord;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = position;\n"
		"    v_TexCoord = texCoord;\n"
		"}\n";

	const std::string fragmentShader =
		"#version 410 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec2 v_TexCoord;\n"
		"uniform sampler2D u_Texture;\n"
		"void main()\n"
		"{\n"
		"    vec4 texColor = texture(u_Texture, v_TexCoord);\n"
		"    color = texColor;\n"
		"}\n";
}


Texture::Texture(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP, uint32_t slot,
                 float x, float y, float length, float rotate, float rotateAxisX, float rotateAxisY) : m_rendererId(0),
                                         m_localBuffer(localBuffer),
	                                     m_width(width),
	                                     m_height(height),
                                         m_length(length),
	                                     m_BPP(BPP),
	                                     m_activeSlot(slot),
                                         m_x(x),
                                         m_y(y),
                                         m_rotate(rotate),
                                         m_rotateAxisX(rotateAxisX),
                                         m_rotateAxisY(rotateAxisY),
                                         vertexBuffer(nullptr),
                                         indexBuffer(nullptr),
                                         shader(std::make_unique<Shader>(vertexShader, fragmentShader))
{
	GLCall(glGenTextures(1, &m_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    std::shared_ptr<std::vector<float>> vertexData = std::make_shared<std::vector<float>>();
    std::shared_ptr<std::vector<uint32_t>> indexData = std::make_shared<std::vector<uint32_t>>();

    vertexData->push_back(m_x);
    vertexData->push_back(m_y - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);
    vertexData->push_back(m_x + m_length);
    vertexData->push_back(m_y - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);
    vertexData->push_back(m_x + m_length);
    vertexData->push_back(m_y);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);
    vertexData->push_back(m_x);
    vertexData->push_back(m_y);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);


    indexData->push_back(0);
    indexData->push_back(1);
    indexData->push_back(2);
    indexData->push_back(2);
    indexData->push_back(3);
    indexData->push_back(0);
    vertexBuffer = std::make_unique<VertexBuffer>(vertexData);
    indexBuffer  = std::make_unique<IndexBuffer>(indexData);
    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 4 * sizeof(float));
    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 4 * sizeof(float));

}

void Texture::bind()
{
    vertexBuffer->bind();
    indexBuffer->bind();
    shader->bind();
    shader->setUniformValue("u_Texture", int(0));
    GLCall(glActiveTexture(GL_TEXTURE0 + m_activeSlot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
}

void Texture::unbind()
{
	GLCall(glActiveTexture(GL_TEXTURE0 + m_activeSlot));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

const std::vector<VertexElement>& Texture::getVertexElements()
{
    return vertexElements;
}

unsigned int Texture::getIndicesCount()
{
    return 6;
}


Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_rendererId));
}