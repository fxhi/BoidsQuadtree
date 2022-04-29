#ifndef APPLICATIONGL_HPP
#define APPLICATIONGL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowGL.hpp"
#include "SceneGL.hpp"
#include "Time.hpp"

#include <vector>

class ApplicationGL {
public:
    ApplicationGL() = default;
    ApplicationGL(int width, int height, std::string name) : m_window(width, height, name) {
            assert(width > 0 && height > 0 && "ERROR::WINDOW:: width and height of the window need to be > 0.");
    };


    void run() {
        init();
        update();
        terminate();
    }

    void update() {
        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_window.get())))
        {
            m_window.processInput();

            // m_scene.update(time);
            m_scene->update(time);
            // m_scene[0]->update(time);

            m_window.update();

            time.updateTime();
        }
    }

    WindowGL& getWindow() {
        return m_window;
    }

    // void setScene(SceneGL scene) {
    //     m_scene = scene;
    // }
    void setScene(SceneGL* scene) {
        m_scene = scene;
        // m_scene[0] = scene;
        // m_scene.push_back(scene);
    }

    // This does not work
    // void setScene(SceneGL* scene) {
    //     m_scene = scene;
    // }

// private:
    void init() {
        m_window.init();
    }

    void terminate() {
        glfwTerminate();
    }

private:
    WindowGL m_window;
    SceneGL* m_scene = nullptr;
    // SceneGL* m_scene;
    // SceneGL* m_scene[1];
    // std::vector<SceneGL*> m_scene;
    Time time;
};

#endif