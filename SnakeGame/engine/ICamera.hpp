#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

class ICamera
{
    public:
		ICamera() = default;
		ICamera(const glm::mat4& projection, const glm::mat4& view)
			: m_ProjectionMatrix(projection), m_ViewMatrix(view), m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
        {

        }

		virtual ~ICamera() = default;

        const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation, glm::vec3 rotationAxis = {0, 0, 1})
        {
            m_Rotation = rotation;
            m_rotationAxis = rotationAxis;
            RecalculateViewMatrix();
        }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


	protected:
        virtual void RecalculateViewMatrix()
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), m_rotationAxis);

            m_ViewMatrix = glm::inverse(transform);
            m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        }

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
        float m_Rotation = 0.0f;
        glm::vec3 m_rotationAxis = {0, 0, 1};
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
};