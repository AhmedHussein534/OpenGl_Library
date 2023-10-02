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
		Texture(std::shared_ptr<TextureAsset> texAsset, float x, float y, float length, float width, float tiling = 1.0f);

		void setTexAsset(std::shared_ptr<TextureAsset> texAsset)
		{
			m_texAsset = texAsset;
		}

		std::shared_ptr<TextureAsset> getTexAsset()
		{
			return m_texAsset;
		}

		virtual void setShaderData(Shader &shader, const glm::mat4& projectionView)
        {
			int arr[32] = {};
			for (int i = 0; i < 32; i++)
			{
				arr[i] = i;
			}

			shader.setUniformValue("u_Textures", static_cast<int*>(arr), static_cast<uint32_t>(32));
			shader.setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(projectionView)));
        }

		const std::vector<VertexElement>& getVertexElements() override;

		size_t getIndicesSize() const override;

        size_t getVerticesSize() const override;

        size_t getIndicesCount() const override;

        std::pair<std::string, std::string> getShaderText() override;

		virtual glm::vec3 getCenter() override;

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

		~Texture() = default;

	private:
		float m_x;
		float m_y;
		float m_length;
		float m_width;
		float m_tiling;
		glm::vec3 m_center;
		std::vector<VertexElement> vertexElements;
		std::shared_ptr<TextureAsset> m_texAsset;
	};
}
