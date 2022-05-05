#ifndef IMGUIGL_HPP   
#define IMGUIGL_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../WindowGL.hpp"
#include <GLFW/glfw3.h>


class ImGuiGL
{
public:
    ImGuiGL() {}

    void setWindow(WindowGL* window) {
        m_window = window;
    }

    void init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // -- Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // -- Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_window->get()), true);
        const char* glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    
    void update() {
        ImGuiIO& io = ImGui::GetIO(); (void)io; // Maybe delete this and pass it in the function arguments
        
        // -- Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Configurations", NULL, ImGuiWindowFlags_AlwaysAutoResize);  
        ImGui::Text("Hello Imgui");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }



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