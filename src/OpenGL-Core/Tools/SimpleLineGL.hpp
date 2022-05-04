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

    std::vector<float> getVertices() {
        glm::vec2 p1p2;
        if (m_p1.y <= m_p2.y) {
            std::swap(m_p1, m_p1);
        }
        p1p2 = glm::vec2(m_p2.x - m_p1.x, m_p2.y - m_p1.y);
        
        glm::vec2 norm_p1p2(p1p2/glm::distance(m_p2, m_p1));

        // float theta = std::acos(glm::dot(norm_p1p2, glm::vec2(1.0f,0.0f)));
        float cos_theta = glm::dot(norm_p1p2, glm::vec2(1.0f,0.0f));
        float sin_theta = glm::dot(norm_p1p2, glm::vec2(0.0f,1.0f));

        float half_width = m_width/2.0f;

        std::vector<float> circleVertices {
            m_p1.x-sin_theta*half_width, m_p1.y+cos_theta*half_width, 0.0f,
            m_p2.x-sin_theta*half_width, m_p2.y+cos_theta*half_width, 0.0f,
            m_p2.x+sin_theta*half_width, m_p2.y-cos_theta*half_width, 0.0f,
            m_p1.x+sin_theta*half_width, m_p1.y-cos_theta*half_width, 0.0f
        };

        return circleVertices;
    }

    std::vector<unsigned int> getIndices() {
        // unsigned int* circleIndices;
        // circleIndices =  new unsigned int[3*2]{
        //         0, 2, 1,
        //         0, 3, 2
        // };
    
        // return circleIndices;
        std::vector<unsigned int> circleIndices {
                0, 2, 1,
                0, 3, 2
        };
    
        return circleIndices;
    }

    void createVAO() {

        std::vector<float> p_circleVertices = getVertices();
        std::vector<unsigned int> p_circleIndices = getIndices();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, p_circleVertices.size() * sizeof(float), &p_circleVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_circleIndices.size() * sizeof(unsigned int), &p_circleIndices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0); 
    }
};

#endif