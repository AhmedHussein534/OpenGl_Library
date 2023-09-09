#include "Texture.hpp"
#include "Renderer.hpp"
#include <GL/glew.h>

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
                                         vertexBuffer(std::make_unique<VertexBuffer>()),
                                         indexBuffer(std::make_unique<IndexBuffer>())
{
    std::cout << "Texture width = " << m_width << std::endl;
    std::cout << "Texture height = " << m_height << std::endl;
    
	GLCall(glGenTextures(1, &m_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));


    position[0] = m_x;
    position[1] = m_y - m_length;
    position[2] = 0.0f;
    position[3] = 0.0f;

    position[4] = m_x + m_length;
    position[5] = m_y - m_length;
    position[6] = 1.0f;
    position[7] = 0.0f;

    position[8] = m_x + m_length;
    position[9] = m_y;
    position[10] = 1.0f;
    position[11] = 1.0f;

    position[12] = m_x;
    position[13] = m_y;
    position[14] = 0.0f;
    position[15] = 1.0f;

    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 4 * sizeof(float));
    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 4 * sizeof(float));
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
    
    std::cout << "TEXTURE CONSTRUCTOR DONE!" << std::endl;
}

void Texture::bind()
{
    vertexBuffer->addVertexData(position.data(), sizeof(position));
    indexBuffer->addIndexData(indices.data(), sizeof(indices));
    shader = std::make_unique<Shader>(vertexShader, fragmentShader);
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