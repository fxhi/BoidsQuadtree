#ifndef INSTANCEDCIRCLEGL_HPP
#define INSTANCEDCIRCLEGL_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO : maybe use std::vector rather that array, like this do not need to deduce the number of instance.
class InstancedCircleGL {

public:
    unsigned int VAO;

public:
    InstancedCircleGL(unsigned int circleEdgeNumber) : circleEdgeNumber(circleEdgeNumber) {
        // Nothing
        createVAO();
    }

    ~InstancedCircleGL() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw() {
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_numberInstance, &m_modelMatrices[0], GL_STATIC_DRAW); // https://stackoverflow.com/questions/40248360/opengl-glbufferdata-why-vertices0-instead-of-vertices-as-data-parameter
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, circleEdgeNumber*3, GL_UNSIGNED_INT, 0, m_numberInstance);

        glBindVertexArray(0);
    }

    void setNumberInstance(unsigned int numberInstance) {
        m_numberInstance = numberInstance;
    }

    void setModelMatrices(glm::mat4* modelMatrices) {
        m_modelMatrices = modelMatrices;
    }

    // Maybe add a way to check if the number of elements in m_modelMatrices is equal to m_numberInstance.

// private:
public:
    unsigned int VBO, EBO;
    unsigned int m_instanceVBO;
    unsigned int circleEdgeNumber;
    unsigned int m_numberInstance;
    glm::mat4* m_modelMatrices = nullptr;

private:
    void createVAO() {
        // Vertices and indices creation
        // -----------------------------
        float angle = 2.0f*M_PI/circleEdgeNumber;
        float circleVertices[(circleEdgeNumber+1)*3];
        
        // center of the circle
        circleVertices[0] = 0.0f;
        circleVertices[1] = 0.0f;
        circleVertices[2] = 0.0f;

        // Loop over the circle, get the the vertices position
        for (auto i=0; i<circleEdgeNumber+1; i++) {
            circleVertices[0+(i+1)*3] = std::cos(angle*i);
            circleVertices[1+(i+1)*3] = std::sin(angle*i);
            circleVertices[2+(i+1)*3] = 0.0f;
        }

        // Get the associated indices
        unsigned int circleIndices[circleEdgeNumber*3];
        for (auto i=0; i<circleEdgeNumber; i++){
            circleIndices[0+i*3] = 0;
            circleIndices[1+i*3] = 1+i;
            circleIndices[2+i*3] = 2+i;
        }

        // Last indices is the same as the second point (first one is the circle center), to close the circle.
        circleIndices[circleEdgeNumber*3-1] = 1;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleIndices), circleIndices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        //Instance data
        // ------------
    
        //create VBO
        glGenBuffers(1, &m_instanceVBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO); // this attribute comes from a different vertex buffer
    
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
    
        // tell OpenGL this is an instanced vertex attribute.
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        // Clear 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
    }

};

#endif