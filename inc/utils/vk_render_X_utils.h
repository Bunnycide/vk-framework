//
// Created by jae on 05/04/23.
//

#ifndef VK_FRAMEWORK_VK_RENDER_X_UTILS_H
#define VK_FRAMEWORK_VK_RENDER_X_UTILS_H

VkFormat H_findSupportedFormat(VkPhysicalDevice& ,
                               const std::vector<VkFormat>& ,
                               VkImageTiling ,
                               VkFormatFeatureFlags );

VkFormat H_findDepthFormat(VkPhysicalDevice&);



void H_createRenderPass(VkDevice&,
                        VkFormat,
                        VkFormat,
                        VkRenderPass&);

void H_createPipelineLayout(VkDevice&,
                            VkPipelineLayout& );

void H_createRenderPipeline();

#endif //VK_FRAMEWORK_VK_RENDER_X_UTILS_H
