#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "OpenGL-Core/ApplicationGL.hpp"
#include "Scene.hpp"

class Application : public ApplicationGL
{
public:
    Application() {
        setScene(new Scene);
    }
};

#endif