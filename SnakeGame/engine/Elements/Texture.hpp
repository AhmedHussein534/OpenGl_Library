#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>

#include "IElement.hpp"
#include "TextureAsset.hpp"

namespace GL_ENGINE
{

    class Texture : public IElement
	{
	public:
		Texture(std::shared_ptr<TextureAsset> texAsset, float x, float y, float length);

		const std::vector<VertexElement>& getVertexElements() override;

		size_t getIndicesSize() const override;

        size_t getVerticesSize() const override;

        size_t getIndicesCount() const override;

        std::pair<std::string, std::string> getShaderText() override;

		virtual glm::vec3 getCenter() override;

		~Texture();

		virtual ElementType getElementType() override
        {
            return std::type_index(typeid(Texture));
        }

		virtual std::string getElementName() override
		{
			return "Texture";
		}

		virtual bool fillVertices(void* v_ptr, int &size) override;

		virtual bool fillIndices(void* v_ptr, int &offset, int &size) override;

	private:
		uint32_t m_rendererId;
		uint8_t* m_localBuffer;
		uint32_t m_activeSlot;
		float m_x;
		float m_y;
		float m_length;
		glm::vec3 m_center;
		std::vector<VertexElement> vertexElements;
	};
}
