#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
public:
    Camera() = default;

    void setViewMatrix(glm::mat4 matrix);

    void setProjectionMatrix(glm::mat4 matrix);

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

private:
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;

    int m_tmp;
};

glm::mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top, float front, float back);

glm::mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top);

#endif