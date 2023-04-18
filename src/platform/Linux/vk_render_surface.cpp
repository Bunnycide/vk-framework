//
// Created by jae on 04/04/23.
//
#include "includes.h"

GLFWwindow* H_createWindow(int width, int height, const char* name){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* mWindow = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if(mWindow == nullptr){
        H_deleteRenderWindow(mWindow);
        return mWindow;
    }

    return mWindow;
}

bool H_createRenderSurface(VkInstance instance, VkSurfaceKHR& surface, GLFWwindow* window){
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    return (result == VK_SUCCESS);
}

void H_deleteRenderWindow(GLFWwindow* window){
    if(window != nullptr) glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
}
