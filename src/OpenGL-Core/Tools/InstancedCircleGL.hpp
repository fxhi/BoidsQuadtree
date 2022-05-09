#ifndef INSTANCEDCIRCLEGL_HPP
#define INSTANCEDCIRCLEGL_HPP

#define _USE_MATH_DEFINES

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO : 
//  - Maybe use std::vector rather that array, like this do not need to deduce the number of instance.
//  - Maybe add a way to check if the number of elements in m_modelMatrices is equal to m_numberInstance.

class InstancedCircleGL {

public:
    unsigned int VAO;

public:
    InstancedCircleGL(unsigned int circleEdgeNumber);
    
    ~InstancedCircleGL();
    
    void draw();
    
    void setNumberInstance(unsigned int numberInstance);
    
    void setModelMatrices(glm::mat4* modelMatrices);
    
private:
    unsigned int VBO, EBO;
    unsigned int m_instanceVBO;
    unsigned int circleEdgeNumber;
    unsigned int m_numberInstance;
    glm::mat4* m_modelMatrices = nullptr;

private:
    void createVAO();
};

#endif