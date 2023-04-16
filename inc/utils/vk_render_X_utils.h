//
// Created by jae on 05/04/23.
//

#ifndef VK_FRAMEWORK_VK_RENDER_X_UTILS_H
#define VK_FRAMEWORK_VK_RENDER_X_UTILS_H

void H_createRenderPass(VkDevice&,
                        VkFormat,
                        VkFormat,
                        VkRenderPass&);

void H_createPipelineLayout(VkDevice&,
                            VkPipelineLayout&,
                            std::vector<VkDescriptorSetLayout>&);

void H_createRenderPipeline(VkDevice&,
                            float, float,
                            Shader&,
                            VkPipelineLayout&,
                            VkRenderPass&,
                            VkPipeline&);

void H_parseSpriVToDescriptorLayout();

#endif //VK_FRAMEWORK_VK_RENDER_X_UTILS_H
