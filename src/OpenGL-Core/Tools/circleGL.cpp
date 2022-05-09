#include "circleGL.hpp"

#include <glad/glad.h>

#include <cmath>

CircleGL::CircleGL(int circleEdgeNumber) : circleEdgeNumber(circleEdgeNumber) {
    createVAO();
}

CircleGL::~CircleGL() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void CircleGL::draw() {
    glDrawElements(GL_TRIANGLES, circleEdgeNumber*3, GL_UNSIGNED_INT, 0);
}

void CircleGL::createVAO() {
    // --- Begin : Create a circle vertices and indices to create a EBO
    // unsigned int circleEdgeNumber = 32;
    float angle = 2.0f*M_PI/circleEdgeNumber;
    float circleVertices[(circleEdgeNumber+1)*3];
    circleVertices[0] = 0.0f; // center of the circle
    circleVertices[1] = 0.0f; // center of the circle
    circleVertices[2] = 0.0f; // center of the circle
    for (auto i=0; i<circleEdgeNumber+1; i++) {
        circleVertices[0+(i+1)*3] = std::cos(angle*i);
        circleVertices[1+(i+1)*3] = std::sin(angle*i);
        circleVertices[2+(i+1)*3] = 0.0f;
    }
    unsigned int circleIndices[circleEdgeNumber*3];
    for (auto i=0; i<circleEdgeNumber; i++){
        circleIndices[0+i*3] = 0;
        circleIndices[1+i*3] = 1+i;
        circleIndices[2+i*3] = 2+i;
    }
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

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    // return VAO;
}
