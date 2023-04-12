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

void H_createPipelineLayout(VkDevice& logicalDevice,
                            VkPipelineLayout& pipelineLayout){
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};

    /* const void*                 */  pipelineLayoutCreateInfo.pNext                     = nullptr;
    /* VkPipelineLayoutCreateFlags */  pipelineLayoutCreateInfo.flags                     = 0;
    /* uint32_t                    */  pipelineLayoutCreateInfo.setLayoutCount            = 0;
    /* const VkDescriptorSetLayout**/  pipelineLayoutCreateInfo.pSetLayouts               = nullptr;
    /* uint32_t                    */  pipelineLayoutCreateInfo.pushConstantRangeCount    = 0;
    /* const VkPushConstantRange*  */  pipelineLayoutCreateInfo.pPushConstantRanges       = nullptr;

    VK_CHECK_RESULT(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout))
}

void H_createRenderPipeline(VkDevice& logicalDevice,
                            float viewportWidth, float viewportHeight,
                            VkPipelineLayout& pipelineLayout,
                            VkRenderPass& renderPass,
                            VkPipeline& pipeline){
    Shader shader;
    shader.compileShader(logicalDevice,
                         "/shaders/shader.vert",
                         "/shaders/shader.frag");

    VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo{
/* VkStructureType                  */ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
/* const void*                      */ .pNext = nullptr,
/* VkPipelineShaderStageCreateFlags */ .flags = 0,
/* VkShaderStageFlagBits            */ .stage = VK_SHADER_STAGE_VERTEX_BIT,
/* VkShaderModule                   */ .module = shader.getShaderModule(ShaderType_VERTEX),
/* const char*                      */ .pName = "main",
/* const VkSpecializationInfo*      */ .pSpecializationInfo = nullptr,
    };

    VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo{
/* VkStructureType                  */ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
/* const void*                      */ .pNext = nullptr,
/* VkPipelineShaderStageCreateFlags */ .flags = 0,
/* VkShaderStageFlagBits            */ .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
/* VkShaderModule                   */ .module = shader.getShaderModule(ShaderType_FRAGMENT),
/* const char*                      */ .pName = "main",
/* const VkSpecializationInfo*      */ .pSpecializationInfo = nullptr,
    };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {
            vertShaderStageCreateInfo,
            fragShaderStageCreateInfo
    };

    VkViewport viewport {
        .x = 0,
        .y = 0,
        .width = viewportWidth,
        .height = viewportHeight,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissorRect {
        .offset = {
                0,0
        },
        .extent = {
                static_cast<uint32_t>(viewportWidth),
                static_cast<uint32_t>(viewportHeight)
        }
    };

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};

    /* const void*                        */   viewportStateCreateInfo.pNext         = nullptr;
    /* VkPipelineViewportStateCreateFlags */   viewportStateCreateInfo.flags         = 0;
    /* uint32_t                           */   viewportStateCreateInfo.viewportCount = 1;
    /* const VkViewport*                  */   viewportStateCreateInfo.pViewports    = &viewport;
    /* uint32_t                           */   viewportStateCreateInfo.scissorCount  = 1;
    /* const VkRect2D*                    */   viewportStateCreateInfo.pScissors     = &scissorRect;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};

    /* const void*                             */   rasterizationStateCreateInfo.pNext                   = nullptr;
    /* VkPipelineRasterizationStateCreateFlags */   rasterizationStateCreateInfo.flags                   = 0;
    /* VkBool32                                */   rasterizationStateCreateInfo.depthClampEnable        = VK_FALSE;
    /* VkBool32                                */   rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    /* VkPolygonMode                           */   rasterizationStateCreateInfo.polygonMode             = VK_POLYGON_MODE_FILL;
    /* VkCullModeFlags                         */   rasterizationStateCreateInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
    /* VkFrontFace                             */   rasterizationStateCreateInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    /* VkBool32                                */   rasterizationStateCreateInfo.depthBiasEnable         = VK_FALSE;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasClamp          = 0.0f;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasSlopeFactor    = 0.0f;
    /* float                                   */   rasterizationStateCreateInfo.lineWidth               = 1.0f;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{
            /* VkBool32              */   .blendEnable         = VK_FALSE,
            /* VkBlendFactor         */   .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            /* VkBlendFactor         */   .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            /* VkBlendOp             */   .colorBlendOp        = VK_BLEND_OP_ADD,
            /* VkBlendFactor         */   .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            /* VkBlendFactor         */   .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            /* VkBlendOp             */   .alphaBlendOp        = VK_BLEND_OP_ADD,
            /* VkColorComponentFlags */   .colorWriteMask      =    VK_COLOR_COMPONENT_R_BIT |
                                                                    VK_COLOR_COMPONENT_G_BIT |
                                                                    VK_COLOR_COMPONENT_B_BIT |
                                                                    VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};

    /* const void*                                */   colorBlendStateCreateInfo.pNext             = nullptr;
    /* VkPipelineColorBlendStateCreateFlags       */   colorBlendStateCreateInfo.flags             = 0;
    /* VkBool32                                    */  colorBlendStateCreateInfo.logicOpEnable     = VK_FALSE,
    /* VkLogicOp                                   */  colorBlendStateCreateInfo.logicOp           = VK_LOGIC_OP_COPY,
    /* uint32_t                                   */   colorBlendStateCreateInfo.attachmentCount   = 1;
    /* const VkPipelineColorBlendAttachmentState* */   colorBlendStateCreateInfo.pAttachments      = &colorBlendAttachment;

    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType              = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount  = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates     = dynamicStates.data();

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{
/* VkStructureType                       */ .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
/* const void*                           */ .pNext = nullptr,
/* VkPipelineMultisampleStateCreateFlags */ .flags = 0,
/* VkSampleCountFlagBits                 */ .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
/* VkBool32                              */ .sampleShadingEnable = VK_FALSE,
/* VkBool32                              */ .alphaToCoverageEnable = VK_FALSE,
/* VkBool32                              */ .alphaToOneEnable = VK_FALSE,
    };

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo
            {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};

    depthStencilStateCreateInfo.pNext = nullptr;
    depthStencilStateCreateInfo.flags = 0;
    depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
//    depthStencilStateCreateInfo.front = ;
//    depthStencilStateCreateInfo.back = ;
//    depthStencilStateCreateInfo.minDepthBounds = ;
//    depthStencilStateCreateInfo.maxDepthBounds = ;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    /* const void*                                    */  graphicsPipelineCreateInfo.pNext              = nullptr;
    /* VkPipelineCreateFlags                          */  graphicsPipelineCreateInfo.flags              = 0;
    /* uint32_t                                       */  graphicsPipelineCreateInfo.stageCount         = static_cast<uint32_t>(shaderStages.size());
    /* const VkPipelineShaderStageCreateInfo*         */  graphicsPipelineCreateInfo.pStages            = shaderStages.data();
    /* const VkPipelineVertexInputStateCreateInfo*    */  graphicsPipelineCreateInfo.pVertexInputState  = nullptr;
    /* const VkPipelineInputAssemblyStateCreateInfo*  */  graphicsPipelineCreateInfo.pInputAssemblyState= nullptr;
    /* const VkPipelineTessellationStateCreateInfo*   */  graphicsPipelineCreateInfo.pTessellationState = nullptr;
    /* const VkPipelineViewportStateCreateInfo*       */  graphicsPipelineCreateInfo.pViewportState     = &viewportStateCreateInfo;
    /* const VkPipelineRasterizationStateCreateInfo*  */  graphicsPipelineCreateInfo.pRasterizationState= &rasterizationStateCreateInfo;
    /* const VkPipelineMultisampleStateCreateInfo*    */  graphicsPipelineCreateInfo.pMultisampleState  = &multisampleStateCreateInfo;
    // TODO: Remember to create depth state (later)
    /* const VkPipelineDepthStencilStateCreateInfo*   */  graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
    /* const VkPipelineColorBlendStateCreateInfo*     */  graphicsPipelineCreateInfo.pColorBlendState   = &colorBlendStateCreateInfo;
    /* const VkPipelineDynamicStateCreateInfo*        */  graphicsPipelineCreateInfo.pDynamicState      = &dynamicState;
    /* VkPipelineLayout                               */  graphicsPipelineCreateInfo.layout             = pipelineLayout;
    /* VkRenderPass                                   */  graphicsPipelineCreateInfo.renderPass         = renderPass;
    /* uint32_t                                       */  graphicsPipelineCreateInfo.subpass            = 0;
    /* VkPipeline                                     */  graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    /* int32_t                                        */  graphicsPipelineCreateInfo.basePipelineIndex  = -1;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(logicalDevice,
                                              nullptr,
                                              1,
                                              &graphicsPipelineCreateInfo,
                                              nullptr,
                                              &pipeline))

    shader.deleteShader(logicalDevice);
}

void H_createDepthResource(VkPhysicalDevice physicalDevice,
                           VkDevice logicalDevice,
                           BufferInfo* depthBuffer){
    // TODO : Test function improve implementation to work
    depthBuffer->usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    depthBuffer->bufSz = 512;

    H_createBuffer(logicalDevice, *depthBuffer);
}