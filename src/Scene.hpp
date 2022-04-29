#ifndef SCENE_HPP
#define SCENE_HPP

#include "OpenGL-Core/SceneGL.hpp"
#include "OpenGL-Core/LayerGL.hpp"
#include "Layers/LayersPath.hpp"

class Scene : public SceneGL {
public:
    Scene() {

    }

    void update(const Time& time) override {
        background.render();
        particles.update(time);
        particles.render();
    }


private:
    BackgroundLayer background;
    ParticleLayer particles;
    Time time;

};

#endif