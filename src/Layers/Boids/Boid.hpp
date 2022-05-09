#ifndef BOID_HPP
#define BOID_HPP

#define _USE_MATH_DEFINES

#include <vector>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Tools/RandomNumber.hpp"

#include <cmath>

// Can't put class instances into each other recursively
// Only works with pointers (or references) to instances. --> https://stackoverflow.com/questions/396084/headers-including-each-other-in-c

class Flock; // forward definition of Flock, needed to be know to update boid position


// -- Struct to store all the boid parameters
struct BoidsParameter {
        float max_velocity = 0.5;
        float max_acceleration = 0.3;

        float dist_alignment = 0.25;
        float dist_cohesion = 0.25;
        float dist_separation = 0.15;

        float ratio_alignment = 1.0;
        float ratio_cohesion = 1.0;
        float ratio_separation = 1.5;

        float m_size = 0.05;
        // float m_size = 1.0;
};

class Boid {
public:
    Boid();
    Boid(BoidsParameter const &boids_param);

    void set_boid_parameter(BoidsParameter const &boids_param);

    void compute_dist_to_boids(std::vector<Boid*> &boids);

    glm::vec2 compute_cohesion_rule();
    glm::vec2 compute_alignment_rule(std::vector<Boid*> const & boids);
    glm::vec2 compute_separation_rule();

    void update(float const delta_t, std::vector<Boid*> & boids);

    glm::vec2 get_position() const;
    glm::vec2 get_velocity() const;

    friend class Flock;

protected:
    glm::vec2 m_velocity;
    glm::vec2 m_position;

    float m_max_velocity;
    float m_max_acceleration;

    float m_dist_cohesion;
    float m_dist_alignment;
    float m_dist_separation;

    float m_ratio_cohesion;
    float m_ratio_alignment;
    float m_ratio_separation;

    float m_size;

    std::vector<float> m_dist_boids; // distance from our boid to oters our-->other
    std::vector<float> m_dx_boids; // distance in x-direction from our boid to oters our-->other
    std::vector<float> m_dy_boids; // distance in y-direction from our boid to oters our-->other
};

#endif