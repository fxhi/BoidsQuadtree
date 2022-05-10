#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

#include "OpenGL-Core/SceneGL.hpp"
#include "OpenGL-Core/LayerGL.hpp"

#include "Layers/LayersPath.hpp"

#include "Tools/Geometry/Rectangle.hpp"

#include "OpenGL-Core/Tools/CircleGL.hpp"
#include "OpenGL-Core/Tools/SimpleLineGL.hpp"

#include "OpenGL-Core/WindowGL.hpp"

//use share_ptr for camera and boundary ?

class Scene : public SceneGL {
public:
    Scene();

    void update(const Time& m_time) override;

private:
    BackgroundLayer m_background;
    Flock m_flock;

    Time m_time;

    std::shared_ptr<Camera> m_camera;
    Rectangle<>* m_boundary;

    ParticleLayer m_particles;

};

#endif