#pragma once

#include "engine/ICamera.hpp"
#include "glm/glm.hpp"

namespace GL_ENGINE
{
	class OrthographicCamera : public ICamera
	{
		public:
			OrthographicCamera(float left, float right, float bottom, float top);

			void SetProjection(float left, float right, float bottom, float top);

			const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
			const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
			const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	};
}
