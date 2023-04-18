//
// Created by jae on 18/04/23.
//

#include "includes.h"

VkDrawUtils::VkDrawUtils(int width, int height) {
    clearValues[0] = {{{0.7f, 0.7f, 0.7f, 1.0f}}};
    clearValues[1].depthStencil = {1.0f, 0};

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    scissor.offset = {0, 0};
    scissor.extent = {800, 600};

    renderPassBeginInfo = {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr,
//            .renderPass     = vulkanRender.renderPass,
//            .framebuffer    = vulkanSwapChain.swapChainFrameBuffers[0],
            .renderArea     = {
                    {0,0},
                    800, 600
            },

            .clearValueCount    = static_cast<uint32_t>(clearValues.size()),
            .pClearValues       = clearValues.data()
    };
}

void VkDrawUtils::startDraw(VkCommandBuffer& commandBuffer,
                            VkRenderPass& renderPass,
                            VkFramebuffer& frameBuffer){
    renderPassBeginInfo.framebuffer = frameBuffer;
    renderPassBeginInfo.renderPass = renderPass;

    H_beginCommandBufferRecording(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0,1, &scissor);
}

