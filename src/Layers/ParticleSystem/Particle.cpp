#include "Particle.hpp"

#include <cmath>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../OpenGL-Core/LayerGL.hpp"
#include "../../OpenGL-Core/Shader.hpp"
#include "../../OpenGL-Core/Time.hpp"
#include "../../OpenGL-Core/Tools/CircleGL.hpp"
#include "../../OpenGL-Core/Tools/SimpleLineGL.hpp"

#include "../../Mesh/Quadtree.hpp"

#include "../../Tools/RandomNumber.hpp"
#include "../../Tools/Geometry/Rectangle.hpp"
#include "../../Tools/Geometry/Line.hpp"

#include "../Camera.hpp"

// Public
// ------

ParticleLayer::ParticleLayer(unsigned int numberParticles, int numberEdges)
     : m_shader("src/shader/vertexShader.glsl", "src/shader/fragmentShader.glsl"),
       m_numberEdges(numberEdges),
       m_circleGL(m_numberEdges),
       m_numberParticles(numberParticles),
       m_shaderInstancing("src/shader/InstancingVertexShader.glsl", "src/shader/InstancingFragmentShader.glsl"),
       m_instancedCircleGL(m_numberEdges) {
    
    srand(time(NULL));
    for (size_t i=0; i<m_numberParticles; i++) {
        float x = 0.0;
        float y = 0.0;
        // float x = randomFloat(-1.0, 1.0);
        // float y = randomFloat(-1.0, 1.0);
        float z = 0.0;
        float angle = randomAngle();
        float vx = std::cos(angle)/2;
        float vy = std::sin(angle)/2;
        // float vx = 0.0;
        // float vy = 0.0;
        float vz = 0.0f;
        vec_particles.push_back(Particle(x,y,z, vx,vy,vz, 0.05));
    }
}

ParticleLayer::~ParticleLayer() {
    glDeleteProgram(m_shader.ID);
}

void ParticleLayer::initQuadtree() {
    m_quadtree.m_boundary = *m_boundary;
}

// TODO : try to fill the quadtree with pointers.
void ParticleLayer::fillQuadtree() {
    // Insert the particle, into the quadtree, as a pointer to the last element of the vector
    // &vec_particles.back() : get the reference of the last element of the vector
    for(auto p : vec_particles) {
        m_quadtree.insert(&p, p.position.x, p.position.y);
    }
}

void ParticleLayer::update(const Time& time) {
    for (auto & particle : vec_particles) {
        particle.position += particle.velocity * glm::vec3(time.getTimeStep());
    }
    applyBoundaryCondition();
}

void ParticleLayer::render() {
    bool useInstancingCircle = true;

    bool do_rendering = true;
    
    if(do_rendering) {
        if (useInstancingCircle) {
            // Instancing
            //-----------
            m_shaderInstancing.use();

            // Get and set view matrix
            glm::mat4 viewMatrix = m_camera->getViewMatrix();
            unsigned int ID_viewMatrixInstancing = glGetUniformLocation(m_shaderInstancing.ID, "u_view");
            glUniformMatrix4fv(ID_viewMatrixInstancing, 1, GL_FALSE, glm::value_ptr(viewMatrix));

            glm::mat4* modelMatrix = new glm::mat4[vec_particles.size()];
            int index = 0;

            for(auto const & particle : vec_particles) {
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform, glm::vec3(particle.position.x, particle.position.y, 0.0f));
                transform = glm::scale(transform, glm::vec3(particle.size, particle.size, particle.size));

                modelMatrix[index] = transform;
                index++;
            }

            m_instancedCircleGL.setNumberInstance(vec_particles.size());
            m_instancedCircleGL.setModelMatrices(modelMatrix);
            m_instancedCircleGL.draw();
        } else {
            m_shader.use();
            // Get and set view matrix
            glm::mat4 viewMatrix = m_camera->getViewMatrix();
            unsigned int ID_viewMatrix = glGetUniformLocation(m_shader.ID, "u_view");
            glUniformMatrix4fv(ID_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            // Get ID matrix for model matrix
            unsigned int ID_transform = glGetUniformLocation(m_shader.ID, "transform");

            glBindVertexArray(m_circleGL.VAO);
            for(auto const & particle : vec_particles) {
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform, glm::vec3(particle.position.x, particle.position.y, 0.0f));
                transform = glm::scale(transform, glm::vec3(particle.size, particle.size, particle.size));

                glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));

                m_circleGL.draw();
            }

        }
    }

    // Update the quadtree
    m_quadtree.clear();
    fillQuadtree();

    bool do_render_mesh = false;

    if(do_render_mesh) {
        // Render the quadress
        std::vector<Line<float>> linesToDraw = m_quadtree.getLineToDrawShape();
        // std::cout << "Line to draw size : " << linesToDraw.size() << std::endl;
        // std::cout << " Total number of point : " << m_quadtree.m_total_points << std::endl;
        // std::cout << " Max depth : " << m_quadtree.getMaxDepth()  << std::endl;

        unsigned int ID_transform = glGetUniformLocation(m_shader.ID, "transform");
        glm::mat4 transform = glm::mat4(1.0f);
        glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));
        // glUseProgram(0);
        for(auto const & line : linesToDraw) {
            // TODO : not good need to create a line object everytime !
            // SimpleLineGL lineGL(line.x1, line.y1, line.x1, line.y1, 0.01);
            SimpleLineGL lineGL(line.x2, line.y2, line.x1, line.y1, 0.02);
            // SimpleLineGL lineGL(line.x1, line.y1, line.x2, line.y2, 0.1);
            // std::cout << " " << line.x1 << " " << line.y1 << " " << line.x2 << " " << line.y2 << std::endl;
            glBindVertexArray(lineGL.VAO);
            lineGL.draw();
        }
    }
}

void ParticleLayer::setBoundary(Rectangle<>* boundary) {
    m_boundary = boundary;
}

void ParticleLayer::setCamera(std::shared_ptr<Camera> camera) {
    m_camera = camera;
}

// Private 
// -------

void ParticleLayer::applyBoundaryCondition() {
    for (auto & particle : vec_particles) {
        //using a modulo instead ?
        if (particle.position.x < m_boundary->getLeft()) {particle.position.x = m_boundary->getRight();}
        if (m_boundary->getRight() < particle.position.x) {particle.position.x = m_boundary->getLeft();}

        if(particle.position.y < m_boundary->getBottom()) {particle.position.y = m_boundary->getTop();}
        if(m_boundary->getTop() < particle.position.y) { particle.position.y = m_boundary->getBottom();}
    }
}