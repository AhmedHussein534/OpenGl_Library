#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


namespace TextureData
{
	constexpr size_t positionCount = 16;
	constexpr size_t indexCount = 6;
}

class Texture : public IElement
{
public:
	Texture(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP, uint32_t slot, 
		    float x, float y, float length, float rotate, float rotateAxisX, float rotateAxisY);

	void bind() override;

	void unbind() override;

	const std::vector<VertexElement>& getVertexElements() override;

	unsigned int getIndicesCount() override;

	~Texture();

private:
	
	uint32_t m_rendererId;
	uint8_t* m_localBuffer;
	int m_width;
	int m_height;
	int m_BPP;
	uint32_t m_activeSlot;
	std::array<float, TextureData::positionCount> position;
	std::array<unsigned int, TextureData::indexCount> indices;
	float m_x;
	float m_y;
	float m_length;
	float m_rotate;
	float m_rotateAxisX;
	float m_rotateAxisY;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<Shader> shader;
	std::vector<VertexElement> vertexElements;
	
	std::string vertexShader =
		"#version 410 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 texCoord;\n"
		"out vec2 v_TexCoord;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = position;\n"
		"    v_TexCoord = texCoord;\n"
		"}\n";

	std::string fragmentShader =
		"#version 410 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec2 v_TexCoord;\n"
		"uniform sampler2D u_Texture;\n"
		"void main()\n"
		"{\n"
		"    vec4 texColor = texture(u_Texture, v_TexCoord);\n"
		"    color = texColor;\n"
		"}\n";



};