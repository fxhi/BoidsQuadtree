#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

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

struct Particle {
    Particle(float x, float y, float z, float vx, float vy, float vz, float size)
        : position(glm::vec3(x,y,z)), velocity(glm::vec3(vx,vy,vz)), size(size) {}

    glm::vec3 position;
    glm::vec3 velocity;
    float size;
};

class ParticleLayer : public LayerGL {
public:

    ParticleLayer(unsigned int m_numberParticles, int numberEdges=4);

    void initQuadtree();

    ~ParticleLayer();

    // TODO : try to fill the quadtree with pointers.
    void fillQuadtree();

    void update(const Time& time);

    void render() override;

    void setBoundary(Rectangle<>* boundary);

    void setCamera(Camera* camera);

private:

    void applyBoundaryCondition();

private:
    size_t m_numberParticles;
    std::vector<Particle> vec_particles;
    Rectangle<>* m_boundary = nullptr;

    // rendering
    Shader m_shader;
    int m_numberEdges;
    CircleGL m_circleGL;
    Camera* m_camera = nullptr;

    //Mesh
    Quadtree<Particle*> m_quadtree;
};

#endif