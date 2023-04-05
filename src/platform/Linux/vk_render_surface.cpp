//
// Created by jae on 04/04/23.
//
#include "includes.h"

GLFWwindow* window = nullptr;

bool H_createWindow(int width, int height, const char* name){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if(window == nullptr){
        H_deleteRenderWindow();
        return false;
    }

    return true;
}

bool H_createRenderSurface(VkInstance instance, VkSurfaceKHR& surface){
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);

    return (result == VK_SUCCESS);
}

void H_deleteRenderWindow(){
    if(window != nullptr) glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
}