#include "Scene.hpp"

#include <vector>

#include "OpenGL-Core/SceneGL.hpp"
#include "OpenGL-Core/LayerGL.hpp"

#include "Layers/LayersPath.hpp"

#include "Tools/Geometry/Rectangle.hpp"

#include "OpenGL-Core/Tools/CircleGL.hpp"
#include "OpenGL-Core/Tools/SimpleLineGL.hpp"

#include "OpenGL-Core/WindowGL.hpp"

// Public
// ------
Scene::Scene() 
    : m_camera(std::make_shared<Camera>()),
        m_boundary(new Rectangle<>()),  // rectangle centered in (0,0), with a width and height of 2.
        m_flock(10000), //400 is good
        m_particles(10)
{
    m_background.setColorRGB255(50, 54, 57);

    // m_boundary->resizeLeftRightBottomTop(-2.0f, 2.0f, -2.0f, 2.0f);
    m_boundary->resizeLeftRightBottomTop(-5.0f, 5.0f, -5.0f, 5.0f);

    m_camera->setViewMatrix(getOrthographicProjectionMatrix(
        m_boundary->getLeft()-0.1, m_boundary->getRight()+0.1,
        m_boundary->getBottom()-0.1, m_boundary->getTop()+0.1));

    m_flock.setBoundary(m_boundary);
    m_flock.setCamera(m_camera);

    m_particles.setBoundary(m_boundary);
    m_particles.initQuadtree();
    m_particles.fillQuadtree();
    m_particles.setCamera(m_camera);

    //Finalized initialization
    m_flock.initQuadtree();
    m_flock.fillQuadtree();
    m_flock.checkInitialization();

}

void Scene::update(const Time& m_time) {
    m_background.render();

    m_flock.update(m_time);
    m_flock.render();

    // m_particles.update(m_time);
    // m_particles.render();

}