//
// Created by jae on 14/04/23.
//

#include "includes.h"

void H_create2DImageAndView(VkDevice logicalDevice,
                            VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                            VkFormat format,
                            VkExtent2D size,
                            uint32_t numMipmaps,
                            uint32_t numLayers) {
    ImageInfo imageInfo{};

    imageInfo.imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT;

    H_createImage(logicalDevice, imageInfo, VK_FALSE, VK_IMAGE_TYPE_2D, format,
                  VkExtent3D{size.width, size.height, 1},
                  numMipmaps,
                  numLayers,
                  VK_SAMPLE_COUNT_1_BIT);

    H_allocateAndBindMemoryObjectToImage(logicalDevice,
                                         imageInfo,
                                         physicalDeviceMemoryProperties);

    H_createImageView(logicalDevice,
                      imageInfo.image,
                      format,
                      VK_IMAGE_VIEW_TYPE_2D,
                      VK_IMAGE_ASPECT_COLOR_BIT,
                      imageInfo.imageView);
};