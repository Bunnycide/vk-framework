//
// Created by jae on 04/04/23.
//

#ifndef VK_FRAMEWORK_VK_RENDER_SURFACE_WAYLAND_X11_H
#define VK_FRAMEWORK_VK_RENDER_SURFACE_WAYLAND_X11_H

GLFWwindow* H_createWindow(int, int, const char*);

bool H_createRenderSurface(VkInstance,
                           VkSurfaceKHR&,
                           GLFWwindow* window);

void H_deleteRenderWindow(GLFWwindow* window);

#endif //VK_FRAMEWORK_VK_RENDER_SURFACE_WAYLAND_X11_H
