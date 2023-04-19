//
// Created by jae on 19/04/23.
//

#ifndef VK_FRAMEWORK_TEXTURE2D_H
#define VK_FRAMEWORK_TEXTURE2D_H

bool H_LoadImageDataFromFile(const char * filePath,
                             ImageInfo& imageInfo);

class Texture2D {
public:
    ImageInfo texture;
    VkSampler  sampler;

    Texture2D() = default;

    void create(VkPhysicalDevice, VkDevice,
              VkQueue ,
              VkCommandBuffer ,
              VkPhysicalDeviceMemoryProperties,
              const char* );
};


#endif //VK_FRAMEWORK_TEXTURE2D_H
