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
    ApplicationGL() {
        init(); // Can't do this in "run" function, otherwise get a segmentation fault.
    }
    ApplicationGL(int width, int height, std::string name) : m_window(new WindowGL(width, height, name)) {
        assert(width > 0 && height > 0 && "ERROR::WINDOW:: width and height of the window need to be > 0.");
        ApplicationGL();
    };

    void run() {
        update();
        terminate();
    }

    void update() {
        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_window->get())))
        {
            m_window->processInput();

            m_scene->update(time);
            m_Imgui->update();

            m_window->update();

            time.updateTime();
            time.displayFPS();
        }
    }

    WindowGL* getWindow() {
        return m_window;
    }

    void setScene(SceneGL* scene) {
        m_scene = scene;
    }

private:
    void init() {
        m_window = new WindowGL();
        m_window->init();

        m_Imgui = new ImGuiGL();
        m_Imgui->setWindow(m_window);
        m_Imgui->init();
    }

    void terminate() {
        glfwTerminate();
    }

private:
    WindowGL* m_window = nullptr;
    ImGuiGL* m_Imgui = nullptr;
    SceneGL* m_scene = nullptr;

    Time time;
};

#endif