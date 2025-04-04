#include "Renderer.h"
#include "InputManager.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Aethereal", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Aethereal::Renderer renderer;

    float zoom = 1.5f;
    float offsetX = -0.5f;
    float offsetY = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static char fxBuffer[256] = "x*x - y*y";
        static char fyBuffer[256] = "2.0*x*y";
        static float uC = 0.0f, uK = 0.0f;
        static bool regenerateShader = false;

        ImGui::Begin("Menu");
        ImGui::InputText("fx", fxBuffer, IM_ARRAYSIZE(fxBuffer));
        ImGui::InputText("fy", fyBuffer, IM_ARRAYSIZE(fyBuffer));
        
        ImGui::SliderFloat("uC", &uC, -2.0f, 2.0f);
        ImGui::SliderFloat("uK", &uK, -2.0f, 2.0f);
        
        if (ImGui::Button("Render")) {
            regenerateShader = true;
        }
        ImGui::End();

        ImGui::Begin("Controls");
        ImGui::SliderFloat("Zoom", &zoom, 0.1f, 3.0f);
        ImGui::SliderFloat("Offset X", &offsetX, -2.0f, 2.0f);
        ImGui::SliderFloat("Offset Y", &offsetY, -2.0f, 2.0f);
        ImGui::End();

        if (regenerateShader) {
            regenerateShader = false;
            renderer.SetFormula(fxBuffer, fyBuffer);
        }        

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.Render(zoom, offsetX, offsetY, uC, uK);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}