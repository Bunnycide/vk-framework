//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_WINDOWLINUX_H
#define VK_FRAMEWORK_WINDOWLINUX_H

#include "../../draw_looper.h"

class WindowLinux : public Window{
public:
    WindowLinux(int &width, int &height, const char* name);

    void getWindowSize(int &width, int & height) override;
    void createWindowSurface(VkInstance) override;
    void deleteWindow() override;

    VkSurfaceKHR getVkSurface() override;

    void setLooper(DrawLooper* drawLooper);

    void loop(float deltaT) override;

private:
    GLFWwindow* window = nullptr;
    DrawLooper* mDrawLooper = nullptr;
};

#endif //VK_FRAMEWORK_WINDOWLINUX_H
