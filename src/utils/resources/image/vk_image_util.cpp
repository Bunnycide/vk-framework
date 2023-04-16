//
// Created by jae on 13/04/23.
//

#include "includes.h"

void H_createImage(VkDevice logicalDevice,
                   ImageInfo& imageInfo ,
                   VkBool32 isCubeMap,
                   VkImageType type,
                   VkFormat imageFormat,
                   VkExtent3D size,
                   uint32_t numMipMaps,
                   uint32_t numArrayLayers,
                   VkSampleCountFlagBits samples) {
    VkImageCreateInfo imageCreateInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

/* const void*           */   imageCreateInfo.pNext = nullptr;
/* VkImageCreateFlags    */   imageCreateInfo.flags = isCubeMap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
/* VkImageType           */   imageCreateInfo.imageType = type;
/* VkFormat              */   imageCreateInfo.format = imageFormat;
/* VkExtent3D            */   imageCreateInfo.extent = size;
/* uint32_t              */   imageCreateInfo.mipLevels = numMipMaps;
/* uint32_t              */   imageCreateInfo.arrayLayers = numArrayLayers;
/* VkSampleCountFlagBits */   imageCreateInfo.samples = samples;
/* VkImageTiling         */   imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
/* VkImageUsageFlags     */   imageCreateInfo.usage = imageInfo.imageUsageFlags;
/* VkSharingMode         */   imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
/* uint32_t              */   imageCreateInfo.queueFamilyIndexCount = 0;
/* const uint32_t*       */   imageCreateInfo.pQueueFamilyIndices = nullptr;
/* VkImageLayout         */   imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VK_CHECK_RESULT(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &imageInfo.image))
}

void H_allocateAndBindMemoryObjectToImage(VkDevice logicalDevice,
                                          ImageInfo& imageInfo,
                                          VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties){
    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(logicalDevice, imageInfo.image, &memory_requirements);

    imageInfo.memoryObj = VK_NULL_HANDLE;

    for(uint32_t type = 0; type < physicalDeviceMemoryProperties.memoryTypeCount; ++type){

        /**
         *First section to decide if type index is correct
         *(memory_requirements.memoryTypeBits & (1 << type))
         *
         * Second section checks for memory type being present
         *
         * ((physical_device_memory_properties.memoryTypes[type].propertyFlags &
         *    imageInfo.memoryProperties) == imageInfo.memoryProperties)
         */

        if( (memory_requirements.memoryTypeBits & (1 << type)) &&
            ((physicalDeviceMemoryProperties.memoryTypes[type].propertyFlags &
              imageInfo.memoryProperties) == imageInfo.memoryProperties) ){

            VkMemoryAllocateInfo buffer_memory_allocate_info = {
                    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                    nullptr,
                    memory_requirements.size,
                    type
            };

            VkResult result = vkAllocateMemory(logicalDevice,
                                               &buffer_memory_allocate_info,
                                               nullptr,
                                               &imageInfo.memoryObj);

            if(result == VK_SUCCESS){
                break;
            }
        }
    }

    if(VK_NULL_HANDLE == imageInfo.memoryObj){
        Log::error("Could not allocate memory for image");
        return;
    }

    VK_CHECK_RESULT(vkBindImageMemory(logicalDevice, imageInfo.image, imageInfo.memoryObj, 0))
}

void H_setupImageMemoryBarrier(ImageInfo& imageInfo,
                               std::vector<ImageTransition>& imageTransitions,
                               VkCommandBuffer commandBuffer,
                               VkPipelineStageFlags generatingStages,
                               VkPipelineStageFlags consumingStages){
    std::vector<VkImageMemoryBarrier> imageMemoryBarriers(imageTransitions.size());

    for(int i = 0; i < imageTransitions.size(); i++){
        imageMemoryBarriers[i] = VkImageMemoryBarrier{
/* VkStructureType         */   .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
/* const void*             */   .pNext = nullptr,
/* VkAccessFlags           */   .srcAccessMask = imageTransitions[i].currentAccess,
/* VkAccessFlags           */   .dstAccessMask = imageTransitions[i].newAccess,
/* VkImageLayout           */   .oldLayout = imageTransitions[i].currentLayout,
/* VkImageLayout           */   .newLayout = imageTransitions[i].newLayout,
/* uint32_t                */   .srcQueueFamilyIndex = imageTransitions[i].currentQueueFamily,
/* uint32_t                */   .dstQueueFamilyIndex = imageTransitions[i].newQueueFamily,
/* VkImage                 */   .image = imageInfo.image,
/* VkImageSubresourceRange */   .subresourceRange = {
    /* VkImageAspectFlags */   .aspectMask = imageTransitions[i].aspect,
    /* uint32_t           */   .baseMipLevel = 0,
    /* uint32_t           */   .levelCount = VK_REMAINING_MIP_LEVELS,
    /* uint32_t           */   .baseArrayLayer = 0,
    /* uint32_t           */   .layerCount = VK_REMAINING_ARRAY_LAYERS
            },
        };
    }

    H_beginCommandBufferRecording(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdPipelineBarrier(commandBuffer,
                         generatingStages,consumingStages,
                         0, 0, nullptr, 0, nullptr,
                         static_cast<uint32_t>(imageMemoryBarriers.size()),
                         &imageMemoryBarriers[0]);

    H_endCommandBufferRecording(commandBuffer);
}

void H_createImageView(VkDevice logicalDevice,
                       VkImage image,
                       VkFormat format,
                       VkImageViewType viewType,
                       VkImageAspectFlags aspect,
                       VkImageView& imageView){

    VkImageViewCreateInfo image_view_create_info = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            nullptr,
            0,
            image,
            viewType,
            format,
            {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY
            },
            {
                    aspect,
                    0,
                    VK_REMAINING_MIP_LEVELS,
                    0,
                    VK_REMAINING_ARRAY_LAYERS
            }
    };

    VK_CHECK_RESULT(vkCreateImageView( logicalDevice,
                                       &image_view_create_info,
                                       nullptr, &imageView ))

}

void H_freeImage(VkDevice logicalDevice,
                 ImageInfo& imageInfo){
    vkDestroyImage(logicalDevice, imageInfo.image, nullptr);
    vkFreeMemory(logicalDevice, imageInfo.memoryObj, nullptr);
    vkDestroyImageView(logicalDevice, imageInfo.imageView, nullptr);
}
