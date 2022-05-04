#ifndef MULTIPLESIMPLELINEGL_HPP
#define MULTIPLESIMPLELINEGL_HPP

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <cmath>

// Possible way of improving it : https://vicrucann.github.io/tutorials/osg-shader-3dlines/
// https://stackoverflow.com/questions/60440682/drawing-a-line-in-modern-opengl



class MultipleSimpleLineGL {

public:
    unsigned int VAO;

public:
    MultipleSimpleLineGL(std::vector<float> linesPoints, float width)
     : m_linesPoints(linesPoints), m_width(width) {
        m_linesPoints = linesPoints;
        createVAO();
    }

    ~MultipleSimpleLineGL() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw() {
        glDrawElements(GL_TRIANGLES, m_linesPoints.size()*2*3, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int VBO, EBO;
    float m_width;

    std::vector<float> m_linesPoints; // vector containing a vector with line coordinate (x1,y1,x2,y2)

private:

   std::vector<float> getVertices(float x1, float y1, float x2, float y2) {
        glm::vec2 p1(x1,y1);
        glm::vec2 p2(x2,y2);

        glm::vec2 p1p2;
        if (p1.y <= p2.y) {
            std::swap(p1, p1);
        }
        p1p2 = glm::vec2(p2.x - p1.x, p2.y - p1.y);
        
        glm::vec2 norm_p1p2(p1p2/glm::distance(p2, p1));

        // float theta = std::acos(glm::dot(norm_p1p2, glm::vec2(1.0f,0.0f)));
        float cos_theta = glm::dot(norm_p1p2, glm::vec2(1.0f,0.0f));
        float sin_theta = glm::dot(norm_p1p2, glm::vec2(0.0f,1.0f));

        float half_width = m_width/2.0f;

        std::vector<float> linesVertices {
            p1.x-sin_theta*half_width, p1.y+cos_theta*half_width, 0.0f,
            p2.x-sin_theta*half_width, p2.y+cos_theta*half_width, 0.0f,
            p2.x+sin_theta*half_width, p2.y-cos_theta*half_width, 0.0f,
            p1.x+sin_theta*half_width, p1.y-cos_theta*half_width, 0.0f
        };

        return linesVertices;
    }

 
    std::vector<unsigned int> getIndices(unsigned int pos) {
    
        std::vector<unsigned int> linesIndices {
            0+pos, 2+pos, 1+pos,
            0+pos, 3+pos, 2+pos
        };
    
        return linesIndices;
    }

    void createVAO() {

        std::vector<float> linesVertices;
        std::vector<int> linesIndices;

        for(auto i=0; i<m_linesPoints.size(); i=i+4) {
            std::vector<float> vertices = getVertices(m_linesPoints[i], m_linesPoints[i+1], m_linesPoints[i+2], m_linesPoints[i+3]);
            linesVertices.insert(linesVertices.end(), vertices.begin(), vertices.end());
            
            std::vector<int> indices {
                0+i, 2+i, 1+i,
                0+i, 3+i, 2+i
            };
            // std::vector<unsigned int> indices = getIndices(i*4); // 4 indices
            linesIndices.insert(linesIndices.end(), indices.begin(), indices.end());
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, linesVertices.size() * sizeof(float), &linesVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, linesIndices.size() * sizeof(unsigned int), &linesIndices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0); 
    }
};

#endif