#ifndef SIMPLELINEGL_HPP
#define SIMPLELINEGL_HPP

#define _USE_MATH_DEFINES

#include <glm/glm.hpp>

#include <vector>

// Possible way of improving it : https://vicrucann.github.io/tutorials/osg-shader-3dlines/
// https://stackoverflow.com/questions/60440682/drawing-a-line-in-modern-opengl

class SimpleLineGL {

public:
    unsigned int VAO;

public:
    SimpleLineGL(float x1, float y1, float x2, float y2, float width = 0.01);

    SimpleLineGL();

    ~SimpleLineGL();

    void draw();

private:
    unsigned int VBO, EBO;
    glm::vec2 m_p1, m_p2;
    float m_width;
    unsigned int shaderProgram;

private:
    std::vector<float> getVertices();

    std::vector<unsigned int> getIndices();

    void createVAO();
};

#endif