#include "PerspectiveCamera.hpp"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far) : ICamera(glm::perspective(glm::radians(fov), aspectRatio, near, far),
                                                                                                    glm::mat4(1.0f))
{

}

void PerspectiveCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

}

