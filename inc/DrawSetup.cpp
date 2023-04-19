//
// Created by jae on 18/04/23.
//

#include "includes.h"
#include "DrawSetup.h"


void DrawSetup::initDraw(VkDevice logicalDevice,
                     int width, int height, int swapChainSize) {
    clearValues[0] = {{{0.7f, 0.7f, 0.7f, 1.0f}}};
    clearValues[1].depthStencil = {1.0f, 0};

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    scissor.offset = {0, 0};
    scissor.extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    renderPassBeginInfo = VkRenderPassBeginInfo {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr,
//          .renderPass     = <render pass handle>,
//          .framebuffer    = <Next frame buffer>,
            .renderArea     = {
                    {0,0},
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            },

            .clearValueCount    = static_cast<uint32_t>(clearValues.size()),
            .pClearValues       = clearValues.data()
    };

    // Create required fence and sync objects
    imgAvailableSemaphores.resize(swapChainSize);
    renderFinishedSemaphores.resize(swapChainSize);
    syncFences.resize(swapChainSize);

    for(int i = 0; i < swapChainSize; i++){
        H_createFence(logicalDevice, VK_TRUE, syncFences[i]);
        H_createSemaphore(logicalDevice, imgAvailableSemaphores[i]);
        H_createSemaphore(logicalDevice, renderFinishedSemaphores[i]);
    }

    // Define wait stage
    waitStages.push_back(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
}

void DrawSetup::startDraw(VkDevice logicalDevice,
                          VkRenderPass renderPass,
                          std::vector<VkFramebuffer>& framebuffers,
                          VkSwapchainKHR swapChain) {
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = framebuffers[imgIndx];

    // Wait for fence to be signaled
    vkWaitForFences(logicalDevice,
                    1,
                    &syncFences[imgIndx],
                    VK_TRUE,
                    UINT64_MAX);

    vkResetFences(logicalDevice,
                  1,
                  &syncFences[imgIndx]);

    vkAcquireNextImageKHR(logicalDevice,
                          swapChain,
                          UINT64_MAX,
                          imgAvailableSemaphores[imgIndx],
                          VK_NULL_HANDLE,
                          &imgIndx);

}

void DrawSetup::endDraw(VkDevice logicalDevice,
                        VkSwapchainKHR swapchain,
                        VkQueue queue,
                        VkCommandBuffer& commandBuffer){
    VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &imgAvailableSemaphores[imgIndx],
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &renderFinishedSemaphores[imgIndx]
    };

    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, syncFences[imgIndx]))

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderFinishedSemaphores[imgIndx],
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &imgIndx,
        .pResults = nullptr,
    };

    VK_CHECK_RESULT(vkQueuePresentKHR(queue, &presentInfo))

    imgIndx = (imgIndx + 1) % 2;
}
