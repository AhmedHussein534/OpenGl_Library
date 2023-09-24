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
        const glm::vec3& GetTarget() const { return m_target; }
        const glm::vec3& GetUp() const { return m_up; }
		void setDirection(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up=glm::vec3(0.0f, 1.0f, 0.0f))
        {
            m_Position = position;
            m_target = target;
            m_up = up;
            RecalculateViewMatrix();
        }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


	protected:
        virtual void RecalculateViewMatrix()
        {

            m_ViewMatrix = glm::lookAt(m_Position,
                                       m_target,
                                       m_up);

            //glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
            //m_ViewMatrix = glm::inverse(transform);
            std::cout << glm::to_string(m_ViewMatrix) << std::endl;
            m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        }

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_target = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };

};