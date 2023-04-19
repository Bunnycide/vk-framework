//
// Created by jae on 19/04/23.
//

#include "includes.h"

bool H_LoadImageDataFromFile(const char * filePath,
                                      ImageInfo& imageInfo) {

    int imgWidth, imgHeight, n;

    imageInfo.imageData = stbi_load( filePath, &imgWidth, &imgHeight, &n, 4);

    imageInfo.imgHeight = imgHeight;
    imageInfo.imgWidth = imgWidth;
    imageInfo.imgSize = imgWidth * imgHeight * 4;

    if(imageInfo.imageData == nullptr){
        return false;
    }

    return true;

}

void Texture2D::create(VkPhysicalDevice physicalDevice,
                        VkDevice logicalDevice,
                     VkQueue queue,
                     VkCommandBuffer commandBuffer,
                     VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                     const char* filePath ){

    std::string absolute_path;
    getAssetPath(filePath, absolute_path);

    H_LoadImageDataFromFile(absolute_path.c_str(), texture);

    texture.imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT|
                            VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    texture.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    H_create2DImageAndView(logicalDevice,
                           physicalDeviceMemoryProperties,
                           VK_FORMAT_R8G8B8A8_SRGB,
                           {
                                static_cast<uint32_t>(texture.imgWidth),
                                static_cast<uint32_t>(texture.imgHeight)
                           },
                           0,
                           0,
                           texture);
    // Staging buffer
    BufferInfo stagingBufferInfo;
    stagingBufferInfo.bufSz = texture.imgSize;
    stagingBufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    H_createBuffer(logicalDevice, stagingBufferInfo);

    H_allocateAndBindMemoryObjectToBuffer(physicalDevice,
                                          logicalDevice,
                                          physicalDeviceMemoryProperties,
                                          stagingBufferInfo);
    VK_CHECK_RESULT(vkMapMemory(logicalDevice,
                                stagingBufferInfo.memoryObj,
                                0,
                                texture.imgSize,
                                0,
                                &stagingBufferInfo.memoryPointer))

    std::memcpy(stagingBufferInfo.memoryPointer,
                texture.imageData,
                texture.imgSize);

    vkUnmapMemory(logicalDevice,
                  stagingBufferInfo.memoryObj);

    stbi_image_free(texture.imageData);

    VkBufferImageCopy region {
            .bufferOffset      = 0,
            .bufferRowLength   = 0,
            .bufferImageHeight = 0,
            .imageSubresource  = {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel       = 0,
                    .baseArrayLayer = 0,
                    .layerCount     = 1,
            },

            .imageOffset       = VkOffset3D{0, 0, 0},
            .imageExtent       = VkExtent3D  {
                    static_cast<uint32_t>(texture.imgWidth),
                    static_cast<uint32_t>(texture.imgHeight),
                    1
            }
    };

    std::vector<ImageTransition> image_transition;

    image_transition.push_back({
        .image = texture.image,
        .currentAccess = 0,
        .newAccess = VK_ACCESS_TRANSFER_WRITE_BIT,
        .currentLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .currentQueueFamily = VK_QUEUE_FAMILY_IGNORED,
        .newQueueFamily = VK_QUEUE_FAMILY_IGNORED,
        .aspect = VK_IMAGE_ASPECT_COLOR_BIT
    });

    H_setupImageMemoryBarrier(texture, image_transition, commandBuffer,
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    H_beginCommandBufferRecording(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdCopyBufferToImage(commandBuffer,
                           stagingBufferInfo.buffer,
                           texture.image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &region);

    H_endCommandBufferRecording(commandBuffer);

    VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer
    };

    vkQueueSubmit(queue,
                  1,
                  &submitInfo,
                  VK_NULL_HANDLE);

    vkQueueWaitIdle(queue);

    image_transition[0] = {
            .image = texture.image,
            .currentAccess = VK_ACCESS_TRANSFER_WRITE_BIT,
            .newAccess = VK_ACCESS_SHADER_READ_BIT,
            .currentLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .currentQueueFamily = VK_QUEUE_FAMILY_IGNORED,
            .newQueueFamily = VK_QUEUE_FAMILY_IGNORED,
            .aspect = VK_IMAGE_ASPECT_COLOR_BIT
    };

    H_setupImageMemoryBarrier(texture, image_transition, commandBuffer,
                              VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

    vkQueueSubmit(queue,
                  1,
                  &submitInfo,
                  VK_NULL_HANDLE);

    vkQueueWaitIdle(queue);

    H_CreateTexture2DSampler(logicalDevice, sampler);
}
