#pragma once

#include <memory>
#include <array>

#include "IElement.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"


namespace GL_ENGINE
{
    class Square : public IElement
    {
    public:
        Square(float x, float y, float length,
            float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool isDataNormalized = true);

        void bind(const glm::mat4 &viewProjection, const glm::mat4 &model) override;

        void unbind() override;

        float getLength()
        {
            return m_length;
        }

        const std::vector<VertexElement>& getVertexElements() override;

        unsigned int getIndicesCount() override;

        virtual glm::vec3 getCenter() override;

        virtual ElementType getElementType() override
        {
            return std::type_index(typeid(Square));
        }

        virtual std::string getElementName() override
		{
			return "Square";
		}


    private:
        float m_x;
        float m_y;
        float m_length;
        float m_r;
        float m_g;
        float m_b;
        float m_a;
        glm::vec3 m_center;
        std::vector<VertexElement> vertexElements;
    };
}
