//
// Created by jae on 14/04/23.
//

#ifndef VK_FRAMEWORK_TEXTURE_UTIL_H
#define VK_FRAMEWORK_TEXTURE_UTIL_H

void H_create2DImageAndView(VkDevice logicalDevice,
                            VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                            VkFormat format,
                            VkExtent2D size,
                            uint32_t numMipmaps,
                            uint32_t numLayers);

#endif //VK_FRAMEWORK_TEXTURE_UTIL_H
