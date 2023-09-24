#include "OrthographicCamera.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) : ICamera(glm::ortho(left, right, bottom, top, right, left), glm::mat4(1.0f))
{

}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

}

