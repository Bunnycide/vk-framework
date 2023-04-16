//
// Created by jae on 14/04/23.
//

#include "includes.h"

VkFormat H_findSupportedFormat(VkPhysicalDevice& physicalDevice,
                               const std::vector<VkFormat>& candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features) {
    for(VkFormat format :candidates){
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice,
                                            format,
                                            &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    return VK_FORMAT_UNDEFINED;
}

VkFormat H_findDepthFormat(VkPhysicalDevice& physicalDevice) {
    return H_findSupportedFormat(physicalDevice,
                                 {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                 VK_IMAGE_TILING_OPTIMAL,
                                 VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void H_createDepthResources(VkPhysicalDevice physicalDevice,
                            VkDevice logicalDevice,
                            VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                            VkCommandBuffer commandBuffer,
                            VkExtent3D size,
                            ImageInfo& depthImageInfo){
    depthImageInfo.imageUsageFlags = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depthImageInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    VkFormat depthFormat = H_findDepthFormat(physicalDevice);

    H_createImage(logicalDevice,
                  depthImageInfo,
                  VK_FALSE,
                  VK_IMAGE_TYPE_2D,
                  depthFormat,
                  size,
                  1,
                  1,
                  VK_SAMPLE_COUNT_1_BIT);

    H_allocateAndBindMemoryObjectToImage(logicalDevice,
                                         depthImageInfo,
                                         physicalDeviceMemoryProperties);

    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_DEPTH_BIT;

    if(hasStencilComponent(depthFormat)){
        aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    H_createImageView(logicalDevice,
                      depthImageInfo.image,
                      depthFormat,
                      VK_IMAGE_VIEW_TYPE_2D,
                      aspect,
                      depthImageInfo.imageView);

    std::vector<ImageTransition> image_transition;

    image_transition.push_back({
        .image = depthImageInfo.image,
        .currentAccess = 0,
        .newAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .currentLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
        .currentQueueFamily = VK_QUEUE_FAMILY_IGNORED,
        .newQueueFamily = VK_QUEUE_FAMILY_IGNORED,
        .aspect = aspect
    });
    
    H_setupImageMemoryBarrier(depthImageInfo,
                              image_transition,
                              commandBuffer,
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
}
