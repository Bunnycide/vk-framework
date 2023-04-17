//
// Created by jae on 17/04/23.
//

#ifndef VK_FRAMEWORK_VK_DRAW_UTIL_H
#define VK_FRAMEWORK_VK_DRAW_UTIL_H

void
H_clearColor(VkCommandBuffer commandBuffer,
              VkImage image,
              VkImageLayout imageLayout,
              std::vector<VkImageSubresourceRange>& imageSubResource,
              VkClearColorValue clearColorValue);

void
H_clearColor(VkCommandBuffer commandBuffer,
              VkImage image,
              VkImageLayout imageLayout,
              std::vector<VkImageSubresourceRange>& imageSubResource,
              VkClearDepthStencilValue clearColorValue);

void
H_clearRenderPassAttachments(VkCommandBuffer,
                             std::vector<VkClearAttachment>&,
                             std::vector<VkClearRect>&);

void
H_bindVertexBuffers(VkCommandBuffer,
                    uint32_t,
                    std::vector<VertexBufferParameters>&);

void
H_bindIndexBuffer(VkCommandBuffer,
                  VkBuffer,
                  VkDeviceSize,
                  VkIndexType);

#endif //VK_FRAMEWORK_VK_DRAW_UTIL_H
