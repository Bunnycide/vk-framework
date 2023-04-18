//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_DRAW_LOOPER_H
#define VK_FRAMEWORK_DRAW_LOOPER_H

class DrawLooper{
public:
    DrawLooper() = default;
    ~DrawLooper() = default;
    virtual void draw() = 0;
};

#endif //VK_FRAMEWORK_DRAW_LOOPER_H
