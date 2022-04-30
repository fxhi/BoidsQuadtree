#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
public:
    Camera() = default;
    void setViewMatrix(glm::mat4 matrix) {
        m_viewMatrix = matrix;
    }

    void setProjectionMatrix(glm::mat4 matrix) {
        m_projectionMatrix = matrix;
    }

    glm::mat4 getViewMatrix() {
        return m_viewMatrix;
    }

    glm::mat4 getProjectionMatrix() {
        return m_projectionMatrix;
    }

private:
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
};

glm::mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top, float front, float back) {
    return glm::ortho(left, right, bottom, top, front, back);
}

glm::mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top) {
    return getOrthographicProjectionMatrix(left, right, bottom, top, -1.0f, 1.0f);
}


#endif