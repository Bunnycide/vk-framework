//
// Created by jae on 05/04/23.
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

void H_createRenderPass(VkDevice& logicalDevice,
                        VkFormat colorAttachmentFormat,
                        VkFormat depthAttachmentFormat,
                        VkRenderPass& pRenderPass){
    VkAttachmentDescription colorPassAttachment;

    /* VkAttachmentDescriptionFlags */   colorPassAttachment.flags               = 0;
    /* VkFormat                     */   colorPassAttachment.format              = colorAttachmentFormat;
    /* VkSampleCountFlagBits        */   colorPassAttachment.samples             = VK_SAMPLE_COUNT_1_BIT;
    /* VkAttachmentLoadOp           */   colorPassAttachment.loadOp              = VK_ATTACHMENT_LOAD_OP_CLEAR;
    /* VkAttachmentStoreOp          */   colorPassAttachment.storeOp             = VK_ATTACHMENT_STORE_OP_STORE;
    /* VkAttachmentLoadOp           */   colorPassAttachment.stencilLoadOp       = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    /* VkAttachmentStoreOp          */   colorPassAttachment.stencilStoreOp      = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    /* VkImageLayout                */   colorPassAttachment.initialLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
    /* VkImageLayout                */   colorPassAttachment.finalLayout         = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference{};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthPassAttachment;
    /* VkAttachmentDescriptionFlags */   depthPassAttachment.flags               = 0;
    /* VkFormat                     */   depthPassAttachment.format              = depthAttachmentFormat;
    /* VkSampleCountFlagBits        */   depthPassAttachment.samples             = VK_SAMPLE_COUNT_1_BIT;
    /* VkAttachmentLoadOp           */   depthPassAttachment.loadOp              = VK_ATTACHMENT_LOAD_OP_CLEAR;
    /* VkAttachmentStoreOp          */   depthPassAttachment.storeOp             = VK_ATTACHMENT_STORE_OP_STORE;
    /* VkAttachmentLoadOp           */   depthPassAttachment.stencilLoadOp       = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    /* VkAttachmentStoreOp          */   depthPassAttachment.stencilStoreOp      = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    /* VkImageLayout                */   depthPassAttachment.initialLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
    /* VkImageLayout                */   depthPassAttachment.finalLayout         = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentReference{};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::array<VkAttachmentDescription, 2> attachments = {colorPassAttachment, depthPassAttachment};

    VkSubpassDescription subpassDescription{};
    
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

    VkSubpassDependency subpassDependency{};

    /* uint32_t             */   subpassDependency.srcSubpass      = VK_SUBPASS_EXTERNAL;
    /* uint32_t             */   subpassDependency.dstSubpass      = 0;
    /* VkPipelineStageFlags */   subpassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    /* VkPipelineStageFlags */   subpassDependency.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    /* VkAccessFlags        */   subpassDependency.srcAccessMask   = 0;
    /* VkAccessFlags        */   subpassDependency.dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    /* const void*                     */  renderPassCreateInfo.pNext             = nullptr;
    /* VkRenderPassCreateFlags         */  renderPassCreateInfo.flags             = 0;
    /* uint32_t                        */  renderPassCreateInfo.attachmentCount   = static_cast<uint32_t>(attachments.size());
    /* const VkAttachmentDescription*  */  renderPassCreateInfo.pAttachments      = attachments.data();
    /* uint32_t                        */  renderPassCreateInfo.subpassCount      = 1;
    /* const VkSubpassDescription*     */  renderPassCreateInfo.pSubpasses        = &subpassDescription;
    /* uint32_t                        */  renderPassCreateInfo.dependencyCount   = 1;
    /* const VkSubpassDependency*      */  renderPassCreateInfo.pDependencies     = &subpassDependency;

    VK_CHECK_RESULT(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &pRenderPass))
}

void H_createRenderPipeline(){
//    vkCreatePipelineLayout
}