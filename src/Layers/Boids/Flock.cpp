#include "Flock.hpp"

#include <vector>

#include <exception>

#include <algorithm> //std::max with multiple value : std::max({1.0, 2.0, 3.0})

#include <random>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../OpenGL-Core/LayerGL.hpp"
#include "../../OpenGL-Core/Shader.hpp"
#include "../../OpenGL-Core/Time.hpp"
#include "../../OpenGL-Core/Tools/CircleGL.hpp"
#include "../../OpenGL-Core/Tools/InstancedCircleGL.hpp"
#include "../../OpenGL-Core/Tools/SimpleLineGL.hpp"
#include "../../OpenGL-Core/Tools/MultipleSimpleLineGL.hpp"


// #include "../../Tools/RandomNumber.hpp"
#include "../../Tools/Geometry/Rectangle.hpp"
#include "../../Mesh/Quadtree.hpp"

#include "../Camera.hpp"

#include "Boid.hpp"

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