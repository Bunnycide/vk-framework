//
// Created by jae on 06/04/23.
//

#ifndef VK_FRAMEWORK_VK_SHADER_UTILS_H
#define VK_FRAMEWORK_VK_SHADER_UTILS_H

VkResult buildShaderFromSource(VkDevice logicalDevice,
                               std::string &shaderSource,
                               VkShaderStageFlagBits type,
                               VkShaderModule* shaderOut);

class Shader{
private:
    std::map<VkShaderModule, ShaderTypes> shaderModules;

    static void getShaderString(const std::string& ,
                                std::string &);
public:
    void compileShader(VkDevice,
                       const char*, const char*);
    void setValue();

    Shader() = default;
    ~Shader() = default;
};

#endif //VK_FRAMEWORK_VK_SHADER_UTILS_H
