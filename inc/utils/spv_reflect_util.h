//
// Created by jae on 16/04/23.
//

std::vector<DescriptorSetLayoutData> generateDescriptorInfo(const std::vector<uint32_t>&);

VertexInfo
generateVertexInfo(const std::vector<uint32_t>&);

VkPipelineVertexInputStateCreateInfo
createVertexStateInfo(const std::vector<uint32_t>&);
