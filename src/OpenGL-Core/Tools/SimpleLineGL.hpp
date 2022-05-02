#ifndef SIMPLELINEGL_HPP
#define SIMPLELINEGL_HPP

#include <glm/glm.hpp>

#include <glad/glad.h>


class SimpleLineGL {

public:
    unsigned int VAO;

public:
    SimpleLineGL(float x1, float y1, float x2, float y2, float width = 0.01) : m_p1(x1, y1), m_p2(x2, y2), m_width(width) {
        createVAO();
    }
    SimpleLineGL() {
        createVAO();
    }

    ~SimpleLineGL() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw() {
        glDrawElements(GL_TRIANGLES, 2*3, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int VBO, EBO;
    glm::vec2 m_p1, m_p2;
    float m_width;
    unsigned int shaderProgram;

private:
    void createVAO() {

        float circleVertices[4*3];
        if (m_p1.y != m_p2.y) {
            float tmp[4*3] = {
                m_p1.x-m_width/2.0f, m_p1.y, 0.0f,
                m_p2.x-m_width/2.0f, m_p2.y, 0.0f,
                m_p2.x+m_width/2.0f, m_p2.y, 0.0f,
                m_p1.x+m_width/2.0f, m_p1.y, 0.0f
            };
            std::copy(std::begin(tmp), std::end(tmp), std::begin(circleVertices));
        } else {
            float tmp[4*3] = {
                m_p1.x, m_p1.y-m_width/2.0f, 0.0f,
                m_p2.x, m_p2.y-m_width/2.0f, 0.0f,
                m_p2.x, m_p2.y+m_width/2.0f, 0.0f,
                m_p1.x, m_p1.y+m_width/2.0f, 0.0f
            };
            std::copy(std::begin(tmp), std::end(tmp), std::begin(circleVertices));
            // circleVertices = tmp;
        }
        // if glEnable(GL_CULL_FACE); 
        unsigned int circleIndices[4*3] = {
                0, 2, 1,
                0, 3, 2
        };
        // unsigned int circleIndices[] = {
        //     0, 1, 2,
        //     0, 2, 3
        // };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleIndices), circleIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0); 
        // return VAO;
    }
};

#endif