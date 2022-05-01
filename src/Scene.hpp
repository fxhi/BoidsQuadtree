#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "OpenGL-Core/SceneGL.hpp"
#include "OpenGL-Core/LayerGL.hpp"

#include "Layers/LayersPath.hpp"

#include "Tools/Geometry/Rectangle.hpp"

class Scene : public SceneGL {
public:
    Scene() 
        : m_camera(new Camera),
          m_boundary(new Rectangle()),  // rectangle centered in (0,0), with a width and height of 2.
          m_particles(40)
    {
        m_background.setColorRGB255(50, 54, 57);
        m_boundary->resizeLeftRightBottomTop(-2.0f, 2.0f, -2.0f, 2.0f);
        m_camera->setViewMatrix(getOrthographicProjectionMatrix(
            m_boundary->getLeft(), m_boundary->getRight(),
            m_boundary->getBottom(), m_boundary->getTop()));
        m_particles.setBoundary(m_boundary);
        m_particles.setCamera(m_camera);
        
    }

    void update(const Time& m_time) override {
        m_background.render();
        m_particles.update(m_time);
        m_particles.render();
    }


private:
    BackgroundLayer m_background;
    ParticleLayer m_particles;
    Time m_time;

    Camera* m_camera;
    Rectangle* m_boundary;

};

#endif