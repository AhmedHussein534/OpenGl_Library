#include "Texture3D.hpp"
#include "Renderer.hpp"
#include <GL/glew.h>
#include "external/stb_image/stb_image.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

	const std::string vertexShader =
		"#version 410 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 texCoord;\n"
		"out vec2 v_TexCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projectionview;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projectionview * model * position;\n"
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


Texture3D::Texture3D(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP, uint32_t slot,
                 float x, float y, float z, float length, float rotate, float rotateAxisX, float rotateAxisY) : m_rendererId(0),
                                         m_localBuffer(localBuffer),
	                                     m_width(width),
	                                     m_height(height),
                                         m_length(length),
	                                     m_BPP(BPP),
	                                     m_activeSlot(slot),
                                         m_x(x),
                                         m_y(y),
                                         m_z(z),
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



    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);


    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(0.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z - m_length);
    vertexData->push_back(1.0f);
    vertexData->push_back(1.0f);

    vertexData->push_back(x + m_length);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(1.0f);
    vertexData->push_back(0.0f);

    vertexData->push_back(x);
    vertexData->push_back(y - m_length);
    vertexData->push_back(z);
    vertexData->push_back(0.0f);
    vertexData->push_back(0.0f);





    indexData->push_back(0);
    indexData->push_back(1);
    indexData->push_back(2);
    indexData->push_back(3);
    indexData->push_back(4);
    indexData->push_back(5);
    indexData->push_back(6);
    indexData->push_back(7);
    indexData->push_back(8);
    indexData->push_back(9);
    indexData->push_back(10);
    indexData->push_back(11);
    indexData->push_back(12);
    indexData->push_back(13);
    indexData->push_back(14);
    indexData->push_back(15);
    indexData->push_back(16);
    indexData->push_back(17);
    indexData->push_back(18);
    indexData->push_back(19);
    indexData->push_back(20);
    indexData->push_back(21);
    indexData->push_back(22);
    indexData->push_back(23);
    indexData->push_back(24);
    indexData->push_back(25);
    indexData->push_back(26);
    indexData->push_back(27);
    indexData->push_back(28);
    indexData->push_back(29);
    indexData->push_back(30);
    indexData->push_back(31);
    indexData->push_back(32);
    indexData->push_back(33);
    indexData->push_back(34);
    indexData->push_back(35);
    vertexBuffer = std::make_unique<VertexBuffer>(vertexData);
    indexBuffer  = std::make_unique<IndexBuffer>(indexData);
    vertexElements.emplace_back(3, ElementDataType::FLOAT, true, 5 * sizeof(float));
    vertexElements.emplace_back(2, ElementDataType::FLOAT, true, 5 * sizeof(float));

}

void Texture3D::bind(const glm::mat4 &viewProjection)
{
    vertexBuffer->bind();
    indexBuffer->bind();
    shader->bind();
    shader->setUniformValue("u_Texture", int(0));
    shader->setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(viewProjection)));
    shader->setUniformValue("model", 1, false, const_cast<float*>(glm::value_ptr(*m_model)));
    GLCall(glActiveTexture(GL_TEXTURE0 + m_activeSlot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
}

void Texture3D::unbind()
{
	GLCall(glActiveTexture(GL_TEXTURE0 + m_activeSlot));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

const std::vector<VertexElement>& Texture3D::getVertexElements()
{
    return vertexElements;
}

unsigned int Texture3D::getIndicesCount()
{
    return 36;
}


Texture3D::~Texture3D()
{
	GLCall(glDeleteTextures(1, &m_rendererId));
}



StbTexture3D::StbTexture3D(const std::string &path)
{
	int width = 0;
	int height = 0;
	int bpp = 0;
	stbi_set_flip_vertically_on_load(1);
	uint8_t* m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	if (m_localBuffer)
	{
		texPtr = std::make_shared<Texture3D>(m_localBuffer, width, height, bpp, 0, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 1.0f);
		stbi_image_free(m_localBuffer);
	}
	else
	{
		std::cout << "FAILED TO LOAD IMAGE" << std::endl;
	}
}

std::shared_ptr<Texture3D> StbTexture3D::getTex()
{
	return texPtr;
}

StbTexture3D::~StbTexture3D()
{
}