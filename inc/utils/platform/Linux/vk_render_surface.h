//
// Created by jae on 04/04/23.
//

#ifndef VK_FRAMEWORK_VK_RENDER_SURFACE_WAYLAND_X11_H
#define VK_FRAMEWORK_VK_RENDER_SURFACE_WAYLAND_X11_H

#include "../third-party/glfw-3.3.8/include/GLFW/glfw3.h"

extern GLFWwindow* window;

bool H_createWindow(int, int, const char*);

bool H_createRenderSurface(VkInstance,
                           VkSurfaceKHR&);
void H_deleteRenderWindow();

#endif //VK_FRAMEWORK_VK_RENDER_SURFACE_WAYLAND_X11_H
