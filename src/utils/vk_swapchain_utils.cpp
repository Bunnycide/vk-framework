//
// Created by jae on 04/04/23.
//

#include "includes.h"

VkSurfaceFormatKHR
H_findPresentSurfaceImageFormat(VkPhysicalDevice& physicalDevice,
                                VkSurfaceKHR& surface){
    uint32_t surfaceFormatCount;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr))

    if(surfaceFormatCount <= 0){
        Log::info("No suitable surface format, found 0 formats, using default");
        return {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    auto* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats))

    for(uint32_t i = 0; i < surfaceFormatCount; i++){
        if( (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) &&
            (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
        {
            return surfaceFormats[i];
        }
    }

    Log::info("No suitable surface format found, using default");

    delete[] surfaceFormats;

    return {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
}

VkExtent2D
H_findSupportedImageSize(VkSurfaceCapabilitiesKHR& surfaceCapabilities,
                         uint32_t defaultWidth,
                         uint32_t defaultHeight){

    VkExtent2D result  = {
            defaultWidth,
            defaultHeight
    };

    if( (defaultHeight < surfaceCapabilities.minImageExtent.height) ||
        (defaultWidth < surfaceCapabilities.minImageExtent.width)){
        result = surfaceCapabilities.minImageExtent;
    }

    if( (defaultHeight > surfaceCapabilities.maxImageExtent.height) ||
        (defaultWidth > surfaceCapabilities.maxImageExtent.width)){
        result = surfaceCapabilities.maxImageExtent;
    }

    return result;
}

VkImageUsageFlags
H_findPresentSurfaceImageUsage(VkPhysicalDevice&,
                               VkSurfaceKHR&){
    return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
}

VkSharingMode
H_findPresentSurfaceImageSharingMode(VkPhysicalDevice&,
                                     VkSurfaceKHR&){
    return VK_SHARING_MODE_EXCLUSIVE;
}

uint32_t H_findPresentationQueueFamily(VkPhysicalDevice& physicalDevice,
                                   VkSurfaceKHR& surface,
                                   std::vector<QueueInfo>& queueInfos){
    VkBool32 presentationSupported = false;
    for(auto & queueInfo : queueInfos){
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,
                                                             queueInfo.queueFamilyIndex,
                                                             surface,
                                                             &presentationSupported))
        if(presentationSupported){
            return static_cast<int>(queueInfo.queueFamilyIndex);
        }
    }

    return -1;
}

VkPresentModeKHR H_findPresentationMode(VkPhysicalDevice& physicalDevice,
                                        VkSurfaceKHR& surface){
    uint32_t numPresentModes = 0;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numPresentModes, nullptr))

    auto* presentModes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * numPresentModes);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numPresentModes, presentModes))

    for(uint32_t i = 0; i < numPresentModes; i++){
        if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) return presentModes[i];

    }

    delete presentModes;

    return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

void H_createSwapChainImageViews(VkDevice& logicalDevice,
                                 VkSurfaceFormatKHR& surfaceFormat,
                                 std::vector<VkImage>& swapChainImages,
                                 std::vector<VkImageView>& swapChainImageViews){
    swapChainImageViews.resize(swapChainImages.size());

    VkImageViewCreateInfo imageViewCreateInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};

    for(uint32_t i =  0; i < swapChainImages.size(); i++){
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = swapChainImages[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = surfaceFormat.format;
        imageViewCreateInfo.components = {
                VK_COMPONENT_SWIZZLE_R,
                VK_COMPONENT_SWIZZLE_G,
                VK_COMPONENT_SWIZZLE_B,
                VK_COMPONENT_SWIZZLE_A
        };

        imageViewCreateInfo.subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
        };

        vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]);
    }
}

void H_createSwapChainFrameBuffers(VkDevice&                    logicalDevice,
                                   ImageInfo&                   depthResource,
                                   VkRenderPass&                renderPass,
                                   std::vector<VkImageView>&    imageViews,
                                   std::vector<VkFramebuffer>&  frameBuffers){
    frameBuffers.resize(imageViews.size());

    for(int i = 0; i < frameBuffers.size(); i ++){
        std::array<VkImageView, 2> attachments = {
            imageViews[i],
            depthResource.imageView
        };

        VkFramebufferCreateInfo framebufferCreateInfo {
                /* VkStructureType          */.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                /* const void*              */.pNext = nullptr,
                /* VkFramebufferCreateFlags */.flags = 0,
                /* VkRenderPass             */.renderPass = renderPass,
                /* uint32_t                 */.attachmentCount = static_cast<uint32_t>(attachments.size()),
                /* const VkImageView*       */.pAttachments = attachments.data(),
                /* uint32_t                 */.width = 800,
                /* uint32_t                 */.height = 600,
                /* uint32_t                 */.layers = 1
        };

        VK_CHECK_RESULT(vkCreateFramebuffer(logicalDevice,
                                            &framebufferCreateInfo,
                                            nullptr,
                                            &frameBuffers[i]))
    }
}

void H_destroyFrameBuffers(VkDevice logicalDevice,
                           std::vector<VkFramebuffer>& frameBuffers){
    for(auto& frameBuffer : frameBuffers){
        vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
    }
}

void H_getSwapChainImages(VkDevice& logicalDevice, VkSwapchainKHR& swapChain  ,std::vector<VkImage>& swapChainImages){
    uint32_t swapChainImageCount = 0;
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(logicalDevice, swapChain, &swapChainImageCount, nullptr))

    if(swapChainImageCount <= 0) return;
    swapChainImages.resize(swapChainImageCount);
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(logicalDevice, swapChain, &swapChainImageCount, swapChainImages.data()));
}

void H_createSwapChain(VkPhysicalDevice& physicalDevice,
                       VkDevice& logicalDevice,
                       VkSurfaceKHR& surface,
                       uint16_t width,
                       uint32_t height,
                       std::vector<QueueInfo>& queueInfos,
                       VkSwapchainKHR& swapchain) {

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities))

    uint32_t presentationFamilyIndex =
            H_findPresentationQueueFamily(physicalDevice, surface, queueInfos);
    VkSurfaceFormatKHR surfaceFormat =
            H_findPresentSurfaceImageFormat(physicalDevice, surface);
    VkExtent2D extent = H_findSupportedImageSize(surfaceCapabilities, width, height);
    VkPresentModeKHR presentMode = H_findPresentationMode(physicalDevice, surface);

    VkSwapchainCreateInfoKHR swapChainCreateInfo{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};

    swapChainCreateInfo.pNext = nullptr;
    swapChainCreateInfo.flags = 0;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = surfaceCapabilities.minImageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.queueFamilyIndexCount = 1;
    swapChainCreateInfo.pQueueFamilyIndices = &presentationFamilyIndex;
    swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    // TODO : handle this later for alpha blending
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    // TODO : handle this to create new swap-chain
    swapChainCreateInfo.oldSwapchain = nullptr;

    VK_CHECK_RESULT(vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr, &swapchain))
}