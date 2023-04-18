//
// Created by jae on 18/04/23.
//

#include "includes.h"

void GpuRenderPass::createRenderPass(
        VkDevice logicalDevice,
        VkPhysicalDevice physicalDevice,
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
        VkFormat surfaceFormat,
        VkExtent3D imageSize,
        VkCommandBuffer& commandBuffer,
        const char* vertPath, const char* fragPath){

    shader.compileShader(logicalDevice,
                         vertPath,
                         fragPath);

    VkDescriptorSetLayout layout;

    H_createDescriptorSetLayout(logicalDevice,
                                shader.layout_bindings,
                                layout);

    descriptorData.layouts = { layout };

    std::vector<VkDescriptorPoolSize> descPoolSizes =
            H_extractDescriptorSetTypes(shader.layout_bindings);

    VkFormat depthFormat = H_findDepthFormat(physicalDevice);

    H_createDescriptorPool(logicalDevice,
                           descPoolSizes,
                           1,
                           VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
                           descriptorData.descriptorPool);

    H_allocateDescriptorSets(logicalDevice,
                             descriptorData.descriptorPool,
                             descriptorData.layouts,
                             descriptorData.descriptorSets);

    H_createDepthResources(physicalDevice,
                           logicalDevice,
                           physicalDeviceMemoryProperties,
                           commandBuffer,
                           imageSize,
                           vulkanRender.depthResource);

    H_createPipelineLayout(logicalDevice,
                           vulkanRender.pipelineLayout,
                           descriptorData.layouts);

    H_createRenderPass(logicalDevice,
                       surfaceFormat,
                       depthFormat,
                       vulkanRender.renderPass);

    H_createRenderPipeline(logicalDevice,
                           (float)imageSize.width, (float)imageSize.height,
                           shader,
                           vulkanRender.pipelineLayout,
                           vulkanRender.renderPass,
                           vulkanRender.gfxPipeline);
}