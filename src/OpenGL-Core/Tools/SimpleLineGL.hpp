#ifndef SIMPLELINEGL_HPP
#define SIMPLELINEGL_HPP

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <cmath>

// Possible way of improving it : https://vicrucann.github.io/tutorials/osg-shader-3dlines/
// https://stackoverflow.com/questions/60440682/drawing-a-line-in-modern-opengl



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

        glm::vec2 p1p2;
        if (m_p1.y <= m_p2.y) {
            p1p2 = glm::vec2(m_p2.x - m_p1.x, m_p2.y - m_p1.y);
        } else {
            p1p2 = glm::vec2(m_p1.x - m_p2.x, m_p1.y - m_p2.y);
        }
        
        glm::vec2 norm_p1p2(p1p2/glm::distance(m_p2, m_p1));

        // float theta = std::acos(glm::dot(norm_p1p2, glm::vec2(1.0f,0.0f)));
        float cos_theta = glm::dot(norm_p1p2, glm::vec2(1.0f,0.0f));
        float sin_theta = glm::dot(norm_p1p2, glm::vec2(0.0f,1.0f));

        float half_width = m_width/2.0f;

        float circleVertices[4*3] = {
            m_p1.x-sin_theta*half_width, m_p1.y+cos_theta*half_width, 0.0f,
            m_p2.x-sin_theta*half_width, m_p2.y+cos_theta*half_width, 0.0f,
            m_p2.x+sin_theta*half_width, m_p2.y-cos_theta*half_width, 0.0f,
            m_p1.x+sin_theta*half_width, m_p1.y-cos_theta*half_width, 0.0f
        };
        // }
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