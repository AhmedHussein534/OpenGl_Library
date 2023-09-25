#pragma once

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <memory>
#include <typeinfo>
#include <GL/glew.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
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
		return static_cast<std::underlying_type_t<ElementDataType>>(dataType);
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

	std::shared_ptr<VertexBuffer> getVertexBuffer()
	{
		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> getIndexBuffer()
	{
		return indexBuffer;
	}

	virtual void bind(const glm::mat4 &viewProjection = glm::mat4(1.0f)) = 0;

	virtual void unbind() = 0;

	virtual const std::vector<VertexElement>& getVertexElements() = 0;

	virtual unsigned int getIndicesCount() = 0;

	virtual glm::vec3 getCenter() = 0;

	std::shared_ptr<glm::mat4> getModel()
	{
		return m_model;
	}

	virtual ~IElement() = default;

protected:
	std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
	std::shared_ptr<glm::mat4> m_model;
};