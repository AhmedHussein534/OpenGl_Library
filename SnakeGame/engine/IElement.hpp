#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

namespace GL_ENGINE
{
	using ElementType = std::type_index;
    enum class ElementDataType : uint32_t
	{
		FLOAT = GL_FLOAT,
	};

	struct VertexElement
	{
		uint32_t count;
		ElementDataType dataType;
		bool normalized;
		uint32_t stride;

		VertexElement(uint32_t m_count, ElementDataType m_dataType, bool m_normalized, uint32_t m_stride) : count(m_count),
																											dataType(m_dataType),
																											normalized(m_normalized),
																											stride(m_stride)
		{

		}

		size_t getDataSize() const
		{
			switch (dataType)
			{
				case ElementDataType::FLOAT:
					return sizeof(float);
				default:
					return 0;
			}
		}

		uint32_t getGlDataType() const
		{
			return static_cast<uint32_t>(dataType);
		}
	};


	class IElement
	{
	public:
		IElement() : m_model(std::make_shared<glm::mat4>(1.0f))
		{

		}

		IElement(std::shared_ptr<glm::mat4> model) : m_model(model)
		{

		}

		std::shared_ptr<glm::mat4> getModel()
		{
			return m_model;
		}

		static GL_ENGINE::ElementType getDefaultElementId()
		{
			return typeid(IElement);
		}

		virtual ElementType getElementType() = 0;

		virtual size_t getIndicesSize() const = 0;

        virtual size_t getVerticesSize() const = 0;

		virtual size_t getIndicesCount() const = 0;

		virtual bool fillVertices(void* v_ptr, int &size) = 0;

		virtual bool fillIndices(void* v_ptr, int &offset, int &size) = 0;

		virtual const std::vector<VertexElement>& getVertexElements() = 0;

		virtual glm::vec3 getCenter() = 0;

		virtual std::string getElementName() = 0;

		virtual std::pair<std::string, std::string> getShaderText() = 0;

		virtual ~IElement() = default;

	protected:
		std::shared_ptr<glm::mat4> m_model;
	};
}
