#ifndef WINDOWGL_HPP
#define WINDOWGL_HPP

#include <string>
#include <cassert>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>



class WindowGL {
public:
    WindowGL(int width=800, int height=800, std::string name="Project");

    void* get();

    void init();

    void processInput();

    void update();

private:
    GLFWwindow* m_window = nullptr;

    int m_width;
    int m_height;
    std::string m_name;

private:
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window);
};

#endif