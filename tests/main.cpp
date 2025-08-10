#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "OxygenRender/Renderer.h"
#include <iostream>

int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW 初始化失败\n";
        return -1;
    }

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "OxygenStudio Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "窗口创建失败\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD 初始化失败\n";
        return -1;
    }

    Renderer renderer;

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        renderer.clear(); // 清屏

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
