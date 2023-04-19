//
// Created by jae on 14/04/23.
//

#include "includes.h"

void H_CreateTexture2DSampler(VkDevice logicalDevice,
                              VkSampler& sampler){

    VkSamplerCreateInfo samplerCreateInfo{
            .sType              = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .magFilter          = VK_FILTER_LINEAR,
            .minFilter          = VK_FILTER_LINEAR,
            .mipmapMode         = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU       = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV       = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW       = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias         = 0.0f,
            .anisotropyEnable   = VK_FALSE,
            .maxAnisotropy      = 1.0f,
            .compareEnable      = VK_FALSE,
            .compareOp          = VK_COMPARE_OP_ALWAYS,
            .minLod             = 0.0f,
            .maxLod             = 0.0f,
            .borderColor        = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates    =   false
    };

    VK_CHECK_RESULT(vkCreateSampler(logicalDevice,
                                    &samplerCreateInfo,
                                    nullptr,
                                    &sampler));
}

void H_create2DImageAndView(VkDevice logicalDevice,
                            VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                            VkFormat format,
                            VkExtent2D size,
                            uint32_t numMipmaps,
                            uint32_t numLayers,
                            ImageInfo& imageInfo) {

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