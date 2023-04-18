//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_VK_DRAWSETUP_H
#define VK_FRAMEWORK_VK_DRAWSETUP_H

class VkDrawUtils {
private:
    std::array<VkClearValue, 2> clearValues{};
    VkViewport viewport{};
    VkRect2D scissor{};
    VkDeviceSize offset = 0;
    VkRenderPassBeginInfo renderPassBeginInfo;

public:
    VkDrawUtils(int width, int height);
    ~VkDrawUtils() = default;

    void startDraw(VkCommandBuffer& ,
                   VkRenderPass& renderPass,
                   VkFramebuffer& );

};

#endif //VK_FRAMEWORK_VK_DRAWSETUP_H
