//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_WINDOW_H
#define VK_FRAMEWORK_WINDOW_H


class Window {
protected:
    int width = 0, height = 0;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

public:
    Window() = default;
    Window(int & width, int & height, const char* name) : width(width), height(height){}
    ~Window() = default;

    virtual VkSurfaceKHR getVkSurface() = 0;
    virtual VkExtent2D getWindowSize() = 0;

    virtual void createWindowSurface(VkInstance) = 0;
    virtual void deleteWindow() = 0;
    virtual void loop(float deltaT) = 0;
};

#endif //VK_FRAMEWORK_WINDOW_H
