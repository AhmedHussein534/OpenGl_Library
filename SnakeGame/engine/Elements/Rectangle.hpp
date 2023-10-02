#pragma once

#include <memory>

#include "IElement.hpp"


namespace GL_ENGINE
{
    class Rectangle : public IElement
    {
    public:
        Rectangle(float x, float y, float z, float length, float width,
                float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, bool isDataNormalized = true);

        virtual void setShaderData(Shader &shader, const glm::mat4& projectionView)
        {
            shader.setUniformValue("projectionview", 1, false, const_cast<float*>(glm::value_ptr(projectionView)));
        }

        const std::vector<VertexElement>& getVertexElements() override;

        size_t getIndicesSize() const override;

        size_t getVerticesSize() const override;

        size_t getIndicesCount() const override;

        std::pair<std::string, std::string> getShaderText() override;

        float getLength()
        {
            return m_length;
        }

        float getWidth()
        {
            return m_width;
        }

        virtual ElementType getElementType() override
        {
            return std::type_index(typeid(Rectangle));
        }


        virtual glm::vec3 getCenter() override;

        virtual std::string getElementName() override
		{
			return "Rectangle";
		}

        virtual bool fillVertices(void* v_ptr, int &size) override;

		virtual bool fillIndices(void* v_ptr, int &offset, int &size) override;


    private:
        float m_x;
        float m_y;
        float m_z;
        float m_length;
        float m_width;
        float m_r;
        float m_g;
        float m_b;
        float m_a;
        glm::vec3 m_center;
        std::vector<VertexElement> vertexElements;
    };
}
