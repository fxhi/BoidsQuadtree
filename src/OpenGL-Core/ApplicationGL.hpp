#ifndef APPLICATIONGL_HPP
#define APPLICATIONGL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowGL.hpp"
#include "SceneGL.hpp"
#include "Time.hpp"
#include "ImGuiGL/ImGuiGLPath.hpp"

#include <vector>

class ApplicationGL {
public:
    ApplicationGL();

    ApplicationGL(int width, int height, std::string name);

    void run();

    void update();

    WindowGL* getWindow();

    void setScene(SceneGL* scene);

private:
    void init();

    void terminate();

private:
    WindowGL* m_window = nullptr;
    ImGuiGL* m_Imgui = nullptr;
    SceneGL* m_scene = nullptr;

    Time time;
};

#endif