#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Public
// ------

void Camera::setViewMatrix(glm::mat4 matrix) {
    m_viewMatrix = matrix;
}

void Camera::setProjectionMatrix(glm::mat4 matrix) {
    m_projectionMatrix = matrix;
}

glm::mat4 Camera::getViewMatrix() {
    return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return m_projectionMatrix;
}

// Non-member functions
// --------------------

glm::mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top, float front, float back) {
    return glm::ortho(left, right, bottom, top, front, back);
}

glm::mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top) {
    return getOrthographicProjectionMatrix(left, right, bottom, top, -1.0f, 1.0f);
}
