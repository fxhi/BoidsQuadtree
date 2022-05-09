#include "Application.hpp"

#include "OpenGL-Core/ApplicationGL.hpp"
#include "Scene.hpp"

Application::Application() {
    setScene(new Scene);
}