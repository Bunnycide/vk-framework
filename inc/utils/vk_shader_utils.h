//
// Created by jae on 06/04/23.
//

#ifndef VK_FRAMEWORK_VK_SHADER_UTILS_H
#define VK_FRAMEWORK_VK_SHADER_UTILS_H

static void getShaderString(const std::string&,
                            std::string &);

std::vector<uint32_t> compileShaderToSpv(std::string &,
                                         VkShaderStageFlagBits);

VkResult buildShaderFromSource(VkDevice logicalDevice,
                               const std::vector<uint32_t>&,
                               VkShaderModule* shaderOut);

class Shader{
private:
    std::map<ShaderTypes, VkShaderModule> shaderModules;

public:
    VertexInfo  vertexInfo;

    std::vector<VkDescriptorSetLayoutBinding> layout_bindings;

    // Shader creation and deletion
    void compileShader(VkDevice,
                       const char*, const char*);
    void deleteShader(VkDevice);

    // TODO: Future use, to set values on shader object
    void setValue();

    // Getters
    VkShaderModule getShaderModule(ShaderTypes);

    Shader() = default;
    ~Shader() = default;
};

#endif //VK_FRAMEWORK_VK_SHADER_UTILS_H
