#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <vector>

#include <exception>

#include <algorithm> //std::max with multiple value : std::max({1.0, 2.0, 3.0})

#include <random>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../OpenGL-Core/LayerGL.hpp"
#include "../../OpenGL-Core/shader.hpp"
#include "../../OpenGL-Core/Time.hpp"
#include "../../OpenGL-Core/Tools/CircleGL.hpp"
#include "../../OpenGL-Core/Tools/InstancedCircleGL.hpp"
#include "../../OpenGL-Core/Tools/SimpleLineGL.hpp"
#include "../../OpenGL-Core/Tools/InstancedSimpleLineGL.hpp"


// #include "../../Tools/RandomNumber.hpp"
#include "../../Tools/Geometry/Rectangle.hpp"
#include "../../Mesh/Quadtree.hpp"

#include "../Camera.hpp"

#include "Boid.hpp"

class Flock : public LayerGL {

    public:
        //Constructor
        Flock() = delete; // Can not declare a flock object without any arguments
        Flock(int nb_boids);

        ~Flock() {
            for(auto & boid : m_boids) {
                delete boid;
                boid = nullptr;
            }
        }

        void applyBoundaryCondition();

        void update(const Time& time);

        void set_boids_parameter();

        void restart();

        void restart(BoidsParameter const & in_boids_param);

        virtual void render() override{
            bool useInstancingCircle = true;

            bool renderFlock = true;
            
            if(renderFlock) {

                if (useInstancingCircle) {
                    // Instancing
                    //-----------
                    m_shaderInstancing.use();

                    // Get and set view matrix
                    glm::mat4 viewMatrix = m_camera->getViewMatrix();
                    unsigned int ID_viewMatrixInstancing = glGetUniformLocation(m_shaderInstancing.ID, "u_view");
                    glUniformMatrix4fv(ID_viewMatrixInstancing, 1, GL_FALSE, glm::value_ptr(viewMatrix));

                    glm::mat4* modelMatrix = new glm::mat4[m_boids.size()];
                    int index = 0;

                    for(auto const & boid : m_boids) {
                        glm::mat4 transform = glm::mat4(1.0f);
                        transform = glm::translate(transform, glm::vec3(boid->m_position.x, boid->m_position.y, 0.0f));
                        transform = glm::scale(transform, glm::vec3(boid->m_size, boid->m_size, boid->m_size));

                        modelMatrix[index] = transform;
                        index++;
                    }

                    m_instancedCircleGL.setNumberInstance(m_boids.size());
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
                    for(auto const & boid : m_boids) {
                        glm::mat4 transform = glm::mat4(1.0f);
                        transform = glm::translate(transform, glm::vec3(boid->m_position.x, boid->m_position.y, 0.0f));
                        transform = glm::scale(transform, glm::vec3(boid->m_size, boid->m_size, boid->m_size));

                        glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));

                        m_circleGL.draw();
                    }

                }
            }

            bool renderMesh = true;
            bool useMultipleSimpleLineGL = false;

            // Update the quadtree
            m_quadtree.clear();
            fillQuadtree();

            if(renderMesh) {

                float lineWidth = 0.02;
                if (useMultipleSimpleLineGL) {
                    m_shader.use();
                    // Get and set view matrix
                    glm::mat4 viewMatrix = m_camera->getViewMatrix();
                    unsigned int ID_viewMatrix = glGetUniformLocation(m_shader.ID, "u_view");
                    glUniformMatrix4fv(ID_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

                    glm::mat4 transform = glm::mat4(1.0f);
                    unsigned int ID_transform = glGetUniformLocation(m_shader.ID, "transform");
                    glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));

                    // Render the quadtree
                    std::vector<Line<float>> linesToDraw = m_quadtree.getLineToDrawShape();
                    std::vector<float> linesPositions;
                    for(auto const & line : linesToDraw) {
                        std::vector<float> position = {line.x2, line.y2, line.x1, line.y1};
                        linesPositions.insert(linesPositions.end(), position.begin(), position.end());
                    }
                    MultipleSimpleLineGL linesGL(linesPositions, lineWidth);
                    glBindVertexArray(linesGL.VAO);
                    linesGL.draw();
                } else {
                    m_shader.use();
                    // Get and set view matrix
                    glm::mat4 viewMatrix = m_camera->getViewMatrix();
                    unsigned int ID_viewMatrix = glGetUniformLocation(m_shader.ID, "u_view");
                    glUniformMatrix4fv(ID_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

                    glm::mat4 transform = glm::mat4(1.0f);
                    unsigned int ID_transform = glGetUniformLocation(m_shader.ID, "transform");
                    glUniformMatrix4fv(ID_transform, 1, GL_FALSE, glm::value_ptr(transform));

                    // Render the quadress
                    std::vector<Line<float>> linesToDraw = m_quadtree.getLineToDrawShape();
                    // std::cout << "Line to draw size : " << linesToDraw.size() << std::endl;
                    // std::cout << " Total number of point : " << m_quadtree.m_total_points << std::endl;
                    // std::cout << " Max depth : " << m_quadtree.getMaxDepth()  << std::endl;
                    for(auto const & line : linesToDraw) {
                        // TODO : not good need to create a line object everytime !
                        SimpleLineGL lineGL(line.x2, line.y2, line.x1, line.y1, lineWidth);
                        // SimpleLineGL lineGL(line.x2, line.y2, line.x1, line.y1, 0.05);
                        // std::cout << " " << line.x1 << " " << line.y1 << " " << line.x2 << " " << line.y2 << std::endl;
                        glBindVertexArray(lineGL.VAO);
                        lineGL.draw();
                    }
                }
            }

        }

        void setBoundary(Rectangle<>* boundary) {
            m_boundary = boundary;
        }

        void setCamera(Camera* camera) {
            m_camera = camera;
        }
            
        void initQuadtree() {
            m_quadtree.m_boundary = *m_boundary;
        }

        void fillQuadtree() {
            // Insert the particle, into the quadtree, as a pointer to the last element of the vector
            // &vec_particles.back() : get the reference of the last element of the vector
            for(auto boid : m_boids) {
                m_quadtree.insert(boid, boid->m_position.x, boid->m_position.y);
            }
        }

        void checkInitialization() {
            if(!m_camera) {throw std::runtime_error("Flock::Uninitialized m_camera pointer.");}
            if(!m_boundary) {throw std::runtime_error("Flock::Uninitialized m_boundary pointer");}

            // std::cout << "Flock::Quadtree::Boundary:: width and heigh:" << m_quadtree.m_boundary.getWidth() << " " << m_quadtree.m_boundary.getHeight() << std::endl;
            if(m_quadtree.m_boundary.getWidth()==0.0f || m_quadtree.m_boundary.getHeight()==0.0f) {throw std::runtime_error("Flock::Quadtree::Uninitialized m_quadtree boundary.");}
        }

    public:
        int m_nb_boids;

        // Size of the domaine where boids can move
        Rectangle<>* m_boundary = nullptr;

        // check https://stackoverflow.com/questions/15802006/how-can-i-create-objects-while-adding-them-into-a-vector
        std::vector<Boid*> m_boids; //use a smart pointer if add or delete m_boids
        BoidsParameter boids_param;

        // Rendering
        Shader m_shader;
        Shader m_shaderInstancing;
        CircleGL m_circleGL;
        InstancedCircleGL m_instancedCircleGL;
        Camera* m_camera = nullptr;

        Quadtree<Boid*> m_quadtree;


};

//Constructor
Flock::Flock(int nb_boids)
        : m_nb_boids(nb_boids),
          m_shader("src/shader/vertexShader.glsl", "src/shader/fragmentShader.glsl"),
          m_shaderInstancing("src/shader/InstancingVertexShader.glsl", "src/shader/InstancingFragmentShader.glsl"),
          m_circleGL(4),
          m_instancedCircleGL(4)
{
    for (int i=0; i<m_nb_boids; i++) {
        m_boids.push_back(new Boid());
    }
} 

void Flock::applyBoundaryCondition() {
    for (auto & boid : m_boids) {
        // Maybe a faster way but was fun to play like this
        // boid->m_position.x = std::fmod(boid->m_position.x, 1.0) - 1.0 * std::round(boid->m_position.x) * std::floor(std::abs(boid->m_position.x));
        // boid->m_position.y = std::fmod(boid->m_position.y, 1.0) - 1.0 * std::round(boid->m_position.y) * std::floor(std::abs(boid->m_position.y));
        if(boid->m_position.x < m_boundary->getLeft()) {
            boid->m_position.x = m_boundary->getRight();
        }
            
        if(m_boundary->getRight() < boid->m_position.x) {
            boid->m_position.x = m_boundary->getLeft();
        }

        if(boid->m_position.y < m_boundary->getBottom()) {
            boid->m_position.y = m_boundary->getTop();
        }
        
        if (m_boundary->getTop() < boid->m_position.y) {
            boid->m_position.y = m_boundary->getBottom();
        }
    }
}

void Flock::update(const Time& time) {
    // std::cout << "total point quadtree : " << m_quadtree.m_total_points << std::endl;
    // std::cout << "total boids : " << m_boids.size() << std::endl;
    for (auto & boid : m_boids) {
        std::vector<Boid*> p_closeBoids;
        // p_closeBoids = m_quadtree.queryRadius(boid->m_position.x, boid->m_position.y, 1);
        p_closeBoids = m_quadtree.queryRadius(boid->m_position.x, boid->m_position.y, std::max({boid->m_dist_cohesion, boid->m_dist_alignment, boid->m_dist_separation}));

        // std::cout << p_closeBoids.size() << std::endl;

        // std::vector<Boid> closeBoids;
        // for(auto & p_boid : p_closeBoids) {
        //     std::cout << (*p_boid).m_position.x << std::endl;
        //     // closeBoids.push_back(bb);
        // }
        // std::cout << closeBoids.size() << std::endl;

        // Only took a sample of the boids, can shuffle the vector or try to exctrat randomly, without repetition,  values.
        // //https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
        // if(p_closeBoids.size() > 20) {
        //     auto rng = std::default_random_engine {}; // store it somewhere else, since reused it.
        //     std::shuffle(std::begin(p_closeBoids), std::end(p_closeBoids), rng);
        //     p_closeBoids.resize(20);
        // }
        boid->update(time.getTimeStep(), p_closeBoids);
        // boid->update(time.getTimeStep(), m_boids);
    }
    applyBoundaryCondition();
}

void Flock::set_boids_parameter() {
    for (auto & boid : m_boids) {
        boid->set_boid_parameter(boids_param);
    }
}

void Flock::restart() {
    m_boids.clear();
    for (int i=0; i<m_nb_boids; i++) {
        m_boids.push_back(new Boid());
    }
}

void Flock::restart(BoidsParameter const & in_boids_param) {
    restart();
    boids_param = in_boids_param;
    set_boids_parameter();
}

#endif