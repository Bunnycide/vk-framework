//
// Created by jae on 06/04/23.
//

#ifndef VK_FRAMEWORK_VK_SHADER_UTILS_H
#define VK_FRAMEWORK_VK_SHADER_UTILS_H

class Shader{
    Shader() = default;
    ~Shader() = default;
private:
    std::vector<VkShaderModule> shaderModules;
public:
    void compileShader();
    void setValue();
};

#endif //VK_FRAMEWORK_VK_SHADER_UTILS_H
