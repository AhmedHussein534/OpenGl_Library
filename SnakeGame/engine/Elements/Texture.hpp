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

	void bind(const glm::mat4 &viewProjection, const glm::mat4 &model) override;

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
};