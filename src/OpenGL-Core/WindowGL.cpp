#include "WindowGL.hpp"

#include <string>
#include <cassert>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



// Public
// ------

WindowGL::WindowGL(int width, int height, std::string name) :
    m_width(width), m_height(height), m_name(name) {
        assert(width > 0 && height > 0 && "ERROR::WINDOW:: width and height of the window need to be > 0.");
    };

void* WindowGL::get() {
    return m_window;
}

void WindowGL::init() {
    // -- GLFW: Initialize and configure
    // ---------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // -- glfw window creation
    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);

    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE); // TODO: throwing exception rather than exit
    }

    // -- Make the window's context current
    glfwMakeContextCurrent(m_window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    // /!\ need to have a gl context -> glewInit() need to be after glfwMakeContextCurrent
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE); // TODO: throwing exception rather than exit
    }

    //-- Display the GL version
    std::cout << "GL version : " << glGetString(GL_VERSION) << std::endl;

    //-- OpenGL state
    glEnable(GL_CULL_FACE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Ensure we can caputre the escape key being pressed
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void WindowGL::processInput() {
    processInput(m_window);
}

void WindowGL::update() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

// Private
// -------

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void WindowGL::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}