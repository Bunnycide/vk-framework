//
// Created by jae on 17/04/23.
//
#include "includes.h"

void
H_clearColor(VkCommandBuffer commandBuffer,
                  VkImage image,
                  VkImageLayout imageLayout,
                  std::vector<VkImageSubresourceRange>& imageSubResource,
                  VkClearColorValue clearColorValue){
    vkCmdClearColorImage(commandBuffer,
                         image,
                         imageLayout,
                         &clearColorValue,
                         static_cast<uint32_t>(imageSubResource.size()),
                         imageSubResource.data());
}

void
H_clearColor(VkCommandBuffer commandBuffer,
                  VkImage image,
                  VkImageLayout imageLayout,
                  std::vector<VkImageSubresourceRange>& imageSubResource,
                  VkClearDepthStencilValue clearColorValue){
    vkCmdClearDepthStencilImage(commandBuffer,
                         image,
                         imageLayout,
                         &clearColorValue,
                         static_cast<uint32_t>(imageSubResource.size()),
                         imageSubResource.data());
}

void
H_clearRenderPassAttachments(VkCommandBuffer commandBuffer,
                             std::vector<VkClearAttachment>& attachments,
                             std::vector<VkClearRect>& clearRect){
    vkCmdClearAttachments(commandBuffer,
                          static_cast<uint32_t>(attachments.size()),
                          attachments.data(),
                          static_cast<uint32_t>(clearRect.size()),
                          clearRect.data());
}

void
H_bindVertexBuffers(VkCommandBuffer commandBuffer,
                    uint32_t firstBinding,
                    std::vector<VertexBufferParameters>& vertexBufferParameters){

    std::vector<VkBuffer> buffers(vertexBufferParameters.size());
    std::vector<VkDeviceSize> offsets(vertexBufferParameters.size());

    for(auto& bufferParameters : vertexBufferParameters){
        buffers.push_back(bufferParameters.Buffer);
        offsets.push_back(bufferParameters.MemoryOffset);
    }

    vkCmdBindVertexBuffers(commandBuffer,
                           firstBinding,
                           static_cast<uint32_t>(vertexBufferParameters.size()),
                           buffers.data(),
                           offsets.data());

}

void
H_bindIndexBuffer(VkCommandBuffer commandBuffer,
                  VkBuffer indexBuffer,
                  VkDeviceSize memoryOffset,
                  VkIndexType indexType){

    vkCmdBindIndexBuffer(commandBuffer,
                         indexBuffer,
                         memoryOffset,
                         indexType);
}

void
H_updatePushConstants(VkCommandBuffer commandBuffer,
                      VkPipelineLayout& pipelineLayout,
                      VkShaderStageFlags pipelineStages,
                      uint32_t offset,
                      uint32_t size,
                      void* data){
    vkCmdPushConstants(commandBuffer,
                       pipelineLayout,
                       pipelineStages,
                       offset,
                       size,
                       data);
}
