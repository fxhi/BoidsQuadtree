#ifndef SCENEGL_HPP
#define SCENEGL_HPP

#include <iostream>

#include "LayerGL.hpp"
#include "Time.hpp"

class SceneGL {
public:
    // ~SceneGL() {
    //     for (auto layer : vec_layer) {
    //         delete layer;
    //     }
    // }
    
    virtual void update(const Time& time) {
        std::cout << "sceneGL" << std::endl;
        // for (const auto layer : vec_layer ) {
        //     layer->render();
        // }
    }

private:
    // std::vector<Layer*> vec_layer;
};

#endif