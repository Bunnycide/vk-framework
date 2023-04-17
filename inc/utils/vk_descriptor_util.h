//
// Created by jae on 16/04/23.
//

#ifndef VK_FRAMEWORK_VK_DESCRIPTOR_UTIL_H
#define VK_FRAMEWORK_VK_DESCRIPTOR_UTIL_H

void
H_allocateDescriptorSets(VkDevice ,
                              VkDescriptorPool& ,
                              const std::vector<VkDescriptorSetLayout>& ,
                              std::vector<VkDescriptorSet>& );

void
H_createDescriptorSetLayout(VkDevice ,
                                 const std::vector<VkDescriptorSetLayoutBinding>& ,
                                 VkDescriptorSetLayout& );

std::vector<VkDescriptorPoolSize>
H_extractDescriptorSetTypes(std::vector<VkDescriptorSetLayoutBinding>& );

void
H_createDescriptorPool(VkDevice,
                       const std::vector<VkDescriptorPoolSize>&,
                       uint32_t,
                       VkDescriptorPoolCreateFlags,
                       VkDescriptorPool&);

void
H_updateDescriptorSets(VkDevice ,
                       const std::vector<BufferDescriptorInfo>& ,
                       const std::vector<CopyDescriptorInfo>& );

//vkCmdBindDescriptorSets( command_buffer, pipeline_type, pipeline_layout,
//        index_for_first_set, static_cast<uint32_t>(descriptor_sets.size()),
//        descriptor_sets.data(), static_cast<uint32_t>(dynamic_offsets.size()),
//        dynamic_offsets.data() )

void
H_destroyDescriptorData(VkDevice ,
                             DescriptorData &);

#endif //VK_FRAMEWORK_VK_DESCRIPTOR_UTIL_H
