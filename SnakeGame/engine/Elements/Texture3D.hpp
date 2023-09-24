#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


class Texture3D : public IElement
{
public:
	Texture3D(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP, uint32_t slot,
		    float x, float y, float z, float length, float rotate, float rotateAxisX, float rotateAxisY);

	void bind(const glm::mat4 &viewProjection) override;

	void unbind() override;

	const std::vector<VertexElement>& getVertexElements() override;

	unsigned int getIndicesCount() override;

	~Texture3D();

private:

	uint32_t m_rendererId;
	uint8_t* m_localBuffer;
	int m_width;
	int m_height;
	int m_BPP;
	uint32_t m_activeSlot;
	float m_x;
	float m_y;
    float m_z;
	float m_length;
	float m_rotate;
	float m_rotateAxisX;
	float m_rotateAxisY;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<Shader> shader;
	std::vector<VertexElement> vertexElements;
};

class StbTexture3D
{
public:
	StbTexture3D(const std::string& path);

	std::shared_ptr<Texture3D> getTex();

	~StbTexture3D();
private:
	std::shared_ptr<Texture3D> texPtr;
};