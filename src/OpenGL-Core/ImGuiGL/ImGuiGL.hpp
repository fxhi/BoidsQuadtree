#ifndef IMGUIGL_HPP   
#define IMGUIGL_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../WindowGL.hpp"
#include <GLFW/glfw3.h>

class ImGuiGL {
public:
    ImGuiGL();

    void setWindow(WindowGL* window);

    void init();
    
    void update();

    void terminate();

private:
    WindowGL* m_window = nullptr;

private:

// void Engin::init_ImGui_window() {
//     ImGuiIO& io = ImGui::GetIO(); (void)io; // Maybe delete this and pass it in the function arguments

//     // -- Variables to setting up the ImGui window
//     float display_width = (float) io.DisplaySize.x;
//     float display_height = (float)io.DisplaySize.y;
//     window_pos_x = display_width * 0.10;
//     window_pos_y = display_height * 0.10;
//     window_size_x = display_width * 0.80;
//     window_size_y = display_height * 0.60;
// }

};

#endif