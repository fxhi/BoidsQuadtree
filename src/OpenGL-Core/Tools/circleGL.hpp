#ifndef CIRCLEGL_HPP
#define CIRCLEGL_HPP

#define _USE_MATH_DEFINES

class CircleGL {

public:
    unsigned int VAO;

public:
    CircleGL(int circleEdgeNumber);

    ~CircleGL();

    void draw();

private:
    unsigned int VBO, EBO;
    int circleEdgeNumber;

private:
    void createVAO();
};

#endif