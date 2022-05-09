#ifndef MULTIPLESIMPLELINEGL_HPP
#define MULTIPLESIMPLELINEGL_HPP

#define _USE_MATH_DEFINES

#include <vector>

// Possible way of improving it : https://vicrucann.github.io/tutorials/osg-shader-3dlines/
// https://stackoverflow.com/questions/60440682/drawing-a-line-in-modern-opengl


class MultipleSimpleLineGL {

public:
    unsigned int VAO;

public:
    MultipleSimpleLineGL(std::vector<float> linesPoints, float width);

    ~MultipleSimpleLineGL();

    void draw();

private:
    unsigned int VBO, EBO;
    float m_width;

    std::vector<float> m_linesPoints; // vector containing a vector with line coordinate (x1,y1,x2,y2)

private:

   std::vector<float> getVertices(float x1, float y1, float x2, float y2);
 
    std::vector<unsigned int> getIndices(unsigned int pos);

    void createVAO();
};

#endif