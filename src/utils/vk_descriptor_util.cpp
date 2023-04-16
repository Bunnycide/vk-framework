//
// Created by jae on 16/04/23.
//


#include "includes.h"

void H_allocateDescriptorSets(VkDevice logicalDevice,
                              VkDescriptorPool& descriptorPool,
                              const std::vector<VkDescriptorSetLayout>& layouts,
                              std::vector<VkDescriptorSet>& descriptorSets){

    descriptorSets.resize(layouts.size());

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext              = nullptr,
        .descriptorPool     = descriptorPool,
        .descriptorSetCount = static_cast<uint32_t>(layouts.size()),
        .pSetLayouts        = layouts.data(),
    };

    VK_CHECK_RESULT(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &descriptorSets[0]))
}

void H_createDescriptorSetLayout(VkDevice logicalDevice,
                                 const std::vector<VkDescriptorSetLayoutBinding>& layout_bindings,
                                 VkDescriptorSetLayout& layout){

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(layout_bindings.size()),
        .pBindings = layout_bindings.data()
    };

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(logicalDevice,
                                                &layoutCreateInfo,
                                                nullptr,
                                                &layout))
}

std::vector<VkDescriptorPoolSize>
H_extractDescriptorSetTypes(std::vector<VkDescriptorSetLayoutBinding>& layout_bindings){

    std::vector<VkDescriptorPoolSize> descriptor_types;

    std::map<VkDescriptorType , uint32_t> descriptorCounts;

    for(auto& type : layout_bindings){
        if(descriptorCounts.find(type.descriptorType) == descriptorCounts.end()){
            descriptorCounts[type.descriptorType] = 1;
        } else {
            descriptorCounts[type.descriptorType] += 1;
        }
    }

    descriptor_types.reserve(descriptorCounts.size());

    for(auto& descriptorCount : descriptorCounts){
        descriptor_types.push_back({
                                           descriptorCount.first, descriptorCount.second
                                   });
    }

    std::reverse(descriptor_types.begin(), descriptor_types.end());

    return descriptor_types;
}

void H_createDescriptorPool(VkDevice logicalDevice,
                            const std::vector<VkDescriptorPoolSize>& descriptorTypes,
                            uint32_t maxSets,
                            VkDescriptorPoolCreateFlags flags,
                            VkDescriptorPool& descriptorPool)
{

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.flags = flags;
    descriptorPoolCreateInfo.maxSets = maxSets;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorTypes.size());
    descriptorPoolCreateInfo.pPoolSizes = descriptorTypes.data();

    VkResult result = vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &descriptorPool);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool");
    }
}

void H_destroyDescriptorData(VkDevice logicalDevice,
                             DescriptorData &descriptorData){
    for(auto& layout : descriptorData.layouts){
        vkDestroyDescriptorSetLayout(logicalDevice, layout, nullptr);
    }

    vkDestroyDescriptorPool(logicalDevice, descriptorData.descriptorPool, nullptr);
}

