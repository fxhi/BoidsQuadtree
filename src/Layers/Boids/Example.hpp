#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../OpenGL-Core/LayerGL.hpp"
#include "../../OpenGL-Core/shader.hpp"
#include "../../OpenGL-Core/Time.hpp"
#include "../../OpenGL-Core/Tools/CircleGL.hpp"

#include "../../Tools/RandomNumber.hpp"
#include "../../Tools/Geometry/Rectangle.hpp"

#include "../Camera.hpp"

struct Particle {
    Particle(float x, float y, float z, float vx, float vy, float vz, float size)
        : position(glm::vec3(x,y,z)), velocity(glm::vec3(vx,vy,vz)), size(size) {}

    glm::vec3 position;
    glm::vec3 velocity;
    float size;
};

class ParticleLayer : public LayerGL {
public:

    ParticleLayer(unsigned int m_numberParticles, int numberEdges=4) :
        m_shader("src/shader/vertexShader.glsl", "src/shader/fragmentShader.glsl"), m_numberEdges(numberEdges), m_circleGL(m_numberEdges) {
        
        srand(time(NULL));
        for (auto i=0; i<m_numberParticles; i++) {
            float x = 0.0;
            float y = 0.0;
            // float x = randomFloat(-1.0, 1.0);
            // float y = randomFloat(-1.0, 1.0);
            float z = 0.0;
            float angle = randomAngle();
            float vx = std::cos(angle);
            float vy = std::sin(angle);
            // float vx = 0.0;
            // float vy = 0.0;
            float vz = 0.0f;
            vec_particles.push_back(Particle(x,y,z, vx,vy,vz, 0.05));
        }
    }

    ~ParticleLayer() {
        glDeleteProgram(m_shader.ID);
    }

    void update(const Time& time) {
        for (auto & particle : vec_particles) {
            particle.position += particle.velocity * glm::vec3(time.getTimeStep());
        }
        applyBoundaryCondition();
    }

    void render() override {
        m_shader.use();
        unsigned int ID_transform = glGetUniformLocation(m_shader.ID, "transform");
        glBindVertexArray(m_circleGL.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        
        glm::mat4 viewMatrix = m_camera->getViewMatrix();
        unsigned int ID_viewMatrix = glGetUniformLocation(m_shader.ID, "u_view");
        glUniformMatrix4fv(ID_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        for (auto const & particle : vec_particles) {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(particle.position.x, particle.position.y, particle.position.z));
            transform = glm::scale(transform, glm::vec3(particle.size, particle.size, particle.size));

            glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));

            m_circleGL.draw();
        }
    }

    void setBoundary(Rectangle* boundary) {
        m_boundary = boundary;
    }

    void setCamera(Camera* camera) {
        m_camera = camera;
    }

private:
    void applyBoundaryCondition() {
        for (auto & particle : vec_particles) {
            //using a modulo instead ?
            if (particle.position.x < m_boundary->getLeft()) {particle.position.x = m_boundary->getRight();}
            if (m_boundary->getRight() < particle.position.x) {particle.position.x = m_boundary->getLeft();}

            if(particle.position.y < m_boundary->getBottom()) {particle.position.y = m_boundary->getTop();}
            if(m_boundary->getTop() < particle.position.y) { particle.position.y = m_boundary->getBottom();}
        }
    }

private:
    unsigned int m_numberParticles;
    std::vector<Particle> vec_particles;
    Rectangle* m_boundary = nullptr;

    // rendering
    Shader m_shader;
    int m_numberEdges;
    CircleGL m_circleGL;
    Camera* m_camera = nullptr;
};


#endif