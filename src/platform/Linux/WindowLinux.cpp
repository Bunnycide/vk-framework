//
// Created by jae on 18/04/23.
//

#include "includes.h"
#include "utils/platform/Linux/WindowLinux.h"


void WindowLinux::getWindowSize(int& width, int& height){
    width = Window::width;
    height = Window::height;
}

void WindowLinux::createWindowSurface(VkInstance instance){
    H_createRenderSurface(instance, WindowLinux::surface, WindowLinux::window);
}

WindowLinux::WindowLinux(int &width, int &height, const char *name) : Window(width, height, name) {
    WindowLinux::window = H_createWindow(width, height, name);
}

VkSurfaceKHR WindowLinux::getVkSurface() {
    return surface;
}

void WindowLinux::deleteWindow(){
    H_deleteRenderWindow(window);
}

void WindowLinux::loop(float deltaT){

    if(window == nullptr) return;

    while(!glfwWindowShouldClose(window)){
        mDrawLooper->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void WindowLinux::setLooper(DrawLooper* drawLooper) {
    mDrawLooper = drawLooper;
}
