#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


namespace GL_ENGINE
{
    class Texture : public IElement
	{
	public:
		Texture(uint8_t* localBuffer, int32_t width, int32_t height,
				int32_t BPP, uint32_t slot, float x, float y, float length);

		void bind(const glm::mat4 &viewProjection, const glm::mat4 &model) override;

		void unbind() override;

		const std::vector<VertexElement>& getVertexElements() override;

		unsigned int getIndicesCount() override;

		virtual glm::vec3 getCenter() override;

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
		glm::vec3 m_center;
		std::vector<VertexElement> vertexElements;
	};


	class StbTexture
	{
	public:
		StbTexture(const std::string& path);

		std::shared_ptr<Texture> getTex();

		~StbTexture();
	private:
		std::shared_ptr<Texture> texPtr;
	};
}
