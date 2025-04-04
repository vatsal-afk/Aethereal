#include "InputManager.h"
#include <GLFW/glfw3.h>

namespace Aethereal {

void InputManager::HandleInput(float& zoom, float& offsetX, float& offsetY) {
    float speed = 0.01f * zoom;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
        offsetY += speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
        offsetY -= speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
        offsetX -= speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
        offsetX += speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
        zoom *= 1.01f;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
        zoom *= 0.99f;
}

}
