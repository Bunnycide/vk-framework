//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_DRAWSETUP_H
#define VK_FRAMEWORK_DRAWSETUP_H

class DrawSetup {
public:
    // Renderpass infos
    VkDeviceSize offset = 0;
    uint32_t imgIndx = 0;

    std::array<VkClearValue, 2> clearValues{};

    VkViewport viewport{};
    VkRect2D scissor{};

    VkRenderPassBeginInfo renderPassBeginInfo{};

    //Sync objects
    std::vector<VkSemaphore> imgAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<VkFence>    syncFences;

    // Wait stages
    std::vector<VkPipelineStageFlags> waitStages;

    ~DrawSetup() = default;
    DrawSetup() = default;

    void initDraw(VkDevice, int , int , int);
    void startDraw(VkDevice ,
                   VkRenderPass ,
                   std::vector<VkFramebuffer>& ,
                   VkSwapchainKHR );

    void endDraw(VkDevice ,
                 VkSwapchainKHR ,
                 VkQueue ,
                 VkCommandBuffer& );

};

#endif //VK_FRAMEWORK_DRAWSETUP_H
