#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <vector>

#include <exception>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../OpenGL-Core/LayerGL.hpp"
#include "../../OpenGL-Core/shader.hpp"
#include "../../OpenGL-Core/Time.hpp"
#include "../../OpenGL-Core/Tools/circleGL.hpp"
#include "../../OpenGL-Core/Tools/circleGL.hpp"


// #include "../../Tools/RandomNumber.hpp"
#include "../../Tools/Geometry/Rectangle.hpp"

#include "../Camera.hpp"

#include "Boid.hpp"

class Flock : public LayerGL {

    public:
        //Constructor
        Flock() = delete; // Can not declare a flock object without any arguments
        Flock(int nb_boids);

        void applyBoundaryCondition();

        void update(const Time& time);

        void set_boids_parameter();

        void restart();

        void restart(BoidsParameter const & in_boids_param);

        virtual void render() override{
            m_shader.use();
            glBindVertexArray(m_circleGL.VAO);

            glm::mat4 viewMatrix = m_camera->getViewMatrix();
            unsigned int ID_viewMatrix = glGetUniformLocation(m_shader.ID, "u_view");
            glUniformMatrix4fv(ID_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

            unsigned int ID_transform = glGetUniformLocation(m_shader.ID, "transform");
            for(auto const & boid : m_boids) {
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform, glm::vec3(boid.m_position.x, boid.m_position.y, 0.0f));
                transform = glm::scale(transform, glm::vec3(boid.m_size, boid.m_size, boid.m_size));

                glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));

                m_circleGL.draw();
            }
            glUseProgram(0);
        }

        void setBoundary(Rectangle<>* boundary) {
            m_boundary = boundary;
        }

        void setCamera(Camera* camera) {
            m_camera = camera;
        }

        void checkInitialization() {
            if(!m_camera) {throw std::runtime_error("Flock::Uninitialized m_camera pointer.");}
            if(!m_boundary) {throw std::runtime_error("Flock::Uninitialized m_boundary pointer");}

        }

    public:
        int m_nb_boids;

        // Size of the domaine where boids can move
        Rectangle<>* m_boundary = nullptr;

        // check https://stackoverflow.com/questions/15802006/how-can-i-create-objects-while-adding-them-into-a-vector
        std::vector<Boid> m_boids; //use a smart pointer if add or delete m_boids
        BoidsParameter boids_param;

        // Rendering
        Shader m_shader;
        CircleGL m_circleGL;
        Camera* m_camera = nullptr;


};

//Constructor
Flock::Flock(int nb_boids)
        : m_nb_boids(nb_boids),
          m_shader("src/shader/vertexShader.glsl", "src/shader/fragmentShader.glsl"),
          m_circleGL(4)
{
    for (int i=0; i<m_nb_boids; i++) {
        m_boids.push_back(Boid());
    }
} 

void Flock::applyBoundaryCondition() {
    for (auto & boid : m_boids) {
        // Maybe a faster way but was fun to play like this
        // boid.m_position.x = std::fmod(boid.m_position.x, 1.0) - 1.0 * std::round(boid.m_position.x) * std::floor(std::abs(boid.m_position.x));
        // boid.m_position.y = std::fmod(boid.m_position.y, 1.0) - 1.0 * std::round(boid.m_position.y) * std::floor(std::abs(boid.m_position.y));
        if(boid.m_position.x < m_boundary->getLeft()) {
            boid.m_position.x = m_boundary->getRight();
        }
            
        if(m_boundary->getRight() < boid.m_position.x) {
            boid.m_position.x = m_boundary->getLeft();
        }

        if(boid.m_position.y < m_boundary->getBottom()) {
            boid.m_position.y = m_boundary->getTop();
        }
        
        if (m_boundary->getTop() < boid.m_position.y) {
            boid.m_position.y = m_boundary->getBottom();
        }
    }
}

void Flock::update(const Time& time) {
    for (auto & boid : m_boids) {
        boid.update(time.getTimeStep(), m_boids);
    }
    applyBoundaryCondition();
}

void Flock::set_boids_parameter() {
    for (auto & boid : m_boids) {
        boid.set_boid_parameter(boids_param);
    }
}

void Flock::restart() {
    m_boids.clear();
    for (int i=0; i<m_nb_boids; i++) {
        m_boids.push_back(Boid());
    }
}

void Flock::restart(BoidsParameter const & in_boids_param) {
    restart();
    boids_param = in_boids_param;
    set_boids_parameter();
}

#endif