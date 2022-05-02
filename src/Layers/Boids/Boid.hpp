#ifndef BOID_HPP
#define BOID_HPP

// Math library
// #include <glm/glm.hpp>

// vector
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Tools/RandomNumber.hpp"

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

    void compute_dist_to_boids(std::vector<Boid> &boids);

    glm::vec2 compute_cohesion_rule();
    glm::vec2 compute_alignment_rule(std::vector<Boid> const & boids);
    glm::vec2 compute_separation_rule();

    void update(float const delta_t, std::vector<Boid> & boids);

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

Boid::Boid() {

    m_position = glm::vec2(0,0);

    double angle = randomAngle();
    m_velocity = glm::vec2(std::cos(angle), -std::sin(angle));

    // m_max_velocity = 1.0;
    // m_max_acceleration = 0.3;
    
    // m_dist_alignment = 0.25;
    // m_dist_cohesion = 0.25;
    // m_dist_separation = 0.1;

    // m_size = 0.05;
    BoidsParameter boid_param = BoidsParameter();
    set_boid_parameter(boid_param);
}

Boid::Boid(BoidsParameter const &boids_param) {
    Boid();
    set_boid_parameter(boids_param);
}

void Boid::set_boid_parameter(BoidsParameter const &boids_param) {

    m_max_velocity = boids_param.max_velocity;
    m_max_acceleration = boids_param.max_acceleration;

    m_dist_alignment = boids_param.dist_alignment;
    m_dist_cohesion = boids_param.dist_cohesion;
    m_dist_separation = boids_param.dist_separation;

    m_ratio_alignment = boids_param.ratio_alignment;
    m_ratio_cohesion = boids_param.ratio_cohesion;
    m_ratio_separation = boids_param.ratio_separation;

    m_size = boids_param.m_size;
}

void Boid::compute_dist_to_boids(std::vector<Boid> &boids) {
    m_dist_boids.clear(); // -- clear previous vector since boids number can change
    m_dx_boids.clear(); // -- clear previous vector since boids number can change
    m_dy_boids.clear(); // -- clear previous vector since boids number can change
    for (auto & boid : boids) {
        // distance from our boid to oters our-->other
        m_dist_boids.push_back(glm::length(boid.m_position - m_position));
        m_dx_boids.push_back(boid.m_position.x - m_position.x);
        m_dy_boids.push_back(boid.m_position.y - m_position.y);
    }
}

glm::vec2 Boid::compute_cohesion_rule() {
    // compute cohesion rule
    // Move the current boid toward the barycenter of nearby boids
    glm::vec2 barycenter(0,0);
    int count = 0;
    for (auto i=0; i < m_dist_boids.size(); i++) {
        if ( 0.0 < std::abs(m_dist_boids[i]) && std::abs(m_dist_boids[i]) < m_dist_cohesion) {
            barycenter.x +=  m_dx_boids[i];
            barycenter.y += m_dy_boids[i];
            count += 1;
        }
    }
    if (count != 0) {
        barycenter /= count;
        return barycenter;
        // return glm::normalize(barycenter);
    } else
    {
        return glm::vec2(0,0);
    }

    // return acc; // return the computed acceleration
}

glm::vec2 Boid::compute_alignment_rule(std::vector<Boid> const & boids) {
    // compute alignment_rule
    glm::vec2 velocity(0,0);
    int count = 0;
    for (auto i = 0; i < m_dist_boids.size(); i++) {
        if ( 0.0 < std::abs(m_dist_boids[i]) && std::abs(m_dist_boids[i]) < m_dist_alignment ) {
            velocity += boids[i].m_velocity;
            count += 1;
        }
    }

    if (count != 0) {
        velocity /= count;
        // return velocity - m_velocity; // return a unit vector which indicate toward wich position the boids need to go to align with the other
        return velocity - m_velocity; // return a unit vector which indicate toward wich position the boids need to go to align with the other
        // return glm::normalize(velocity - m_velocity); // return a unit vector which indicate toward wich position the boids need to go to align with the other
    }
    return glm::vec2(0,0);
};

glm::vec2 Boid::compute_separation_rule() {
    // compute separation rule
    glm::vec2 dir(0,0);
    for (auto i = 0; i < m_dist_boids.size(); i++){
        if (0.0 < std::abs(m_dist_boids[i])  && std::abs(m_dist_boids[i]) < m_dist_separation) {
            // dir += -glm::vec2(m_dx_boids[i], m_dy_boids[i]);
            // std::cout << static_cast<float>(std::pow(m_dist_boids[i],2)) << std::endl;
            dir += -glm::vec2(m_dx_boids[i], m_dy_boids[i]) / (float)std::pow(m_dist_boids[i]*10,2);
        }
    }

    if (glm::length(dir) != 0) {
        return dir;
        // return glm::normalize(dir);
    } 
    else {
        return glm::vec2(0,0);
    }
};

void Boid::update(float const delta_t, std::vector<Boid> & boids) {
    // Compute distance vector to each boids
    compute_dist_to_boids(boids);
    // update position and velocity of the boid
    glm::vec2 acc(0.0, 0.0); // acceleration
    acc += compute_cohesion_rule() * m_ratio_cohesion;
    acc += compute_alignment_rule(boids) * m_ratio_alignment;
    acc += compute_separation_rule() * m_ratio_separation;

    m_velocity += acc*delta_t;
    float vec_size = std::sqrt(std::pow(m_velocity.x,2) + std::pow(m_velocity.y,2));
    

    if (std::abs(vec_size) < m_max_velocity) {
        m_velocity *= 1.0 + m_max_acceleration*delta_t;
    }
    if (std::abs(vec_size) > m_max_velocity) {
        m_velocity *= m_max_velocity/vec_size;
    }

    m_position += m_velocity * delta_t; // this is stange position = acc, not position = 1/2 acc*delta_t**2 (even if delta_t=1)
    // m_position += m_velocity; // this is stange position = acc, not position = 1/2 acc*delta_t**2 (even if delta_t=1)
}

glm::vec2 Boid::get_velocity() const {
    return m_velocity;
}

glm::vec2 Boid::get_position() const {
    return m_position;
}

#endif